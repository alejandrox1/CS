# Emulating an MPI Cluster with Containers

This is adpated from [alpine-mpich](https://github.com/NLKNguyen/alpine-mpich).

The `cluster.sh` script can boostrap and tear down a cluster consisting of 1
MPI master node and a variable `NUM_WORKERS` MPI worker nodes.

For a quick test, run:
```bash
./cluster.sh up && ./cluster login
```

Once you are loged in inside the master MPI node, run:
```bash
make clean && make
```


For usage information run `./cluster [-h|--help]`:
```
Usage:

     ./cluster.sh COMMAND [OPTIONS]

Commands:

     --help print usage and command description (this message).

     --version print information about this script.

     up: bootstrap the cluster with one worker.
         ./cluster up -size INT

     down: tear down cluster.
         ./cluster down

     login: loging to the container acting as the MPI master node for 
            interactive usage.
         ./cluster login

     exec: execute shell command at the container acting as the MPI master 
           node.
         ./cluster.sh exec CMD

     scale: scale the number of containers acting as MPI worker nodes.
         ./cluster scale INT

To login into the master node, try:
     
     . ./config.sh
     ssh -o "StrictHostKeyChecking no" -i ssh/id_rsa -p $CLUSTER_SSH_PORT mpi@localhost

```
