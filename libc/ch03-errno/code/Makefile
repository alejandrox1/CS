CC=gcc
CFLAGS=-Wall -Werror -g
IFLAGS=-I./../local/include
LDFLAGS=-L./../local/lib -Wl,-rpath=./../local/lib

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=$(OBJ:.o=)


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) -lmyc

%.o: %.c
	$(CC) $(CFLASG) $(IFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJ)

clean-all:
	rm -f $(OBJ)
	rm -f $(EXEC)
