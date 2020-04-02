CC := g++
CFLAGS := -std=c++17 -g -Wall -Wextra

PRJ := bst 
SRC := $(wildcard *.cpp) 
OBJ := $(SRC:%.cpp=%.o)
EXE := $(PRJ:%=%.x)

all: $(EXE)

%.x: %.o
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXE)

mem: all
	valgrind ./$(EXE) --leak-check=full -s

clean:
	rm -f *.o *.x 

.PHONY: all run clean

bst.x: bst.o
