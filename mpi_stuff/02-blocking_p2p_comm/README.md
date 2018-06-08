# Blocking Point-to-Point Communications

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
