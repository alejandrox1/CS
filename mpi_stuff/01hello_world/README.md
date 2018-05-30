A common problem that may arise when starting out with mpi is the following:
```
$ make clean && make 
$ mpirun -np 2 hello_world
[proxy:0:0@N501VW] HYDU_create_process (utils/launch/launch.c:74): execvp error on file hello_world (No such file or directory)

```

The problem is that the executable `hello_world` is not in your `$PATH`, so 
insteadtry running it this way:
```
$ make clean && make && mpirun -np 2 ./hello_world 

```
