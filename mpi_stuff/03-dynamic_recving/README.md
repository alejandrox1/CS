# Dynamic Receiving with MPI Probe and MPI Status

In the previous section [blocking p2p communication](../02-blocking_p2p_comm)
we deal with the messages whose length we knew right from the start.
So now, we will generalize!


Find out the length of a mesage:
```
MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);
```
A call to `MPI_Get_count` will return the number of `datatype` elements that
were received.


To make a query on a received message,
```
MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status* status);
```
notice that it looks like `MPI_Recv` it just does not receive a message.
**Like `MPI_Recv`, `MPI_Probe` will block for a message with a matching source
and tag**.


```
├── project
│   ├── check_status.c
│   ├── Makefile
│   └── probe.c
...
```

1. `check_status.c` check how many messages were received from the sender.

2. `probe.c` query information from the `MPI_Status`.
