# Ticker Similarity Viewer

A Streamlit application for visualizing ticker similarity based on Dynamic Time Warping (DTW) distance. This tool allows you to select a ticker and find the most similar and least similar tickers, with visualizations comparing their price movements.

## Features

- **Ticker Selection**: Choose from available tickers in the dataset
- **Similarity Analysis**: Automatically finds the most similar and least similar tickers based on DTW distance
- **Dual Visualization**: 
  - Original price data comparison
  - Normalized price data comparison
- **Interactive Charts**: Plotly-powered interactive graphs for detailed analysis

## Prerequisites

- Python 3.8 or higher
- Required data files (must exist in the project structure):
  - `../dtw/master_tickers_br_EUA_2years_1d.csv` - DTW distance matrix
  - `../data/master_tickers_br_EUA_2years_1d.csv` - Original time series data
  - `../normalized/master_tickers_br_EUA_2years_1d.csv` - Normalized time series data

## Installation

1. Navigate to the streamlit-visu directory:
```bash
cd python/streamlit-visu
```

2. Install the required dependencies:
```bash
pip install -r requirements.txt
```

## Running the Application

Start the Streamlit application:

```bash
streamlit run app.py
```

The application will automatically open in your default web browser (typically at `http://localhost:8501`).

## Usage

1. **Select a Ticker**: Use the dropdown menu to select a ticker from the available options
2. **View Results**: The app will automatically display:
   - The most similar ticker (lowest DTW distance)
   - The least similar ticker (highest DTW distance)
3. **Explore Visualizations**: 
   - View original price comparisons in the top section
   - View normalized price comparisons in the bottom section
   - Check the DTW distance metrics at the bottom

## Data Files

The application expects the following data files in the parent directories:

- **DTW Distance File** (`../dtw/master_tickers_br_EUA_2years_1d.csv`): Contains pairwise DTW distances between tickers in semicolon-separated format: `ticker1; ticker2; distance`

- **Original Data File** (`../data/master_tickers_br_EUA_2years_1d.csv`): Contains original time series data with columns: `DateTime, Open, High, Low, Close, Adj Close, Volume, Ticker`

- **Normalized Data File** (`../normalized/master_tickers_br_EUA_2years_1d.csv`): Contains normalized time series data with the same structure as the original file

## Dependencies

- streamlit>=1.28.0
- pandas>=2.0.0
- plotly>=5.17.0

## Notes

- The application uses Streamlit's caching to improve performance when loading large datasets
- All visualizations are interactive and can be zoomed, panned, and hovered for detailed information
- The DTW distance is calculated based on the close price time series
