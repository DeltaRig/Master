import streamlit as st
import pandas as pd
import plotly.graph_objects as go
from pathlib import Path

# Set page configuration
st.set_page_config(
    page_title="Ticker Similarity Viewer",
    page_icon="📈",
    layout="wide"
)

# File paths
DTW_FILE = Path(__file__).parent.parent / "../dtw" / "master_tickers_br_EUA_2years_1d.csv"
ORIGINAL_DATA_FILE = Path(__file__).parent.parent / "../data" / "master_tickers_br_EUA_2years_1d.csv"
NORMALIZED_DATA_FILE = Path(__file__).parent.parent / "../normalized" / "master_tickers_br_EUA_2years_1d.csv"

@st.cache_data
def load_dtw_distances():
    df = pd.read_csv(
        DTW_FILE,
        sep=';',
        header=None,
        names=['ticker1', 'ticker2', 'distance', 'extra'],
        skipinitialspace=True
    )

    df = df[['ticker1', 'ticker2', 'distance']]

    df['ticker1'] = df['ticker1'].astype(str).str.strip()
    df['ticker2'] = df['ticker2'].astype(str).str.strip()

    df['distance'] = pd.to_numeric(df['distance'], errors='coerce')

    df = df.dropna(subset=['distance'])

    all_tickers = sorted(
        set(df['ticker1'])
        | set(df['ticker2'])
    )

    return df, all_tickers

@st.cache_data
def load_time_series(file_path):
    """Load time series data."""
    df = pd.read_csv(file_path)
    df['DateTime'] = pd.to_datetime(df['DateTime'])
    return df

def find_similar_tickers(dtw_df, selected_ticker, n=1):
    """Find the n most similar and least similar tickers."""
    # Filter rows where selected ticker is either ticker1 or ticker2
    filtered = dtw_df[(dtw_df['ticker1'] == selected_ticker) | (dtw_df['ticker2'] == selected_ticker)].copy()
    
    # Normalize to always have selected_ticker as ticker1
    filtered['other_ticker'] = filtered.apply(
        lambda row: row['ticker2'] if row['ticker1'] == selected_ticker else row['ticker1'],
        axis=1
    )
    
    # Sort by distance
    filtered = filtered.sort_values('distance')
    
    # Get most similar (lowest distance) and least similar (highest distance)
    most_similar = filtered.head(n)['other_ticker'].tolist()
    least_similar = filtered.tail(n)['other_ticker'].tolist()
    
    return most_similar, least_similar

def plot_ticker_comparison(ticker1, ticker2, data_df, title_suffix):
    """Create a line plot comparing two tickers."""
    # Filter data for both tickers
    df1 = data_df[data_df['Ticker'] == ticker1].sort_values('DateTime')
    df2 = data_df[data_df['Ticker'] == ticker2].sort_values('DateTime')
    
    fig = go.Figure()
    
    # Add ticker1
    fig.add_trace(go.Scatter(
        x=df1['DateTime'],
        y=df1['Close'],
        mode='lines',
        name=f'{ticker1} (Close)',
        line=dict(color='blue')
    ))
    
    # Add ticker2
    fig.add_trace(go.Scatter(
        x=df2['DateTime'],
        y=df2['Close'],
        mode='lines',
        name=f'{ticker2} (Close)',
        line=dict(color='red')
    ))
    
    fig.update_layout(
        title=f'{ticker1} vs {ticker2} - {title_suffix}',
        xaxis_title='DateTime',
        yaxis_title='Close Price',
        hovermode='x unified',
        height=400
    )
    
    return fig

def main():
    st.title("📈 Ticker Similarity Viewer")
    st.markdown("This tool helps you find the most similar and least similar tickers based on DTW distance.")
    
    # Load data
    with st.spinner("Loading data..."):
        dtw_df, all_tickers = load_dtw_distances()
        original_df = load_time_series(ORIGINAL_DATA_FILE)
        normalized_df = load_time_series(NORMALIZED_DATA_FILE)
    
    st.success(f"Loaded {len(all_tickers)} tickers and DTW distances!")
    
    # Ticker selection
    selected_ticker = st.selectbox(
        "Select a ticker to find similar tickers:",
        all_tickers,
        index=0
    )
    
    # Find similar and dissimilar tickers
    most_similar, least_similar = find_similar_tickers(dtw_df, selected_ticker, n=1)
    
    if most_similar and least_similar:
        similar_ticker = most_similar[0]
        dissimilar_ticker = least_similar[0]
        
        # Display results
        col1, col2 = st.columns(2)
        
        with col1:
            st.info(f"**Most Similar:** {similar_ticker}")
        
        with col2:
            st.warning(f"**Least Similar:** {dissimilar_ticker}")
        
        # Original data plots
        st.header("Original Data Comparison")
        
        col1, col2 = st.columns(2)
        
        with col1:
            st.subheader(f"{selected_ticker} vs {similar_ticker}")
            fig_orig_similar = plot_ticker_comparison(selected_ticker, similar_ticker, original_df, "Original Data")
            st.plotly_chart(fig_orig_similar, use_container_width=True)
        
        with col2:
            st.subheader(f"{selected_ticker} vs {dissimilar_ticker}")
            fig_orig_dissimilar = plot_ticker_comparison(selected_ticker, dissimilar_ticker, original_df, "Original Data")
            st.plotly_chart(fig_orig_dissimilar, use_container_width=True)
        
        # Normalized data plots
        st.header("Normalized Data Comparison")
        
        col1, col2 = st.columns(2)
        
        with col1:
            st.subheader(f"{selected_ticker} vs {similar_ticker}")
            fig_norm_similar = plot_ticker_comparison(selected_ticker, similar_ticker, normalized_df, "Normalized Data")
            st.plotly_chart(fig_norm_similar, use_container_width=True)
        
        with col2:
            st.subheader(f"{selected_ticker} vs {dissimilar_ticker}")
            fig_norm_dissimilar = plot_ticker_comparison(selected_ticker, dissimilar_ticker, normalized_df, "Normalized Data")
            st.plotly_chart(fig_norm_dissimilar, use_container_width=True)
        
        # DTW distance info
        st.header("DTW Distance Information")
        
        # Get distances
        similar_distance = dtw_df[
            ((dtw_df['ticker1'] == selected_ticker) & (dtw_df['ticker2'] == similar_ticker)) |
            ((dtw_df['ticker2'] == selected_ticker) & (dtw_df['ticker1'] == similar_ticker))
        ]['distance'].values[0]
        
        dissimilar_distance = dtw_df[
            ((dtw_df['ticker1'] == selected_ticker) & (dtw_df['ticker2'] == dissimilar_ticker)) |
            ((dtw_df['ticker2'] == selected_ticker) & (dtw_df['ticker1'] == dissimilar_ticker))
        ]['distance'].values[0]
        
        col1, col2 = st.columns(2)
        
        with col1:
            st.metric(f"DTW Distance to {similar_ticker}", f"{similar_distance:.4f}")
        
        with col2:
            st.metric(f"DTW Distance to {dissimilar_ticker}", f"{dissimilar_distance:.4f}")

if __name__ == "__main__":
    main()
