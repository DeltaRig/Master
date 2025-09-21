import streamlit as st
import pandas as pd
import plotly.graph_objects as go
import os

# ==============================
# CONFIG
# ==============================
DATA_DIR = "../data"
NORM_DIR = "../normalized"
DTW_DIR = "../dtw"

PERIODS = {
    "2 years (1d)": "test_m_2years_1d.csv",
    "7 days (1h)": "test_m_7days_1h.csv",
    "24 hours (1m)": "test_m_24hours_1m.csv"
}

# ==============================
# HELPERS
# ==============================
@st.cache_data
def load_data(filename):
    return pd.read_csv(filename, parse_dates=["DateTime"])

@st.cache_data
def load_dtw(filename):
    path = os.path.join(DTW_DIR, filename)
    return pd.read_csv(path, sep=";", names=["Ticker1", "Ticker2", "Distance"])
'''
When you use return pd.read_csv(path, sep=";", names=["Ticker1", "Ticker2", "Distance"])
         Ticker1   Ticker2  Distance
ABOS        ACAD  3.961624       NaN
ABOS        CMPX  5.828242       NaN
ABOS   GOGL34.SA  6.726583       NaN
ABOS        GOOG  5.953081       NaN
ABOS       GOOGL  5.950151       NaN
'''

def plot_candlestick(df, ticker, title=""):
    df_t = df[df["Ticker"] == ticker].copy()
    if df_t.empty:
        st.warning(f"No data for {ticker}")
        return None

    fig = go.Figure(
        data=[go.Candlestick(
            x=df_t["DateTime"],
            open=df_t["Open"],
            high=df_t["High"],
            low=df_t["Low"],
            close=df_t["Close"],
            name=ticker
        )]
    )
    fig.update_layout(title=title, xaxis_rangeslider_visible=False)
    return fig

def plot_overlay(df, ticker1, ticker2):
    df1 = df[df["Ticker"] == ticker1]
    df2 = df[df["Ticker"] == ticker2]

    if df1.empty or df2.empty:
        st.warning("Not enough data to plot overlay")
        return None

    merged = pd.merge(df1[["DateTime", "Close"]],
                      df2[["DateTime", "Close"]],
                      on="DateTime", suffixes=(f"_{ticker1}", f"_{ticker2}"))

    fig = go.Figure()
    fig.add_trace(go.Scatter(x=merged["DateTime"], y=merged[f"Close_{ticker1}"],
                             mode="lines", name=ticker1))
    fig.add_trace(go.Scatter(x=merged["DateTime"], y=merged[f"Close_{ticker2}"],
                             mode="lines", name=ticker2))

    fig.update_layout(title=f"Overlay: {ticker1} vs {ticker2}",
                      xaxis_title="Date", yaxis_title="Close")
    return fig

# ==============================
# STREAMLIT APP
# ==============================
st.set_page_config(page_title="📊 Asset Similarity Explorer", layout="wide")
st.title("📊 Asset Similarity Explorer (DTW-based)")

# --- Select period
period_label = st.selectbox("Select period", list(PERIODS.keys()))
filename = PERIODS[period_label]

df = load_data(os.path.join(DATA_DIR, filename))
df_norm = load_data(os.path.join(NORM_DIR, filename))
df_dtw = load_dtw(filename)

tickers = sorted(df["Ticker"].unique())
ticker = st.selectbox("Choose a ticker", tickers)

# --- Candlestick
st.subheader(f"Candlestick chart: {ticker}")
fig_candle = plot_candlestick(df, ticker, title=f"{ticker} - {period_label}")
if fig_candle:
    st.plotly_chart(fig_candle, use_container_width=True)

# --- Similarity ranking
st.subheader(f"DTW Similarity for {ticker}")

subset = df_dtw[(df_dtw["Ticker1"] == ticker) | (df_dtw["Ticker2"] == ticker)].copy()
subset["Other"] = subset.apply(lambda r: r["Ticker2"] if r["Ticker1"] == ticker else r["Ticker1"], axis=1)
subset = subset[["Other", "Distance"]].sort_values("Distance")

col1, col2 = st.columns(2)

with col1:
    st.write("✅ Most similar (lowest DTW)")
    st.dataframe(subset.head(5))

with col2:
    st.write("❌ Least similar (highest DTW)")
    st.dataframe(subset.tail(5))

# --- Compare two tickers
st.subheader("Compare two tickers")

ticker1 = st.selectbox("Ticker 1", tickers, index=0)
ticker2 = st.selectbox("Ticker 2", tickers, index=1)

fig_overlay = plot_overlay(df_norm, ticker1, ticker2)
if fig_overlay:
    st.plotly_chart(fig_overlay, use_container_width=True)

# Show DTW value if available
dist_val = df_dtw[((df_dtw["Ticker1"] == ticker1) & (df_dtw["Ticker2"] == ticker2)) |
                  ((df_dtw["Ticker1"] == ticker2) & (df_dtw["Ticker2"] == ticker1))]
if not dist_val.empty:
    st.success(f"DTW distance between **{ticker1}** and **{ticker2}**: {dist_val['Distance'].values[0]:.4f}")
else:
    st.warning("No DTW distance available for this pair.")
