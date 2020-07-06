#!/bin/bash
set -e

#set vars
EXEC=/data/build/lib/rpidrone


# RUN INPUT
if [ "$1" = 'start' ]; then
    $EXEC
else 
    exec "$@"
fi
