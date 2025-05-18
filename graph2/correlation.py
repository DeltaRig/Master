import pandas as pd
import numpy as np
from fastdtw import fastdtw
from scipy.spatial.distance import euclidean
from scipy.stats import spearmanr
from itertools import combinations

# === Parte 1: Calcular e salvar as correlações ===
def load_data(csv_path):
    df = pd.read_csv(csv_path, parse_dates=['Date'])
    pivot_df = df.pivot(index='Date', columns='Ticker', values='Adj Close').dropna()
    return pivot_df

def save_sorted_correlation(matrix, output_file, descending=True):
    pairs = []
    for i in matrix.columns:
        for j in matrix.columns:
            if i < j:
                value = matrix.loc[i, j]
                pairs.append((i, j, value))
    sorted_pairs = sorted(pairs, key=lambda x: abs(x[2]), reverse=descending)

    print(matrix)

    pd.DataFrame(sorted_pairs, columns=['Ticker1', 'Ticker2', 'Correlation']).to_csv(output_file, index=False)

# Pearson
def compute_pearson(df):
    corr = df.corr(method='pearson')
    save_sorted_correlation(corr, "pearson_sorted.csv")

# Spearman
def compute_spearman(df):
    corr = df.corr(method='spearman')
    save_sorted_correlation(corr, "spearman_sorted.csv")

# DTW using euclidean
def compute_dtw(df):
    tickers = df.columns.tolist()
    dtw_matrix = pd.DataFrame(index=tickers, columns=tickers, dtype=float)
    for t1, t2 in combinations(tickers, 2):
        dist, _ = fastdtw(df[t1].values, df[t2].values, dist=euclidean)
        dtw_matrix.loc[t1, t2] = dist
        dtw_matrix.loc[t2, t1] = dist
    np.fill_diagonal(dtw_matrix.values, 0.0)
    save_sorted_correlation(dtw_matrix, "dtw_sorted.csv", descending=False)

if __name__ == "__main__":
    df = load_data("/home/dani/Documents/git/Master/finviz-platform/dados/acoes_ibov.csv") 
    compute_pearson(df)
    compute_spearman(df)
   # compute_dtw(df)
