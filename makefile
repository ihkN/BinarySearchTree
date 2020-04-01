CC := g++
CFLAGS := -std=c++17 -g -Wall -Wextra

PRJS := bst 
SRCS := $(wildcard *.cpp) 
OBJS := $(SRCS:%.cpp=%.o)
EXES := $(PRJS:%=%.x)

all: $(EXES)

%.x: %.o
	$(CC) $(CFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(EXES)

clean:
	rm -f *.o *.x 

.PHONY: all run clean

bst.x: bst.o
