#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "call_aggregation.h"
#include "aggregation.h"
#include "load_from_csv.h"

void run_aggregation(int num_series, double *result_dtw, TickerSeries *series, int aggregation_type, bool result_already_computed) {
    if (!result_already_computed) {
        printf("Loading DTW result from file...\n");
        if (!load_result_from_csv("dtw_result.csv", result_dtw, num_series)) {
            printf("Error: failed to load dtw_result.csv.\n");
            return;
        }
    }

    int *labels = malloc(sizeof(int) * num_series);

    int k = 50;
    switch (aggregation_type) {
        case 1: {
            printf("Running K-Medoids aggregation (k = %d).\n", k);
            aggregate_kmedoids(num_series, result_dtw, series, k, labels);
            break;
        }
        case 2: {
            float eps = 200.0;
            int minPts = 2;
            printf("Running DBSCAN aggregation (eps = %.2f, minPts = %d).\n", eps, minPts);
            dbscan(num_series, result_dtw, series, eps, minPts, labels);
            break;
        }
        case 3: {
            printf("Running Hierarchical Clustering (k = %d).\n", k);
            hierarchical_clustering(num_series, result_dtw, series, k, labels);
            break;
        }
        default:
            printf("Unknown aggregation type: %d\n", aggregation_type);
            return;
    }


    // Output
//    save_cluster_labels_csv("dtw_clusters.csv", num_series, series, labels, false, -1);
  //  save_distance_matrix_csv(num_series, result_dtw, series);
    //printf("clustering completed and saved.\n");

    printf("Aggregation complete.\n");
    double score = silhouette_score(num_series, result_dtw, labels, k);
    printf("Silhouette Score: %.4f\n", score);
    double dunn = dunn_index(num_series, result_dtw, labels, k);
    printf("Dunn's Index: %.4f\n", dunn);

}
