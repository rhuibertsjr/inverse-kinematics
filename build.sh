#!/bin/bash

if [ ! -d "./bin/" ]; then
    mkdir -p "./bin"
fi

cd ./bin

gcc -g ../src/core.c ../src/math.c

cd ..
