import pandas as pd
import numpy as np
from dtaidistance import dtw
from itertools import combinations
import matplotlib.pyplot as plt
import time
from sklearn.cluster import AgglomerativeClustering
import seaborn as sns

# ===== 1. Carregar dados =====
def load_data_from_single_csv(csv_path):
    df = pd.read_csv(csv_path, parse_dates=['Date'])
    df = df[['Date', 'Ticker', 'Adj Close']].dropna()
    pivot_df = df.pivot(index='Date', columns='Ticker', values='Adj Close')
    return pivot_df.dropna(axis=0)  # Remove datas com valores ausentes

def dtw_distance(s1, s2):
    n, m = len(s1), len(s2)
    dtw_matrix = np.full((n+1, m+1), np.inf)
    dtw_matrix[0, 0] = 0

    for i in range(1, n+1):
        for j in range(1, m+1):
            cost = abs(s1[i-1] - s2[j-1])
            last_min = min(
                dtw_matrix[i-1, j],    # Inserção
                dtw_matrix[i, j-1],    # Deleção
                dtw_matrix[i-1, j-1]   # Match
            )
            dtw_matrix[i, j] = cost + last_min

    return dtw_matrix[n, m]

# ===== 2. Calcular DTW para todos os pares e salvar =====
def compute_dtw_all_pairs(price_df):
    dtw_results = []
    tickers = price_df.columns.tolist()

    for t1, t2 in combinations(tickers, 2):
        s1 = price_df[t1].to_numpy()
        s2 = price_df[t2].to_numpy()
        dist = dtw.distance_fast(s1, s2)
        dtw_results.append((t1, t2, dist))

    return sorted(dtw_results, key=lambda x: x[2])  # Ordenar por menor distância

# ===== 3. Gerar gráfico de dois ativos =====
def plot_most_correlated(price_df, ticker1, ticker2):
    series_1 = price_df[ticker1]
    series_2 = price_df[ticker2]

    # Garantir mesmo tamanho
    min_length = min(len(series_1), len(series_2))
    series_1 = series_1.iloc[:min_length]
    series_2 = series_2.iloc[:min_length]
    data_index = price_df.index[:min_length]

    plt.figure(figsize=(10, 6))
    plt.plot(data_index, series_1, label=ticker1, color="blue")
    plt.plot(data_index, series_2, label=ticker2, color="green")
    plt.title(f"Stock Price Comparison: {ticker1} vs {ticker2}")
    plt.xlabel("Date")
    plt.ylabel("Adjusted Closing Price")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()
    plt.savefig('bestCorrelation.png')

def save_dtw_results_to_csv(dtw_results, output_path="dtw_correlacoes.csv"):
    df = pd.DataFrame(dtw_results, columns=["Ticker1", "Ticker2", "DTW_Distance"])
    df.to_csv(output_path, index=False)
    print(f"Resultados salvos em: {output_path}")

import networkx as nx

def create_dtw_graph(dtw_results, top_percent=0.1):
    num_edges = int(len(dtw_results) * top_percent)
    top_edges = dtw_results[:num_edges]

    G = nx.Graph()
    for t1, t2, dist in top_edges:
        G.add_edge(t1, t2, weight=dist)
    
    return G

def plot_dwt_graph(G):
    # Plotar
    plt.figure(figsize=(12, 8))
    pos = nx.spring_layout(G, seed=42, k=0.15)
    weights = [1 / G[u][v]['weight'] for u, v in G.edges]  # Inverter DTW para grosso = mais parecido
    nx.draw_networkx(G, pos, with_labels=True, node_size=500, width=weights, edge_color='gray')
    plt.title("Rede de Ativos com Maior Correlação (menor DTW)")
    plt.axis('off')
    plt.tight_layout()
    plt.show()
    plt.savefig('StickerGraph.png')

def cluster_dtw_and_plot_network(G, dtw_results, n_clusters=4):
    # Obter lista única de tickers
    tickers = list(set([t for triplet in dtw_results for t in triplet[:2]]))
    ticker_index = {ticker: idx for idx, ticker in enumerate(tickers)}

    # Criar matriz de distâncias
    n = len(tickers)
    dist_matrix = np.zeros((n, n)) + np.inf
    for t1, t2, dist in dtw_results:
        i, j = ticker_index[t1], ticker_index[t2]
        dist_matrix[i, j] = dist
        dist_matrix[j, i] = dist

    # Preencher a diagonal com 0
    np.fill_diagonal(dist_matrix, 0)

    # Clustering
    clustering = AgglomerativeClustering(n_clusters=n_clusters, metric='precomputed', linkage='average')
    labels = clustering.fit_predict(dist_matrix)

    # Atribuir cores por cluster
    color_map = {}
    palette = sns.color_palette("hls", n_clusters)
    for ticker, label in zip(tickers, labels):
        color_map[ticker] = palette[label]

    node_colors = [color_map[node] for node in G.nodes]

    # Plotar grafo com clusters coloridos
    plt.figure(figsize=(12, 8))
    pos = nx.spring_layout(G, seed=42)
    nx.draw_networkx(G, pos, node_color=node_colors, with_labels=True, edge_color='lightgray', node_size=500)
    plt.title(f"Clusters de Ativos com DTW (n_clusters={n_clusters})")
    plt.axis('off')
    plt.tight_layout()
    plt.show()
    plt.savefig('Clusters.png')



# ==========
csv_path = "/home/dani/Documents/git/Master/dados/master_tickers.csv"
price_df = load_data_from_single_csv(csv_path)

start_time = time.time()

# Calcular distâncias DTW
dtw_results = compute_dtw_all_pairs(price_df)

end_time = time.time()
print(f"Executado com {len(price_df.columns)} ativos")
print(f"Tempo total: {end_time - start_time:.2f} segundos")

save_dtw_results_to_csv(dtw_results)

# Mostrar os dois mais correlacionados
most_similar = dtw_results[0]
print(f"Mais similares: {most_similar[0]} vs {most_similar[1]} com DTW = {most_similar[2]:.2f}")

# Plotar gráfico
plot_most_correlated(price_df, most_similar[0], most_similar[1])

# Gerar grafo com top 10% pares mais correlacionados
G = create_dtw_graph(dtw_results, top_percent=0.1)
plot_dwt_graph(G)

#cluster_dtw_and_plot_network(G, dtw_results, n_clusters=5)
