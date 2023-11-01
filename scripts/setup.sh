#!/bin/bash

conan install conanfile.txt --output-folder build --build missing --settings=build_type=Debug
if [ $? -eq 0 ]; then
    cd ./build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
    cmake --build .
fi;
