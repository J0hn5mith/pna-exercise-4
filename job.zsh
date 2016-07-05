#!/bin/zsh
#PJM -L "rscgrp=lecture" 
#PJM -L "node=8"
#PJM --mpi "proc=128"
#PJM -L "elapse=10:00" 
#PJM -j

mpiexec ./app
