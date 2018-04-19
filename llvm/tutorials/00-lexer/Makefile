CC=clang++
CFLAGS=-g -O3 -Wno-unknown-warning-option

SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
EXC=$(OBJ:.o=)

all: $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $(EXC) `llvm-config --cxxflags`

clean:
	rm -f $(OBJ)
	rm -f $(EXC)
