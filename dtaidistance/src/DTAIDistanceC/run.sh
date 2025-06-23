#!/bin/bash
OMP_NUM_THREADS=1 ./dtadistance/src/DTAIDistanceC/example dados/master_tickers.csv 10 0
OMP_NUM_THREADS=6 ./dtadistance/src/DTAIDistanceC/example dados/master_tickers.csv 10 0
OMP_NUM_THREADS=6 ./example /home/dani/Documents/git/Master/dados/master_tickers.csv 10 1