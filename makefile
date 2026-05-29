SRC_DIR := ./src

CXX := g++
CXXFLAGS := -I./src -std=c++20
LDFLAGS :=

COMMON_OBJS := main.o gameboy.o cpu.o instructions.o \
               extended-instructions.o mmu.o MBC.o \
               MBC0.o timer.o serial.o

gameboy: $(COMMON_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# =====================================================
# COMPILATION RULES
# =====================================================
main.o: main.cpp
	$(CXX) -c $< $(CXXFLAGS)

gameboy.o: src/gameboy.cpp src/gameboy.hpp
	$(CXX) -c $< $(CXXFLAGS)

cpu.o: src/cpu.cpp src/cpu.hpp
	$(CXX) -c $< $(CXXFLAGS)

instructions.o: src/instructions.cpp src/instructions.hpp
	$(CXX) -c $< $(CXXFLAGS)

extended-instructions.o: src/extended-instructions.cpp src/instructions.hpp
	$(CXX) -c $< $(CXXFLAGS)

mmu.o: src/mmu.cpp src/mmu.hpp
	$(CXX) -c $< $(CXXFLAGS)

MBC.o: src/MBC.cpp src/MBC.hpp
	$(CXX) -c $< $(CXXFLAGS)

MBC0.o: src/MBC0.cpp src/MBC0.hpp
	$(CXX) -c $< $(CXXFLAGS)

timer.o: src/timer.cpp src/timer.hpp
	$(CXX) -c $< $(CXXFLAGS)

serial.o: src/serial.cpp src/serial.hpp
	$(CXX) -c $< $(CXXFLAGS)

# =====================================================
# UTILITIES
# =====================================================

clean:
	rm -f *.o gameboy

format:
	find . \( -name "*.cpp" -o -name "*.hpp" \) | \
	xargs clang-format -style=llvm -i

lint:
	cpplint $(shell find . \( -name "*.cpp" -o -name "*.hpp" \))

.PHONY: clean format lint
