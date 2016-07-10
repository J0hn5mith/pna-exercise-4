#!/bin/zsh
#PJM -L "rscgrp=lecture" 
#PJM -L "node=8"
#PJM -L "elapse=10:00" 
#PJM -j

export OMP_NUM_THREADS=8
mpiexec ./app
