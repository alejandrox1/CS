# Dynamic Receiving with MPI Probe and MPI Status

In the previous section [blocking p2p communication](../02-blocking_p2p_comm)
we deal with the messages whose length we knew right from the start.
So now, we will generalize!


Find out the length of a mesage:
```
MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);
```

```
├── project
│   ├── Makefile
│   ├── ping_pong.c
│   ├── ring.c
│   └── send_recv.c
...
```

1. `send_recv.c` shows the usage of `MPI_Send` and `MPI_Recv` to pass a number
   between two MPI tasks.

2. `ping_pong.c` spawns processes that send a number back and forth until it 
   reaches a specifid limit.

3. `ring.c` sends a mesage from task to task ending with task rank 0 receiving
   the message from task rank `world_size-1`.
