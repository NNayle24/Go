# Compiler settings - Can use g++ for both C and C++
CC = g++
CFLAGS = -g -Wall -c
CPPFLAGS = $(CFLAGS)  # Add C++ specific flags if necessary
LFLAGS = -lpthread -lm -lsfml-graphics -lsfml-window -lsfml-system

# Output binary
OUT = classiGO

# Object files
OBJS = board.o item.o list.o go_interface.o

# Source and header files (used only for dependency)
SOURCE = board.c item.c list.c go_interface.cpp
HEADER = board.h const.h item.h list.h struct.h

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LFLAGS)

# Pattern rule for C source
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Pattern rule for C++ source
%.o: %.cpp
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(OUT)

.PHONY: all clean

