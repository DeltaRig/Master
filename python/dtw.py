import sys
import time
import pandas as pd
from itertools import combinations
from fastdtw import fastdtw
from scipy.spatial.distance import euclidean

# Ensure correct arguments
if len(sys.argv) < 3:
    print("Usage: python dtw.py <filename.csv> <number_of_stocks>")
    sys.exit(1)

# Read command line arguments
filename = sys.argv[1]
num_assets = int(sys.argv[2])

# Start timer
start_time = time.time()

# Read CSV file
try:
    df = pd.read_csv(filename)
except Exception as e:
    print(f"Error reading file: {e}")
    sys.exit(1)

# Ensure correct columns
if "Ticker" not in df.columns or "Adj Close" not in df.columns:
    print("CSV must contain 'Ticker' and 'Adj Close' columns")
    sys.exit(1)

# Get all unique tickers
all_assets = df["Ticker"].unique()

# If requested number is larger than available, take all
assets = all_assets[:min(num_assets, len(all_assets))]

# Store adjusted close price series for each asset
series_dict = {}
for asset in assets:
    series = df[df["Ticker"] == asset].sort_values("Date")["Adj Close"].tolist()
    series_dict[asset] = series

print(f"Computing DTW distances for {len(assets)} assets...")
print(series_dict.keys())
import numpy as np

import numpy as np

# Compute DTW distances
results = []
for a1, a2 in combinations(assets, 2):
    # Wrap each value as a list, so it's treated as 1-D
    x = np.array([[v] for v in series_dict[a1]], dtype=float)
    y = np.array([[v] for v in series_dict[a2]], dtype=float)

    distance, _ = fastdtw(x, y, dist=euclidean)
    results.append((a1, a2, distance))



# Save results to CSV (semicolon separated)
with open("dtw_result_py.csv", "w") as f:
    for a1, a2, dist in results:
        f.write(f"{a1}; {a2}; {dist:.6f};\n")

# End timer
end_time = time.time()
execution_time = end_time - start_time
print(f"Execution time: {execution_time:.2f} seconds")
