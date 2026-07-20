#!/bin/bash
# OpenMP Original DTW execution script
# Usage: ./run_openmp_original.sh <csv_path> <series_quantity> <num_threads> <parallel_type> <output_file>

CSV_PATH=${1:-"../../dados/master_tickers.csv"}
SERIES_QUANTITY=${2:-100}
NUM_THREADS=${3:-8}
PARALLEL_TYPE=${4:-1}
OUTPUT_FILE=${5:-"../../results/openmp/dtw_original_result.csv"}

cd ../implementations/openmp
make

export OMP_NUM_THREADS=$NUM_THREADS
echo "Running OpenMP Original DTW with $NUM_THREADS threads..."
./example_original "$CSV_PATH" "$SERIES_QUANTITY" "$PARALLEL_TYPE" "$OUTPUT_FILE"

echo "Results saved to $OUTPUT_FILE"
