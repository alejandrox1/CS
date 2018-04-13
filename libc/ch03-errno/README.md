# errno.h - Error Handling

* Have each function return value be the answer to a requested system call.
  This may, however, make it difficult to report errors in a way that is easy
  to test.

* Have each function return its value as a success or a failure indication.
  This may complicate getting our answer from the returned value.

Best solution? Define an error return value that is distinguishable from any
valid answer.
The functions do not exactly report which error occurred.
The specific error is stored in a variable with external linkage.
