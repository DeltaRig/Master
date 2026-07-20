# Hybrid OpenMP+MPI DTW Implementation

## Overview
Hybrid implementation combining MPI for inter-node communication and OpenMP for intra-node parallelization, designed for multi-core cluster environments.

## Files
- `mainHybrid1.1.c` - Main hybrid implementation (version 1.1)
- `DTAIDistanceC/` - DTAIDistanceC library files
- `assets/` - Shared utilities (CSV loading)

## Key Features
- **Hybrid parallelization**: MPI for distributed memory, OpenMP for shared memory
- **Two-level hierarchy**: Node-level (MPI) and core-level (OpenMP) parallelism
- **Load balancing**: Combined task distribution strategies
- **Scalability**: Designed for large-scale multi-core clusters

## Architecture
- **MPI level**: Distributes work across compute nodes
- **OpenMP level**: Parallelizes within each node using multiple cores
- **Communication**: Optimized for hierarchical systems

## Compilation
```bash
mpicc -o hybrid mainHybrid1.1.c \
    assets/load_from_csv.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_mpi.c DTAIDistanceC/dd_dtw_openmp.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm -I./DTAIDistanceC/
```

## Execution
```bash
# Set OpenMP threads per MPI process
export OMP_NUM_THREADS=4

# Local execution (4 MPI processes, 4 OpenMP threads each)
mpirun -np 4 ./hybrid <csv_path> <series_quantity> <file_result_destination>

# Cluster execution with SLURM
srun -N 2 -n 8 -t 1000 --exclusive ./hybrid dados/master_tickers.csv 800 results_hybrid.csv
```

## Performance Characteristics
- **Scalability**: Best for large-scale multi-core systems
- **Memory**: Distributed across nodes, shared within nodes
- **Communication**: Optimized for hierarchical networks
- **Use case**: Production-scale cluster deployment

## Notes
This hybrid approach combines the strengths of both MPI and OpenMP, providing the best performance for large-scale cluster environments with multi-core nodes.
