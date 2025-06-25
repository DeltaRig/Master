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
void aggregate_kmedoids(int num_series, double *result, TickerSeries *series, int k);
void dbscan(int num_series, double *result, TickerSeries *series, double eps, int minPts);


#endif
