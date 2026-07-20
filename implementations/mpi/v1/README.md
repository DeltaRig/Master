# MPI DTW Implementation - Version 1

## Overview
First MPI implementation of DTW distance calculation using master-slave architecture with basic task distribution.

## Files
- `mainMPIv1m5.c` - Main MPI implementation (version 1, 5 messages per task)
- `DTAIDistanceC/` - DTAIDistanceC library files
- `assets/` - Shared utilities (CSV loading)

## Key Features
- **Architecture**: Master-slave pattern
- **Task distribution**: Basic row-wise distribution
- **Communication**: Standard MPI point-to-point communication
- **Data handling**: Individual message sends per task

## Compilation
```bash
mpicc -o mpi_v1 mainMPIv1m5.c \
    assets/load_from_csv.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_mpi.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm -I./DTAIDistanceC/
```

## Execution
```bash
# Local execution
mpirun -np 4 ./mpi_v1 <csv_path> <series_quantity> <file_result_destination>

# Cluster execution (SLURM)
srun -N 1 -n 24 -t 1000 --exclusive ./mpi_v1 dados/master_tickers.csv 100 results_mpi_v1.csv
```

## Performance Characteristics
- **Scalability**: Limited by communication overhead
- **Load balancing**: Basic static distribution
- **Memory**: Distributed across processes
- **Use case**: Initial MPI parallelization baseline

## Notes
This version represents the initial MPI implementation approach, serving as a baseline for subsequent optimizations in v2 and v3.
