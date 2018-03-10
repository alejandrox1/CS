# Pthreads cool stuff
Compile 
```
gcc -pthread main.c 
```

# Original
```
user -> pthreads$ gcc -pthread original.c 
original.c: In function ‘print_hello_world’:
original.c:12:9: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘void *’ [-Wformat=]
  printf("Hello terminal! This is thread %d\n", tid);
         ^
original.c: In function ‘main’:
original.c:24:65: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
   status = pthread_create(&threads[i], NULL, print_hello_world, (void *)i);
```

* Hack to pass an integer to a thread (`(void *)i`).
An `int` will be 4 bytes long and a pointer will be 4 bytes in `x8632` or 
8 bytes in `x86_64`.

An option to deal with this is to use a `intptr_t` type. 
This type has the same size of a pointer in every architecture.
It is capable of holding a value converted from a void pointer and then be
converted back to that type with a value that compares equal to the original
pointer.
```
int i;
pthread_create(..., (void *)(intptr_t) i);
```


* Checking with `man`:
```
pthread_create(..., (void *)&i);
```

Converting an `int` to `intptr_t` and back is unlikely to lose information.
However, there is no guarantee that `intptr_t` is wider than `int`.

Any pointer to an object or incomplete type can be converted to `void*` and 
back again without loss of information (no such guarantee for pointers to
functions).
Generally, `void *` allows for data of any type to pass by.
