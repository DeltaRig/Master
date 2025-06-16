// create by
// modified by Daniela Rigoli at June 2025


#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "dd_dtw.h"
#include "dd_dtw_openmp.h"
#include "load_series_from_csv.h"
#include <stdio.h>

#define MAX_TICKERS_TO_USE 5

// n is the number of time series
bool save_result(int n, double *result, TickerSeries *series_list){
    FILE *fptr;
    fptr = fopen("dtw_result.txt", "w");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1; // Indicate an error
    }

    int idx = 0;
    for (int r=0; r<n; r++) {
        for (int c=r+1; c<n; c++) {
            fprintf(fptr, "%s %s = %f\n", series_list[r].ticker, series_list[c].ticker, result[idx++]);
            idx++;
        }
    }

    fclose(fptr);
    return 0;
}

void example() {
    if (is_openmp_supported()) {
        printf("OpenMP is supported\n");
    } else {
        printf("OpenMP is not supported\n");
    }

    // load time series 
    printf("Loading time series...\n");
    TickerSeries series[MAX_TICKERS];
    int num_series = 0;
    load_series_from_csv("/home/dani/Documents/git/Master/dados/bdrs.csv", series, &num_series);
    printf("Loaded %d time series\n", num_series);

    double *s[MAX_TICKERS];
    idx_t lengths[MAX_TICKERS];

    for (int i = 0; i < num_series; i++) {
        s[i] = series[i].close;
        lengths[i] = series[i].count;
    }

    idx_t result_length = num_series * (num_series - 1) / 2;
    double *result = (double *)malloc(sizeof(double) * result_length);
    if (!result) {
        printf("Error: cannot allocate memory for result (size=%zu)\n", result_length);
        return;
    }

    printf("DTW Settings and run...\n");
    DTWSettings settings = dtw_settings_default();
    DTWBlock block = dtw_block_empty();
    dtw_distances_ptrs_parallel(s, num_series, lengths, result, &block, &settings);
    printf("DTW Completed\n");

    save_result(num_series, result, series);
    printf("Result saved\n");

    free(result);
}


int main(int argc, const char * argv[]) {
    printf("Run example ...\n");
    time_t start_t, end_t;
    struct timespec start, end;
    double diff_t, diff_t2;
    time(&start_t);
    clock_gettime(CLOCK_REALTIME, &start);

    example();

    time(&end_t);
    clock_gettime(CLOCK_REALTIME, &end);
    diff_t = difftime(end_t, start_t);
    diff_t2 = ((double)end.tv_sec*1e9 + end.tv_nsec) - ((double)start.tv_sec*1e9 + start.tv_nsec);
    printf("Execution time = %f sec = %f ms\n", diff_t, diff_t2/1000000);

    return 0;
}