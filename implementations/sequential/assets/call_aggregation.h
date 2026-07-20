#ifndef CALL_AGGREGATION_H
#define CALL_AGGREGATION_H

#include <stdbool.h>
#include "types.h"

void run_aggregation(int num_series, double *result, TickerSeries *series, int aggregation_type, bool result_already_computed);

#endif
