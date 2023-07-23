#!/bin/bash

BUILD_DOCS="OFF"
BUILD_TESTS="OFF"
BUILD_TYPE="Release"

source config.sh

Help()
{
    echo "build the neo project"
    echo
    echo "usage: build.sh [OPTION]..."
    echo "options:"
    echo -e "\tg    enable debug info"
    echo -e "\td    build project docs"
    echo -e "\tt    build unit tests"
    echo -e "\th    print this help message"
}

Main()
{
    # Create the build directory if it does not already exist.
    mkdir -pv $NEO_BUILD_DIR

    pushd $NEO_BUILD_DIR > /dev/null
        cmake ../ \
              -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
              -DPACKAGE_TESTS=$BUILD_TESTS \
              -DCMAKE_INSTALL_PREFIX=$NEO_BIN_DIR \
              -DBUILD_DOCS=$BUILD_DOCS \
              -DCMAKE_BUILD_TYPE=$BUILD_TYPE && \
        make -j$(nproc) all                  && \
        make install

        # Exit if any of the above commands fails.
        if [ $? -ne 0 ];
        then
            exit 1
        fi
    popd > /dev/null
}

while getopts ":hgdt" flag
do
    case "$flag" in
        g) BUILD_TYPE="Debug";;
        d) BUILD_DOCS="ON";;
        t) BUILD_TESTS="ON";;
        h) Help
           exit;;
       \?) echo "error: invalid option '$OPTARG'"
           Help
           exit;;
    esac
done

Main
