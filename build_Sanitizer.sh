#!/bin/bash

BDIR=build
TARGET=unittest_sanitize
TARGET_OTHER=unittest

# Create Build-Directory if it does not exist
if [ ! -d "$BDIR" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
  mkdir build  
fi

cd build
pwd

# Force complete rebuild if normal build is found
if [ -f "$TARGET_OTHER" ]; then
  # Control will enter here if $TARGET_OTHER does exist.
  rm -rf ./*
fi

cmake ..
cmake --build . --target "$TARGET" --config ASAN -- -j

cd ..
pwd

ls build

