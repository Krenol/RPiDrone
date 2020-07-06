#!/bin/bash

mkdir build

cmake -S . -B build/

cmake --build ./build || exit 1

export TRIGGER_PIN=GPIO12
export ECHO_PIN_FRONT=GPIO26
export SOCKET_PORT=8889

./build/lib/rpidrone