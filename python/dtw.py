import sys
import time
import pandas as pd
from itertools import combinations
from dtaidistance import dtw
import numpy as np

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
if "Ticker" not in df.columns or "Adj Close" not in df.columns or "Date" not in df.columns:
    print("CSV must contain 'Date', 'Ticker' and 'Adj Close' columns")
    sys.exit(1)

# Get all unique tickers
all_assets = df["Ticker"].unique()

# If requested number is larger than available, take all
assets = all_assets[:min(num_assets, len(all_assets))]

# Store adjusted close price series for each asset
series_dict = {}
for asset in assets:
    series = df[df["Ticker"] == asset].sort_values("Date")["Close"].astype(float).tolist()
    series_dict[asset] = np.array(series, dtype=float)

print(f"Computing DTW distances for {len(assets)} assets...")

# Compute DTW distances
results = []
for a1, a2 in combinations(assets, 2):
    x = series_dict[a1]
    y = series_dict[a2]

    # dtaidistance fast DTW
    distance = dtw.distance_fast(x, y)  
    results.append((a1, a2, distance, min(len(x), len(y))))

# Save results to CSV (semicolon separated)
with open("dtw_result_py.csv", "w") as f:
    for a1, a2, dist, _ in results:
        f.write(f"{a1}; {a2}; {dist:.6f};\n")

# Save normalized results to CSV (by min length of series)
with open("dtw_result_norm_py.csv", "w") as f:
    for a1, a2, dist, path_len in results:
        f.write(f"{a1}; {a2}; {dist/path_len:.6f};\n")

# End timer
end_time = time.time()
execution_time = end_time - start_time
print(f"Execution time: {execution_time:.2f} seconds")
