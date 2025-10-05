# Master

## 🎯 Project Goals

- Test the efficiency of DTW using varying numbers of threads (OpenMP).
- Compare runtime and performance with and without **data aggregation**.
- Validate and process **stock price data from CSV files**.


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

Example is on dtaidistance/src/DTAIDistanceC that run DTW direct in C.

**DTW with OpenMP implementation** was developed by Merth.

**DTW with MPI version 1** All process read the file and organize in memory all stocks. Master send the position of the stocks and slaves calculate and respond the value to master organize.

**DTW with MPI version 2** Just master have the information in memory and will send by message the sequence to be calculated. Master is responsible to organize the block of results in momery and return it.


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