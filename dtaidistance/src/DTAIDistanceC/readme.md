# DTAIDistanceC

Time series distances (e.g. Dynamic Time Warping) used in the
[DTAI Research Group](https://dtai.cs.kuleuven.be).
The code is developed to be used as part of the Python package DTAIDistance
but can be used also in an only C project.

Documentation Python toolbox: http://dtaidistance.readthedocs.io

Citing this work: [![DOI](https://zenodo.org/badge/80764246.svg)](https://zenodo.org/badge/latestdoi/80764246)

## Requirements

- The parallel functionality assumes OpenMP is available.
- The tests are based on Criterion.


## License

    DTAI distance code.

    Copyright 2020 Wannes Meert, KU Leuven, DTAI Research Group

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0



sudo apt update
sudo apt install libomp-dev

 dpkg -L libomp-dev

from author:
gcc -o example example.c \
DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_openmp.c \
DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
-Wall -g -Xpreprocessor -fopenmp -lomp -I/opt/homebrew/include \
-L/opt/homebrew/lib \
-I./DTAIDistanceC/

changing to compile:
gcc -o example example.c \
    DTAIDistanceC/dd_dtw.c DTAIDistanceC/dd_dtw_openmp.c \
    DTAIDistanceC/dd_ed.c DTAIDistanceC/dd_globals.c \
    -Wall -g -fopenmp -lm \
    -I./DTAIDistanceC/


    