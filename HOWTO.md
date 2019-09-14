# Installation & Compilation

This tutorial is using a Linux environment (examples will be provided for an Ubuntu distribution). Those instructions needs adjustements to be used on Windows & macOS environments.

## Required packages for compilation

The following packets are required: (`GCC`, `GDB`, `make`) and the (`libc`) 

The Installation of those packages can be perfomed using: `sudo apt-get update & sudo apt-get install build-essential gdb`.

## Required librairies

The SDL v1.2 library is required. Its installation can be performed using: `sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev`.

Please ensure that the librairies is correctly added to your "path".

## Compilation

The compilation is made using the Makefile: `cd sources & make`

# Execution

The compiled binary can be found in the *bin* folder and launched using: `./bin/bombeirb`

