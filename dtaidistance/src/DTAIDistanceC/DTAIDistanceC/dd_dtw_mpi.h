/*!
@header dtw_openmp.h
@brief DTAIDistance.dtw

@author Wannes Meert
@copyright Copyright © 2020 Wannes Meert. Apache License, Version 2.0, see LICENSE for details.
*/

#ifndef dtw_mpi_h
#define dtw_mpi_h

#include <stdio.h>
#include <assert.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

#include "dd_dtw.h"


int dtw_distances_prepare_MS(DTWBlock *block, idx_t nb_series_r, idx_t nb_series_c, 
                             idx_t **cbs, idx_t **rls, idx_t *length, DTWSettings *settings);
idx_t dtw_distances_ptrs_parallel_MS(seq_t **ptrs, idx_t nb_ptrs, idx_t* lengths,
                                   seq_t* output, DTWBlock* block, DTWSettings* settings);
idx_t dtw_distances_ptrs_parallel_MS_v2(seq_t **ptrs, idx_t nb_ptrs, idx_t* lengths,
                                      seq_t* output, DTWBlock* block, DTWSettings* settings);

#endif /* dtw_mpi_h */
