#include "test_case.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <vector>

using json = nlohmann::json;

std::vector<TestCase> load_json(const std::string &path) {

  std::ifstream f(path);

  json          j;
  f >> j;

  std::vector<TestCase> tests;

  for (auto &entry : j) {

    TestCase tc;

    tc.name           = entry.value("name", "");

    auto &initial     = entry["initial"];
    auto &final       = entry["final"];

    // initial cpu
    tc.initialCPU.A   = initial["a"];
    tc.initialCPU.B   = initial["b"];
    tc.initialCPU.C   = initial["c"];
    tc.initialCPU.D   = initial["d"];
    tc.initialCPU.E   = initial["e"];
    tc.initialCPU.F   = initial["f"];
    tc.initialCPU.H   = initial["h"];
    tc.initialCPU.L   = initial["l"];
    tc.initialCPU.PC  = initial["pc"];
    tc.initialCPU.SP  = initial["sp"];

    // expected cpu
    tc.expectedCPU.A  = final["a"];
    tc.expectedCPU.B  = final["b"];
    tc.expectedCPU.C  = final["c"];
    tc.expectedCPU.D  = final["d"];
    tc.expectedCPU.E  = final["e"];
    tc.expectedCPU.F  = final["f"];
    tc.expectedCPU.H  = final["h"];
    tc.expectedCPU.L  = final["l"];
    tc.expectedCPU.PC = final["pc"];
    tc.expectedCPU.SP = final["sp"];

    tests.push_back(tc);
  }

  return tests;
}
