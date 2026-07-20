# MPI DTW Implementation - Version 2

## Overview
Second MPI implementation with optimized communication pattern, reducing the number of messages per task.

## Files
- `mainMPI.c` - Main MPI implementation (version 2)
- `DTAIDistanceC/` - DTAIDistanceC library files
- `assets/` - Shared utilities (CSV loading)

## Key Improvements over v1
- **Communication optimization**: Reduced to two messages per task
- **Batch processing**: Improved data aggregation
- **Message efficiency**: Lower communication overhead

## Compilation
```bash
mpicc -o mpi_v2 mainMPI.c \
    assets/load_from_csv.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_mpi.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm -I./DTAIDistanceC/
```

## Execution
```bash
# Local execution
mpirun -np 4 ./mpi_v2 <csv_path> <series_quantity> <file_result_destination>

# Cluster execution with different configurations
srun -N 1 -n 24 -t 1000 --exclusive ./mpi_v2 dados/master_tickers.csv 100 results_mpi_v2.csv
srun -N 2 -n 48 -t 1000 --exclusive ./mpi_v2 dados/master_tickers.csv 800 results_mpi_v2.csv
```

## Performance Characteristics
- **Scalability**: Improved over v1 due to reduced communication
- **Load balancing**: Better than v1
- **Communication**: Optimized message count
- **Use case**: Intermediate MPI optimization

## Notes
This version demonstrates the impact of communication optimization on MPI performance for DTW calculations.
