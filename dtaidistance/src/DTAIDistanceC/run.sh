#!/bin/bash
OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 400 0
OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
OMP_NUM_THREADS=6 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
OMP_NUM_THREADS=12 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
OMP_NUM_THREADS=24 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
#OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 100 0
#OMP_NUM_THREADS=6 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 100 0
#OMP_NUM_THREADS=12 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 100 0
#OMP_NUM_THREADS=24 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 100 0
#OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 200 0
#OMP_NUM_THREADS=6 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 200 0
#OMP_NUM_THREADS=12 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 200 0
#OMP_NUM_THREADS=24 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 200 0
OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 400 0
#OMP_NUM_THREADS=6 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 400 0
#OMP_NUM_THREADS=12 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 400 0
#OMP_NUM_THREADS=24 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 400 0
OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
OMP_NUM_THREADS=6 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
OMP_NUM_THREADS=12 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
OMP_NUM_THREADS=24 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers.csv 800 0
#./example /home/dani/Documents/git/Master/dados/master_tickers.csv 40 1
