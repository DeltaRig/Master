# OpenMP DTW Implementation

## Overview
OpenMP-based parallel implementation of DTW distance calculation. This version modifies the original DTAIDistanceC OpenMP implementation with different scheduling strategies.

## Files
- `openMPDynamic.c` - Modified OpenMP implementation using dynamic scheduling
- `example_original.c` - Original OpenMP implementation for comparison
- `DTAIDistanceC/` - DTAIDistanceC library with modified `dd_dtw_openmp.c`
- `assets/` - Shared utilities (CSV loading, aggregation)

## Key Modifications
The `dd_dtw_openmp.c` file includes two parallel implementations:
1. **`dtw_distances_ptrs_parallel`** - Uses guided scheduling (original)
2. **`dtw_distances_ptrs_parallel_d`** - Uses dynamic scheduling (modified)

### Scheduling Strategy Comparison
- **Guided scheduling**: Assumes rows have different lengths, assigns chunks dynamically
- **Dynamic scheduling**: More adaptive for varying computation times

## Compilation
```bash
# Modified version (dynamic scheduling)
gcc -o openmp_dynamic openMPDynamic.c \
    assets/load_from_csv.c assets/aggregation.c assets/call_aggregation.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_openmp.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm -I./DTAIDistanceC/

# Original version (guided scheduling)
gcc -o example_original example_original.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_openmp.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm -I./DTAIDistanceC/
```

## Execution
```bash
# Set number of threads
export OMP_NUM_THREADS=8

# Run modified version
./openmp_dynamic <csv_path> <series_quantity> 0 <aggregation_flag> <file_result_destination>

# Run original version
./example_original <csv_path> <series_quantity> <parallel_type> <aggregation_flag> <file_result_destination>
```

## Performance Testing
Test with different thread counts to analyze scalability:
- 1, 6, 12, 24 threads on 24-core machine
- 1, 6, 12, 24, 48 threads on 48-core machine

## Notes
This version demonstrates the impact of OpenMP scheduling strategies on DTW parallelization performance.
