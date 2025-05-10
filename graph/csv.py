import pandas as pd
import numpy as np
from fastdtw import fastdtw
from scipy.spatial.distance import euclidean
from itertools import combinations

# Carregar os dados de um único CSV
csv_path = "/home/dani/Documents/git/Master/finviz-platform/dados_1/acoes_ibov.csv"  # Replace with your CSV file

df = pd.read_csv("/home/dani/Documents/git/Master/finviz-platform/dados_1/acoes_ibov.csv", parse_dates=["Date"])  # Substitua pelo seu arquivo

# Preparar os dados pivotados (Adj Close por Ticker)
pivot_df = df.pivot(index='Date', columns='Ticker', values='Adj Close').dropna()

# 1. Matriz de correlação de Pearson
pearson_corr = pivot_df.corr()
pearson_corr.to_csv("pearson_correlation.csv")

# 2. Matriz de distância DTW
tickers = pivot_df.columns.tolist()
dtw_matrix = pd.DataFrame(index=tickers, columns=tickers, dtype=float)

for t1, t2 in combinations(tickers, 2):
    dist, _ = fastdtw(pivot_df[t1].values, pivot_df[t2].values, dist=euclidean)
    dtw_matrix.loc[t1, t2] = dist
    dtw_matrix.loc[t2, t1] = dist

np.fill_diagonal(dtw_matrix.values, 0.0)
dtw_matrix.to_csv("dtw_distance.csv")
