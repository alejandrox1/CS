#!/bin/sh
#
# mpi_bootstrap
#
# Store container service names in /etc/opt/service_names; update the host
# file; manage ssh connections for master and workers.
#

ROLE="undefined"
MPI_MASTER_SERVICE_NAME="mpi_master"
MPI_WORKER_SERVICE_NAME="mpi_worker"

# Parse command line arguments.
while [ "$1" != "" ]; do
    PARAM=$(echo "$1" | awk -F= '{print $1}')
    VALUE=$(echo "$1" | awk -F= '{print $2}')

    case $PARAM in
        role)
            [ "$VALUE" ] && ROLE=$VALUE
            ;;

        mpi_master_service_name)
            [ "$VALUE" ] && MPI_MASTER_SERVICE_NAME=$VALUE
            ;;

        mpi_worker_service_name)
            [ "$VALUE" ] && MPI_WORKER_SERVICE_NAME=$VALUE
            ;;

        *)
            echo "ERROR: Unknown error parameter \"$PARAM\""
            exit 1
            ;;
    esac
    shift
done


cat > /etc/opt/service_names <<- EOF
MPI_MASTER_SERVICE_NAME=${MPI_MASTER_SERVICE_NAME}
MPI_WORKER_SERVICE_NAME=${MPI_WORKER_SERVICE_NAME}
EOF


case $ROLE in
    "master")
        # Auto update default host file.
        auto_update_hosts "${HYDRA_HOST_FILE}" &

        # Start ssh server.
        /usr/sbin/sshd -D
        ;;

    "worker")
        # Start ssh server on background.
        /usr/sbin/sshd -D &

        # Retry indefinetely to connect to the master node.
        while sleep 1; do
            # shellcheck disable=SC2086
            ssh -T -o "StrictHostKeyChecking no"   \
                -i "${USER_HOME}/.ssh/id_rsa"      \
                ${USER}@${MPI_MASTER_SERVICE_NAME} \
                "tail -f /dev/null"
        done
        ;;

    *)
        echo 'role argument only accepts "master" or "worker"'
        exit 1
        ;;
esac
