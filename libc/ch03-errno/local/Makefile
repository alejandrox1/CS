CC=gcc
CFLAGS=-fPIC -g -Wall -Werror
INFLAGS=-Iinclude
LDFLAGS=-shared

INCDIR=include
LIBDIR=lib
TARGET=$(LIBDIR)/libmyc.so

HEADERS=$(shell find $(INCDIR) -name "*.h")
SOURCES=$(shell find . ! -name "test_*" -a -name "*.c")
OBJECTS=$(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTS)

clean-all:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
