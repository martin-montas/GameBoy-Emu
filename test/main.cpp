#include <filesystem>
#include <iostream>
#include <string>

// your test interface
#include "test_case.hpp"

int run_test(const std::string &path);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: test_runner <file_or_directory>\n";
    return 1;
  }

  std::string path = argv[1];

  // CASE 1: single test file
  if (std::filesystem::is_regular_file(path)) {
    int result = run_test(path);
    return result ? 0 : 1;
  }

  // CASE 2: directory of tests
  if (std::filesystem::is_directory(path)) {
    int total  = 0;
    int failed = 0;

    for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
      if (entry.path().extension() == ".json") {
        total++;

        int result = run_test(entry.path().string());
        if (!result) {
          failed++;
          std::cerr << "[FAIL] " << entry.path() << "\n";
        }
      }
    }

    std::cout << "\n====================\n";
    std::cout << "Total : " << total << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Passed: " << (total - failed) << "\n";

    return failed ? 1 : 0;
  }

  std::cerr << "Invalid path: not a file or directory\n";
  return 1;
}
