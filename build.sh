#!/bin/sh
set -eu

# configuration for oybuild
CXX=${CXX:-g++}
CXXFLAGS="-std=c++17 -Wall -Wextra -pedantic -O2"

TARGET="oybuild"
SOURCES="
src/executor.cpp
src/lexer.cpp
src/parser.cpp
src/main.cpp
"

# building...
echo "[bootstrap] Building ${TARGET}..."
$CXX $CXXFLAGS $SOURCES -o $TARGET

echo "[bootstrap] Done."
echo "[bootstrap] Run ./${TARGET} to build using OYBuildfile"
