#!/bin/bash

set -eu


if !(type "brew" > /dev/null 2>&1); then
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | bash
	brew update && brew upgrade
fi

if !(type "Ninja" > /dev/null 2>&1); then
	brew install Ninja
fi

if !(type "Cmake" > /dev/null 2>&1); then
	brew install Cmake
fi

if [ ! -d test ]; then
	mkdir test
fi

if [ ! -d googletest ]; then
	git clone https://github.com/google/googletest.git googletest
fi

cd googletest 

cmake -S . -B build -G Ninja -DCMAKE_INSTALL_PREFIX=../test
cmake --build build
cmake --install build
