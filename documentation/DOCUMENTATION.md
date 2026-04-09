# OYBuild --- OYDVAT build system

# 1. Base syntax
## 1.01: Variables and lists
Variables can be used to store lists \
Example:
```
SOURCES = ["main.c"]
```
With variable expansion:
```
SOURCE = ["setup.c"]
SOURCES = [SOURCE, "main.c"]
```
## 1.02: compile()
Usage:
```
compile(file, [flags])
```
Compiles a C file \
Example:
```
compile("main.c")
```
With variable expansion:
```
FLAGS = ["-Wnull-dereference"]
compile("main.c", FLAGS)
```
With flags:
```
compile("main.c", ["-Iinclude"])
```
## 1.03: Loops
The for keyword is used for loops \
The endfor keyword is used for ending loop \
Example:
```
SOURCES = ["main.c", "util.c", "math.c"]

for (file in SOURCES) do
    compile(file)
endfor
```

# Sorry, documentation isnt fully finished yet
