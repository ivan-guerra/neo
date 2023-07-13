#!/bin/bash

CWD=$(pwd)

# Root directory.
NEO_PROJECT_PATH=$(dirname ${CWD})

# Scripts directory.
NEO_SCRIPTS_PATH="${NEO_PROJECT_PATH}/scripts"

# Binary directory.
NEO_BIN_DIR="${NEO_PROJECT_PATH}/bin"

# CMake build files and cache.
NEO_BUILD_DIR="${NEO_PROJECT_PATH}/build"
