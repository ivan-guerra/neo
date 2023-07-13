#!/bin/bash

source config.sh

# Remove the binary directory.
if [ -d $NEO_BIN_DIR ]
then
    echo "removing '$NEO_BIN_DIR'"
    rm -r $NEO_BIN_DIR
fi

# Remove the CMake build directory.
if [ -d $NEO_BUILD_DIR ]
then
    echo "removing '$NEO_BUILD_DIR'"
    rm -r $NEO_BUILD_DIR
fi
