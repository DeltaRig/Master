#define MAX_TICKERS 100
#define MAX_TIMEPOINTS 3000
#define MAX_TICKER_NAME 32

typedef struct {
    char ticker[MAX_TICKER_NAME];
    double close[MAX_TIMEPOINTS];
    // double open[MAX_TIMEPOINTS];
    int count;
} TickerSeries;

int load_series_from_csv(const char *filename, TickerSeries *series_list, int *num_series);
