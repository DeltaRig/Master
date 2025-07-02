import matplotlib.pyplot as plt

# Read k-distances from file using open()
with open("dtw_result.csv", "r") as f:
    lines = f.readlines()

# Parse each line into a float, skipping empty or malformed lines
dists = [float(line.strip().split(';')[2]) for line in lines if line.strip()]

# Sort descending
dists_sorted = sorted(dists, reverse=True)

# Plot
plt.plot(dists_sorted)
plt.xlabel("Point index (sorted)")
plt.ylabel("distance")
plt.title("distance graph")
plt.grid(True)
plt.show()
plt.savefig('distance.png')
