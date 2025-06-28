# DTAIDistanceC

Time series distances (e.g. Dynamic Time Warping) used in the
[DTAI Research Group](https://dtai.cs.kuleuven.be).
The code is developed to be used as part of the Python package DTAIDistance
but can be used also in an only C project.

Documentation Python toolbox: http://dtaidistance.readthedocs.io

Citing this work: [![DOI](https://zenodo.org/badge/80764246.svg)](https://zenodo.org/badge/latestdoi/80764246)

## Requirements

- The parallel functionality assumes OpenMP is available.
- The tests are based on Criterion.


## License

    DTAI distance code.

    Copyright 2020 Wannes Meert, KU Leuven, DTAI Research Group

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

# Add in Master Degree's Daniela Project

## 🎯 Project Goals

- Test the efficiency of DTW using varying numbers of threads (OpenMP).
- Compare runtime and performance with and without **data aggregation**.
- Validate and process **stock price data from CSV files**.

## 📁 Directory Structure
```
project-root/
│
├── DTAIDistanceC/ # C implementation of DTW and other distances
├── assets/
│ ├── load_from_csv.c # Load time series from CSV
│ └── aggregation.c # Aggregation logic (e.g., sum, average)
├── example.c # Main runner file
├── run.sh # Script to execute the binary
├── dados/ # Folder containing stock datasets
└── README.md # This documentation
```

## ⚙️ Requirements

- Git
- GCC with OpenMP support
- `libomp-dev` (Linux)

### 🧱 Install OpenMP (Ubuntu example):

```
sudo apt update
sudo apt install libomp-dev
```

### 🔧 Compilation Instructions
Use the following gcc command to compile the full application, including CSV parsing and aggregation:
```
gcc -o example example.c \
    assets/load_from_csv.c assets/aggregation.c assets/call_aggregation.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_openmp.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm \
    -I./DTAIDistanceC/
```

### 🚀 Execution

> OMP_NUM_THREADS=8 ./example <csv_path> <series_quantity> <aggregation_flag>

- <csv_path>: Path to your stock dataset CSV file
- <series_quantity>: Number of time series to process
- <aggregation_flag>:
- - 0: Run without aggregation
- - number of k to use in aggregation


### 🧪 Example:
> OMP_NUM_THREADS=4 ./example dados/master_tickers.csv 24 1

This runs the program on 24 series from master_tickers.csv using aggregation and 4 threads.

### 🧵 Thread Testing Matrix
Experiment with different thread counts to analyze performance:

| Threads | Machine cores       |
| ------- | ------------------- |
| 1       | 24, 48              |
| 6       | 24, 48              |
| 12      | 24, 48              |
| 24      | 24, 48              |

With 100, 200, 400 and 800 tickers



dpkg -L libomp-dev

### 📤 File Transfer (for remote experiments)
Transfer project and data using scp:

```
scp -r /home/dani/Documents/git/Master/dtaidistance daniela.rigoli@sparta.pucrs.br:
scp -r /home/dani/Documents/git/Master/dados daniela.rigoli@sparta.pucrs.br:
```

```
scp -r dados daniela.rigoli@pantanal.lad.pucrs.br:
scp -r dtaidistance daniela.rigoli@pantanal.lad.pucrs.br:
```

### ⚙️ Job Execution

srun --exclusive -N 4 -n 8 ./arquivo_executável

ladalloc  -n 2 -t 10 -e
ladrun -np --exclusive -n 3 ./mpi_merg

srun -N 1 -n 1 -t 10 --exclusive ./dtadistance/src/DTAIDistanceC/example dados/master_tickers.csv 8 0 > out.txt

srun -N 1 -n 1 -t 96 --exclusive ./run.sh >> out100.txt

## Aggregation

SDBSCAN
Dataset Size	Recommended minPts
Very small (<50)	2–4
Small–medium (100–500)	4–10
Large (>500)	10–50