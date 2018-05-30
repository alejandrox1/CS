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
    echo -e "${green}docker-compose down${reset}"

    docker-compose down
}

up_registry() {
    echo -e "${green}docker-compose up -d registry${reset}"

    docker-compose up -d registry
}

generate_ssh_keys() {
    if [ -f ssh/id_rsa ] && [ -f ssh/id_rsa.pub ]; then
        return 0
    fi

    echo -e "${green}Generating SSH keys...${reset}"
    
    mkdir -p ssh
    ssh-keygen -f ssh/id_rsa -t rsa -N ''
}

echo -e "${blue}${ASCII_ART_HEADER}${reset}"
down_all
up_registry
generate_ssh_keys
