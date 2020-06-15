#!/bin/bash
set -e

#set vars
EXEC=/data/rpidrone/build/RPiDrone


# RUN INPUT
if [ "$1" = 'start' ]; then
    $EXEC
else 
    exec "$@"
fi
