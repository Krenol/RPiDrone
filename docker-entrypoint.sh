#!/bin/bash
set -e

#set vars
BUILD_DIR=/data/build
EXEC=/data/rpidrone/build/RPiDrone

# SET BUILD_TEST IF IT WASN'T SET
if [ "$BUILD_TEST" != 'ON' ] && [ "$BUILD_TEST" != 'OFF' ]; then
    BUILD_TEST=ON
fi

# RUN INPUT
if [ "$1" = 'start' ]; then
    cmake --build $BUILD_DIR 
    $EXEC
elif [ "$1" = 'build' ]; then
    cmake --build $BUILD_DIR 
fi

exec "$@"
