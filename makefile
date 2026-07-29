SRC_DIR := ./src

CXX := g++
CXXFLAGS := -g -std=c++20  
LDFLAGS := -lSDL2

COMMON_OBJS := main.o gameboy.o cpu.o instructions.o \
               extended-instructions.o bus.o MBC.o \
               MBC0.o timer.o serial.o ppu.o  sdl-utils.o \

GameBoy: $(COMMON_OBJS)
	$(CXX) $^ -g -o $@ $(LDFLAGS)

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

bus.o: src/bus.cpp src/bus.hpp
	$(CXX) -c $< $(CXXFLAGS)

MBC.o: src/MBC.cpp src/MBC.hpp
	$(CXX) -c $< $(CXXFLAGS)

MBC0.o: src/MBC0.cpp src/MBC0.hpp
	$(CXX) -c $< $(CXXFLAGS)

timer.o: src/timer.cpp src/timer.hpp
	$(CXX) -c $< $(CXXFLAGS)

serial.o: src/serial.cpp src/serial.hpp
	$(CXX) -c $< $(CXXFLAGS)

ppu.o: src/ppu.cpp src/ppu.hpp
	$(CXX) -c $< $(CXXFLAGS)

sdl-utils.o: src/sdl-utils.cpp src/sdl-utils.hpp
	$(CXX) -c $< $(CXXFLAGS)

# test-runner.o: src/test/test-runner.cpp src/test/test-runner.hpp
# 	$(CXX) -c $< $(CXXFLAGS)

# =====================================================
# UTILITIES
# =====================================================

clean:
	rm -f *.o GameBoy

format:
	find . \( -name "*.cpp" -o -name "*.hpp" \) | \
	xargs clang-format -style=llvm -i

lint:
	cpplint $(shell find . \( -name "*.cpp" -o -name "*.hpp" \))

count:
	find . \( -name "*.cpp" -o -name "*.hpp"  \) | xargs wc -l

t:
	./sm83.sh

.PHONY: clean format lint
