//  Copyright © 2025 Wannes Meert.
//  Apache License, Version 2.0, see LICENSE for details.
// modified by Daniela Rigoli at June 2025


#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "dd_dtw.h"
#include "dd_dtw_openmp.h"

#include "assets/load_from_csv.h"
#include <stdio.h>


#define VERBOSE 0


// n is the number of time series
bool save_result(int n, double *result, TickerSeries *series_list, const char *filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1; // Indicate an error
    }

    int idx = 0;
    for (int r=0; r<n; r++) {
        for (int c=r+1; c<n; c++) {
            fprintf(fptr, "%s; %s; %f;\n", series_list[r].ticker, series_list[c].ticker, result[idx++]);
        }
    }

    fclose(fptr);
    return 0;
}

// function to run the dtw algorithm from dtaidistance
void example(TickerSeries *series, int num_series, const char *file_result_destination, int parallel_type) {
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
        return;
    }

    #if VERBOSE
      printf("DTW Settings and run...\n");
    #endif
    time_t start_t, end_t;
    struct timespec start, end;
    double diff_t, diff_t2;

    time(&start_t);
    clock_gettime(CLOCK_REALTIME, &start);

    DTWSettings settings = dtw_settings_default();
    DTWBlock block = dtw_block_empty();

    if (parallel_type == 0) {
        // OpenMP version
        #if VERBOSE
          printf("DTW OpenMP...\n");
        #endif
        dtw_distances_ptrs_parallel_d(s, num_series, lengths, result, &block, &settings);
    } // MPI version implemented separeted


    time(&end_t);
    clock_gettime(CLOCK_REALTIME, &end);
    diff_t = difftime(end_t, start_t);
    diff_t2 = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec);
    printf("Execution time = %f sec = %f ms\n", diff_t, diff_t2 / 1000000);

    save_result(num_series, result, series, file_result_destination);
    printf("Result saved\n");

    free(result);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <csv_path> <series_quantity> <output_file>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    int max_assets = atoi(argv[2]);
    const char *result_file = argv[3];

    #if VERBOSE
      printf("Max OpenMP threads = %d\n", omp_get_max_threads());
      if (is_openmp_supported()) {
        printf("OpenMP is supported\n");
        } else {
            printf("OpenMP is not supported\n");
        }
    #endif

    #if VERBOSE
        printf("Loading time series...\n");
    #endif
    TickerSeries *series = malloc(sizeof(TickerSeries) * max_assets);
    if (!series) {
        fprintf(stderr, "Error: cannot allocate memory for series\n");
        return 1;
    }

    int num_series = 0;
    if (load_series_from_csv(file_path, series, &num_series, max_assets) != 0) {
        fprintf(stderr, "Error loading CSV\n");
        free(series);
        return 1;
    }
    #if VERBOSE
      printf("Loaded %d time series\n", num_series);
    #endif

    example(series, num_series, result_file, 0);

    free(series);
    return 0;
}