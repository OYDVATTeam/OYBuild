# OYBuild
OYDVAT build system. \
It handles compiling programs
# 1. Bootstrap program (you probably dont even have it right now)
Make sure to run this first: ``` chmod +x build.sh ```
```
$ ./build.sh
```
# 2. Compile the program
```
$ ./oybuild_bootstrap
```
# 3. (Optional) Test OYBuild
Enter testsuite directory and run:
```
$ python3 test.py
```
Note that this operation requires Python
# 4. Install it
... by copying the oybuild executable to /usr/bin
