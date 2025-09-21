// create by
// modified by Daniela Rigoli at June 2025


#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "dd_dtw.h"
#include "dd_dtw_openmp.h"
#include "assets/load_from_csv.h"
#include "assets/call_aggregation.h"
#include "assets/aggregation.h"
#include <stdio.h>

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
void example(TickerSeries *series, int num_series, int aggregation_type, const char *file_result_destination) {
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

    printf("DTW Settings and run...\n");
    time_t start_t, end_t;
    struct timespec start, end;
    double diff_t, diff_t2;

    time(&start_t);
    clock_gettime(CLOCK_REALTIME, &start);

    DTWSettings settings = dtw_settings_default();
    DTWBlock block = dtw_block_empty();
    dtw_distances_ptrs_parallel(s, num_series, lengths, result, &block, &settings);

    time(&end_t);
    clock_gettime(CLOCK_REALTIME, &end);
    diff_t = difftime(end_t, start_t);
    diff_t2 = ((double)end.tv_sec * 1e9 + end.tv_nsec) - ((double)start.tv_sec * 1e9 + start.tv_nsec);
    printf("Execution time = %f sec = %f ms\n", diff_t, diff_t2 / 1000000);

    if (aggregation_type > 0) {
        run_aggregation(num_series, result, series, aggregation_type, true); // 👈 use result from memory
    }

    save_result(num_series, result, series, file_result_destination);
    printf("Result saved\n");

    free(result);
}

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <caminho_csv> <max_assets> <aggregation_type> <file_result_destination> [--reuse]\n", argv[0]);
        return 1;
    }

    // to jump to aggregation directly when we already have the result
    bool reuse = (argc >= 6 && strcmp(argv[5], "--reuse") == 0);


    const char *file_path = argv[1];
    int max_assets = atoi(argv[2]);
    int aggregation = atoi(argv[3]);
    const char *result_file = argv[4];

    printf("Max OpenMP threads = %d\n", omp_get_max_threads());

    if (is_openmp_supported()) {
        printf("OpenMP is supported\n");
    } else {
        printf("OpenMP is not supported\n");
    }

    printf("Loading time series...\n");
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
    printf("Loaded %d time series\n", num_series);

    if (reuse) {
        printf("Reusing existing DTW result for aggregation.\n"); 

        idx_t result_length = num_series * (num_series - 1) / 2;
        double *result = malloc(sizeof(double) * result_length);
        if (!result) {
            printf("Error allocating memory for result.\n");
            return 1;
        }

        run_aggregation(num_series, result, series, aggregation, false);  // result not computed yet
        free(result);
    } else {
        example(series, num_series, aggregation, result_file);
    }


    free(series);
    return 0;
}