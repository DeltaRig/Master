/******************************************************
 * MPI DTW VERSION 3 — FIX 3 (real contiguous sendbuf)
 * Master/Slave using batches; master builds contiguous buffer
 * and sends single MPI_BYTE message per batch.
 *
 * Usage:
 *   mpirun -np <N> ./example_mpi <csv_path> <max_assets> <batch_size> <result_file>
 *
 * Notes:
 *  - Requires dd_dtw.h + assets/load_from_csv.h from your project.
 *  - BATCH size from argv[3]
 *  - Master rank = 0, slaves = 1..N-1
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

#include "dd_dtw.h"            // dtw_distance, DTWSettings, ...
#include "assets/load_from_csv.h" // load_series_from_csv, TickerSeries

#define WORKTAG   1
#define KILLTAG   2
#define RESULTTAG 3

#define VERBOSE 0

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    typedef struct {
        int len_r;
        int len_c;
    } TaskHeader;

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc < 5) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <csv_path> <max_assets> <batch_size> <result_file>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    double start_time, end_time;

    // Record the start time
    start_time = MPI_Wtime();


    const char *csv_path = argv[1];
    int max_assets = atoi(argv[2]);
    int BATCH_SIZE = atoi(argv[3]);
    const char *result_file = argv[4];

    DTWSettings settings = dtw_settings_default();

    if (rank == 0) {
        /**************** MASTER ****************/
        printf("MASTER: loading CSV...\n");
        TickerSeries *series = malloc(sizeof(TickerSeries) * max_assets);
        if (!series) {
            fprintf(stderr, "MASTER: memory error\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        int num_series = 0;
        if (load_series_from_csv(csv_path, series, &num_series, max_assets) != 0) {
            fprintf(stderr, "MASTER: error loading CSV\n");
            free(series);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        printf("Loaded %d series.\n", num_series);

        /* prepare pointers and lengths */
        double *s[num_series];
        int lengths[num_series];
        for (int i = 0; i < num_series; i++) {
            s[i] = series[i].close;
            lengths[i] = series[i].count;
        }

        /* build tasks (upper triangular pairs) */
        int total_tasks = num_series * (num_series - 1) / 2;
        int (*tasks)[2] = malloc(sizeof(int[2]) * total_tasks);
        int t = 0;
        for (int r = 0; r < num_series; r++) {
            for (int c = r + 1; c < num_series; c++) {
                tasks[t][0] = r;
                tasks[t][1] = c;
                t++;
            }
        }

        /* output buffer (float results) */
        float *result = calloc(total_tasks, sizeof(float));
        if (!result) { fprintf(stderr, "MASTER: cannot alloc result\n"); MPI_Abort(MPI_COMM_WORLD,1); }

        /* last_send map: for each slave, index of first task of its last batch */
        int *last_send = malloc(sizeof(int) * nprocs);
        for (int i = 0; i < nprocs; i++) last_send[i] = -1;

        int next_task = 0;
        /* send initial batch to each slave */
        for (int p = 1; p < nprocs && next_task < total_tasks; p++) {
            int batch_count = (total_tasks - next_task < BATCH_SIZE) ? (total_tasks - next_task) : BATCH_SIZE;

            /* compute total bytes needed for this batch */
            size_t total_bytes = 0;
            for (int b = 0; b < batch_count; b++) {
                int r_idx = tasks[next_task + b][0];
                int c_idx = tasks[next_task + b][1];
                total_bytes += sizeof(TaskHeader);                   // header (len_r, len_c)
                total_bytes += sizeof(double) * lengths[r_idx];      // series_r
                total_bytes += sizeof(double) * lengths[c_idx];      // series_c
            }

            /* allocate contiguous buffer */
            char *sendbuf = malloc(total_bytes);
            if (!sendbuf) { fprintf(stderr, "MASTER: sendbuf OOM\n"); MPI_Abort(MPI_COMM_WORLD,1); }

            /* fill buffer sequentially */
            size_t pos = 0;
            for (int b = 0; b < batch_count; b++) {
                int r_idx = tasks[next_task + b][0];
                int c_idx = tasks[next_task + b][1];
                int len_r = lengths[r_idx];
                int len_c = lengths[c_idx];

                TaskHeader h;
                h.len_r = len_r;
                h.len_c = len_c;

                memcpy(sendbuf + pos, &h, sizeof(TaskHeader)); pos += sizeof(TaskHeader);
                memcpy(sendbuf + pos, s[r_idx], sizeof(double) * len_r); pos += sizeof(double) * len_r;
                memcpy(sendbuf + pos, s[c_idx], sizeof(double) * len_c); pos += sizeof(double) * len_c;
            }

            /* send header: batch_count(int) and total_bytes (size_t as uint64) */
            int header[1];
            header[0] = batch_count;
            MPI_Send(header, 1, MPI_INT, p, WORKTAG, MPI_COMM_WORLD);

            /* send buffer length as 64-bit so 32-bit/64-bit safe */
            uint64_t tbytes = (uint64_t) total_bytes;
            MPI_Send(&tbytes, 1, MPI_UINT64_T, p, WORKTAG, MPI_COMM_WORLD);

            /* send actual bytes as a single message */
            MPI_Send(sendbuf, (int)total_bytes, MPI_BYTE, p, WORKTAG, MPI_COMM_WORLD);

            last_send[p] = next_task;
            next_task += batch_count;

            free(sendbuf);
        }

        /* dynamic loop: receive results and feed new batches until done */
        int kill_count = nprocs - 1;
        MPI_Status status;
        while (kill_count > 0) {
            /* receive result array (float[]) from any slave */
            MPI_Probe(MPI_ANY_SOURCE, RESULTTAG, MPI_COMM_WORLD, &status);
            int source = status.MPI_SOURCE;

            int count = 0;
            MPI_Get_count(&status, MPI_FLOAT, &count);
            float *batch_results = malloc(sizeof(float) * count);
            MPI_Recv(batch_results, count, MPI_FLOAT, source, RESULTTAG, MPI_COMM_WORLD, &status);

            /* write results into global result[] using last_send mapping */
            int start_task = last_send[source];
            if (start_task < 0) {
                fprintf(stderr, "MASTER: bad last_send for %d\n", source);
                start_task = 0;
            }
            for (int i = 0; i < count; i++) {
                int idx_task = start_task + i;
                if (idx_task >= 0 && idx_task < total_tasks) result[idx_task] = batch_results[i];
            }
            free(batch_results);

            /* assign next batch, or send KILLTAG */
            if (next_task < total_tasks) {
                int batch_count = (total_tasks - next_task < BATCH_SIZE) ? (total_tasks - next_task) : BATCH_SIZE;

                /* compute total bytes for batch_count */
                size_t total_bytes = 0;
                for (int b = 0; b < batch_count; b++) {
                    int r_idx = tasks[next_task + b][0];
                    int c_idx = tasks[next_task + b][1];
                    total_bytes += sizeof(TaskHeader);
                    total_bytes += sizeof(double) * lengths[r_idx];
                    total_bytes += sizeof(double) * lengths[c_idx];
                }

                char *sendbuf = malloc(total_bytes);
                if (!sendbuf) { fprintf(stderr, "MASTER: sendbuf OOM2\n"); MPI_Abort(MPI_COMM_WORLD,1); }

                size_t pos = 0;
                for (int b = 0; b < batch_count; b++) {
                    int r_idx = tasks[next_task + b][0];
                    int c_idx = tasks[next_task + b][1];
                    int len_r = lengths[r_idx];
                    int len_c = lengths[c_idx];

                    TaskHeader h;
                    h.len_r = len_r;
                    h.len_c = len_c;

                    memcpy(sendbuf + pos, &h, sizeof(TaskHeader)); pos += sizeof(TaskHeader);
                    memcpy(sendbuf + pos, s[r_idx], sizeof(double)*len_r); pos += sizeof(double)*len_r;
                    memcpy(sendbuf + pos, s[c_idx], sizeof(double)*len_c); pos += sizeof(double)*len_c;
                }

                /* header */
                int header[1];
                header[0] = batch_count;
                MPI_Send(header, 1, MPI_INT, source, WORKTAG, MPI_COMM_WORLD);

                uint64_t tbytes = (uint64_t) total_bytes;
                MPI_Send(&tbytes, 1, MPI_UINT64_T, source, WORKTAG, MPI_COMM_WORLD);

                /* send bytes */
                MPI_Send(sendbuf, (int)total_bytes, MPI_BYTE, source, WORKTAG, MPI_COMM_WORLD);

                last_send[source] = next_task;
                next_task += batch_count;

                free(sendbuf);
            } else {
                /* no more work */
                MPI_Send(NULL, 0, MPI_INT, source, KILLTAG, MPI_COMM_WORLD);
                kill_count--;
            }
        } /* end dynamic loop */


        // Record the end time
        end_time = MPI_Wtime();

        // Calculate and print the elapsed time for each process
        printf("Process %d: Elapsed time = %f seconds\n", rank, end_time - start_time);


        /* Save results to file (ticker names) */
        FILE *fp = fopen(result_file, "w");
        if (!fp) { fprintf(stderr, "MASTER: cannot open output file\n"); }
        int idx = 0;
        for (int r = 0; r < num_series; r++) {
            for (int c = r + 1; c < num_series; c++) {
                fprintf(fp, "%s;%s;%.6f\n", series[r].ticker, series[c].ticker, result[idx++]);
            }
        }
        if (fp) fclose(fp);

        printf("MASTER: Done. Results saved to %s\n", result_file);

        free(result);
        free(last_send);
        free(tasks);
        free(series);
    } /* end master */

    else {
        /**************** SLAVE ****************/
        MPI_Status status;
        while (1) {
            /* wait header or kill */
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == KILLTAG) {
                /* receive the kill (empty) to clear the message */
                MPI_Recv(NULL, 0, MPI_INT, 0, KILLTAG, MPI_COMM_WORLD, &status);
                if (VERBOSE) printf("SLAVE %d: got KILLTAG, exiting\n", rank);
                break;
            }
            else if (status.MPI_TAG == WORKTAG) {
                /* receive header */
                int header[1];
                MPI_Recv(header, 1, MPI_INT, 0, WORKTAG, MPI_COMM_WORLD, &status);
                int batch_count = header[0];

                uint64_t tbytes;
                MPI_Recv(&tbytes, 1, MPI_UINT64_T, 0, WORKTAG, MPI_COMM_WORLD, &status);
                size_t total_bytes = (size_t) tbytes;

                /* now receive contiguous buffer */
                char *recvbuf = malloc(total_bytes);
                if (!recvbuf) { fprintf(stderr, "SLAVE %d: recvbuf OOM\n", rank); MPI_Abort(MPI_COMM_WORLD, 1); }
                MPI_Recv(recvbuf, (int)total_bytes, MPI_BYTE, 0, WORKTAG, MPI_COMM_WORLD, &status);

                /* unpack sequentially */
                size_t pos = 0;
                float *results = malloc(sizeof(float) * batch_count);
                if (!results) { fprintf(stderr, "SLAVE %d: results OOM\n", rank); MPI_Abort(MPI_COMM_WORLD,1); }

                for (int b = 0; b < batch_count; b++) {
                    /* read TaskHeader */
                    TaskHeader h;
                    memcpy(&h, recvbuf + pos, sizeof(TaskHeader)); pos += sizeof(TaskHeader);

                    double *series_r = malloc(sizeof(double) * h.len_r);
                    if (!series_r) { fprintf(stderr,"SLAVE OOM r\n"); MPI_Abort(MPI_COMM_WORLD,1); }
                    memcpy(series_r, recvbuf + pos, sizeof(double) * h.len_r); pos += sizeof(double) * h.len_r;

                    double *series_c = malloc(sizeof(double) * h.len_c);
                    if (!series_c) { fprintf(stderr,"SLAVE OOM c\n"); MPI_Abort(MPI_COMM_WORLD,1); }
                    memcpy(series_c, recvbuf + pos, sizeof(double) * h.len_c); pos += sizeof(double) * h.len_c;

                    /* compute DTW */
                    double d = dtw_distance(series_r, (idx_t)h.len_r, series_c, (idx_t)h.len_c, &settings);
                    results[b] = (float) d;

                    free(series_r);
                    free(series_c);
                }

                /* send results array back */
                MPI_Send(results, batch_count, MPI_FLOAT, 0, RESULTTAG, MPI_COMM_WORLD);

                free(results);
                free(recvbuf);
            } else {
                /* other tags (should not happen) */
                MPI_Recv(NULL, 0, MPI_INT, 0, status.MPI_TAG, MPI_COMM_WORLD, &status);
            }
        } /* end while */
    } /* end slave */
    MPI_Finalize();
    return 0;
}
