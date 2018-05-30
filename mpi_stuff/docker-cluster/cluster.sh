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

. ./utils.sh
. ./config.sh

down_all() {
    echo -e "${blue}${ASCII_ART_HEADER}${reset}"
    echo -e "${green}$docker-compose down registry${reset}"

    docker-compose down registry
}

up_registry() {
    echo -e "${blue}${ASCII_ART_HEADER}${reset}" 
    echo -e "${green}$docker-compose up -d registry${reset}"

    docker-compose up -d registry
}
