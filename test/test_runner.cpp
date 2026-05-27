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
  TestCase       tc = load_json(path);

  Timer          timer;
  Serial         serial;
  Cpu            cpu;
  MMU            mmu("dummy.rom", &timer, &serial);
  InstructionSet instruction(mmu, cpu);

  // 1. load CPU state
  cpu.setState(tc.initialCPU);

  // 2. load memory snapshot
  for (auto &[addr, val] : tc.initialMemory) {
    mmu.write8(addr, val);
  }

  // 3. execute N steps
  for (int i = 0; i < tc.steps; i++) {
    cpu.step();
  }

  // 4. check result
  // CPUState got = cpu.getState();
  // CPUState got = cpu.expectedCPU;

  // if (compareCPU(got, tc.expectedCPU)) {
  //   std::cout << "[PASS] " << path << "\n";
  //   return 0;
  // } else {
  //   std::cout << "[FAIL] " << path << "\n";

  //   std::cout << "Expected PC: " << tc.expectedCPU.PC << " Got: " << got.PC << "\n";

  //   std::cout << "Expected A: " << (int)tc.expectedCPU.A << " Got: " << (int)got.A << "\n";

  //   return 1;
  // }
}
