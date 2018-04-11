CC=gcc
CFLAGS=-Wall
EXTRA_FLAGS=-Iheaders
HEADERS=$(wildcard headers/*.h)
SRC=$(shell find $(pwd) -name "*.c")
OBJ=$(SRC:.c=.o)
EXEC=main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)
	rm -f $(EXEC)
