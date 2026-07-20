#!/bin/bash
# Hybrid MPI+OpenMP DTW execution script
# Usage: ./run_hybrid.sh <csv_path> <series_quantity> <num_processes> <batch_size> <num_threads> <output_file>

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CSV_PATH=${1:-"$SCRIPT_DIR/../dados/master_tickers.csv"}
SERIES_QUANTITY=${2:-100}
NUM_PROCESSES=${3:-4}
BATCH_SIZE=${4:-10}
NUM_THREADS=${5:-4}
OUTPUT_FILE=${6:-"$SCRIPT_DIR/../results/hybrid/dtw_result.csv"}

cd "$SCRIPT_DIR/../implementations/hybrid"
make

export OMP_NUM_THREADS=$NUM_THREADS
echo "Running Hybrid DTW with $NUM_PROCESSES MPI processes, $NUM_THREADS OpenMP threads per process, and batch size $BATCH_SIZE..."
mpirun -np $NUM_PROCESSES ./hybrid "$CSV_PATH" "$SERIES_QUANTITY" "$BATCH_SIZE" "$OUTPUT_FILE"

echo "Results saved to $OUTPUT_FILE"
