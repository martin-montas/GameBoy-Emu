# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation
# *****************************************************
SRC_DIR := ./src
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.hpp')

# Compiler and flags
CXX := g++
CXXFLAGS := -I./src -std=c++17
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Target for the main program
gameboy: main.o cpu.o game-boy.o instructions.o extended-instructions.o mmu.o
	$(CXX) main.o cpu.o game-boy.o instructions.o extended-instructions.o mmu.o -o gameboy $(LDFLAGS)

# Object file rules
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

game-boy.o: ./src/game-boy.cpp ./src/game-boy.hpp
	$(CXX) $(CXXFLAGS) -c ./src/game-boy.cpp

instructions.o: ./src/instructions.cpp ./src/instructions.hpp
	$(CXX) $(CXXFLAGS) -c ./src/instructions.cpp

extended-instructions.o: ./src/extended-instructions.cpp ./src/instructions.hpp
	$(CXX) $(CXXFLAGS) -c ./src/extended-instructions.cpp

cpu.o: ./src/cpu/cpu.cpp ./src/cpu/cpu.hpp
	$(CXX) $(CXXFLAGS) -c ./src/cpu/cpu.cpp

mmu.o: ./src/memory/mmu.cpp ./src/memory/mmu.hpp
	$(CXX) $(CXXFLAGS) -c ./src/memory/mmu.cpp


# Clean rule
clean:
	rm -f *.o gameboy test_binary

# Lint rule
lint:
	@cpplint $(CPP_FILES)

# *****************************************************
# Rules for Boost.Test
# *****************************************************

# Target for unit tests
TEST_SRC := ./test/test_main.cpp ./test/test_cpu.cpp ./test/test_game-boy.cpp ./test/test_mmu.cpp ./test/test_instructions.cpp ./test/test_extended-instructions.cpp
TEST_BIN := test_binary

test: $(TEST_BIN)
	./$(TEST_BIN) # Run tests after building

$(TEST_BIN): $(TEST_SRC) cpu.o game-boy.o instructions.o mmu.o
	$(CXX) $(CXXFLAGS) $(TEST_SRC) cpu.o game-boy.o instructions.o -o $(TEST_BIN) \
		-lboost_unit_test_framework

# Clean tests
clean-test:
	rm -f $(TEST_BIN)
