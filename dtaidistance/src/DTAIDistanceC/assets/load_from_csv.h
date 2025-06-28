/*
 * Created by Daniela Rigoli
 * June 2025
 *
 * This file is part of the DTW aggregation and clustering project.
 */
// load_from_csv.h
#ifndef LOAD_SERIES_FROM_CSV_H
#define LOAD_SERIES_FROM_CSV_H

#include "types.h"

int load_series_from_csv(const char *filename, TickerSeries *series_list, int *num_series, int max_assets);
bool load_result_from_csv(const char *filename, double *result, int num_series);

#endif // LOAD_SERIES_FROM_CSV_H
