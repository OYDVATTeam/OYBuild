import os
import sys

print("OYBuild Test Suite")
print("Testing OYBuild...")
try:
    os.chdir("test_empty_oybuildfile")
except Exception as e:
    print("ERROR: test_empty_oybuildfile")
    print(e)
    print("All tests failed. HINT: Please be in testsuite dir before running test.py")
    sys.exit(1)

test1_result = os.system("../../oybuild")
if test1_result == 0:
    print("PASS: test_empty_oybuildfile")
    sys.exit(0)
else:
    print("FAIL: test_empty_oybuildfile")
    sys.exit(1)
