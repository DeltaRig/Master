// create by
// modified by Daniela Rigoli at June 2025

#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>
#include "dd_dtw.h"
#include "dd_dtw_mpi.h"
#include <mpi.h>
#include "assets/load_from_csv.h"
#include <stdio.h>

/* tags */
#define WORKTAG 1
#define KILLTAG 2
#define RESULTTAG 3

#define VERBOSE 0

int dtw_distances_prepare(DTWBlock *block, idx_t nb_series_r, idx_t nb_series_c, idx_t **cbs, idx_t **rls, idx_t *length, DTWSettings *settings) {
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
        for (idx_t r = block->rb; r < block->re; r++) {
            if (r + 1 > block->cb) {
                cb = r + 1;
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

// n is the number of time series
bool save_result(int n, double *result, TickerSeries *series_list, const char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1; // Indicate an error
    }
    int idx = 0;
    for (int r = 0; r < n; r++) {
        for (int c = r + 1; c < n; c++) {
            fprintf(fptr, "%s; %s; %f;\n", series_list[r].ticker, series_list[c].ticker, result[idx++]);
        }
    }
    fclose(fptr);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        // expecting 5 or 6 arguments
        fprintf(stderr, "Uso: %s <caminho_csv> <max_assets> <openmp/mpi> <aggregation_type> <file_result_destination> [--reuse]\n", argv[0]);
        fprintf(stderr, "<openmp/mpi>: 0 to use OpenMP, 1 to use MPI version 1, 2 to use MPI version 2\n");
        fprintf(stderr, "<aggregation_type> don't run aggregation if <= 0\n");
        fprintf(stderr, "[--reuse] optional flag to reuse existing DTW result for aggregation\n");
        fprintf(stderr, "Example: %s data/prices.csv 100 openmp 1 results/dtw_result.csv --reuse\n", argv[0]);
        return 1;
    }

    // to jump to aggregation directly when we already have the result
    //bool reuse = (argc == 7 && strcmp(argv[6], "--reuse") == 0);

    const char *file_path = argv[1];
    int max_assets = atoi(argv[2]);
    //int parallel_type = atoi(argv[3]); // 0 for openmp, 1 for mpi
    //int aggregation = atoi(argv[4]);
    const char *result_file = argv[5];

    #if VERBOSE
      printf("Max OpenMP threads = %d\n", omp_get_max_threads());
    #endif

    MPI_Init(&argc, &argv);

    // start all variables for dtw distances ptrs parallel
    int i;
    int my_rank;     	       // process id
    int proc_n;                // number of processes (np at command line)
    int kill_msg;              // number of sended killl messages to slaves (initialized with np-1 slaves)
    int next_task;             // next task to be processed in bag
    MPI_Status status;         // MPI_RECV/PROBE struct with return status
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // get process id (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    idx_t r, c, r_i, c_i;
    idx_t length;
    idx_t *cbs, *rls;

    time_t start_t, end_t;
    struct timespec start, end;
    double diff_t, diff_t2;

    time(&start_t);
    clock_gettime(CLOCK_REALTIME, &start);


    DTWSettings settings = dtw_settings_default();
    MPI_Barrier(MPI_COMM_WORLD); // Espera todos chegarem aqui

    if (my_rank == 0) {
        #if VERBOSE
          printf("Master[%d]: loading time series...\n", my_rank);
        #endif
        TickerSeries *series = malloc(sizeof(TickerSeries) * max_assets);
        if (!series) {
            fprintf(stderr, "Erro: não foi possível alocar memória para séries\n");
            return 1;
        }

        int num_series = 0;
        if (load_series_from_csv(file_path, series, &num_series, max_assets) != 0) {
            fprintf(stderr, "Erro ao carregar CSV\n");
            free(series);
            return 1;
        }
        #if VERBOSE
          printf("Loaded %d time series\n", num_series);
        #endif  

        // example code
        double *s[num_series];
        idx_t lengths[num_series];

        for (int i = 0; i < num_series; i++) {
            s[i] = series[i].close;
            lengths[i] = series[i].count;
        }

        idx_t result_length = num_series * (num_series - 1) / 2;
        double *result = malloc(sizeof(double) * result_length);
        if (!result) {
            printf("Error: cannot allocate memory for result (size=%zu)\n", result_length);
            return 1;
        }

        #if VERBOSE
          printf("DTW Settings and run...\n");
        #endif
        DTWBlock block = dtw_block_empty();

    
        if (dtw_distances_prepare_MS(&block, num_series, num_series, &cbs, &rls, &length, &settings) != 0) {
            return 0;
        }

        int num_tasks = (num_series * num_series - num_series) / 2; // total number of (r,c) pairs excluding (r,r)
        #if VERBOSE
            printf("\nMaster[%d]: bag with %d tasks of size %ld.", my_rank, num_tasks, lengths[0]);
            fflush(stdout);
        #endif


        // Allocate task list
        int (*tasks)[2] = malloc(num_tasks * sizeof *tasks);
        int t = 0;
        for (int r_i = 0; r_i < (block.re - block.rb); r_i++) {
            r = block.rb + r_i;
            if (block.triu)
                c = cbs[r_i];
            else
                c = block.cb;
            for (; c < block.ce; c++) {
                tasks[t][0] = r;
                tasks[t][1] = c;
                t++;
            }
        }
        // send first round of work to the slaves
        next_task = 0;
        // adicionar verificaçao se o numero de processos é maior que o numero de tasks
        for (i = 1; i < proc_n; i++) {  // begin with first slave (process 1, since master is 0)
            // Enviar par de índices
            MPI_Send(tasks[next_task], 2, MPI_INT, i, WORKTAG, MPI_COMM_WORLD);

            // Enviar tamanhos das séries
            int len_r = lengths[tasks[next_task][0]];
            int len_c = lengths[tasks[next_task][1]];
            MPI_Send(&len_r, 1, MPI_INT, i, WORKTAG, MPI_COMM_WORLD);
            MPI_Send(&len_c, 1, MPI_INT, i, WORKTAG, MPI_COMM_WORLD);

            // Enviar os vetores de preços
            MPI_Send(s[tasks[next_task][0]], len_r, MPI_DOUBLE, i, WORKTAG, MPI_COMM_WORLD);
            MPI_Send(s[tasks[next_task][1]], len_c, MPI_DOUBLE, i, WORKTAG, MPI_COMM_WORLD);
            next_task++;
            #if VERBOSE
                printf("\nMaster[%d]: sending new work (task %d) to slave %d with positions [%d,%d].", my_rank, next_task-1, i, tasks[next_task-1][0], tasks[next_task-1][1]);
                fflush(stdout);
            #endif
        }

        // wait for results from slaves
        kill_msg = proc_n - 1; // will send np-1 kill messages before job is done

        while (kill_msg > 0) { // continue while kill_msg kill messages are not send
            // receive dtw result independently from source and tag
            double result_recv[3];
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  // probe first message to read it to the right bag position

            // sera que 3 é tamanho suficiente?
            MPI_Recv(result_recv, 3, MPI_DOUBLE, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

            #if VERBOSE
                printf("\nMaster[%d]: message received from slave %d [%f][%f] with value [%f].", my_rank, status.MPI_SOURCE,
                    result_recv[0], result_recv[1], result_recv[2]);
                fflush(stdout);
            #endif

            // Store result in output
            r = (idx_t) result_recv[0];
            c = (idx_t) result_recv[1];

            // Same indexing as OpenMP version
            if (block.triu) {
                r_i = r - block.rb;
                c_i = c - cbs[r_i];
                result[rls[r_i] + c_i] = result_recv[2];
            } else {
                r_i = r - block.rb;
                c_i = c - block.cb;
                result[(block.ce - block.cb) * r_i + c_i] = result_recv[2];
            }

            if (next_task < num_tasks) {
                // still some work to do, send it to the free slave
                // enviar par de índices
                MPI_Send(tasks[next_task], 2, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);

                // Enviar tamanhos das séries
                int len_r = lengths[tasks[next_task][0]];
                int len_c = lengths[tasks[next_task][1]];
                MPI_Send(&len_r, 1, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
                MPI_Send(&len_c, 1, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);

                // Enviar os vetores de preços
                MPI_Send(s[tasks[next_task][0]], len_r, MPI_DOUBLE, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
                MPI_Send(s[tasks[next_task][1]], len_c, MPI_DOUBLE, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
                #if VERBOSE
                    printf("\nMaster[%d]: sending new work (task %d) to slave %d with positions [%d,%d].", my_rank, next_task, status.MPI_SOURCE, tasks[next_task][0], tasks[next_task][1]);
                    fflush(stdout);
                #endif
                next_task++;
            } else {
                // no more work to do, send a kill to the free slave
                MPI_Send(NULL, 0, MPI_INT, status.MPI_SOURCE, KILLTAG, MPI_COMM_WORLD);
                kill_msg--; // decrease number of kill messages to be sent

                #if VERBOSE
                    printf("\nMaster[%d]: sending KILLTAG to slave %d. Pending kill %d processes", my_rank, status.MPI_SOURCE, kill_msg);
                #endif
            }
        }
        free(tasks);

        time(&end_t);
        clock_gettime(CLOCK_REALTIME, &end);
        diff_t = difftime(end_t, start_t);
        diff_t2 = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec);
        printf("Execution time = %f sec = %f ms\n", diff_t, diff_t2 / 1000000);

        save_result(num_series, result, series, result_file);

        #if VERBOSE
          printf("Result saved\n");
        #endif
        free(result);
        free(series);

    } else {
        // I am the slave!
        int task_counter = 0;
        int (*message) = malloc(2 * sizeof(int)); // slave message buffer

        while (1) {
            MPI_Recv(message, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // receive task

            if (status.MPI_TAG == KILLTAG) {
                #if VERBOSE
                    printf("\nSlave[%d]: kill message revceived from master. Bye bye!\n", my_rank);
                    fflush(stdout);
                #endif
                return 0;
            } else if (status.MPI_TAG == WORKTAG) {
                // Receber tamanhos
                int len_r, len_c;
                MPI_Recv(&len_r, 1, MPI_INT, 0, WORKTAG, MPI_COMM_WORLD, &status);
                MPI_Recv(&len_c, 1, MPI_INT, 0, WORKTAG, MPI_COMM_WORLD, &status);

                // Alocar buffers temporários
                double *series_r = malloc(len_r * sizeof(double));
                double *series_c = malloc(len_c * sizeof(double));

                // Receber dados das séries
                MPI_Recv(series_r, len_r, MPI_DOUBLE, 0, WORKTAG, MPI_COMM_WORLD, &status);
                MPI_Recv(series_c, len_c, MPI_DOUBLE, 0, WORKTAG, MPI_COMM_WORLD, &status);

                double value = dtw_distance(series_r, len_r, series_c, len_c, &settings);

                // Liberar buffers
                free(series_r);
                free(series_c);

                // send result back to master

                #if VERBOSE
                    printf("\nSlave[%d]: task [%d] sending result", my_rank, task_counter);
                    fflush(stdout);
                #endif
                // Enviar resultado ao mestre
                double result_send[3] = { (double) message[0], (double) message[1], value };
                MPI_Send(result_send, 3, MPI_DOUBLE, 0, RESULTTAG, MPI_COMM_WORLD);
                task_counter++;
            } else {
                printf("\nSlave[%d]: unknown message tag %d received from master!", my_rank, status.MPI_TAG);
                fflush(stdout);
            }
        }
        printf("\n\n");
    }






    MPI_Finalize();
    return 0;
}