#!/bin/bash
#-
#-                          :-) Build KNC Examples (-:
#-
#- Build script for a variety of sample applications to test drive systems with
#- HPC-type workloads.
#-
##
## Usage:
##
##      ./build.sh [-v|-h] [-i|--input INPUT_FILE]
##
##      -v|--version print to stdout any information related to this script.
##
##      -h|--help print to stdout any help information included in the header
##                of the script.
##
##      -no-opt normal compilation (no autovectorization enabled).
##
set -e
set -o pipefail
export reset="\e[0m"
export red="\e[1;31m"
export green="\e[32m"

NO_OPTIMIZATION="false"
KNL="false"
SKYLAKE="false"
HASWELL="false"

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
        -no-opt)
            NO_OPTIMIZATION="true"
            ;;
        -knl)
            KNL="true"
            ;;
        -skylake)
            SKYLAKE="true"
            ;;
        -haswell)
            HASWELL="true"
            ;;
        *)
            ( >&2 echo "Unknown command-line option: '${arg}'.")
            exit 1
            ;;
    esac
    shift
done


if [ "${NO_OPTIMIZATION}" == "true" ]; then post="out"; fi
echo -e "${red}Proceding with${post} auto-vectorization...${reset}"

. ./config.sh

$MAKE clean && $MAKE
