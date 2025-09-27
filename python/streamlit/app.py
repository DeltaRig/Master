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
    "6 months (4h)": "test_m_6months_4h.csv",
    "7 days (60m)": "test_m_7days_60m.csv",
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
    df = pd.read_csv(path, sep=";", names=["Ticker1", "Ticker2", "Distance", ""], header=None)
    df["Ticker2"] = df["Ticker2"].str.strip()
    return df

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

# --- Select ticker first, then period ---
if "ticker" not in st.session_state:
    st.session_state.ticker = None

# Load all tickers from all periods for initial selection
all_tickers = set()
for file in PERIODS.values():
    df_tmp = load_data(os.path.join(DATA_DIR, file))
    all_tickers.update(df_tmp["Ticker"].unique())
all_tickers = sorted(all_tickers)

ticker = st.selectbox(
    "Choose a ticker", all_tickers,
    index=all_tickers.index(st.session_state.ticker) if st.session_state.ticker in all_tickers else 0,
    key="ticker"
)

period_label = st.selectbox("Select period", list(PERIODS.keys()))

filename = PERIODS[period_label]
df = load_data(os.path.join(DATA_DIR, filename))
tickers = sorted(df["Ticker"].unique())

# If the selected ticker is not in the current period, show warning and stop
if ticker not in tickers:
    st.warning(f"No information for ticker **{ticker}** in period **{period_label}**.")
    st.stop()

# Load data
df_norm = load_data(os.path.join(NORM_DIR, filename))
df_dtw = load_dtw(filename)


# --- Candlestick
st.subheader(f"Candlestick chart: {ticker}")
fig_candle = plot_candlestick(df, ticker, title=f"{ticker} - {period_label}")
if fig_candle:
    st.plotly_chart(fig_candle, use_container_width=True)

# --- Similarity ranking with DTW distances for all periods
st.subheader(f"DTW Similarity for {ticker}")

subset = df_dtw[(df_dtw["Ticker2"] == ticker) | (df_dtw["Ticker1"] == ticker)].copy()
subset["Other"] = subset.apply(
    lambda r: r["Ticker2"] if r["Ticker1"] == ticker else r["Ticker1"],
    axis=1
)
subset = subset[["Other"]].copy()  # Remove 'Distance' column

# Add DTW distances for all periods with readable column names
for label, file in PERIODS.items():
    df_dtw_period = load_dtw(file)
    subset[label] = subset["Other"].apply(
        lambda other: df_dtw_period[
            ((df_dtw_period["Ticker1"] == ticker) & (df_dtw_period["Ticker2"] == other)) |
            ((df_dtw_period["Ticker1"] == other) & (df_dtw_period["Ticker2"] == ticker))
        ]["Distance"].values[0] if not df_dtw_period[
            ((df_dtw_period["Ticker1"] == ticker) & (df_dtw_period["Ticker2"] == other)) |
            ((df_dtw_period["Ticker1"] == other) & (df_dtw_period["Ticker2"] == ticker))
        ].empty else None
    )

def highlight_period(data):
    # Highlights the selected period column
    return pd.DataFrame(
        '', index=data.index, columns=data.columns
    ).assign(**{period_label: 'background-color: #006326'})

most_similar = subset.sort_values(period_label).head(5)
least_similar = subset.sort_values(period_label, ascending=False).head(5)

col1, col2 = st.columns(2)

with col1:
    st.write("✅ Most similar (lowest DTW)")
    st.dataframe(most_similar.style.apply(highlight_period, axis=None))

with col2:
    st.write("❌ Least similar (highest DTW)")
    st.dataframe(least_similar.style.apply(highlight_period, axis=None))

# --- Compare two tickers normalized
st.subheader("Compare two tickers")

ticker1 = st.selectbox("Ticker 1", tickers, index=0)
ticker2 = st.selectbox("Ticker 2", tickers, index=1)

fig_overlay = plot_overlay(df_norm, ticker1, ticker2)
if fig_overlay:
    st.plotly_chart(fig_overlay, use_container_width=True)

# --- Gráfico de sobreposição (usando dados originais, não normalizados)
fig_overlay = plot_overlay(df, ticker, ticker2)
if fig_overlay:
    st.plotly_chart(fig_overlay, use_container_width=True)


# Show DTW value if available for selected period
dtw_rows = []
for label, file in PERIODS.items():
    df_dtw_period = load_dtw(file)
    dist_val_period = df_dtw_period[
        ((df_dtw_period["Ticker1"] == ticker1) & (df_dtw_period["Ticker2"] == ticker2)) |
        ((df_dtw_period["Ticker1"] == ticker2) & (df_dtw_period["Ticker2"] == ticker1))
    ]
    if not dist_val_period.empty:
        dtw_rows.append({"Period": label, f"DTW Distance {ticker1} and {ticker2}": dist_val_period['Distance'].values[0]})
    else:
        dtw_rows.append({"Period": label, f"DTW Distance {ticker1} and {ticker2}": "N/A"})

dtw_df = pd.DataFrame(dtw_rows)
st.markdown("### DTW distance for all periods")
st.dataframe(dtw_df)

