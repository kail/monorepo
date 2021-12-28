# Snoot - Dog Activity Monitor

## Setup
1. Install ARM GNU toolchain: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
1. Install OpenOCD: https://openocd.org/pages/getting-openocd.html
1. Fetch all submodules, if that hasn't been done: `git submodule update --init --recursive`

## Organization
 * *app*: Contains application code
 * *board*: Contains board-specific code for different board revisions (including linker and startup files)
 * *vendor/fw/STM32CubeL1*: Submodule containing STM32 HAL

## Building and running
1. Build the binary: `make`
1. Debug the application `make debug`
