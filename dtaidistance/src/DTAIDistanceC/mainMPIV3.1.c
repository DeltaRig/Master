// create by
// modified by Daniela Rigoli at June 2025
// Version 3 is to increase batch size
// sending packed messages, with 4 packs per send

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
#define MAX_SERIES_LEN 4000
#define BUF_SIZE (int)(batch_size * (16 + 16 * MAX_SERIES_LEN) * 1.2)  // adjust depending on expected pack size // bytes_per_pair ≈ 4*4 (ints) + 8*(len_r + len_c)

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
        fprintf(stderr, "Uso: %s <caminho_csv> <max_assets> <batch_size> <non used> <file_result_destination> [--reuse]\n", argv[0]);
        fprintf(stderr, "<batch_size>: size of the batch to process (must be > 0)\n");
        fprintf(stderr, "[--reuse] optional flag to reuse existing DTW result for aggregation\n");
        fprintf(stderr, "Example: %s data/prices.csv 100 openmp 1 results/dtw_result.csv --reuse\n", argv[0]);
        return 1;
    }
    //fprintf(stderr, "starting mainMPIv3.1\n");


    const char *file_path = argv[1];
    int max_assets = atoi(argv[2]);
    // add batch size to control how many pairs to send at once
    int batch_size = atoi(argv[3]); 
    const char *result_file = argv[5];

    #if VERBOSE
      printf("Max OpenMP threads = %d\n", omp_get_max_threads());
    #endif

    MPI_Init(&argc, &argv);

    // start all variables for dtw distances ptrs parallel
    int my_rank;     	       // process id
    int proc_n;                // number of processes (np at command line)
    int kill_msg;              // number of sended killl messages to slaves (initialized with np-1 slaves)
    int next_task;             // next task to be processed in bag
    MPI_Status status;         // MPI_RECV/PROBE struct with return status
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);    // get process id (rank)
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

    idx_t r, c;
    idx_t length;
    idx_t *cbs, *rls;

    time_t start_t, end_t;
    struct timespec start, end;
    double diff_t, diff_t2;

    time(&start_t);
    clock_gettime(CLOCK_REALTIME, &start);


    DTWSettings settings = dtw_settings_default();
    

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
        int (*last_send) = malloc(proc_n * sizeof(int));
        // adicionar verificaçao se o numero de processos é maior que o numero de tasks
        for (int i = 1; i < proc_n && next_task < num_tasks; i++) {
            int batch_count = (num_tasks - next_task < batch_size)
                                ? (num_tasks - next_task)
                                : batch_size;
            last_send[i] = next_task;

            int position = 0;
            char buffer[BUF_SIZE];

            // *************** added on version 3 ***************
            // Pack the batch count // added on version 3
            MPI_Pack(&batch_count, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);

            // Pack each pair's series (length + values)
            for (int b = 0; b < batch_count; b++) {
                int r = tasks[next_task + b][0];
                int c = tasks[next_task + b][1];
                int len_r = lengths[r];
                int len_c = lengths[c];

                MPI_Pack(&len_r, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
                MPI_Pack(s[r], len_r, MPI_DOUBLE, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);

                MPI_Pack(&len_c, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
                MPI_Pack(s[c], len_c, MPI_DOUBLE, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
            }

        // Send entire packed message
        MPI_Send(buffer, position, MPI_PACKED, i, WORKTAG, MPI_COMM_WORLD);
        next_task += batch_count;

        #if VERBOSE
            printf("Master[%d]: sent batch of %d tasks starting at %d to slave %d\n",
                my_rank, batch_count, last_send[i], i);
        #endif
        }

        // wait for results from slaves
        kill_msg = proc_n - 1; // will send np-1 kill messages before job is done
        float *results_batch = malloc(batch_size * sizeof(float));

        while (kill_msg > 0) {
            int batch_count;
            MPI_Status status;
            MPI_Probe(MPI_ANY_SOURCE, RESULTTAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_FLOAT, &batch_count);
            MPI_Recv(results_batch, batch_count, MPI_FLOAT, status.MPI_SOURCE, RESULTTAG, MPI_COMM_WORLD, &status);

            int start_idx = last_send[status.MPI_SOURCE];
            #if VERBOSE
                printf("\nMaster[%d]: message received from slave %d [%d][%d] with value [%f].", my_rank, status.MPI_SOURCE,
                    tasks[last_send[status.MPI_SOURCE]][0], tasks[last_send[status.MPI_SOURCE]][1], result_recv);
                fflush(stdout);
            #endif
            for (int b = 0; b < batch_count; b++) {
                int r = tasks[start_idx + b][0];
                int c = tasks[start_idx + b][1];
                float value = results_batch[b];

                if (block.triu) {
                    int r_i = r - block.rb;
                    int c_i = c - cbs[r_i];
                    result[rls[r_i] + c_i] = value;
                } else {
                    int r_i = r - block.rb;
                    int c_i = c - block.cb;
                    result[(block.ce - block.cb) * r_i + c_i] = value;
                }
            }

            // assign next batch or send KILLTAG
            if (next_task < num_tasks) {
                // *************** added on version 3 ***************
                // Determine how many tasks will be included in the next batch, if have less than batch_size, send the remaining
                int batch_count = (num_tasks - next_task < batch_size)
                                    ? (num_tasks - next_task)
                                    : batch_size;

                last_send[status.MPI_SOURCE] = next_task;

                int position = 0;
                char buffer[BUF_SIZE];  // same BUF_SIZE as used before

                // Pack batch count
                MPI_Pack(&batch_count, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);

                // Pack the series data
                for (int b = 0; b < batch_count; b++) {
                    int r = tasks[next_task + b][0];
                    int c = tasks[next_task + b][1];
                    int len_r = lengths[r];
                    int len_c = lengths[c];

                    // lengths + series values
                    MPI_Pack(&len_r, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
                    MPI_Pack(s[r], len_r, MPI_DOUBLE, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);

                    MPI_Pack(&len_c, 1, MPI_INT, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
                    MPI_Pack(s[c], len_c, MPI_DOUBLE, buffer, BUF_SIZE, &position, MPI_COMM_WORLD);
                }

                // Send packed batch to the slave that just finished
                MPI_Send(buffer, position, MPI_PACKED, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);

                next_task += batch_count;

                #if VERBOSE
                    printf("Master[%d]: sent next batch of %d tasks (starting %d) to slave %d\n",
                        my_rank, batch_count, last_send[status.MPI_SOURCE], status.MPI_SOURCE);
                    fflush(stdout);
                #endif
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
        free(results_batch);

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
        //int task_counter = 0;
        while (1) {
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == KILLTAG) {
                #if VERBOSE
                    printf("\nSlave[%d]: kill message revceived from master. Bye bye!\n", my_rank);
                    fflush(stdout);
                #endif
                break;
            } else if (status.MPI_TAG == WORKTAG) {
                int count;
                MPI_Get_count(&status, MPI_PACKED, &count);
                char *buffer = malloc(count);
                MPI_Recv(buffer, count, MPI_PACKED, 0, WORKTAG, MPI_COMM_WORLD, &status);

                int position = 0;
                int batch_count;
                MPI_Unpack(buffer, count, &position, &batch_count, 1, MPI_INT, MPI_COMM_WORLD);

                float *results = malloc(batch_count * sizeof(float));

                for (int b = 0; b < batch_count; b++) {
                    int len_r, len_c;
                    MPI_Unpack(buffer, count, &position, &len_r, 1, MPI_INT, MPI_COMM_WORLD);
                    double *series_r = malloc(len_r * sizeof(double));
                    MPI_Unpack(buffer, count, &position, series_r, len_r, MPI_DOUBLE, MPI_COMM_WORLD);

                    MPI_Unpack(buffer, count, &position, &len_c, 1, MPI_INT, MPI_COMM_WORLD);
                    double *series_c = malloc(len_c * sizeof(double));
                    MPI_Unpack(buffer, count, &position, series_c, len_c, MPI_DOUBLE, MPI_COMM_WORLD);

                    results[b] = (float) dtw_distance(series_r, len_r, series_c, len_c, &settings);

                    free(series_r);
                    free(series_c);
                }

                #if VERBOSE
                    printf("\nSlave[%d]: task [%d] sending result", my_rank, task_counter);
                    fflush(stdout);
                #endif
                // Enviar resultado ao mestre
                // send results as one message
                MPI_Send(results, batch_count, MPI_FLOAT, 0, RESULTTAG, MPI_COMM_WORLD);

                free(buffer);
                free(results);
            } else {
                printf("\nSlave[%d] message tag %d received from master!", my_rank, status.MPI_TAG);
                fflush(stdout);
            }
        }
        printf("\n\n");
    }

    MPI_Finalize();
    return 0;
}