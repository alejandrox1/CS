# MPI Container Cluster

Build an MPI cluster using containers :-)

All MPI "nodes" are based off two images:
1. [MPI Base Image](mpi-base_image) Install MPICH and whatever necessary setup
   is required.

2. [MPI Node Image](mpi-node_image) Use the [MPI Base Image](mpi-base_image) to
   build a container and setup all the networking necessary to have the
   conatiners connected.

At the time of this writing, there is only a single cluster mode, [Cluster
Compose](cluster-compose), which is powered by `docker-compose` to provide
orchestration.

In the future, we plan to expand the cluster to use `docker swarm` and
`kubernetes`.
