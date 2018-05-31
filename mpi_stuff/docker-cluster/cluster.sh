#!/bin/bash
#-
#-                       :-) A MPI Container Cluster (-:
#-
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

down_master() {
    echo -e "${green}docker-compose stop master && docker-compose rm -f master${reset}"
    docker-compose stop master && docker-compose rm -f master
}

down_workers() {
    echo -e "${green}docker-compose stop worker && docker-compose rm -f worker${reset}"
    docker-compose stop worker && docker-compose rm -f worker
}

exec_on_mpi_master() {
    docker exec -it -u mpi $(docker-compose ps | grep 'master'| awk '{print $1}') "$@"
}

cmd_up=0
cmd_down=0
cmd_login=0
cmd_scale=0
NUM_WORKERS=0

# Parse command line arguments.                                                 
while [[ "$#" > 0 ]]; do
    arg="$1"

    case "${arg}" in
        -v|--version)
            echo "$(grep "^#-" ${BASH_SOURCE[0]} | cut -c 4-)"
            exit 0
            ;;
        -h|--help)
            echo "$(grep "^##" ${BASH_SOURCE[0]} | cut -c 4-)"
            exit 0
            ;;
        up)
            cmd_up=1
            ;;
        down)
            cmd_down=1
            ;;
        login)
            cmd_login=1
            ;;
        -s|-scale)
            cmd_scale=1
            NUM_WORKERS="$2"
            shift
            ;;
        *)
            echo "Unknown command-line option: '${arg}'."
            exit 1
            ;;
    esac
    shift
done


if [ $cmd_up == 1 ]; then
    echo -e "${blue}${ASCII_ART_UP1}${reset}"
    
    down_all
    up_registry
    generate_ssh_keys
    build_and_push_images
    up_master
    scale_workers 1

    echo -e "${blue}${ASCII_ART_UP2}${reset}"

elif [ $cmd_down == 1 ]; then
    echo -e "${blue}${ASCII_ART_DOWN}${reset}"
    down_all

elif [ $cmd_login == 1 ]; then
    echo -e "${blue}${ASCII_ART_LOGIN}${reset}"
    exec_on_mpi_master ash

elif [ $cmd_scale == 1 ]; then
    echo -e "${blue}${ASCII_ART_SCALE}${reset}"
    down_master
    down_workers
    up_master
    scale_workers $NUM_WORKERS

else
    echo whats gucci?
fi
