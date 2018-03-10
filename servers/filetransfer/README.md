# TO DO:
* Concurrency (client/server)

# Cool things to do
* Make sure files created are unique (set).
* Optimize IO ops.

# Refs
[C- Socket : Programming a Client/Server-Application to send a file](https://stackoverflow.com/questions/9875735/c-socket-programming-a-client-server-application-to-send-a-file)

* `send(...)`
 If the socket is non-blocking, then check the socket error for 
 WSAEWOULDBLOCK/EAGAIN (depending on platform) and if true then use select() 
 to wait for a small period of time to see if the socket becomes writable again
 before failing	the	transfer.

* `serverSendFile`
 If you need to handle files > 2GB, be sure to use a 64bit integer, and a 
 host-to-network function that can handle 64bit integers.

* `recv(...)`
 If the socket is non-blocking, then check the socket error for 
 WSAEWOULDBLOCK/EAGAIN (depending on platform) and if true then use select() to 
 wait for a small period of time to see if the socket becomes readable again 
 before failing the transfer.


# Notes
* [Functionality to read large files (> 3 GB) in chunks](https://codereview.stackexchange.com/questions/28039/functionality-to-read-large-files-3-gb-in-chunks)

* [64 bit ntohl() in C++?](https://stackoverflow.com/questions/809902/64-bit-ntohl-in-c)

* [How to calculate the MD5 hash of a large file in C?](https://stackoverflow.com/questions/10324611/how-to-calculate-the-md5-hash-of-a-large-file-in-c)
