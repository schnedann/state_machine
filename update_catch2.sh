#!/bin/bash

if test ! -d ./external
then
  mkdir external  
fi

echo "$(tput setaf 6)Update Catch2 Unittest Framework$(tput sgr 0)"

src_dir="./external/Catch2"

cd external

if [ -d "Catch2" ]
then
  echo "Cloning...";
  cd Catch2
  git clone https://github.com/catchorg/Catch2.git;
  cd ..
else
  echo "Fetching...";
  git fetch https://github.com/catchorg/Catch2.git;
fi

cd ..
