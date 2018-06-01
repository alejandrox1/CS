#!/bin/sh
#
# get_hosts
#

# Include the variables that store the Docker service names.
. /etc/opt/service_names

( netstat -t | grep ESTABLISHED | awk '{print $5}' | grep "$MPI_WORKER_SERVICE_NAME" | cut -d: -f1  \
& getent hosts "$MPI_MASTER_SERVICE_NAME" | cut -d' ' -f1 ) | sort -u
