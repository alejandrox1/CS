# Using IPv4 protocol

Valid socket types are:

* `SOCK_STREAM` corresponds to a tcp connection.

* `SOCK_DGRAM` corresponds to an udp connection.

* `SOCK_RAW` opens a raw socket to access the IP protocol directly.


When a process should bind a socket to a local interface address using `bind`
when looking to receive an incoming packet or connection (`man ip`).

# TO-DO:
* Graceful server shut down (at the very least catch SIGINT, SIGKILL, SIGHUP).
