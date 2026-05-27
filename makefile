SRC_DIR := ./src
TEST_DIR := ./test

CXX := g++
CXXFLAGS := -I./src -std=c++20
LDFLAGS :=

# -------------------------
# EMULATOR OBJECTS
# -------------------------
EMU_OBJS := main.o cpu.o gameboy.o instructions.o extended-instructions.o \
            mmu.o MBC.o MBC0.o timer.o serial.o

gameboy: $(EMU_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# -------------------------
# TEST RUNNER OBJECTS
# -------------------------
TEST_OBJS := test_runner.o test_case.o cpu.o mmu.o timer.o serial.o MBC.o MBC0.o instructions.o extended-instructions.o

test_runner: $(TEST_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# -------------------------
# COMPILATION RULES
# -------------------------
main.o: main.cpp
	$(CXX) -c $< $(CXXFLAGS)

cpu.o: ./src/cpu.cpp ./src/cpu.hpp
	$(CXX) -c $< $(CXXFLAGS)

gameboy.o: ./src/gameboy.cpp ./src/gameboy.hpp
	$(CXX) -c $< $(CXXFLAGS)

instructions.o: ./src/instructions.cpp ./src/instructions.hpp
	$(CXX) -c $< $(CXXFLAGS)

extended-instructions.o: ./src/extended-instructions.cpp ./src/instructions.hpp
	$(CXX) -c $< $(CXXFLAGS)

mmu.o: ./src/mmu.cpp ./src/mmu.hpp
	$(CXX) -c $< $(CXXFLAGS)

MBC.o: ./src/MBC.cpp ./src/MBC.hpp
	$(CXX) -c $< $(CXXFLAGS)

MBC0.o: ./src/MBC0.cpp ./src/MBC0.hpp
	$(CXX) -c $< $(CXXFLAGS)

timer.o: ./src/timer.cpp ./src/timer.hpp
	$(CXX) -c $< $(CXXFLAGS)

serial.o: ./src/serial.cpp ./src/serial.hpp
	$(CXX) -c $< $(CXXFLAGS)

test_case.o: ./test/test_case.cpp ./test/test_case.hpp
	$(CXX) -c $< $(CXXFLAGS)

test_runner.o: ./test/test_runner.cpp ./test/test_case.hpp
	$(CXX) -c $< $(CXXFLAGS)

# -------------------------
# UTILITIES
# -------------------------
clean:
	rm -f *.o gameboy test_runner

format:
	find . \( -name "*.cpp" -o -name "*.hpp" \) | xargs clang-format -style=llvm -i

lint:
	cpplint $(shell find . \( -name "*.cpp" -o -name "*.hpp" \))

.PHONY: clean format lint
