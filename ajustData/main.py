import pandas as pd
from sklearn.preprocessing import MinMaxScaler

# Load CSV file
df = pd.read_csv('google.csv')

# Apply Min-Max normalization to 'Close' column per ticker
def normalize_close(group):
    scaler = MinMaxScaler()
    group['Close'] = scaler.fit_transform(group[['Close']])
    return group

# Group by Ticker and normalize
df_normalized = df.groupby('Ticker').apply(normalize_close)

# Save to new CSV file
df_normalized.to_csv('normalized_close.csv', index=False)
