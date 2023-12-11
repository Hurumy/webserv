#!/bin/bash

set -e
apt update

echo "${0}: install telnet."
apt install -y telnet
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install telnet: $status"
  exit $status
fi

echo "${0}: install siege."
apt install -y siege
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install siege: $status"
  exit $status
fi

echo "${0}: install utilities for Google test."
apt install -y cmake
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install cmake: $status"
  exit $status
fi

apt install -y ninja-build
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install ninja: $status"
  exit $status
fi

if [ ! -d googletest ]; then
  git clone https://github.com/google/googletest.git googletest
  status=$?
  if [ $status -ne 0 ]; then
    echo "Failed to download googletest library: $status"
    exit $status
  fi
fi

cd googletest 

cmake -S . -B build -G Ninja -DCMAKE_INSTALL_PREFIX=../test
cmake --build build
cmake --install build

cd ..

echo "${0}: install python libraries"
python -m pip install --upgrade pip setuptools && python -m pip install -U -r requirement.txt
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to install python-dotenv: $status"
  exit $status
fi

echo "${0}: compile sources."
make fclean
make debug
status=$?
if [ $status -ne 0 ]; then
  echo "Failed to compile: $status"
  exit $status
fi

sleep infinity
