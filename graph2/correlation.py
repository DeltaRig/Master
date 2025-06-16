import pandas as pd
import numpy as np
from fastdtw import fastdtw
from scipy.spatial.distance import euclidean
from scipy.stats import spearmanr
from itertools import combinations
import time

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

    # print(matrix)

    pd.DataFrame(sorted_pairs, columns=['Ticker1', 'Ticker2', 'Correlation']).to_csv(output_file, index=False)

# Pearson
def compute_pearson(df):
    corr = df.corr(method='pearson', min_periods=100)
    save_sorted_correlation(corr, "pearson_sorted.csv")

# Spearman
def compute_spearman(df):
    #print(df)
    corr = df.corr(method='spearman')
    save_sorted_correlation(corr, "spearman_sorted.csv")

if __name__ == "__main__":
    filename = "master_tickers"
    df = load_data(f"/home/dani/Documents/git/Master/finviz-platform/dados/{filename}.csv") 
    start_time = time.time()
    compute_pearson(df)
    #compute_spearman(df)

    end_time = time.time()
    elapsed = end_time - start_time
    print(f"Executed for {filename} file 623 ativos")
    print(f"Elapsed time: {elapsed} seconds")
