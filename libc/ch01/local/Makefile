CC=gcc
CFLAGS=-fPIC -g -Wall -Werror
LDFLAGS=-shared

INCDIR=include
LIBDIR=lib
TARGET=$(LIBDIR)/libc.so

HEADERS=$(shell find $(INCDIR) -name "*.h")
SOURCES=$(shell find . ! -name "test_*" -a -name "*.c")
OBJECTS=$(SOURCES:.c=.o)


SRC=main.c
OBJ=$(SRC:.c=.o)
EXEC=$(SRC:.o=)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

run: $(TARGET) $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -L$(LIBDIR) -Wl,-rpath=$(LIBDIR) -o $@ $(OBJ) -lc

%.o: %.c
	$(CC) -Wall -c -o $@ $<

#EXEC=main
#SRC=$(shell find . ! -name "test_*" -a -name "*.c")
#OBJ=$(SRC:.c=.o)

#TEST=""
#TSRC="$(TEST)/test_$(TEST).c"
#TOBJ=$(TSRC:.c=.o)
#TEXEC=$(TSRC:.c=)


#all: $(EXEC)

#test: $(TEXEC)

#$(EXEC): $(OBJ)
#	$(CC) -o $@ $(OBJ)

#$(TEXEC): $(TOBJ) $(TOBJ) $	(filter-out $(EXEC).o, $(OBJ))
#	$(CC) -o $@ $(TOBJ) $(filter-out $(EXEC).o, $(OBJ))

#%.o: %.c
#	$(CC) $(CFLAGS) $(EXTRA_FLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJ)

clean-all:
	rm -f $(OBJECTS)
	rm -f $(OBJ)
	rm -f $(TARGET)
