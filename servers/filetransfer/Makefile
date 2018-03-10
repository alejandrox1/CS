client:
	gcc -g -Wall -pthread client.c -o fclient -lssl -lcrypto

server: 
	gcc -g -Wall -pthread server.c -o fserver -lssl -lcrypto

clean:
	rm -rf fserver fclient
