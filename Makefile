
# Variables
CPP = g++
PROGRAM_NAME = cop4520-assignment-03
CFLAGS = -std=c++17 -O3 -I include 

CPP_SRC = $(wildcard source/*.cpp)
CPP_OBJ = $(patsubst source/%.cpp,int/%.o,$(CPP_SRC))

BIN = bin/$(PROGRAM_NAME)

# Default target
all: directories $(BIN)

$(BIN): $(CPP_OBJ)
	$(CPP) $(CFLAGS) -o $@ $^

int/%.o: source/%.cpp
	$(CPP) $(CFLAGS) -c -o $@ $<

# Phony targets
.PHONY: clean directories

clean:
	rm -rf int bin

directories: int bin

int:
	mkdir -p int

bin:
	mkdir -p bin
