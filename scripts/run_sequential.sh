#!/bin/bash
# Sequential DTW execution script
# Usage: ./run_sequential.sh <csv_path> <series_quantity> <aggregation_flag> <output_file>

CSV_PATH=${1:-"../../dados/master_tickers.csv"}
SERIES_QUANTITY=${2:-100}
AGGREGATION_FLAG=${3:-0}
OUTPUT_FILE=${4:-"../../results/sequential/dtw_result.csv"}

cd ../implementations/sequential
make

echo "Running sequential DTW..."
./dtw_seq "$CSV_PATH" "$SERIES_QUANTITY" "$AGGREGATION_FLAG" "$OUTPUT_FILE"

echo "Results saved to $OUTPUT_FILE"
