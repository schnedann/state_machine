#!/bin/bash

BDIR=build

# Cleanup Build-Directory if it exists
if [ -d "$BDIR" ]; then
  # Control will enter here if $DIRECTORY doesn't exist.
  cd "$BDIR"
  pwd

  rm -rf ./*
  ls
  
  cd ..
  pwd

fi

