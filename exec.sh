#!/bin/bash

mkdir build

cmake -S . -B build/

cmake --build ./build || exit 1

./build/lib/rpidrone