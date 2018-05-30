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

build_and_push_images() {
    echo -e "${green}docker build -t ${CLUSTER_REGISTRY}/${IMAGE_NAME} .${reset}"
    docker build -t "${CLUSTER_REGISTRY}/${IMAGE_NAME}" .

    echo -e "${green}docker push ${CLUSTER_REGISTRY}/${IMAGE_NAME}${reset}"
    docker push "${CLUSTER_REGISTRY}/${IMAGE_NAME}"
}

up_master() {
    echo -e "${green}docker-compose up -d master${reset}"
    docker-compose up -d master
}

scale_workers() {
    num_workers="$1"
    echo -e "${green}docker-compose up -d --scale worker=${num_workers}${reset}"
    docker-compose up -d --scale worker=${num_workers}
}

echo -e "${blue}${ASCII_ART_HEADER1}${reset}"

down_all
up_registry
generate_ssh_keys
build_and_push_images
up_master
scale_workers 3

echo -e "${blue}${ASCII_ART_HEADER2}${reset}"
