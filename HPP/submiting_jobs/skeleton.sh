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
##      ./skeleton.sh [-v|-h]
##
##      -v print to stdout any information related to this script.
##
##      -h print to stdout any help information included in the header of the
##         script.
##

# Parse command line arguments.
while [[ "$#" > 0 ]]; do
    arg="$1"

    case "${arg}" in
        -v)
            echo "$(grep "^#-" ${BASH_SOURCE[0]} | cut -c 4-)"
            exit 0
            ;;
        -h)
            echo "$(grep "^##" ${BASH_SOURCE[0]} | cut -c 4-)"
            exit 0
            ;;
        *)
            ( >&2 echo "Unknown command-line option.")
            exit 1
            ;;
    esac
done
