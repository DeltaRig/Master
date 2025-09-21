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