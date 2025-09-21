# Yahoo Finance

> $ pip install yfinance


*Main components*
* Ticker: single ticker data

* Tickers: multiple tickers' data

* download: download market data for multiple tickers

* Market: get information about a market

* WebSocket and AsyncWebSocket: live streaming data

* Search: quotes and news from search

* Sector and Industry: sector and industry information

* EquityQuery and Screener: build query to screen market


# Project files
## colector.py
Collects information from Yahoo Finance for different periods:

- Last 2 years (daily)

- Last week (hourly)

- Last day (minute)

Saves raw data in data/ and normalized data in normalize/.

## normalize.py
Normalizes the close column from files collected by colector.py.

## dtw.py
Compares time series using Dynamic Time Warping (DTW), with C-based performance.

## streamlit/
Contains visualization scripts using Streamlit.