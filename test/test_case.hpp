#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct CPUState {
  uint8_t  A, B, C, D, E, H, L, F;
  uint16_t PC, SP;
};

struct TestCase {
  CPUState                              initialCPU;
  CPUState                              expectedCPU;
  std::unordered_map<uint16_t, uint8_t> initialMemory;
  int                                   steps;
  std::string                           name;
};

std::vector<TestCase> load_json(const std::string &path);
