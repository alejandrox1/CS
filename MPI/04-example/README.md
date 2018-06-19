# A Random Walk

Parallelizing a one dimensional random walk with periodic boundary conditions

1. Split the domain across MPI processes (domain decomposition).
   1. Each process determines their part in the domain.
   2. Each process initializes N walkers that start at the first value of their
      domain.
   3. Each walker has 2 associated values with it: its current position and the
      number of steps left to take.


```
├── project
│   ├── check_status.c
│   ├── Makefile
│   └── probe.c
...
```

1. `random_walk.c` MPI-parallelized one-diemnsional random walk with periodic
   boundary conditions.
