import os
import pandas as pd
import yfinance as yf
from sklearn.preprocessing import MinMaxScaler
from datetime import datetime, timedelta, timezone

# --------------------------
# CONFIG
# --------------------------
sample = ['GOGL34.SA', 'GOOGL', 'GOOG', 'PETR3.SA', 'PETR4.SA']
configs = [
    ("10years", "1d", timedelta(days=365*10)),
    ("7days", "1h", timedelta(days=7)),
    ("24hours", "1m", timedelta(days=1))
]

# --------------------------
# HELPERS
# --------------------------
def build_filename(period_name, list_name, interval):
    return f"{period_name}_{list_name}_{interval}.csv"

def normalize_close(df, filename):
    def _normalize(group):
        scaler = MinMaxScaler()
        group['Close'] = scaler.fit_transform(group[['Close']])
        return group

    df_norm = df.groupby('Ticker').apply(_normalize, include_groups=False)
    df_norm.to_csv(f"normalized_{filename}", index=False)
    print(f"✅ Normalized saved: normalized_{filename}")

def download_or_update(period_name, list_name, tickers, interval, keep_window):
    filename = build_filename(period_name, list_name, interval)

    if os.path.exists(filename):
        df = pd.read_csv(filename, parse_dates=["DateTime"])
        last_date = df["DateTime"].max()
        start = last_date + pd.Timedelta(seconds=1)
        print(f"🔄 Updating {filename} from {start}...")
    else:
        df = pd.DataFrame()
        start = datetime.now(timezone.utc) - keep_window
        print(f"⬇️ Creating {filename} from scratch, starting {start}...")

    end = datetime.now(timezone.utc)

    # Download from yfinance
    data = yf.download(
        tickers, start=start, end=end,
        interval=interval, group_by="ticker",
        auto_adjust=False, threads=True
    )

    if data.empty:
        print(f"⚠️ No new data for {filename}")
        return df

    # Flatten if multiple tickers
    if len(tickers) > 1:
        records = []
        for t in tickers:
            if t not in data.columns.levels[0]:
                continue
            df_t = data[t].reset_index()
            df_t['Ticker'] = t
            records.append(df_t)
        new_data = pd.concat(records)
    else:
        new_data = data.reset_index()
        new_data['Ticker'] = tickers[0]

    # Ensure DateTime in UTC
    new_data.rename(columns={"Datetime": "DateTime", "Date": "DateTime"}, inplace=True)
    new_data["DateTime"] = pd.to_datetime(new_data["DateTime"], utc=True)

    # Drop rows with missing OHLC
    new_data = new_data.dropna(subset=["Open", "High", "Low", "Close"], how="all")

    # Merge with existing
    df = pd.concat([df, new_data], ignore_index=True)

    # Drop duplicates
    df = df.drop_duplicates(subset=["DateTime", "Ticker"]).sort_values(["DateTime", "Ticker"])

    # Keep only rolling window
    cutoff = datetime.now(timezone.utc) - keep_window
    df = df[df["DateTime"] >= cutoff]

    # Add Date + Time split (but keep DateTime too)
    df["Date"] = df["DateTime"].dt.date
    if interval in ["1h", "1m"]:
        df["Time"] = df["DateTime"].dt.time

    # Save
    df.to_csv(filename, index=False)
    print(f"💾 Saved {filename} with {len(df)} rows")

    # Normalize
    normalize_close(df.copy(), filename)
    return df

# --------------------------
# RUN PIPELINE
# --------------------------
for period_name, interval, window in configs:
    df_final = download_or_update(period_name, "sample", sample, interval, window)

# delete old data
# 