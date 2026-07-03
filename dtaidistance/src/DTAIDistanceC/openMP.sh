#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive

# hibrid
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantNH.log
wait

OMP_NUM_THREADS=48 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48PantH.log
wait

OMP_NUM_THREADS=24 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantNH.log
wait
OMP_NUM_THREADS=24 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantNH.log
wait

OMP_NUM_THREADS=48 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantH.log
wait
OMP_NUM_THREADS=48 srun -N 3 -n 4 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N3n4g48PantH.log
wait



# openMP
OMP_NUM_THREADS=24 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open24.log
wait
OMP_NUM_THREADS=24 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open24.log
wait
OMP_NUM_THREADS=24 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open24.log
wait
OMP_NUM_THREADS=12 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open12.log
wait
OMP_NUM_THREADS=12 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open12.log
wait    
OMP_NUM_THREADS=12 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open12.log
wait
OMP_NUM_THREADS=6 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open6.log
wait
OMP_NUM_THREADS=6 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open6.log
wait
OMP_NUM_THREADS=6 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open6.log
wait
OMP_NUM_THREADS=1 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open1.log
wait
OMP_NUM_THREADS=1 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open1.log
wait

# MPI
srun -N 1 -n 2 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n2Pant.log
wait
srun -N 1 -n 2 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n2Pant.log
wait
srun -N 1 -n 2 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n2Pant.log
wait
srun -N 1 -n 2 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n2Pant.log
wait

srun -N 1 -n 6 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n6Pant.log
wait
srun -N 1 -n 6 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n6Pant.log
wait
srun -N 1 -n 6 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n6Pant.log
wait

srun -N 1 -n 6 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n6Pant.log
wait
srun -N 1 -n 6 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n6Pant.log
wait
srun -N 1 -n 6 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n6Pant.log
wait

srun -N 1 -n 12 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n12Pant.log
wait
srun -N 1 -n 12 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n12Pant.log
wait
srun -N 1 -n 12 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n12Pant.log
wait

srun -N 1 -n 12 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n12Pant.log
wait
srun -N 1 -n 12 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n12Pant.log
wait
srun -N 1 -n 12 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n12Pant.log
wait

srun -N 1 -n 48 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n48Pant.log
wait
srun -N 1 -n 48 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n48Pant.log
wait
srun -N 1 -n 48 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n48Pant.log
wait

srun -N 1 -n 48 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n48Pant.log
wait
srun -N 1 -n 48 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n48Pant.log
wait
srun -N 1 -n 48 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N1n48Pant.log
wait
OMP_NUM_THREADS=48 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log
wait
OMP_NUM_THREADS=48 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log
wait
OMP_NUM_THREADS=48 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log
wait
OMP_NUM_THREADS=48 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log
wait
OMP_NUM_THREADS=48 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log
wait
OMP_NUM_THREADS=48 srun -N 1 --exclusive ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log
wait


OMP_NUM_THREADS=1 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open1.log
OMP_NUM_THREADS=6 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open6.log
OMP_NUM_THREADS=12 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open12.log
OMP_NUM_THREADS=24 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open24.log
OMP_NUM_THREADS=48 ./dtaidistance/src/DTAIDistanceC/example dados/master_tickers_br_us2200.csv 1600 0 0 destiny.csv >> open48.log

