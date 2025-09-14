//
//  example.c
//  DTAIDistanceC
//
//  Copyright © 2025 Wannes Meert.
//  Apache License, Version 2.0, see LICENSE for details.

#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <string.h>

#include "dd_dtw.h"
#include "dd_dtw_openmp.h"

void example() {
    if (is_openmp_supported()) {
        printf("OpenMP is supported\n");
    } else {
        printf("OpenMP is not supported\n");
    }
    int n = 6;
    double s1[] = {4., 2, 4, 4, 4, 4, 4, 0, 0}; // 0
    double s2[] = {0., 1, 2, 0, 0, 0, 0, 0, 0}; // 1
    double s3[] = {1., 0, 1, 1, 1, 1, 1, 1, 1}; // 2
    double s4[] = {0., 1, 1, 1, 1, 1, 1, 1, 1}; // 3
    double s5[] = {0., 1, 2, 0, 0, 1, 0, 0, 0}; // 4
    double s6[] = {1., 2, 0, 0, 0, 0, 0, 1, 1}; // 5
    double *s[] = {s1, s2, s3, s4, s5, s6};

    


    idx_t lengths[n];
    for (int i=0; i<n; i++) {
        lengths[i] = 7;
    }
    idx_t result_length = n*(n-1) / 2;
    double *result = (double *)malloc(sizeof(double) * result_length);
    if (!result) {
        printf("Error: cannot allocate memory for result (size=%zu)\n", result_length);
        return;
    }

    DTWSettings settings = dtw_settings_default();
    DTWBlock block = dtw_block_empty();
    dtw_distances_ptrs_parallel(s, n, lengths, result, &block, &settings);

    int idx = 0;
    for (int r=0; r<n; r++) {
        for (int c=r+1; c<n; c++) {
            printf("dist[%i,%i] = %f\n", r, c, result[idx]);
            idx++;
        }
    }

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
