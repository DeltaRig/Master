/*
 * Created by Daniela Rigoli
 * June 2025
 *
 * This file is part of the DTW aggregation and clustering project.
 */
// types.h
#ifndef TYPES_H
#define TYPES_H

#define MAX_TICKER_NAME 32
#define MAX_TIMEPOINTS 2000

#define MAX_CLUSTERS 1024

typedef struct {
    char ticker[MAX_TICKER_NAME];
    double close[MAX_TIMEPOINTS];
    int count;
} TickerSeries;

typedef struct {
    int *members;
    int size;
    bool active;
} Cluster;

#endif // TYPES_H
