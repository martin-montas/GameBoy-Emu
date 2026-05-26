#include "test_case.hpp"
#include <iostream>

// your emulator
#include "../emu/cpu/cpu.hpp"
#include "../emu/mmu/mmu.hpp"

static bool compareCPU(const CPUState &a, const CPUState &b);

int         run_test(const std::string &path) {
  TestCase tc = load_json(path);

  CPU      cpu;
  MMU      mmu;

  // 1. load CPU state
  cpu.setState(tc.initialCPU);

  // 2. load memory snapshot
  for (auto &[addr, val] : tc.initialMemory) {
    mmu.write(addr, val);
  }

  // 3. execute N steps
  for (int i = 0; i < tc.steps; i++) {
    cpu.step(mmu);
  }

  // 4. check result
  CPUState got = cpu.getState();

  if (compareCPU(got, tc.expectedCPU)) {
    std::cout << "[PASS] " << path << "\n";
    return 0;
  } else {
    std::cout << "[FAIL] " << path << "\n";

    std::cout << "Expected PC: " << tc.expectedCPU.PC << " Got: " << got.PC << "\n";

    std::cout << "Expected A: " << (int)tc.expectedCPU.A << " Got: " << (int)got.A << "\n";

    return 1;
  }
}
