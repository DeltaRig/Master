#!/bin/bash
#SBATCH -N 4
#SBATCH --exclusive

OMP_NUM_THREADS=12 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=12 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=12 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=12 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=24 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=12 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=24 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=24 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=24 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=24 srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait

srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n96.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n96.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n96.log
wait
srun -N 4 -n 96 --exclusive  ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n96.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n96.log
wait

srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 1 master_tickersmpi72to1600v21.csv >> N4n96g1.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 1 master_tickersmpi72to1600v21.csv >> N4n96g1.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 1 master_tickersmpi72to1600v21.csv >> N4n96g1.log
wait
srun -N 4 -n 96 --exclusive  ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 1 master_tickersmpi72to1600v21.csv >> N4n96g1.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 1 master_tickersmpi72to1600v21.csv >> N4n96g1.log
wait

srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 3 master_tickersmpi72to1600v21.csv >> N4n96g3.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 3 master_tickersmpi72to1600v21.csv >> N4n96g3.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 3 master_tickersmpi72to1600v21.csv >> N4n96g3.log
wait
srun -N 4 -n 96 --exclusive  ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 3 master_tickersmpi72to1600v21.csv >> N4n96g3.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 3 master_tickersmpi72to1600v21.csv >> N4n96g3.log


srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 4 master_tickersmpi72to1600v21.csv >> N4n96g4.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 4 master_tickersmpi72to1600v21.csv >> N4n96g4.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 4 master_tickersmpi72to1600v21.csv >> N4n96g4.log
wait
srun -N 4 -n 96 --exclusive  ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 4 master_tickersmpi72to1600v21.csv >> N4n96g4.log
wait
srun -N 4 -n 96 --exclusive ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 4 master_tickersmpi72to1600v21.csv >> N4n96g4.log




srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 6 -n 144 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n144.log
wait
srun -N 6 -n 144 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n144.log
wait

echo "end"

OMP_NUM_THREADS=24 srun -N 8 -n 9 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N8n9g48.log
wait
OMP_NUM_THREADS=24 srun -N 8 -n 9 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N8n9g48.log
wait
OMP_NUM_THREADS=24 srun -N 8 -n 9 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N8n9g48.log
wait
OMP_NUM_THREADS=24 srun -N 8 -n 9 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N8n9g48.log
wait
OMP_NUM_THREADS=24 srun -N 8 -n 9 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N8n9g48.log
wait
OMP_NUM_THREADS=24 srun -N 8 -n 9 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N8n9g48.log
wait
OMP_NUM_THREADS=24 srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
OMP_NUM_THREADS=24 srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
OMP_NUM_THREADS=24 srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
OMP_NUM_THREADS=24 srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
OMP_NUM_THREADS=24 srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
OMP_NUM_THREADS=24 srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
wait
OMP_NUM_THREADS=24
echo "UM SO RODANDO" >> N4n5g48.log
OMP_NUM_THREADS=24srun -N 4 -n 5 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log &
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log &
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
wait
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log &
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log &
OMP_NUM_THREADS=24 srun -N 2 -n 3 --exclusive ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
wait

echo "end"

srun -N 12 -n 288 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N12n288.log
wait
srun -N 12 -n 144 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N12n144.log
wait
srun -N 12 -n 288 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N12n288.log
wait
srun -N 12 -n 144 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N12n144.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N1２n１３g２４.log
wait
OMP_NUM_THREADS=12 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N12n13g48.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait
OMP_NUM_THREADS=24 srun -N 12 -n 13 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N12n13g24.log
wait

srun -N 10 -n 240 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n240.log
wait
srun -N 10 -n 240 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n240.log
wait
srun -N 10 -n 240 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n240.log
wait
srun -N 10 -n 240 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n240.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N10n11g24.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N10n11g24.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N10n11g96.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N10n11g96.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=12 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N10n11g24.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N10n11g24.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N10n11g96.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N10n11g96.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait
OMP_NUM_THREADS=24 srun -N 10 -n 11 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N10n11g48.log
wait


echo "end"

OMP_NUM_THREADS=12
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
OMP_NUM_THREADS=24 srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
OMP_NUM_THREADS=24 srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
OMP_NUM_THREADS=24 srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
OMP_NUM_THREADS=24 srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
OMP_NUM_THREADS=24 srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log

echo "end"


srun -N 10 -n 240 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n240.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait
srun -N 10 -n 240 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n240.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait
srun -N 10 -n 120 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N10n120.log
wait






srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N6n7g24.log
wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N6n7g24.log
wait
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N4n5g24.log
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N4n5g24.log
wait
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N2n3g24.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N2n3g24.log

wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N6n7g48.log
wait
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N4n5g48.log
wait
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 48 master_tickersmpi72to1600v21.csv >> N2n3g48.log

wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N6n7g96.log
wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N6n7g96.log
wait
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N4n5g96.log
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N4n5g96.log
wait
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N2n3g96.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 96 master_tickersmpi72to1600v21.csv >> N2n3g96.log



srun -N 6 -n 144 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n144.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait
srun -N 4 -n 96 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n96.log
srun -N 4 -n 192 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N4n48.log
wait
srun -N 2 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n48.log
srun -N 2 -n 96 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24.log
wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N6n7g24.log
wait
srun -N 6 -n 7 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N6n7g24.log
wait
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N4n5g24.log
srun -N 4 -n 5 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N4n5g24.log
wait
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N2n3g24.log
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N2n3g24.log



srun -N 1 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n48MV3Pantanal.log
wait
srun -N 1 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n48MV3Pantanal.log
wait
srun -N 1 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n48MV3Pantanal.log
wait
srun -N 1 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n48MV3Pantanal.log
wait
srun -N 1 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n48MV3Pantanal.log
wait

srun -N 1 -n 6 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n6MV3Pantanal.log
wait
srun -N 1 -n 12 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n12MV3Pantanal.log
wait
srun -N 1 -n 24 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n24MV3Pantanal.log
wait
srun -N 1 -n 48 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpiData.csv >> N1n48MV3Pantanal.log
wait

srun -N 2 -n 24 --exclusive -t 2000 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24MV3.log
wait
srun -N 2 -n 24 --exclusive -t 2000 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24MV3.log
wait
srun -N 2 -n 24 --exclusive -t 2000 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24MV3.log
wait
srun -N 2 -n 24 --exclusive -t 2000 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24MV3.log
wait
srun -N 2 -n 24 --exclusive -t 2000 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24MV3.log
wait
srun -N 2 -n 24 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N2n24MV3.log
wait
srun -N 2 -n 3 --exclusive -t 2000 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N2n3.log
wait
srun -N 2 -n 3 ./dtaidistance/src/DTAIDistanceC/hybrid dados/master_tickers_br_us2200.csv 1600 24 master_tickersmpi72to1600v21.csv >> N2n3.log
wait
srun -N 6 -n 72 ./dtaidistance/src/DTAIDistanceC/exampleMV3Datatype dados/master_tickers_br_us2200.csv 1600 2 master_tickersmpi72to1600v21.csv >> N6n72.log
wait