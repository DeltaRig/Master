// =======================================================
// Sequential DTW Distance Computation
// Computes DTW for all pairs of time series
// Saves: distance, time(ms), len_r, len_c
// Modified by Daniela Rigoli - 2025
// =======================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include "dd_dtw.h"
#include "assets/load_from_csv.h"

// =======================================================
// Save extended results
// =======================================================
bool save_result_extended(
        int n,
        double *result,
        double *time_ms,
        int *len_r_arr,
        int *len_c_arr,
        TickerSeries *series_list,
        const char *filename)
{
    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error opening file: %s\n", filename);
        return false;
    }

    int idx = 0;
    for (int r = 0; r < n; r++) {
        for (int c = r + 1; c < n; c++) {

            fprintf(fptr, "%s;%s;%f;%f;%d;%d;\n",
                series_list[r].ticker,
                series_list[c].ticker,
                result[idx],
                time_ms[idx],
                len_r_arr[idx],
                len_c_arr[idx]
            );

            idx++;
        }
    }

    fclose(fptr);
    return true;
}


// =======================================================
// Main Sequential DTW Execution
// =======================================================
int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Usage: %s <csv_file> <max_assets> <output_file>\n", argv[0]);
        return 1;
    }

    const char *csv_path     = argv[1];
    int max_assets           = atoi(argv[2]);
    const char *output_file  = argv[3];

    printf("Sequential DTW Computation\n");
    printf("Loading CSV: %s\n", csv_path);

    // =============================
    // Load time series
    // =============================
    TickerSeries *series = malloc(sizeof(TickerSeries) * max_assets);

    int num_series = 0;
    if (load_series_from_csv(csv_path, series, &num_series, max_assets) != 0) {
        printf("ERROR loading CSV!\n");
        free(series);
        return 1;
    }

    printf("Loaded %d time series\n", num_series);

    // pointers and lengths
    double *s[num_series];
    int lengths[num_series];

    for (int i = 0; i < num_series; i++) {
        s[i] = series[i].close;
        lengths[i] = series[i].count;
    }

    int total_pairs = num_series * (num_series - 1) / 2;

    // allocate result arrays
    double *result     = malloc(total_pairs * sizeof(double));
    double *time_ms    = malloc(total_pairs * sizeof(double));
    int *len_r_arr     = malloc(total_pairs * sizeof(int));
    int *len_c_arr     = malloc(total_pairs * sizeof(int));

    DTWSettings settings = dtw_settings_default();

    printf("Computing DTW sequentially for %d pairs...\n", total_pairs);

    // =============================
    // Sequential DTW computation
    // =============================
    int idx = 0;

    for (int r = 0; r < num_series; r++) {
        for (int c = r + 1; c < num_series; c++) {

            struct timespec t1, t2;
            clock_gettime(CLOCK_REALTIME, &t1);

            double dist = dtw_distance(
                s[r], lengths[r],
                s[c], lengths[c],
                &settings
            );

            clock_gettime(CLOCK_REALTIME, &t2);

            double ms =
                (double)(t2.tv_sec * 1e9 + t2.tv_nsec
                        - t1.tv_sec * 1e9 - t1.tv_nsec) / 1e6;

            result[idx]     = dist;
            time_ms[idx]    = ms;
            len_r_arr[idx]  = lengths[r];
            len_c_arr[idx]  = lengths[c];

            printf("(%d,%d) -> dist=%f  time=%.2f ms\n",
                   r, c, dist, ms);

            idx++;
        }
    }

    // =============================
    // SAVE TO FILE
    // =============================
    printf("Saving results to: %s\n", output_file);

    if (!save_result_extended(
            num_series,
            result,
            time_ms,
            len_r_arr,
            len_c_arr,
            series,
            output_file))
    {
        printf("ERROR saving file!\n");
    } else {
        printf("Saved OK.\n");
    }

    // cleanup
    free(result);
    free(time_ms);
    free(len_r_arr);
    free(len_c_arr);
    free(series);

    return 0;
}
