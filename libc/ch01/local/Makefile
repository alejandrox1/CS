CC=gcc
CFLAGS=-fPIC -g -Wall -Werror
LDFLAGS=-shared

INCDIR=include
LIBDIR=lib
TARGET=$(LIBDIR)/libc.so

HEADERS=$(shell find $(INCDIR) -name "*.h")
SOURCES=$(shell find . ! -name "test_*" -a -name "*.c")
OBJECTS=$(SOURCES:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS)

clean-all:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
