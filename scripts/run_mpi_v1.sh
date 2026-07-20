#!/bin/bash
# MPI v1 DTW execution script
# Usage: ./run_mpi_v1.sh <csv_path> <series_quantity> <num_processes> <output_file>

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CSV_PATH=${1:-"$SCRIPT_DIR/../dados/master_tickers.csv"}
SERIES_QUANTITY=${2:-100}
NUM_PROCESSES=${3:-4}
OUTPUT_FILE=${4:-"$SCRIPT_DIR/../results/mpi/v1/dtw_result.csv"}

cd "$SCRIPT_DIR/../implementations/mpi/v1"
make

echo "Running MPI v1 DTW with $NUM_PROCESSES processes..."
mpirun -np $NUM_PROCESSES ./mpi_v1 "$CSV_PATH" "$SERIES_QUANTITY" "$OUTPUT_FILE"

echo "Results saved to $OUTPUT_FILE"
