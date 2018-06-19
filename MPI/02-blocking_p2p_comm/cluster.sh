#!/bin/bash
#-
#-                       :-) A MPI Container Cluster (-:
#-
#- Emulate a cluster via containers. All containers come with MPICH installed. 
#- 
#- Adapted from alpine-mpich: https://github.com/NLKNguyen/alpine-mpich
#-
##
## Usage:
##
##      ./cluster.sh COMMAND [OPTIONS]
##
## Commands:
##
##      --help print usage and command description (this message).
##
##      --version print information about this script.
##
##      up: bootstrap the cluster with one worker.
##          ./cluster up -size INT
##
##      down: tear down cluster.
##          ./cluster down
##
##      login: loging to the container acting as the MPI master node for 
##             interactive usage.
##          ./cluster login
##
##      exec: execute shell command at the container acting as the MPI master 
##            node.
##          ./cluster.sh exec CMD
##
##      scale: scale the number of containers acting as MPI worker nodes.
##          ./cluster scale -size INT
##
## To login into the master node, try:
##      
##      . ./config.sh
##      ssh -o "StrictHostKeyChecking no" -i ssh/id_rsa -p $CLUSTER_SSH_PORT mpi@localhost
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

# Command control variables.
cmd_up=0
cmd_down=0
cmd_login=0
cmd_exec=0
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
            NUM_WORKERS=3
            ;;
        -size)
            NUM_WORKERS="$2"
            shift
            ;;
        down)
            cmd_down=1
            ;;
        login)
            cmd_login=1
            ;;
        exec)
            cmd_exec=1
            ;;
        scale)
            cmd_scale=1
            ;;
        *)
            echo "Unknown command-line option: '${arg}'."
            exit 1
            ;;
    esac
    shift
done

# ENTRYPOINT.
if [ $cmd_up == 1 ]; then
    echo -e "${blue}${ASCII_ART_UP1}${reset}"
    
    down_all
    up_registry
    generate_ssh_keys
    build_and_push_images
    up_master
    scale_workers $NUM_WORKERS

    echo -e "${blue}${ASCII_ART_UP2}${reset}"

elif [ $cmd_down == 1 ]; then
    echo -e "${blue}${ASCII_ART_DOWN}${reset}"
    down_all

elif [ $cmd_login == 1 ]; then
    echo -e "${blue}${ASCII_ART_LOGIN}${reset}"
    exec_on_mpi_master ash

elif [ $cmd_exec == 1 ]; then
    echo -e "${blue}${ASCII_ART_EXEC}${reset}"
    exec_on_mpi_master ash -c "${SHELL_COMMAND}"

elif [ $cmd_scale == 1 ]; then
    echo -e "${blue}${ASCII_ART_SCALE}${reset}"
    down_master
    down_workers
    up_master
    scale_workers $NUM_WORKERS

else
    echo whats gucci?
fi
