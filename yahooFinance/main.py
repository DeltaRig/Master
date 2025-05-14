import yfinance as yf # Make sure yfinance is installed: pip install yfinance

# List of ticker symbols
ticker_symbols = "AAPL"# Note: use .SA for Brazilian stocks like PETR4 on Yahoo Finance


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
print(f"Summary of Historical Data for {ticker}:")
print(historical_data)
