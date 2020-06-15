#!/bin/bash

mkdir build

cmake -S rpidrone/ -B build/

cmake --build ./build || exit 1

export TRIGGER_PIN=SDA1
export ECHO_PIN_FRONT=GPIO26
export SOCKET_PORT=8889

./rpidrone/build/RPiDrone