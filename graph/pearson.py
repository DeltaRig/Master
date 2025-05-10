import os
import pandas as pd
import networkx as nx
from glob import glob

def load_close_prices(csv_folder):
    series = {}
    for file in glob(os.path.join(csv_folder, "acoes_ibov.csv")):
        df = pd.read_csv(file, parse_dates=['Date'])
        ticker = df['Ticker'].iloc[0]
        print(ticker)
        df = df.set_index('Date').sort_index()
        series[ticker] = df['Adj Close']
    return pd.DataFrame(series).dropna()

def build_correlation_graph(price_df, threshold=0.9):
    corr = price_df.corr()
    print(price_df)
    print(corr)
    G = nx.Graph()
    for col in corr.columns:
        G.add_node(col)
    for i in corr.columns:
        for j in corr.columns:
            if i != j and corr.loc[i, j] >= threshold:
                G.add_edge(i, j, weight=round(corr.loc[i, j], 2))
    return G

def graph_to_dot(G):
    dot = "graph G {\n"
    for u, v, d in G.edges(data=True):
        dot += f'    "{u}" -- "{v}" [label="{d["weight"]}"];\n'
    dot += "}"
    return dot

# Example usage
price_df = load_close_prices("/home/dani/Documents/git/Master/finviz-platform/dados_1/")  # path to your CSVs
G_corr = build_correlation_graph(price_df, threshold=0.9)
dot_corr = graph_to_dot(G_corr)
print(dot_corr)
