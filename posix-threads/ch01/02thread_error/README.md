# COol things to look at

This sample should return the following:
```
error 3: No such process
```

Which is an `ESRCH` error and it is the epected behaviour (ost of the time).


Funny thing we noticed while decreasing the buffer size from 4 MB to 1 MB
(setting `ERRSIZE` from 4096 to 1024) is that we got the following:
```
error 22: Invalid argument
```

Which corrsponds to an `EINVAL` error number (invalid error number), where one 
would expect an `ERANGE` (insufficient storage for error description string).

