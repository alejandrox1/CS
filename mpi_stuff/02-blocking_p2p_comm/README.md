# Blocking Point-to-Point Communications

For point-to-point communication we will employ the flowing two MPI methods:
```
MPI_Send(void *data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator);

MPI_Recv(void *data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status *status);
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
