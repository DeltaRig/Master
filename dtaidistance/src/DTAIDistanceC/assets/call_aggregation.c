#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "call_aggregation.h"
#include "aggregation.h"
#include "load_from_csv.h"

void run_aggregation(int num_series, double *result, TickerSeries *series, int aggregation_type, bool result_already_computed) {
    if (!result_already_computed) {
        printf("Loading DTW result from file...\n");
        if (!load_result_from_csv("dtw_result.csv", result, num_series)) {
            printf("Error: failed to load dtw_result.csv.\n");
            return;
        }
    }

    switch (aggregation_type) {
        case 1: {
            int k = 6;
            printf("Running K-Medoids aggregation (k = %d).\n", k);
            aggregate_kmedoids(num_series, result, series, k);
            break;
        }
        case 2: {
            float eps = 200.0;
            int minPts = 2;
            printf("Running DBSCAN aggregation (eps = %.2f, minPts = %d).\n", eps, minPts);
            dbscan(num_series, result, series, eps, minPts);
            break;
        }
        case 3: {
            int k = 6;
            printf("Running Hierarchical Clustering (k = %d).\n", k);
            hierarchical_clustering(num_series, result, series, k);
            break;
        }
        default:
            printf("Unknown aggregation type: %d\n", aggregation_type);
            return;
    }

    printf("Aggregation complete.\n");
}
