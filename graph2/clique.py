import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import sys

INPUT_FILE = sys.argv[1] if len(sys.argv) > 1 else "pearson_sorted.csv"
THRESHOLD = 0.9
IS_DISTANCE = False
NODE = 2

edges_df = pd.read_csv(INPUT_FILE)
if IS_DISTANCE:
    filtered = edges_df[edges_df['Correlation'] <= THRESHOLD]
else:
    filtered = edges_df[edges_df['Correlation'] >= THRESHOLD]

G = nx.Graph()
for _, row in filtered.iterrows():
    G.add_edge(row['Ticker1'], row['Ticker2'], weight=row['Correlation'])

cliques = list(nx.find_cliques(G))
biggest_clique = max(cliques, key=len)

pos = nx.spring_layout(G, seed=42)
plt.figure(figsize=(10, 8))
nx.draw(G, pos, with_labels=True, node_color='lightblue', edge_color='gray')
nx.draw_networkx_nodes(G, pos, nodelist=biggest_clique, node_color='orange')
plt.title("Maior Clique de Tickers")
plt.savefig("clique.png")
plt.show()