CC=gcc
CFLAGS=-fPIC -g -Wall -Werror
INFLAGS=-Iinclude
LDFLAGS=-shared
TCFLAGS=-Wall -Werror -Wno-unused-variable
TLFLAGS=-Llib -Wl,-rpath=lib

INCDIR=include
LIBDIR=lib
TARGET=$(LIBDIR)/libmyc.so

HEADERS=$(shell find $(INCDIR) -name "*.h")
SOURCES=$(shell find . ! -name "test_*" -a -name "*.c")
OBJECTS=$(SOURCES:.c=.o)

TSRC=$(shell find . -name "test_*" -a -name "*.c")
TEXEC=$(TSRC:.c=)



all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p lib
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INFLAGS) -c -o $@ $<


tests: all 
	for t in ${TSRC}; do \
		EXEC="$${t::-2}"; \
		echo -e "\e[32mTesting $${t}...\e[0m\n"; \
		gcc ${TCFLAGS} ${TLFLAGS} ${INFLAGS} -o $${EXEC} $${t} -lmyc && \
		./$${EXEC}; \
	done

clean:
	rm -f $(OBJECTS)

clean-all:
	rm -f $(OBJECTS)
	rm -f $(TEXEC)
	rm -f $(TARGET)
