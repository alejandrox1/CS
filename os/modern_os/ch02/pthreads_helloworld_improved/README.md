Adapted from: [stackoverflow: using of shared variable by 10 pthreads](https://stackoverflow.com/questions/10879420/using-of-shared-variable-by-10-pthreads)

If program is compiled and run as:
```
user -> helloworld_pthreads$ gcc -Wall -pthread main.c && ./a.out
Hello from 2
Hello from 3
Hello from 6
Hello from 3
Hello from 7
Hello from 8
Hello from 4
Hello from 7
Hello from 9
Hello from 9
```

* `main` is unaware of our lock.

* A lock is not enough. It would be better for threads to signal each other -
  we do not want main to increment the variable `i` until a thread is done
  printing it).

  ```                          
  for (i=0; i<NUMBER_OF_THREADS; i++)                                         
  {                                                                           
      params.id = i;                                                          
	  pthread_create(&threads[i], NULL, hello, &params);             
  } 
  ```
  
  An option to achieve this is to use condition variables (`pthread_cond_t`):
  While a `mutex` helps avoid data races protecting an operation (or set of
  operations), it doesn't allow to wait until another thread completes a given
  activity.

### TODO: Add more on `pthread_cond_t` 
* [Condition variables](https://www.cs.nmsu.edu/~jcook/Tools/pthreads/pthread_cond.html)
