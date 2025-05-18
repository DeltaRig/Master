import pandas as pd
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.manifold import SpectralEmbedding
from sklearn.decomposition import PCA

# Parâmetros
CSV_PATH = "/home/dani/Documents/git/Master/finviz-platform/dados/acoes_ibov.csv"  # deve conter todas as séries com Date, Ticker, Adj Close
N_CLUSTERS = 7

# Preparar os dados
df = pd.read_csv(CSV_PATH, parse_dates=['Date'])
pivot_df = df.pivot(index='Date', columns='Ticker', values='Adj Close').dropna()

# Cada ticker é uma linha no X (séries temporais como vetores)
X = pivot_df.T.values
tickers = pivot_df.columns.tolist()

# Padronizar tabular data StandardScaler
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# KMeans
kmeans = KMeans(n_clusters=N_CLUSTERS, random_state=42, n_init='auto')
labels = kmeans.fit_predict(X_scaled)

# PCA para visualização 2D
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X_scaled)

# Plot
plt.figure(figsize=(10, 8))
for i in range(N_CLUSTERS):
    idx = labels == i
    plt.scatter(X_pca[idx, 0], X_pca[idx, 1], label=f'Cluster {i+1}', s=100)

for i, ticker in enumerate(tickers):
    plt.text(X_pca[i, 0], X_pca[i, 1], ticker, fontsize=8)

plt.title("K-Means Clustering de Ativos (k=7)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("kmeans_clusters.png")
plt.show()
