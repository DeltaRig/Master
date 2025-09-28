import os
import pandas as pd
import yfinance as yf
from sklearn.preprocessing import MinMaxScaler
from datetime import datetime, timedelta, timezone

# --------------------------
# CONFIG
# --------------------------
configs = [
    # period_name, interval, window, minimum 
    # stock market need be open
    ("2years", "1d", timedelta(days=365*2), 1),  # 245 days * 2 years - 10% buffer
    ("6months", "4h", "6mo", 60),  # 22 days * 6 months - 40% buffer
    ("7days", "60m", "1wk", 1),
    ("24hours", "1m", "1d", (1440*0.1))  # 1440 minutes in a day - 10% buffer
]

# ============================================
# LISTAS DE TICKERS (MANTIDAS)
# ============================================
ibov_tickers = [
    "ALOS3.SA", "ABEV3.SA", "ASAI3.SA", "AURE3.SA", "AMOB3.SA", "AZUL4.SA", "AZZA3.SA", "B3SA3.SA",
    "BBSE3.SA", 
    #"BBDC3.SA", 
    "BBDC4.SA", "BRAP4.SA", "BBAS3.SA", "BRKM5.SA", "BRAV3.SA", "BRFS3.SA",
    "BPAC11.SA", "CXSE3.SA", "CRFB3.SA", "CCRO3.SA", "CMIG4.SA", "COGN3.SA", "CPLE6.SA", "CSAN3.SA",
    "CPFE3.SA", "CMIN3.SA", "CVCB3.SA", "CYRE3.SA", 
    #"ELET3.SA",
    "ELET6.SA", "EMBR3.SA", "ENGI11.SA",
    "ENEV3.SA", "EGIE3.SA", "EQTL3.SA", "FLRY3.SA", "GGBR4.SA", "GOAU4.SA", "NTCO3.SA", "HAPV3.SA",
    "HYPE3.SA", "IGTI11.SA", "IRBR3.SA", "ISAE4.SA", "ITSA4.SA", "ITUB4.SA", "JBSS3.SA", "KLBN11.SA",
    "RENT3.SA", "LREN3.SA", "LWSA3.SA", "MGLU3.SA", "POMO4.SA", "MRFG3.SA", "BEEF3.SA", "MRVE3.SA",
    "MULT3.SA", "PCAR3.SA", 
    #"PETR3.SA",
    "PETR4.SA", "RECV3.SA", "PRIO3.SA", "PETZ3.SA", "PSSA3.SA",
    "RADL3.SA", "RAIZ4.SA", "RDOR3.SA", "RAIL3.SA", "SBSP3.SA", "SANB11.SA", "STBP3.SA", "SMTO3.SA",
    "CSNA3.SA", "SLCE3.SA", "SUZB3.SA", "TAEE11.SA", "VIVT3.SA", "TIMS3.SA", "TOTS3.SA", "UGPA3.SA",
    "USIM5.SA", "VALE3.SA", "VAMO3.SA", "VBBR3.SA", "VIVA3.SA", "WEGE3.SA", "YDUQ3.SA"
]

bdrs_tickers = [
    "JPMC34.SA", "BKNG34.SA", "BERK34.SA", "TSMC34.SA", "NFLX34.SA", "GOGL34.SA", "MELI34.SA", "M1TA34.SA",
    "RIGG34.SA", "CHVX34.SA", "INBR32.SA", "TSLA34.SA", "MSFT34.SA", "IVVB11.SA", "VISA34.SA", "BOAC34.SA",
    "GMCO34.SA", "COCA34.SA", "MSCD34.SA", "HOME34.SA", "AMGN34.SA", "AMZO34.SA", "JNJB34.SA", "SSFO34.SA",
    "NVDC34.SA", "AAPL34.SA", "CMCS34.SA", "VERZ34.SA", "UNHH34.SA", "DISB34.SA", "AIRB34.SA", "PFIZ34.SA",
    "AURA33.SA", "NIKE34.SA", "ITLC34.SA", "BABA34.SA"
]

commodities_tickers = [
    "BZ=F",  # Brent Crude Oil Futures (Futuros de Petróleo Brent)
    "GC=F",  # Gold Futures (Futuros de Ouro)
    "SI=F",  # Silver Futures (Futuros de Prata)
    "ZS=F",  # Soybean Futures (Futuros de Soja)
    "ZC=F",  # Corn Futures (Futuros de Milho)
    "KC=F",  # Coffee Futures (Futuros de Café)
    "LE=F"   # Live Cattle Futures (Futuros de Boi Gordo)
]
currency_tickers = ["USDBRL=X", "EURBRL=X"]

crypto_tickers = [
    "BTC-USD", "ETH-USD", "BNB-USD", "SOL-USD", "XRP-USD",
    "ADA-USD", "DOGE-USD", "AVAX-USD", "DOT-USD", "MATIC-USD", "LTC-USD",
    "LINK-USD", "SHIB-USD", "TON-USD", "NEAR-USD", "SUI-USD", "UNI-USD"
]

interest_tickers = [
    "^IRX",  # 13-Week Treasury Bill Yield (Taxa de retorno do T-Bill de 13 semanas, curto prazo EUA)
    "^FVX",  # 5-Year Treasury Yield (Taxa de retorno do título de 5 anos do Tesouro dos EUA)
    "^TNX",  # 10-Year Treasury Yield (Taxa de retorno do título de 10 anos do Tesouro dos EUA)
    "^TYX",  # 30-Year Treasury Yield (Taxa de retorno do título de 30 anos do Tesouro dos EUA)
    "EDV"    # Vanguard Extended Duration Treasury ETF (ETF que investe em Treasuries de longa duração)
]

# Get all S&P 500 listed companies from Wikipedia
sp500 = ['MMM','AOS','ABT','ABBV','ACN','ADBE','AMD','AES','AFL','A','APD','ABNB','AKAM','ALB','ARE','ALGN','ALLE','LNT','ALL','GOOGL','GOOG','MO','AMZN','AMCR','AEE','AEP','AXP','AIG','AMT','AWK','AMP','AME','AMGN','APH','ADI','AON','APA','APO','AAPL','AMAT','APTV','ACGL','ADM','ANET','AJG','AIZ','T','ATO','ADSK','ADP','AZO','AVB','AVY','AXON','BKR','BALL','BAC','BAX','BDX','BRK.B','BBY','TECH','BIIB','BLK','BX','XYZ','BK','BA','BKNG','BSX','BMY','AVGO','BR','BRO','BF.B','BLDR','BG','BXP','CHRW','CDNS','CZR','CPT','CPB','COF','CAH','KMX','CCL','CARR','CAT','CBOE','CBRE','CDW','COR','CNC','CNP','CF','CRL','SCHW','CHTR','CVX','CMG','CB','CHD','CI','CINF','CTAS','CSCO','C','CFG','CLX','CME','CMS','KO','CTSH','COIN','CL','CMCSA','CAG','COP','ED','STZ','CEG','COO','CPRT','GLW','CPAY','CTVA','CSGP','COST','CTRA','CRWD','CCI','CSX','CMI','CVS','DHR','DRI','DDOG','DVA','DAY','DECK','DE','DELL','DAL','DVN','DXCM','FANG','DLR','DG','DLTR','D','DPZ','DASH','DOV','DOW','DHI','DTE','DUK','DD','EMN','ETN','EBAY','ECL','EIX','EW','EA','ELV','EMR','ENPH','ETR','EOG','EPAM','EQT','EFX','EQIX','EQR','ERIE','ESS','EL','EG','EVRG','ES','EXC','EXE','EXPE','EXPD','EXR','XOM','FFIV','FDS','FICO','FAST','FRT','FDX','FIS','FITB','FSLR','FE','FI','F','FTNT','FTV','FOXA','FOX','BEN','FCX','GRMN','IT','GE','GEHC','GEV','GEN','GNRC','GD','GIS','GM','GPC','GILD','GPN','GL','GDDY','GS','HAL','HIG','HAS','HCA','DOC','HSIC','HSY','HPE','HLT','HOLX','HD','HON','HRL','HST','HWM','HPQ','HUBB','HUM','HBAN','HII','IBM','IEX','IDXX','ITW','INCY','IR','PODD','INTC','IBKR','ICE','IFF','IP','IPG','INTU','ISRG','IVZ','INVH','IQV','IRM','JBHT','JBL','JKHY','J','JNJ','JCI','JPM','K','KVUE','KDP','KEY','KEYS','KMB','KIM','KMI','KKR','KLAC','KHC','KR','LHX','LH','LRCX','LW','LVS','LDOS','LEN','LII','LLY','LIN','LYV','LKQ','LMT','L','LOW','LULU','LYB','MTB','MPC','MKTX','MAR','MMC','MLM','MAS','MA','MTCH','MKC','MCD','MCK','MDT','MRK','META','MET','MTD','MGM','MCHP','MU','MSFT','MAA','MRNA','MHK','MOH','TAP','MDLZ','MPWR','MNST','MCO','MS','MOS','MSI','MSCI','NDAQ','NTAP','NFLX','NEM','NWSA','NWS','NEE','NKE','NI','NDSN','NSC','NTRS','NOC','NCLH','NRG','NUE','NVDA','NVR','NXPI','ORLY','OXY','ODFL','OMC','ON','OKE','ORCL','OTIS','PCAR','PKG','PLTR','PANW','PSKY','PH','PAYX','PAYC','PYPL','PNR','PEP','PFE','PCG','PM','PSX','PNW','PNC','POOL','PPG','PPL','PFG','PG','PGR','PLD','PRU','PEG','PTC','PSA','PHM','PWR','QCOM','DGX','RL','RJF','RTX','O','REG','REGN','RF','RSG','RMD','RVTY','ROK','ROL','ROP','ROST','RCL','SPGI','CRM','SBAC','SLB','STX','SRE','NOW','SHW','SPG','SWKS','SJM','SW','SNA','SOLV','SO','LUV','SWK','SBUX','STT','STLD','STE','SYK','SMCI','SYF','SNPS','SYY','TMUS','TROW','TTWO','TPR','TRGP','TGT','TEL','TDY','TER','TSLA','TXN','TPL','TXT','TMO','TJX','TKO','TTD','TSCO','TT','TDG','TRV','TRMB','TFC','TYL','TSN','USB','UBER','UDR','ULTA','UNP','UAL','UPS','URI','UNH','UHS','VLO','VTR','VLTO','VRSN','VRSK','VZ','VRTX','VTRS','VICI','V','VST','VMC','WRB','GWW','WAB','WMT','DIS','WBD','WM','WAT','WEC','WFC','WELL','WST','WDC','WY','WSM','WMB','WTW','WDAY','WYNN','XEL','XYL','YUM','ZBRA','ZBH','ZTS']

# 90 ativos Itaiwa
EWT=['2330.TW','2454.TW','2317.TW','2303.TW','2308.TW','3711.TW','2412.TW','1301.TW','3034.TW','2891.TW','1303.TW','2882.TW','1216.TW','2881.TW','2886.TW','3008.TW','2002.TW','1326.TW','2327.TW','2884.TW','2382.TW','2379.TW','6415.TW','5871.TW','2357.TW','2885.TW','2892.TW','6488.TW','1101.TW','5880.TW','2344.TW','3481.TW','2207.TW','2603.TW','2409.TW','2395.TW','4938.TW','2408.TW','1590.TW','3045.TW','2880.TW','3105.TW','2912.TW','2049.TW','5347.TW','2345.TW','5876.TW','2301.TW','2883.TW','2887.TW','2474.TW','2890.TW','3037.TW','6505.TW','4904.TW','2324.TW','2492.TW','6669.TW','9921.TW','2377.TW','2354.TW','2353.TW','2888.TW','3231.TW','8299.TW','2105.TW','6239.TW','3702.TW','2801.TW','1102.TW','1402.TW','8464.TW','1476.TW','9910.TW','9904.TW','4958.TW','2347.TW','2356.TW','5269.TW','2633.TW','2385.TW','2834.TW','2823.TW','9945.TW','2542.TW','4743.TW','1227.TW','XTSLA.TW','TWD.TW','2418.TW']

EWJ=['7203.T','6758.T','8306.T','6501.T','8316.T','7974.T','6098.T','6861.T','8766.T','7011.T','8035.T','8058.T','8411.T','9983.T','8001.T','4063.T','9433.T','8031.T','9984.T','4502.T','4568.T','9434.T','7267.T','6702.T','7741.T','6857.T','6503.T','2914.T','4519.T','3382.T','9432.T','6701.T','8725.T','6367.T','8002.T','8750.T','7751.T','8053.T','6301.T','8630.T','6752.T','6902.T','8801.T','6954.T','8591.T','4901.T','4543.T','6981.T','4661.T','5108.T','2802.T','4452.T','6273.T','6723.T','4578.T','6146.T','6762.T','8802.T','8267.T','5401.T','7182.T','7832.T','2502.T','1925.T','7269.T','9020.T','8308.T','8604.T','6201.T','4503.T','9613.T','6178.T','8309.T','9022.T','7936.T','7013.T','6594.T','9101.T','9735.T','6971.T','7733.T','5802.T','4307.T','9766.T','8015.T','5020.T','1928.T','6988.T','4507.T','7532.T','1605.T','9104.T','8830.T','5803.T','8795.T','4684.T','7309.T','2503.T','8697.T','6326.T','9531.T','7270.T','1812.T','9697.T','9503.T','4689.T','4704.T','1802.T','9021.T','3402.T','9532.T','8601.T','8113.T','1801.T','3407.T','8473.T','4755.T','6383.T','8136.T','6869.T','9843.T','6920.T','9502.T','4523.T','6586.T','4612.T','4188.T','8951.T','7202.T','7272.T','3626.T','5411.T','7186.T','4911.T','2413.T','3088.T','4204.T','7259.T','8593.T','1878.T','4091.T','2801.T','7911.T','6504.T','9042.T','9005.T','9602.T','3659.T','5201.T','7735.T','7912.T','5713.T','7201.T','6841.T','7701.T','2269.T','9107.T','7550.T','8331.T','2267.T','3064.T','9435.T','5019.T','6479.T','7752.T','9719.T','3038.T','4768.T','3003.T','6645.T','4716.T','2587.T','3092.T','7181.T','4324.T','2897.T','6465.T','4528.T','4151.T','9023.T','9143.T','9202.T','9201.T','XTSLA.T']

UAE=['EMAAR.DB','FAB.AD','EANDETB','EMIRATESNBD.DB','ADIB.AD','DIB.DB','ADCB.AD','ALDAR.AD','ADNOCDRILL.AD','ADNOCGAS.AD','DEWA.DB','ADNOCDIST.AD','AIRARABIA.DB','SALIK.DB','EMAARDEV.DB','AMR.AD','DIC.DB','PARKIN.DB','ADNH.AD','AGILITY.AD','DANA.AD','SIB.AD','EMPOWER.DB','DFM.DB','GULFNAV.DB','ARMX.DB','RAKPROP.AD','SPACE42.AD','AMLAK.DB','RAKCEC.AD','PHX.AD','AGTHIA.AD','ICAP.AD','AMANAT.DB','ESHRAQ.AD','TAALEEM.DB','SHUAA.DB','DEYAAR.DB','XTSLA','MULTIPLY.AD','QHOLDING.AD','NMDCENR.AD','GGICO.DB','ARTC.DB']

MCHI=['TCEHY','BABA','XIACF','MPNGF','CICHF','PDD','BYD','ICICHF','NTES','BACHF','JD','PANGF','TCOMF','CMBHF','BIDU']

# Franca
EWQ=['MC','SU','TTE','AI','AIR','SAN','SAF','OR','BNP','RMS','CS','ELE','DG','BN','SGO','ENGI','GLE','LR','HOT','CAP','ORA','ML','DSY','PUB','VIE','RIP','ACA','STM','KER','AMS','ENX','URW','BVI','ACC']

INDY=['ADANIENT','ADANIPORTS','APOLLOHOSP','ASIANPAINT','AXISBANK','BAJAJ-AUTO','BAJFINANCE','BAJAJFINSV','BEL','BHARTIARTL','CIPLA','COALINDIA','DRREDDY','EICHERMOT','ETERNAL','GRASIM','HCLTECH','HDFCBANK','HDFCLIFE','HEROMOTOCO','HINDALCO','HINDUNILVR','ICICIBANK','INDUSINDBK','INFY','ITC','JIOFIN','JSWSTEEL','KOTAKBANK','LT','M\&M','MARUTI','NESTLEIND','NTPC','ONGC','POWERGRID','RELIANCE','SBILIFE','SHRIRAMFIN','SBIN','SUNPHARMA','TCS','TATACONSUM','TATAMOTORS','TATASTEEL','TECHM','TITAN','TRENT','ULTRACEMCO','WIPRO']

global_tickers= ibov_tickers + sp500 + EWT + EWJ + UAE + MCHI + EWQ + INDY

with open("../dados/pre-NASDAQ.csv", "r") as f:
    lines = f.readlines()
nasdaq = [line.strip().split(',')[0] for line in lines if line.strip()]

master_tickers_br_EUA = list(set(ibov_tickers + sp500 + nasdaq)) 

sample = ['GOGL34.SA', 'GOOGL', 'GOOG', 'PETR3.SA', 'PETR4.SA']
test_m = ['CMPX', 'CMPOW', 'BRLSW', 'ACAD', 'ABOS', 'ABVEW', 'GOGL34.SA', 'GOOGL', 'GOOG', 'PETR3.SA', 'PETR4.SA']

br_tickers = list(set(ibov_tickers + bdrs_tickers))

# --------------------------
# HELPERS
# --------------------------
def build_filename(period_name, list_name, interval):
    return f"{list_name}_{period_name}_{interval}.csv"

from datetime import datetime

def save_with_info(df, filename,log_file="data_summary.log"):
    # Count rows and tickers
    n_rows = len(df)
    n_tickers = df["Ticker"].nunique() if "Ticker" in df.columns else 0

    sysdate = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    # Prepare log entry (one line per file)
    log_line = f"{sysdate},normalized,{filename},{n_rows},{n_tickers}\n"

    # Breakdown per ticker
    if "Ticker" in df.columns:
        counts = df["Ticker"].value_counts().sort_index()
        log_line += "   Rows per ticker:"
        for t, c in counts.items():
            log_line += f"\n     {t}: {c}"

    # Write header if file doesn’t exist
    if not os.path.exists(log_file):
        with open(log_file, "w") as f:
            f.write("\nsysdate,tag,filename,n_rows,n_tickers\n")

    # Append log line
    with open(log_file, "a") as f:
        f.write(log_line)

    # (optional) still print minimal info to console
    # print(log_line.strip())



def normalize_prices(df, filename):
    def _normalize(group):
        scaler = MinMaxScaler()
        cols_to_scale = ["Open", "High", "Low", "Close", "Adj Close"]
        group[cols_to_scale] = scaler.fit_transform(group[cols_to_scale])
        return group

    df_norm = df.groupby('Ticker').apply(_normalize, include_groups=False).reset_index(drop=False)

    # Keep same column order if they exist
    cols = ["DateTime", "Open", "High", "Low", "Close", "Adj Close", "Volume", "Ticker"]
    if "Time" in df_norm.columns:
        cols.append("Time")

    df_norm = df_norm[cols]

    df_norm.to_csv(f"normalized/{filename}", index=False)
    print(f"✅ Normalized saved: normalized_{filename}")

    # save summary in log file
    save_with_info(df_norm, f"normalized/{filename}")


def download_or_update(period_name, list_name, tickers, interval, keep_window, minimum_rows):
    filename = build_filename(period_name, list_name, interval)

    # --- Case 1: short periods (not yearly) → always refresh with `period`
    if "year" not in period_name.lower():
        print(f"⬇️ Fresh download for {filename} using period={period_name}, interval={interval}")

        data = yf.download(
            tickers,
            period=window,          # <-- use period string here
            interval=interval,
            group_by="ticker",
            auto_adjust=False,
            threads=True
        )

        # Remove old file if it exists
        if os.path.exists(f"data/{filename}"):
            os.remove(f"data/{filename}")

        df = pd.DataFrame()

    # --- Case 2: yearly or longer → incremental update with start/end
    else:
        if os.path.exists(f"data/{filename}"):
            df = pd.read_csv(f"data/{filename}", parse_dates=["DateTime"])
            last_date = df["DateTime"].max()
            start = last_date + pd.Timedelta(seconds=1)
            print(f"🔄 Updating {filename} from {start}...")
        else:
            df = pd.DataFrame()
            start = datetime.now(timezone.utc) - keep_window
            print(f"⬇️ Creating {filename} from scratch, starting {start}...")

        end = datetime.now(timezone.utc)

        data = yf.download(
            tickers, # GOOGL, GOOG, 
            start=start, end=end,
            interval=interval,
            group_by="ticker",
            auto_adjust=False,
            threads=True
        )

    if data.empty:
        print(f"⚠️ No new data for {filename}")
        return df

    # Flatten if multiple tickers
    records = []
    if len(tickers) > 1: # 
        for t in tickers:
            if t not in data.columns.levels[0]:
                continue
            df_t = data[t].reset_index()  # ✅ always bring index as column
            df_t.rename(columns={"Date": "DateTime", "Datetime": "DateTime"}, inplace=True)

            if "DateTime" not in df_t.columns:
                df_t["DateTime"] = df_t.index

            df_t["DateTime"] = pd.to_datetime(df_t["DateTime"], utc=True)
            df_t["Ticker"] = t

            # Drop rows with missing OHLC
            df_t = df_t.dropna(subset=["Open", "High", "Low", "Close"], how="all")

            print("ticker",  len(df_t))
            if len(df_t) < minimum_rows:
                print(f"⚠️ Skipping {t}: only {len(df_t)} rows (< {minimum_rows})")
                continue

            records.append(df_t)
        if not records:
            print(f"⚠️ No tickers passed minimum row filter for {filename}")
            return df
        
        new_data = pd.concat(records)
    

    

    # Merge with existing
    df = pd.concat([df, new_data], ignore_index=True)

    # Drop duplicates & sort
    df = df.drop_duplicates(subset=["DateTime", "Ticker"]).sort_values(["DateTime", "Ticker"])

    # Keep only rolling window
    if "year" in period_name.lower():
        cutoff = datetime.now(timezone.utc) - keep_window
        df = df[df["DateTime"] >= cutoff]

    # Add Date + Time split (but keep DateTime too)
    df["Date"] = df["DateTime"].dt.date
    if interval in ["1h", "1m", "60m"]:
        df["Time"] = df["DateTime"].dt.time

    # Reorder columns
    cols = ["DateTime", "Open", "High", "Low", "Close", "Adj Close", "Volume", "Ticker"]
    if "Time" in df.columns:
        cols.append("Time")
    df = df[cols]

    # Save
    df.to_csv(f"data/{filename}", index=False)
    print(f"💾 Saved {filename} with {len(df)} rows")

    # Normalize
    normalize_prices(df.copy(), filename)
    return df

# --------------------------
# RUN PIPELINE
# --------------------------
for period_name, interval, window, minimum in configs:
    df_final = download_or_update(period_name, "test_m", test_m, interval, window, minimum)

# delete old data
# 