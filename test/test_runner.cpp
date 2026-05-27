#include "test_case.hpp"
#include <iostream>

// your emulator
#include "./../src/cpu.hpp"
#include "./../src/instructions.hpp"
#include "./../src/mmu.hpp"
#include "./../src/serial.hpp"
#include "./../src/timer.hpp"

static bool compareCPU(const CPUState &a, const CPUState &b);

int         run_test(const std::string &path) {

  std::vector<TestCase> tests = load_json(path);

  for (auto &tc : tests) {

    Serial         serial;
    Timer          timer;
    MMU            mmu("test.rom", &timer, &serial);
    Cpu            cpu;
    InstructionSet instr(&mmu, &cpu);

    // setup cpu state
    cpu.PC         = tc.initialCPU.PC;
    cpu.SP         = tc.initialCPU.SP;

    cpu.A          = tc.initialCPU.A;
    cpu.B          = tc.initialCPU.B;
    cpu.C          = tc.initialCPU.C;
    cpu.D          = tc.initialCPU.D;
    cpu.E          = tc.initialCPU.E;
    cpu.F          = tc.initialCPU.F;
    cpu.H          = tc.initialCPU.H;
    cpu.L          = tc.initialCPU.L;

    // fetch opcode
    uint8_t opcode = mmu.read8(cpu.PC);

    // execute instruction
    instr.execute(opcode);

    // compare result
    if (cpu.A != tc.expectedCPU.A) {
      std::cout << "FAIL: " << tc.name << "\n";
      return 1;
    }
  }

  std::cout << "PASS\n";

  return 0;
}
