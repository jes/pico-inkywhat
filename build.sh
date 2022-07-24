#!/bin/sh

export PICO_SDK_PATH=~/pico-sdk
cd build
cmake ..
make

echo ---
echo If all went well, load build/inkywhat.elf with a debugger or build/inkywhat.uf2 on the filesystem.
