// DTW implementation created by 2025 Meert Wannes
// modified by Daniela Rigoli at January 2026 to add Hybrid OpenMP+MPI support
// version 2 MPI just send two messages per task

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>
#include <omp.h>

#include "dd_dtw.h"
#include "assets/load_from_csv.h"

#define WORKTAG   1
#define KILLTAG   2
#define RESULTTAG 3

typedef struct {
    int len_r;
    double *r;

    int len_c;
    double *c;
} Task;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (argc < 5) {
        if (rank == 0)
            printf("Usage: %s <csv> <max_assets> <batch_size> <output>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    const char *csv_path = argv[1];
    int max_assets = atoi(argv[2]);
    int BATCH_SIZE = atoi(argv[3]);
    const char *result_file = argv[4];

    DTWSettings settings = dtw_settings_default();

    double start = MPI_Wtime();

    /**************** MASTER ****************/
    if (rank == 0) {
        printf("Max OpenMP threads = %d\n", omp_get_max_threads());

        printf("MASTER: loading CSV...\n");
        TickerSeries *series = malloc(sizeof(TickerSeries) * max_assets);
        int num_series = 0;

        load_series_from_csv(csv_path, series, &num_series, max_assets);
        
        double *s[num_series];
        int lengths[num_series];

        for (int i = 0; i < num_series; i++) {
            s[i] = series[i].close;
            lengths[i] = series[i].count;
        }

        int total_tasks = num_series * (num_series - 1) / 2;

        int (*tasks)[2] = malloc(sizeof(int[2]) * total_tasks);
        int t = 0;
        for (int r = 0; r < num_series; r++)
            for (int c = r + 1; c < num_series; c++)
                tasks[t][0] = r, tasks[t][1] = c, t++;

        float *result = calloc(total_tasks, sizeof(float));

        int *last_send = malloc(sizeof(int) * nprocs);
        for (int i = 0; i < nprocs; i++) last_send[i] = -1;

        int next_task = 0;

        // initial distribution
        for (int p = 1; p < nprocs && next_task < total_tasks; p++) {
            int batch = (total_tasks - next_task < BATCH_SIZE)
                        ? (total_tasks - next_task) : BATCH_SIZE;

            size_t bytes = sizeof(int); // batch header

            for (int b = 0; b < batch; b++) {
                int r = tasks[next_task + b][0];
                int c = tasks[next_task + b][1];
                bytes += sizeof(int) + lengths[r]*sizeof(double);
                bytes += sizeof(int) + lengths[c]*sizeof(double);
            }

            char *buf = malloc(bytes);
            size_t pos = 0;

            memcpy(buf + pos, &batch, sizeof(int)); pos += sizeof(int);

            for (int b = 0; b < batch; b++) {
                int r = tasks[next_task + b][0];
                int c = tasks[next_task + b][1];

                memcpy(buf+pos, &lengths[r], sizeof(int)); pos += sizeof(int);
                memcpy(buf+pos, s[r], lengths[r]*sizeof(double)); pos += lengths[r]*sizeof(double);

                memcpy(buf+pos, &lengths[c], sizeof(int)); pos += sizeof(int);
                memcpy(buf+pos, s[c], lengths[c]*sizeof(double)); pos += lengths[c]*sizeof(double);
            }

            MPI_Send(buf, pos, MPI_BYTE, p, WORKTAG, MPI_COMM_WORLD);

            last_send[p] = next_task;
            next_task += batch;
            free(buf);
        }

        MPI_Status status;
        int alive = nprocs - 1;

        while (alive > 0) {
            MPI_Probe(MPI_ANY_SOURCE, RESULTTAG, MPI_COMM_WORLD, &status);

            int src = status.MPI_SOURCE;
            int count;
            MPI_Get_count(&status, MPI_FLOAT, &count);

            float *res = malloc(sizeof(float)*count);
            MPI_Recv(res, count, MPI_FLOAT, src, RESULTTAG, MPI_COMM_WORLD, &status);

            int start_idx = last_send[src];
            for (int i = 0; i < count; i++)
                result[start_idx + i] = res[i];

            free(res);

            if (next_task < total_tasks) {
                int batch = (total_tasks - next_task < BATCH_SIZE)
                            ? (total_tasks - next_task) : BATCH_SIZE;

                size_t bytes = sizeof(int);
                for (int b = 0; b < batch; b++) {
                    int r = tasks[next_task + b][0];
                    int c = tasks[next_task + b][1];
                    bytes += sizeof(int) + lengths[r]*sizeof(double);
                    bytes += sizeof(int) + lengths[c]*sizeof(double);
                }

                char *buf = malloc(bytes);
                size_t pos = 0;

                memcpy(buf+pos, &batch, sizeof(int)); pos += sizeof(int);

                for (int b = 0; b < batch; b++) {
                    int r = tasks[next_task + b][0];
                    int c = tasks[next_task + b][1];

                    memcpy(buf+pos, &lengths[r], sizeof(int)); pos += sizeof(int);
                    memcpy(buf+pos, s[r], lengths[r]*sizeof(double)); pos += lengths[r]*sizeof(double);

                    memcpy(buf+pos, &lengths[c], sizeof(int)); pos += sizeof(int);
                    memcpy(buf+pos, s[c], lengths[c]*sizeof(double)); pos += lengths[c]*sizeof(double);
                }

                MPI_Send(buf, pos, MPI_BYTE, src, WORKTAG, MPI_COMM_WORLD);

                last_send[src] = next_task;
                next_task += batch;

                free(buf);
            } else {
                MPI_Send(NULL, 0, MPI_INT, src, KILLTAG, MPI_COMM_WORLD);
                alive--;
            }
        }

        printf("Time: %f sec\n", MPI_Wtime() - start);

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
    }

    /**************** SLAVE ****************/
    else {
        MPI_Status status;

        while (1) {
            MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (status.MPI_TAG == KILLTAG) {
                MPI_Recv(NULL, 0, MPI_INT, 0, KILLTAG, MPI_COMM_WORLD, &status);
                break;
            }

            int count;
            MPI_Get_count(&status, MPI_BYTE, &count);

            char *buf = malloc(count);
            MPI_Recv(buf, count, MPI_BYTE, 0, WORKTAG, MPI_COMM_WORLD, &status);

            size_t pos = 0;
            int batch;
            memcpy(&batch, buf+pos, sizeof(int)); pos += sizeof(int);
            Task *tasks = malloc(sizeof(Task) * batch);

            for (int b = 0; b < batch; b++) {
                int len_r;
                memcpy(&len_r, buf + pos, sizeof(int));
                pos += sizeof(int);

                double *r = (double *)(buf + pos);  // 👈 pointer INTO buffer
                pos += sizeof(double) * len_r;

                int len_c;
                memcpy(&len_c, buf + pos, sizeof(int));
                pos += sizeof(int);

                double *c = (double *)(buf + pos);  // 👈 pointer INTO buffer
                pos += sizeof(double) * len_c;

                tasks[b].len_r = len_r;
                tasks[b].r = r;
                tasks[b].len_c = len_c;
                tasks[b].c = c;
            }

            /* -------------------------------
            * STEP 2: parallel compute
            * ------------------------------- */
            float *results = malloc(sizeof(float) * batch);

            #pragma omp parallel for schedule(dynamic)
            for (int b = 0; b < batch; b++) {
                results[b] = (float) dtw_distance(
                    tasks[b].r, tasks[b].len_r,
                    tasks[b].c, tasks[b].len_c,
                    &settings
                );
            }

            /* -------------------------------
            * SEND BACK
            * ------------------------------- */
            MPI_Send(results, batch, MPI_FLOAT, 0, RESULTTAG, MPI_COMM_WORLD);

            free(results);
            free(tasks);
            free(buf);
        }
    }

    MPI_Finalize();
    return 0;
}