/*
 * Created by Daniela Rigoli
 * June 2025
 *
 * This file is part of the DTW aggregation and clustering project.
 */
#ifndef AGGREGATION_H
#define AGGREGATION_H

#include "types.h" 

void save_distance_matrix_csv(int n, double *result, TickerSeries *series);
void aggregate_kmedoids(int num_series, double *result, TickerSeries *series, int k, int *labels);
void dbscan(int num_series, double *result, TickerSeries *series, double eps, int minPts, int *labels);
void hierarchical_clustering(int n, double *result, TickerSeries *series, int desired_k, int *labels);

// evaluate aggregation
double silhouette_score(int n, double *result, int *labels, int k);
double dunn_index(int n, double *result, int *labels, int k);



#endif
