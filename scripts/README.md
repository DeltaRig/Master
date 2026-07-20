# Execution Scripts

This directory contains example execution scripts for all DTW implementation versions to ensure reproducibility for the article publication.

## Local Execution Scripts

These scripts are designed for running experiments on local machines or development environments.

### Sequential
```bash
./run_sequential.sh <csv_path> <series_quantity> <aggregation_flag> <output_file>
# Example:
./run_sequential.sh ../../dados/master_tickers.csv 100 0 ../../results/sequential/dtw_result.csv
```

### OpenMP
```bash
# Dynamic scheduling version
./run_openmp_dynamic.sh <csv_path> <series_quantity> <num_threads> <aggregation_flag> <output_file>
# Example:
./run_openmp_dynamic.sh ../../dados/master_tickers.csv 100 8 0 ../../results/openmp/dtw_dynamic_result.csv

# Original version (guided scheduling)
./run_openmp_original.sh <csv_path> <series_quantity> <num_threads> <parallel_type> <aggregation_flag> <output_file>
# Example:
./run_openmp_original.sh ../../dados/master_tickers.csv 100 8 1 0 ../../results/openmp/dtw_original_result.csv
```

### MPI Versions
```bash
# MPI v1 (5 messages per task)
./run_mpi_v1.sh <csv_path> <series_quantity> <num_processes> <output_file>
# Example:
./run_mpi_v1.sh ../../dados/master_tickers.csv 100 4 ../../results/mpi/v1/dtw_result.csv

# MPI v2 (2 messages per task)
./run_mpi_v2.sh <csv_path> <series_quantity> <num_processes> <output_file>
# Example:
./run_mpi_v2.sh ../../dados/master_tickers.csv 100 4 ../../results/mpi/v2/dtw_result.csv

# MPI v3 (datatype optimization)
./run_mpi_v3.sh <csv_path> <series_quantity> <num_processes> <batch_size> <output_file>
# Example:
./run_mpi_v3.sh ../../dados/master_tickers.csv 100 4 10 ../../results/mpi/v3/dtw_result.csv
```

### Hybrid
```bash
./run_hybrid.sh <csv_path> <series_quantity> <num_processes> <num_threads> <output_file>
# Example:
./run_hybrid.sh ../../dados/master_tickers.csv 100 4 4 ../../results/hybrid/dtw_result.csv
```

## Testing on Local Machine

These scripts are designed for local testing and development. For MPI and Hybrid versions, ensure you have MPI installed locally.

### Requirements
- **Sequential/OpenMP**: GCC with OpenMP support
- **MPI versions**: MPI implementation (OpenMPI, MPICH, etc.)
- **Hybrid**: Both OpenMP and MPI

### Local MPI Testing
For local MPI testing, you can run multiple processes on a single machine:
```bash
# Test with 4 processes on local machine
mpirun -np 4 ./mpi_v3 <csv_path> <series_quantity> <batch_size> <output_file>
```

## Performance Testing Matrix

For the article experiments, test with the following configurations:

### Thread/Process Counts
- **1, 6, 12, 24** on 24-core machines
- **1, 6, 12, 24, 48** on 48-core machines

### Dataset Sizes
- **100, 200, 400, 800** time series

### Example Batch Execution
```bash
# Test OpenMP with different thread counts
for threads in 1 6 12 24; do
    ./run_openmp_dynamic.sh ../../dados/master_tickers.csv 100 $threads 0 ../../results/openmp/dtw_${threads}threads.csv
done

# Test MPI versions with different process counts
for procs in 1 6 12 24; do
    ./run_mpi_v3.sh ../../dados/master_tickers.csv 100 $procs 10 ../../results/mpi/v3/dtw_${procs}procs.csv
done
```

## Notes

- All scripts automatically compile the implementation before execution
- Results are saved to the `results/` directory organized by implementation version
- Modify default parameters in scripts or pass them as command-line arguments
- Ensure dataset paths are correct for your environment
- For cluster execution, adjust module loads and paths as needed for your system
