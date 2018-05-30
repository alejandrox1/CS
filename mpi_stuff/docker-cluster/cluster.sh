#!/bin/bash
#-
#-                       :-) A MPI Container Cluster (-:
#- Adapted from alpine-mpich: https://github.com/NLKNguyen/alpine-mpich
#-
##
## 
##
set -e
set -o pipefail

REGISTRY_ADDR=localhost
REGISTRY_PORT=5000
SSH_PORT=2222
IMAGE_NAME=mpi
