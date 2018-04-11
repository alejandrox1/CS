CC=gcc
CFLAGS=-fPIC -g -Wall -Werror
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

$(SOURCES:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SOURCES:.c=.d)

clean:
	rm -f $(OBJECTS)

clean-all:
	rm -f $(OBJECTS)
	rm -f $(SOURCES:.c=.d)
	rm -f $(TARGET)
