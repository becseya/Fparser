#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

apt-get install libgtest-dev cmake
cd /usr/src/gtest
cmake CMakeLists.txt
make
cp *.a /usr/lib