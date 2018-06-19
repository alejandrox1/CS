#!/bin/bash

# Local container image registry configurations.
export CLUSTER_REGISTRY_ADDR=localhost                                                 
export CLUSTER_REGISTRY_PORT=5000                                                      
export CLUSTER_REGISTRY="${CLUSTER_REGISTRY_ADDR}:${CLUSTER_REGISTRY_PORT}"

# Master ssh port.
export CLUSTER_SSH_PORT=2222                                                                   

# Name of Container image to be used for master/workers.
export IMAGE_NAME=mpi 
