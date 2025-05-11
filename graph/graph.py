import pandas as pd
import networkx as nx
from dtaidistance import dtw
import numpy as np
from itertools import combinations

def load_data_from_single_csv(csv_path):
    df = pd.read_csv(csv_path, parse_dates=['Date'])
    df = df[['Date', 'Ticker', 'Adj Close']].dropna()
    pivot_df = df.pivot(index='Date', columns='Ticker', values='Adj Close')
    return pivot_df.dropna(axis=0)  # Drop dates with missing values

def build_correlation_graph(price_df, threshold=0.9):
    corr = price_df.corr()
    G = nx.Graph()
    for col in corr.columns:
        G.add_node(col)
    for i in corr.columns:
        for j in corr.columns:
            if i != j and corr.loc[i, j] >= threshold:
                G.add_edge(i, j, weight=round(corr.loc[i, j], 2))
    return G

def build_dtw_graph(price_df, threshold=40):
    G = nx.Graph()
    tickers = price_df.columns.tolist()
    for ticker in tickers:
        G.add_node(ticker)
    for i in range(len(tickers)):
        for j in range(i + 1, len(tickers)):
            s1 = price_df[tickers[i]].to_numpy()
            s2 = price_df[tickers[j]].to_numpy()
            dist = dtw.distance_fast(s1, s2)
            if round(dist, 2) <= round(threshold,2):
                G.add_edge(tickers[i], tickers[j], weight=round(dist, 2))
    return G

def graph_to_dot(G, graph_name="G"):
    dot = f"graph {graph_name} {{\n"
    for u, v, d in G.edges(data=True):
        dot += f'    "{u}" -- "{v}" [label="{d["weight"]}"];\n'
    dot += "}"
    return dot

# ==== MAIN ====
csv_path = "/home/dani/Documents/git/Master/finviz-platform/dados_1/acoes_ibov.csv"  # Replace with your CSV file
price_df = load_data_from_single_csv(csv_path)

# === Calcular correlações ===
pearson_results = []
dtw_results = []

# Pearson correlation graph
G_corr = build_correlation_graph(price_df, threshold=0.90)
dot_corr = graph_to_dot(G_corr, "CorrelationGraph")
print("Correlation Graph DOT:\n", dot_corr)
with open("graph_pearson.dot", "a") as f:
  f.write(dot_corr)


# DTW distance graph
G_dtw = build_dtw_graph(price_df, threshold=20)
dot_dtw = graph_to_dot(G_dtw, "DTWGraph")
print("\nDTW Graph DOT:\n", dot_dtw)
with open("graph_dwt.dot", "a") as f:
  f.write(dot_dtw)