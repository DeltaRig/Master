# Sequential DTW Implementation

## Overview
This is the baseline sequential implementation of Dynamic Time Warping (DTW) distance calculation. It serves as the reference point for comparing parallel implementations.

## Files
- `dtwSequential.c` - Main sequential DTW implementation
- `DTAIDistanceC/` - Original DTAIDistanceC library files
- `assets/` - Shared utilities (CSV loading, aggregation)

## Compilation
```bash
gcc dtwSequential.c -o dtw_seq \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    assets/load_from_csv.c assets/aggregation.c assets/call_aggregation.c \
    -lm -I./DTAIDistanceC/
```

## Execution
```bash
./dtw_seq <csv_path> <series_quantity> <aggregation_flag> <file_result_destination>
```

## Performance Characteristics
- **Baseline performance**: Single-threaded execution
- **Memory usage**: Standard DTW memory requirements
- **Use case**: Reference for speedup calculations

## Notes
This implementation provides the baseline performance metrics used in the article to quantify the improvements achieved by parallel versions.
