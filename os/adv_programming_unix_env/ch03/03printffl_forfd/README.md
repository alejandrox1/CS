# Print file flags for specified descriptor

```
$ ./a.out 1
read and write, append

$ ./a.out 0
read and write, append

$ ./a.out 3
fcntl error for 3: Bad file descriptor

$ ./a.out 0 < /dev/tty
read only

$ ./a.out 1 > temp.foo
$ cat temp.foo 
write only

$ ./a.out 2 2>>temp.foo 
write only, append

$ ./a.out 5 5<>temp.foo
read and write
```
