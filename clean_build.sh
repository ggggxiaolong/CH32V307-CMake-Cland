#!/bin/sh

if [ -d "build/" ]; then
    echo "Found build/"
    rm -rf build
    mkdir build
    cd build
    cmake -GNinja ../
    ninja
else
    echo "Not Found build/"
    mkdir build
    cd build
    cmake -GNinja ../
    ninja
fi
