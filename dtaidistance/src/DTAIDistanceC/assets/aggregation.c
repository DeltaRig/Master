/*
 * Created by Daniela Rigoli
 * June 2025
 *
 * This file is part of the DTW aggregation and clustering project.
 */

// k medoids
#include <float.h>   // For DBL_MAX
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>
#include "types.h" 

void save_distance_matrix_csv(int n, double *result, TickerSeries *series) {
    FILE *f = fopen("dtw_distance_matrix.csv", "w");
    if (!f) {
        perror("fopen");
        return;
    }

    // Header
    fprintf(f, "Ticker");
    for (int j = 0; j < n; j++) {
        fprintf(f, ",%s", series[j].ticker);
    }
    fprintf(f, "\n");

    // Full matrix
    for (int i = 0; i < n; i++) {
        fprintf(f, "%s", series[i].ticker);
        for (int j = 0; j < n; j++) {
            double dist;
            if (i == j) dist = 0;
            else if (i < j) dist = result[(j * (j - 1)) / 2 + i];
            else          dist = result[(i * (i - 1)) / 2 + j];
            fprintf(f, ",%f", dist);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    printf("Full distance matrix saved to dtw_distance_matrix.csv\n");
}

void aggregate_kmedoids(int num_series, double *result, TickerSeries *series, int k) {
    if (k >= num_series) {
        printf("k must be less than the number of series\n");
        return;
    }

    int medoids[k];

    // 1. Initialize medoids randomly (simple greedy init: pick first k)
    for (int i = 0; i < k; i++) {
        medoids[i] = i;
    }

    int labels[num_series];
    bool changed = true;
    int max_iter = 100, iter = 0;

    while (changed && iter++ < max_iter) {
        changed = false;

        // 2. Assign each point to the nearest medoid
        for (int i = 0; i < num_series; i++) {
            double min_dist = DBL_MAX;
            int best = -1;
            for (int m = 0; m < k; m++) {
                int medoid = medoids[m];
                int idx = (i < medoid) ? (medoid * (medoid - 1)) / 2 + i : (i * (i - 1)) / 2 + medoid;
                if (i == medoid) {
                    min_dist = 0;
                    best = m;
                    break;
                }
                double dist = result[idx];
                if (dist < min_dist) {
                    min_dist = dist;
                    best = m;
                }
            }
            if (labels[i] != best) {
                changed = true;
                labels[i] = best;
            }
        }

        // 3. Update medoids
        for (int m = 0; m < k; m++) {
            double best_cost = DBL_MAX;
            int best_idx = -1;

            for (int i = 0; i < num_series; i++) {
                if (labels[i] != m) continue;
                double cost = 0.0;
                for (int j = 0; j < num_series; j++) {
                    if (labels[j] != m || i == j) continue;
                    int idx = (i < j) ? (j * (j - 1)) / 2 + i : (i * (i - 1)) / 2 + j;
                    cost += result[idx];
                }
                if (cost < best_cost) {
                    best_cost = cost;
                    best_idx = i;
                }
            }
            medoids[m] = best_idx;
        }
    }

    // 4. Print the clusters
    printf("\n=== K-Medoids Clusters (k=%d) ===\n", k);
    for (int m = 0; m < k; m++) {
        printf("Cluster %d (medoid: %s):\n", m, series[medoids[m]].ticker);
        for (int i = 0; i < num_series; i++) {
            if (labels[i] == m) {
                printf("  - %s\n", series[i].ticker);
            }
        }
    }

    // Save cluster result to CSV
    // send to method
    FILE *f = fopen("dtw_clusters.csv", "w");
    if (!f) {
        perror("fopen");
        return;
    }
    fprintf(f, "Ticker,Cluster\n");
    for (int i = 0; i < num_series; i++) {
        fprintf(f, "%s,%d\n", series[i].ticker, labels[i]);
    }
    fclose(f);
    printf("Cluster assignments saved to dtw_clusters.csv\n");
    //
    save_distance_matrix_csv(num_series, result, series);
}