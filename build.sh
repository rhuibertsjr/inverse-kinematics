#!/bin/bash

if [ ! -d "./bin" ]; then
    mkdir "./bin"
fi

pushd "./bin"

if [ "$1" == "--no-raylib" ]; then
    gcc -g ../src/core.c ../src/math.c -o myprogram.exe
else
    gcc -g -D RAYLIB ../src/core.c ../src/math.c -o myprogram.exe -lraylib -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32
fi

popd
