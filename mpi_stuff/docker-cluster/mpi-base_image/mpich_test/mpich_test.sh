#!/bin/bash
set -e
set -o pipefail

mpicc --version > /dev/null
mpiexec --version > /dev/null
mpicc -o mpi_hello_world mpi_hello_world.c > /dev/null
mpirun -n 4 ./mpi_hello_world > /dev/null
