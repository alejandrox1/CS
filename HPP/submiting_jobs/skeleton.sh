#!/bin/bash
#-
#-                              :-) Skeleton (-:
#-
#- This is to serve as a skeleton for job pipelines, and emphasis is placed in
#- in working and setting up environmnets for high performance computing.
#-
##
## Usage:
##
##      ./skeleton.sh [-v|-h] [-i|--input INPUT_FILE]
##
##      -v|--version print to stdout any information related to this script.
##
##      -h|--help print to stdout any help information included in the header
##                of the script.
##
##      -i|--input [JSON] file with input parameters.
##
set -e
set -o pipefail
reset="\e[0m"
red="\e[1;31m"

# Input parameters
INPUT_FILE="parameters.json"

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
        -i|--input)
            INPUT_FILE="$2"
            shift
            ;;
        *)
            ( >&2 echo "Unknown command-line option: '${arg}'.")
            exit 1
            ;;
    esac
    shift
done

echo -e "${red}Proceding with input parameter file ${INPUT_FILE}${reset}"
REQUESTED_NODES=`jq .job_parameters.nodes "${INPUT_FILE}"`
REQUESTED_CORES=`jq .job_parameters.cores "${INPUT_FILE}"`
echo Number of nodes requested: ${REQUESTED_NODES}"
echo Number of cores requested: ${REQUESTED_CORES}"
