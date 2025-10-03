/*!
@file dtw_mpi.c
@brief DTAIDistance.dtw

@author Wannes Meert
@copyright Copyright © 2020 Wannes Meert. Apache License, Version 2.0, see LICENSE for details.

Adapted version by Daniela Rigoli to use MPI
*/

#include "dd_dtw_mpi.h"
#include "dd_dtw.h" 
#include <mpi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

/*!
Distance matrix for n-dimensional DTW, executed on a list of pointers to arrays and in parallel.

@see dtw_distances_ptrs

# MPI setup
Use MPI_Comm_rank to know if process is master (rank 0) or slave (rank > 0).

Master distributes pairs (r,c) to slaves.

Inicialização
O mestre faz um MPI_Bcast de todas as séries (ptrs e lengths) para os escravos.
Cada escravo guarda cópia local de todas as séries.

Distribuição de tarefas
Mestre envia apenas um vetor [r, c] (dois inteiros) para cada escravo disponível.

Slaves compute DTW distances for those pairs (using OpenMP internally if multiple cores per node).

Results are sent back to master.

After all tasks are done, master sends a “KILL” tag so slaves can exit gracefully.
*/

/* tags */
#define WORKTAG 1
#define KILLTAG 2
#define RESULTTAG 3

#define VERBOSE 0    // 1 for progress messages, 0 for none

int dtw_distances_prepare_MS(DTWBlock *block, idx_t nb_series_r, idx_t nb_series_c, idx_t **cbs, idx_t **rls, idx_t *length, DTWSettings *settings) {
    idx_t cb, rs, ir;
    
    *length = dtw_distances_length(block, nb_series_r, nb_series_c);
    if (length == 0) {
        return 1;
    }
    
    // Correct block
    if (block->re == 0) {
        block->re = nb_series_r;
    }
    if (block->ce == 0) {
        block->ce = nb_series_c;
    }
    if (block->re <= block->rb) {
        *length = 0;
        return 1;
    }
    if (block->ce <= block->cb) {
        *length = 0;
        return 1;
    }

    if (block->triu) {
        *cbs = (idx_t *)malloc(sizeof(idx_t) * (block->re - block->rb));
        if (!cbs) {
            printf("Error: dtw_distances_* - cannot allocate memory (cbs length = %zu)", block->re - block->rb);
            *length = 0;
            return 1;
        }
        *rls = (idx_t *)malloc(sizeof(idx_t) * (block->re - block->rb));
        if (!rls) {
            printf("Error: dtw_distances_* - cannot allocate memory (rls length = %zu)", block->re - block->rb);
            *length = 0;
            return 1;
        }
        ir = 0;
        rs = 0;
        assert(block->rb < block->re);
        for (idx_t r=block->rb; r<block->re; r++) {
            if (r + 1 > block->cb) {
                cb = r+1;
            } else {
                cb = block->cb;
            }
            (*cbs)[ir] = cb;
            (*rls)[ir] = rs;
            rs += block->ce - cb;
            ir += 1;
        }
    } else { // triu=false
        *cbs = NULL;
        *rls = NULL;
    }
    return 0;
}

/*
MPI_INIT,MPI_FINALIZE happen at main

ptrs: list of pointers to series
nb_ptrs - number of series // num_vect
array lengths - list of lengths of each series // int size_vect
output - preallocated array to store results
block - DTWBlock defining which part of the distance matrix to compute
settings - DTWSettings with parameters for DTW computation

*/
idx_t dtw_distances_ptrs_parallel_MS(seq_t **ptrs, idx_t nb_ptrs, idx_t* lengths,
                          seq_t* output, DTWBlock* block, DTWSettings* settings) {

    int i;
    int my_rank;     	       // process id
    int proc_n;                // number of processes (np at command line)
    int kill_msg;              // number of sended killl messages to slaves (initialized with np-1 slaves)
    int next_task;             // next task to be processed in bag
    MPI_Status status;         // MPI_RECV/PROBE struct with return status
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // get process id (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);     // get number of active processes

    idx_t r, c, r_i, c_i;
    idx_t length;
    idx_t *cbs, *rls;

    if ( my_rank == 0 )    // what is my role, master or slave?
        {
        // I am the master!
        if (dtw_distances_prepare_MS(block, nb_ptrs, nb_ptrs, &cbs, &rls, &length, settings) != 0) {
            return 0;
        }

        // malloc?

        /*
        DTW runs in a matriz
    [[1,0,1,3]
     [2,4,1,1]
     [2,4,1,0]]
     -->
     bags for DTW :
     - s1 s2
     - s1 s3
     - s2 s3
        */
        
        int num_tasks = (nb_ptrs * nb_ptrs - nb_ptrs)/2; // total number of (r,c) pairs excluding (r,r)
        printf("\nMaster[%d]: bag with %d tasks of size %ld.", my_rank, num_tasks, lengths[0]);
        fflush(stdout);

        // Allocate task list
        int (*tasks)[2] = malloc(num_tasks * sizeof *tasks);
        int t = 0;
        for (int r_i=0; r_i < (block->re - block->rb); r_i++) {
            r = block->rb + r_i;
            if (block->triu)
                c = cbs[r_i];
            else
                c = block->cb;
            for (; c < block->ce; c++) {
                tasks[t][0] = r;
                tasks[t][1] = c;
                t++;
            }
        }
        // send first round of work to the slaves
        next_task = 0;
        // adicionar verificaçao se o numero de processos é maior que o numero de tasks
        for ( i = 1 ; i < proc_n ; i++ )  // begin with first slave (process 1, since master is 0)
        {
            MPI_Send(tasks[next_task], 2, MPI_INT, i, WORKTAG, MPI_COMM_WORLD);
            next_task++;
            #if VERBOSE
                printf("\nMaster[%d]: sending new work (task %d) to slave %d with positions [%d,%d].", my_rank, next_task-1, i, tasks[next_task-1][0], tasks[next_task-1][1]);
                fflush(stdout);
            #endif
        }

        // wait for results from slaves
        kill_msg = proc_n-1;            // will send np-1 kill messages before job is done

        while ( kill_msg > 0 )  // continue while kill_msg kill messages are not send
            {
            // receive dtw result independently from source and tag
            double result[3];
            MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );  // probe first message to read it to the right bag position
            
            // sera que 3 é tamanho suficiente?
            MPI_Recv( result, 3, MPI_DOUBLE, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status );
            
            #if VERBOSE
			    printf("\nMaster[%d]: message received from slave %d [%f][%f] with value [%f].", my_rank, status.MPI_SOURCE,
                    result[0], result[1], result[2]);
                fflush(stdout);
            #endif

            // Store result in output
            r = (idx_t) result[0];
            c = (idx_t) result[1];

            // Same indexing as OpenMP version
            if (block->triu) {
                r_i = r - block->rb;
                c_i = c - cbs[r_i];
                //printf("\nStoring at position %ld (r_i=%d, c_i=%d, r=%ld, c=%ld, cb=%ld) value %f", rls[r_i] + c_i, r_i, c_i, r, c, cbs[r_i], result[2]);
                output[rls[r_i] + c_i] = result[2];
            } else {
                r_i = r - block->rb;
                c_i = c - block->cb;
                output[(block->ce - block->cb) * r_i + c_i] = result[2];
            }

            if ( next_task < num_tasks )
                {
                // still some work to do, send it to the free slave
                MPI_Send( tasks[next_task], 2, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD );
                #if VERBOSE
                    printf("\nMaster[%d]: sending new work (task %d) to slave %d with positions [%d,%d].", my_rank, next_task, status.MPI_SOURCE, tasks[next_task][0], tasks[next_task][1]);
                    fflush(stdout);
                #endif
                next_task++;   
                }
            else
                {
                // no more work to do, send a kill to the free slave
                MPI_Send( NULL, 0, MPI_INT, status.MPI_SOURCE, KILLTAG, MPI_COMM_WORLD );
                kill_msg--; // decrease number of kill messages to be sent

                #if VERBOSE
                    printf("\nMaster[%d]: sending KILLTAG to slave %d. Pending kill %d processes", my_rank, status.MPI_SOURCE, kill_msg);
                #endif
            }
        }

        free(tasks);

    } else {
        // I am the slave!
        int task_counter = 0;
        int (*message) = malloc (2 * sizeof(int));     // slave message buffer

        while(1)
            {
            MPI_Recv(message, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);    // receive task

            if (status.MPI_TAG == KILLTAG)
                {
                #if VERBOSE
                    printf("\nSlave[%d]: kill message revceived from master. Bye bye!\n", my_rank );
                    fflush(stdout);
                #endif
                return 0;

                }
            else if (status.MPI_TAG == WORKTAG)
                {
                // got work to do!
                r = message[0];
                c = message[1];
                double value = dtw_distance(ptrs[r], lengths[r],
                                            ptrs[c], lengths[c], settings);
                // send result back to master
                double result[3];
                result[0] = r;
                result[1] = c;
                result[2] = value;

                #if VERBOSE
                    printf("\nSlave[%d]: task [%d] sending result", my_rank, task_counter);
                    fflush(stdout);
                #endif
                MPI_Send(&result, 3, MPI_DOUBLE, 0, RESULTTAG, MPI_COMM_WORLD);
                task_counter++;
                }
            else
                {
                printf("\nSlave[%d]: unknown message tag %d received from master!", my_rank, status.MPI_TAG);
                fflush(stdout);
                }
            }

        printf("\n\n");
    }
    //MPI_Barrier(MPI_COMM_WORLD); // wait for all processes to reach this point
    MPI_Finalize();  // finalize MPI environment
    return 1;
}
