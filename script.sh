#!/bin/bash
make clean
make
./program
#mpirun -n 2 program
