#include <fstream>
#include <nlohmann/json.hpp>
#include <stdint.h>

using json = nlohmann::json;

TestCase load_json(const std::string &path) {
  std::ifstream f(path);
  json          j;
  f >> j;

  TestCase tc;

  // metadata
  tc.name           = j.value("name", "");
  tc.steps          = j.value("steps", 1);

  // initial CPU
  auto &icpu        = j["initial"]["cpu"];
  tc.initialCPU.A   = icpu["A"];
  tc.initialCPU.B   = icpu["B"];
  tc.initialCPU.C   = icpu["C"];
  tc.initialCPU.D   = icpu["D"];
  tc.initialCPU.E   = icpu["E"];
  tc.initialCPU.H   = icpu["H"];
  tc.initialCPU.L   = icpu["L"];
  tc.initialCPU.F   = icpu["F"];
  tc.initialCPU.PC  = icpu["PC"];
  tc.initialCPU.SP  = icpu["SP"];

  // expected CPU
  auto &ecpu        = j["expected"]["cpu"];
  tc.expectedCPU.A  = ecpu["A"];
  tc.expectedCPU.B  = ecpu["B"];
  tc.expectedCPU.C  = ecpu["C"];
  tc.expectedCPU.D  = ecpu["D"];
  tc.expectedCPU.E  = ecpu["E"];
  tc.expectedCPU.H  = ecpu["H"];
  tc.expectedCPU.L  = ecpu["L"];
  tc.expectedCPU.F  = ecpu["F"];
  tc.expectedCPU.PC = ecpu["PC"];
  tc.expectedCPU.SP = ecpu["SP"];

  // memory
  if (j["initial"].contains("memory")) {
    for (auto &[addr, val] : j["initial"]["memory"].items()) {
      uint16_t a          = std::stoi(addr, nullptr, 0);
      tc.initialMemory[a] = val;
    }
  }

  return tc;
}
