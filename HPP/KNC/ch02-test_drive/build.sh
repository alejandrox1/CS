#!/bin/bash
set -e

export NO_OPTIMIZATION="false"

# Process command-line arguments.
while [[ $# > 0 ]]; do
    key="$1"

    case $key in
        -no-opt)
            NO_OPTIMIZATION="true"
            shift
            ;;
        *)
            echo "Unkown option."
            exit 1
            ;;
    esac
done

. ./config.sh

$MAKE clean && $MAKE 
