# MPI DTW Implementation - Version 3

## Overview
Third MPI implementation with advanced datatype optimizations for efficient data transfer and memory management.

## Files
- `mainMPIV3.2Datatype.c` - Main MPI implementation (version 3.2 with datatype fixes)
- `DTAIDistanceC/` - DTAIDistanceC library files
- `assets/` - Shared utilities (CSV loading)

## Key Improvements over v2
- **Datatype optimization**: Custom MPI datatypes for efficient data transfer
- **Contiguous buffer management**: Real contiguous sendbuf implementation
- **Memory efficiency**: Optimized memory layout for MPI operations
- **Batch processing**: Enhanced master-slave with contiguous buffers

## Technical Details
- **MPI Datatypes**: Uses derived datatypes for structured data
- **Buffer management**: Contiguous memory layout for better cache performance
- **Communication pattern**: Optimized for large-scale data transfers

## Compilation
```bash
mpicc -o mpi_v3 mainMPIV3.2Datatype.c \
    assets/load_from_csv.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_mpi.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -O3 -fopenmp -lm -I./DTAIDistanceC/
```

## Execution
```bash
# Local execution
mpirun -np 24 ./mpi_v3 <csv_path> <series_quantity> <batch_size> <file_result_destination>

# Cluster execution
srun -N 1 -n 24 -t 1000 --exclusive ./mpi_v3 dados/master_tickers.csv 100 10 results_mpi_v3.csv
srun -N 2 -n 48 -t 1000 --exclusive ./mpi_v3 dados/master_tickers.csv 800 10 results_mpi_v3.csv
```

## Performance Characteristics
- **Scalability**: Best among MPI versions
- **Memory efficiency**: Optimized buffer usage
- **Communication**: Most efficient data transfer
- **Use case**: Final optimized MPI implementation

## Notes
This version represents the most optimized MPI implementation, with advanced datatype handling for maximum performance.
