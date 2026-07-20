#!/bin/bash
# MPI v3 DTW execution script
# Usage: ./run_mpi_v3.sh <csv_path> <series_quantity> <num_processes> <batch_size> <output_file>

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CSV_PATH=${1:-"$SCRIPT_DIR/../dados/master_tickers.csv"}
SERIES_QUANTITY=${2:-100}
NUM_PROCESSES=${3:-4}
BATCH_SIZE=${4:-10}
OUTPUT_FILE=${5:-"$SCRIPT_DIR/../results/mpi/v3/dtw_result.csv"}

cd "$SCRIPT_DIR/../implementations/mpi/v3"
make

echo "Running MPI v3 DTW with $NUM_PROCESSES processes and batch size $BATCH_SIZE..."
mpirun -np $NUM_PROCESSES ./mpi_v3 "$CSV_PATH" "$SERIES_QUANTITY" "$BATCH_SIZE" "$OUTPUT_FILE"

echo "Results saved to $OUTPUT_FILE"
