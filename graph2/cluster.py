import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import sys

# Parâmetros
INPUT_FILE = sys.argv[1] if len(sys.argv) > 1 else "pearson_sorted.csv"
THRESHOLD = 0.9  # mínimo absoluto de correlação
USE_ABS = True   # se True, usa abs(corr) para distância

edges_df = pd.read_csv(INPUT_FILE)
if USE_ABS:
    edges_df['AbsCorr'] = edges_df['Correlation'].abs()
    edges_df['Distance'] = 1 - edges_df['AbsCorr']
else:
    edges_df['Distance'] = 1 - edges_df['Correlation']

# Filtro por correlação (forte)
filtered = edges_df[edges_df['AbsCorr'] >= THRESHOLD]

# Criar grafo ponderado
G = nx.Graph()
for _, row in filtered.iterrows():
    G.add_edge(row['Ticker1'], row['Ticker2'], weight=row['AbsCorr'], distance=row['Distance'])

# https://networkx.org/documentation/networkx-2.7.1/reference/algorithms/generated/networkx.algorithms.community.modularity_max.greedy_modularity_communities.html
# Clustering com pesos (ex: greedy modularity)
from networkx.algorithms.community import greedy_modularity_communities
clusters = list(greedy_modularity_communities(G, weight='weight'))

# Layout ponderado
pos = nx.spring_layout(G, seed=42, weight='distance')
plt.figure(figsize=(10, 8))
colors = plt.cm.tab10.colors
for i, cluster in enumerate(clusters):
    nx.draw_networkx_nodes(G, pos, nodelist=list(cluster), node_color=[colors[i % len(colors)]], label=f'Cluster {i+1}')
nx.draw_networkx_edges(G, pos, alpha=0.4)
nx.draw_networkx_labels(G, pos, font_size=9)
plt.title("Clusters de Ativos com Peso por Correlação")
plt.axis('off')
plt.legend()
plt.tight_layout()
plt.savefig("clusters_weighted.png")
plt.show()
