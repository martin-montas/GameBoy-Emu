import json
import subprocess
import glob
import os


EMU_BIN = "./gameboy"
TEST_DIR = "./unit_test/sm83/v1/00.json"


def run_test(test_file):
    # run emulator with test file
    result = subprocess.run(
        [EMU_BIN, test_file],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    return result.stdout.strip(), result.stderr.strip(), result.returncode


def run_all_tests():
    files = sorted(glob.glob(os.path.join(TEST_DIR, "*.json")))

    passed = 0
    failed = 0

    for f in files:
        out, err, code = run_test(f)

        if "PASS" in out:
            print(f"[PASS] {f}")
            passed += 1
        else:
            print(f"[FAIL] {f}")
            print("stdout:")
            print(out)
            print("stderr:")
            print(err)
            failed += 1

    print("\n--- SUMMARY ---")
    print("Passed:", passed)
    print("Failed:", failed)


if __name__ == "__main__":
    run_all_tests()
