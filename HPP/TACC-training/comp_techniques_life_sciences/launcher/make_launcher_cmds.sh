#!/bin/bash
set -e

CONTROL_FILE="launcher_cmds.txt"

for ID in {1..48}; do
    echo "echo $ID - "'$HOSTNAME - $(date +%s)'
done > "${CONTROL_FILE}"


