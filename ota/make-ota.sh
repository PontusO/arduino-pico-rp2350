#!/bin/bash

set -e # Exit on error

export PICO_SDK_PATH="$(cd ../pico-sdk/; pwd)"
export PATH="$(cd ../system/arm-none-eabi/bin; pwd):$PATH"

rm -rf build
mkdir build
cd build
cmake .. -DPICO_PLATFORM=rp2350
make -j # VERBOSE=1
