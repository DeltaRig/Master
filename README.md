# Master

## 🎯 Project Goals

- Test the efficiency of DTW using varying numbers of threads (OpenMP).
- Compare runtime and performance with and without **data aggregation**.
- Validate and process **stock price data from CSV files**.
- Compare different parallelization strategies: Sequential, OpenMP, MPI (v1, v2, v3), and Hybrid (MPI+OpenMP)

## 📁 Repository Structure

```
Master/
├── implementations/          # All DTW implementation versions
│   ├── sequential/          # Baseline sequential implementation
│   ├── openmp/              # OpenMP parallel implementations
│   ├── mpi/                 # MPI implementations by version
│   │   ├── v1/             # Initial MPI implementation
│   │   ├── v2/             # Optimized communication pattern
│   │   └── v3/             # Advanced datatype optimization
│   └── hybrid/              # Hybrid MPI+OpenMP implementation
├── results/                 # Organized results by implementation
├── dados/                   # Dataset files
├── scripts/                 # Execution scripts for all versions
├── python/                  # Python data collection and visualization
└── dtaidistance/           # Original DTAIDistance library
```

## 🚀 Quick Start

Each implementation has its own Makefile and README:

```bash
# Build and run sequential version
cd implementations/sequential
make
./dtw_seq <csv_path> <series_quantity> <aggregation_flag> <output_file>

# Build and run OpenMP version
cd implementations/openmp
make
export OMP_NUM_THREADS=8
./openmp_dynamic <csv_path> <series_quantity> 0 <aggregation_flag> <output_file>

# Build and run MPI versions
cd implementations/mpi/v3  # or v1, v2
make
mpirun -np 4 ./mpi_v3 <csv_path> <series_quantity> <aggregation_flag> <output_file>

# Build and run hybrid version
cd implementations/hybrid
make
export OMP_NUM_THREADS=4
mpirun -np 4 ./hybrid <csv_path> <series_quantity> <aggregation_flag> <output_file>
```

See individual implementation READMEs for detailed compilation and execution instructions.

## 📜 Execution Scripts

For quick execution and reproducibility, use the provided scripts in the `scripts/` directory:

```bash
# Local execution examples
cd scripts
./run_sequential.sh ../../dados/master_tickers.csv 100 0 ../../results/sequential/dtw_result.csv
./run_openmp_dynamic.sh ../../dados/master_tickers.csv 100 8 0 ../../results/openmp/dtw_result.csv
./run_mpi_v3.sh ../../dados/master_tickers.csv 100 4 0 ../../results/mpi/v3/dtw_result.csv
./run_hybrid.sh ../../dados/master_tickers.csv 100 4 4 0 ../../results/hybrid/dtw_result.csv

# Cluster execution (SLURM)
sbatch slurm_openmp.sh
sbatch slurm_mpi_v3.sh
sbatch slurm_hybrid.sh
```

See `scripts/README.md` for detailed usage instructions and examples.

# Python folder project files
### colector.py
Collects information from Yahoo Finance for different periods:

- Last 2 years (daily)

- Last week (hourly)

- Last day (minute)

Saves raw data in data/ and normalized data in normalize/.

### normalize.py
Normalizes the close column from files collected by colector.py.

### dtw.py
Compares time series using Dynamic Time Warping (DTW), with C-based performance.

### streamlit/
Contains visualization scripts using Streamlit.

## How to run
In Linux
> streamlit run app.py

In windowns:
> py -m streamlit run app.py

# C Project

## Implementation Versions

This project compares multiple DTW parallelization strategies:

- **Sequential**: Baseline single-threaded implementation for performance comparison
- **OpenMP**: Shared memory parallelization with dynamic vs guided scheduling strategies
- **MPI v1**: Initial distributed memory implementation with basic task distribution
- **MPI v2**: Optimized MPI with reduced communication overhead (2 messages per task)
- **MPI v3**: Advanced MPI with custom datatypes and contiguous buffer management
- **Hybrid**: Combined MPI+OpenMP for multi-core cluster environments

Each version is documented in its respective `implementations/*/README.md` file.


## MPI how to run (Linux)

mpicc to compile

mpirun -np 2 to execute

## MPI how to run (Windows)
Download MPI Runtime.exe and SDK.msi
https://www.microsoft.com/en-us/download/details.aspx?id=105289

Install C/C++ extension (VS Code)

GCC compiler command: gcc ms_v2.c -o ms_v2.exe  -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include"  -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -lmsmpi

MPI run command: mpiexec -n 4 .\ms_v2.exe

Common options:

-n <num_processes>
-env <env_var_name> <env_var_value>
-wdir <working_directory>
-hosts n host1 [m1] host2 [m2] ... hostn [mn]
-cores <num_cores_per_host>
-lines
-debug [0-3]
-logfile <log file>

Examples:

    mpiexec -n 4 pi.exe
    mpiexec -hosts 1 server1 master : -n 8 worker

# Dataset
Yahoo Finance

# coletor yahoo
github.com/Rodrigo-Palma/fifnviz-platform

# Enviroment
Para utilizar os nossos serviços, uma conta foi criada para você. Os detalhes são os seguintes:
Servidor: pantanal.lad.pucrs.br

Por favor, troque a sua senha assim que for possível.
Procedimento para mudança de senha:
1. Logar na pantanal
$ ssh user@atlantica.lad.pucrs.br
1. Trocar a senha:
$ yppasswd
e seguir as instruções apresentadas.

Pedimos que leia nossa apostila de utilização do LAD. Ela foi derivada de um minicurso para novos grupos de usuários e é fortemente recomendada para que entendas o ambiente do LAD.
https://lad-pucrs.github.io/docs/

