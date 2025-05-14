import os
import pandas as pd
from datetime import datetime, timedelta
import yfinance as yf # Make sure yfinance is installed: pip install yfinance

# List of ticker symbols
ticker_symbols = ["AAPL", "PETR4.SA"]  # Note: use .SA for Brazilian stocks like PETR4 on Yahoo Finance

# Directory to store CSVs
csv_dir = "data"
os.makedirs(csv_dir, exist_ok=True)

# Main function to sync ticker data
def sync_ticker_data(tickers , csv_path):
    if os.path.exists(csv_path):
        # Load existing data
        existing_df = pd.read_csv(csv_path, parse_dates=['Date'], index_col='Date')
        last_date = existing_df.index[-1].date()
        fetch_start_date = last_date + timedelta(days=1)
    else:
        existing_df = None
        fetch_start_date = None


    # Download new data
    new_data = ticker.history(period="50y")
    # yf_download(ticker, start=fetch_start_date, progress=False)

    if new_data.empty:
        print(f"No new data for {ticker}")
        return

    # Combine with existing data
    new_data.index.name = 'Date'
    if existing_df is not None:
        combined_df = pd.concat([existing_df, new_data])
        combined_df = combined_df[~combined_df.index.duplicated(keep='last')]
    else:
        combined_df = new_data

    combined_df.sort_index(inplace=True)
    combined_df.to_csv(csv_path)
    print(f"Updated data for {ticker} → {csv_path}")


# Create a Ticker object
for t in ticker_symbols:
    ticker = yf.Ticker(t)

    csv_path = os.path.join(csv_dir, f"{t.replace('.', '_')}.csv")

    sync_ticker_data(ticker, csv_path)

"""
# Create a Ticker object
ticker = yf.Ticker(ticker_symbols)

# Fetch historical market data
historical_data = ticker.history(period="1y")  # data for the last year


print("Historical Data:")
print(historical_data)

# Fetch basic financials
financials = ticker.financials
print("\nFinancials:")
print(financials)

# Fetch stock actions like dividends and splits
actions = ticker.actions
print("\nStock Actions:")
print(actions)


# Fetch historical market data for the last 30 days
historical_data = ticker.history(period="1mo")  # data for the last month
# Display a summary of the fetched data
print(f"Summary of Historical Data for {ticker_symbol}:")
print(historical_data)

"""