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
#include <math.h>
#include "aggregation.h"

void save_cluster_labels_csv(const char *filename, int num_series, TickerSeries *series, int *labels, bool print_stdout, int noise_label) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen");
        return;
    }

    fprintf(f, "Ticker,Cluster\n");
    for (int i = 0; i < num_series; i++) {
        if (print_stdout) {
            if (labels[i] == noise_label) {
                printf("%s: NOISE\n", series[i].ticker);
            } else {
                printf("%s: Cluster %d\n", series[i].ticker, labels[i]);
            }
        }
        fprintf(f, "%s,%d\n", series[i].ticker, labels[i]);
    }

    fclose(f);
    printf("Cluster assignments saved to %s\n", filename);
}


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

    // 1. Initialize medoids (simple greedy init: pick first k)
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
    save_cluster_labels_csv("dtw_kmedoids_clusters.csv", num_series, series, labels, false, -1);

    save_distance_matrix_csv(num_series, result, series);
}

// DBSCAN is a clustering algorithm that identifies groups of data points that are close to each other, even if they do not have a circular or square shape. 
// Ester et al., 1996
// adapted to work on a condensed distance matrix
// Access distance from the condensed matrix
double get_dtw_dist(int i, int j, double *result) {
    if (i == j) return 0.0;
    if (i < j) return result[(j * (j - 1)) / 2 + i];
    return result[(i * (i - 1)) / 2 + j];
}

#define NOISE -1
#define UNCLASSIFIED -2
#define MAX_NEIGHBORS 1024

// Density-Based Spatial Clustering of Applications with Noise
// Choose eps using domain knowledge or plot the k-distance graph.
void dbscan(int num_series, double *result, TickerSeries *series, double eps, int minPts) {
    int labels[num_series];
    for (int i = 0; i < num_series; i++) labels[i] = UNCLASSIFIED;

    int cluster_id = 0;

    for (int i = 0; i < num_series; i++) {
        if (labels[i] != UNCLASSIFIED)
            continue;

        // Find neighbors
        int neighbors[MAX_NEIGHBORS];
        int neighbor_count = 0;

        for (int j = 0; j < num_series; j++) {
            if (get_dtw_dist(i, j, result) <= eps) {
                neighbors[neighbor_count++] = j;
            }
        }

        if (neighbor_count < minPts) {
            labels[i] = NOISE;
        } else {
            // Create a new cluster
            labels[i] = cluster_id;

            for (int n = 0; n < neighbor_count; n++) {
                int j = neighbors[n];
                if (labels[j] == NOISE)
                    labels[j] = cluster_id;
                if (labels[j] != UNCLASSIFIED)
                    continue;

                labels[j] = cluster_id;

                // Expand
                int sub_neighbors[MAX_NEIGHBORS];
                int sub_count = 0;
                for (int k = 0; k < num_series; k++) {
                    if (get_dtw_dist(j, k, result) <= eps) {
                        sub_neighbors[sub_count++] = k;
                    }
                }

                if (sub_count >= minPts) {
                    // Append new neighbors
                    for (int k = 0; k < sub_count; k++) {
                        if (neighbor_count < MAX_NEIGHBORS)
                            neighbors[neighbor_count++] = sub_neighbors[k];
                    }
                }
            }

            cluster_id++;
        }
    }

    // Print and save results
    printf("\n=== DBSCAN Clusters (eps=%.2f, minPts=%d) ===\n", eps, minPts);
    save_cluster_labels_csv("dtw_dbscan_clusters.csv", num_series, series, labels, true, NOISE);

    save_distance_matrix_csv(num_series, result, series);
}



void hierarchical_clustering(int n, double *result, TickerSeries *series, int desired_k) {
    Cluster clusters[MAX_CLUSTERS];
    int labels[MAX_CLUSTERS];

    for (int i = 0; i < n; i++) {
        clusters[i].members = malloc(sizeof(int));
        clusters[i].members[0] = i;
        clusters[i].size = 1;
        clusters[i].active = true;
        labels[i] = -1;
    }

    int num_active = n;
    while (num_active > desired_k) {
        double min_dist = DBL_MAX;
        int best_a = -1, best_b = -1;

        // Find closest pair of clusters
        for (int a = 0; a < n; a++) {
            if (!clusters[a].active) continue;
            for (int b = a + 1; b < n; b++) {
                if (!clusters[b].active) continue;

                // Compute average linkage distance
                double sum = 0.0;
                for (int i = 0; i < clusters[a].size; i++) {
                    for (int j = 0; j < clusters[b].size; j++) {
                        int idx1 = clusters[a].members[i];
                        int idx2 = clusters[b].members[j];
                        sum += get_dtw_dist(idx1, idx2, result);
                    }
                }
                double avg_dist = sum / (clusters[a].size * clusters[b].size);

                if (avg_dist < min_dist) {
                    min_dist = avg_dist;
                    best_a = a;
                    best_b = b;
                }
            }
        }

        // Merge clusters best_a and best_b
        int new_size = clusters[best_a].size + clusters[best_b].size;
        int *new_members = malloc(sizeof(int) * new_size);
        for (int i = 0; i < clusters[best_a].size; i++)
            new_members[i] = clusters[best_a].members[i];
        for (int i = 0; i < clusters[best_b].size; i++)
            new_members[clusters[best_a].size + i] = clusters[best_b].members[i];

        free(clusters[best_a].members);
        clusters[best_a].members = new_members;
        clusters[best_a].size = new_size;

        free(clusters[best_b].members);
        clusters[best_b].members = NULL;
        clusters[best_b].size = 0;
        clusters[best_b].active = false;

        num_active--;
    }

    // Assign final labels
    int cluster_id = 0;
    for (int i = 0; i < n; i++) {
        if (!clusters[i].active) continue;
        for (int j = 0; j < clusters[i].size; j++) {
            labels[clusters[i].members[j]] = cluster_id;
        }
        cluster_id++;
    }

    // Output
    save_cluster_labels_csv("dtw_hierarchical_clusters.csv", n, series, labels, false, -1);
    save_distance_matrix_csv(n, result, series);
    printf("Hierarchical clustering completed and saved.\n");

    for (int i = 0; i < n; i++) {
        if (clusters[i].members)
            free(clusters[i].members);
    }
}