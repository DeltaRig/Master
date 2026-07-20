#!/bin/bash
# OpenMP Dynamic DTW execution script
# Usage: ./run_openmp_dynamic.sh <csv_path> <series_quantity> <num_threads> <output_file>

CSV_PATH=${1:-"../../dados/master_tickers.csv"}
SERIES_QUANTITY=${2:-100}
NUM_THREADS=${3:-8}
OUTPUT_FILE=${4:-"../../results/openmp/dtw_dynamic_result.csv"}

cd ../implementations/openmp
make

export OMP_NUM_THREADS=$NUM_THREADS
echo "Running OpenMP Dynamic DTW with $NUM_THREADS threads..."
./openmp_dynamic "$CSV_PATH" "$SERIES_QUANTITY" "$OUTPUT_FILE"

echo "Results saved to $OUTPUT_FILE"
