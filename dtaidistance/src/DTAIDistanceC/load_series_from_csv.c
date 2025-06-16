#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_series_from_csv.h"


int load_series_from_csv(const char *filename, TickerSeries *series_list, int *num_series, int max_assets) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    char line[1024];
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp)) {
        // Make a copy of the line to safely tokenize
        char *line_ptr = strdup(line);
        if (!line_ptr) {
            perror("strdup");
            fclose(fp);
            return -1;
        }

        // Tokenize line
        char *token = strtok(line_ptr, ","); // Date
        if (!token) { free(line_ptr); continue; }

        strtok(NULL, ","); // Open
        strtok(NULL, ","); // High
        strtok(NULL, ","); // Low

        char *close_str = strtok(NULL, ","); // Close
        if (!close_str) { free(line_ptr); continue; }

        strtok(NULL, ","); // Adj Close
        strtok(NULL, ","); // Volume

        char *ticker = strtok(NULL, ",\n");
        if (!ticker) { free(line_ptr); continue; }

        if (!close_str || strlen(close_str) == 0) {
            free(line_ptr);
            continue;
        }
        double close_val = atof(close_str);

        // Check if ticker already exists
        int i, found = 0;
        for (i = 0; i < *num_series; i++) {
            if (strcmp(series_list[i].ticker, ticker) == 0) {
                if (series_list[i].count < MAX_TIMEPOINTS) {
                    series_list[i].close[series_list[i].count++] = close_val;
                }
                found = 1;
                break;
            }
        }

        // New ticker
        if (!found && *num_series < max_assets) {
            strncpy(series_list[*num_series].ticker, ticker, MAX_TICKER_NAME - 1);
            series_list[*num_series].ticker[MAX_TICKER_NAME - 1] = '\0'; // Ensure null-terminated
            series_list[*num_series].close[0] = close_val;
            series_list[*num_series].count = 1;
            (*num_series)++;
        }

        free(line_ptr);
    }

    fclose(fp);
    return 0;
}
