import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.manifold import MDS

# Load distance matrix
dist_df = pd.read_csv("dtw_distance_matrix.csv", index_col=0)
labels_df = pd.read_csv("dtw_kmedoids_clusters.csv")

tickers = dist_df.index.to_list()
dist_matrix = dist_df.to_numpy()

# Reduce to 2D
# Multidimensional Scaling (MDS)
mds = MDS(n_components=2, dissimilarity='precomputed', random_state=42)
coords = mds.fit_transform(dist_matrix)

# Merge with labels
coords_df = pd.DataFrame(coords, columns=['x', 'y'])
coords_df['Ticker'] = tickers
coords_df = coords_df.merge(labels_df, on='Ticker')

# Plot
plt.figure(figsize=(10, 7))
for cluster_id in sorted(coords_df['Cluster'].unique()):
    group = coords_df[coords_df['Cluster'] == cluster_id]
    plt.scatter(group['x'], group['y'], label=f"Cluster {cluster_id}")
    for _, row in group.iterrows():
        plt.text(row['x'] + 0.5, row['y'], row['Ticker'], fontsize=8)

plt.legend()
plt.title("DTW Clustering of Tickers")
plt.savefig("dtw_clusters.png", dpi=300)
plt.show()
