#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TICKERS 100
#define MAX_TIMEPOINTS 1000
#define MAX_TICKER_NAME 32

typedef struct {
    char ticker[MAX_TICKER_NAME];
    double close[MAX_TIMEPOINTS];
    int count;
} TickerSeries;


int load_series_from_csv(const char *filename, TickerSeries *series_list, int *num_series) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    char line[1024];
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp)) {
        char *date = strtok(line, ",");
        strtok(NULL, ","); // Open
        strtok(NULL, ","); // High
        strtok(NULL, ","); // Low
        char *close_str = strtok(NULL, ","); // Close
        strtok(NULL, ","); // Adj Close
        strtok(NULL, ","); // Volume
        char *ticker = strtok(NULL, ",\n");

        double close_val = atof(close_str);

        // Check if ticker already exists
        int i, found = 0;
        for (i = 0; i < *num_series; i++) {
            if (strcmp(series_list[i].ticker, ticker) == 0) {
                series_list[i].close[series_list[i].count++] = close_val;
                found = 1;
                break;
            }
        }

        // New ticker
        if (!found && *num_series < MAX_TICKERS) {
            strncpy(series_list[*num_series].ticker, ticker, MAX_TICKER_NAME);
            series_list[*num_series].close[0] = close_val;
            series_list[*num_series].count = 1;
            (*num_series)++;
        }
    }

    fclose(fp);
    return 0;
}

