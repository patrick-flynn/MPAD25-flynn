# MPAD25-flynn
Flynn's repo for the CSE 40679 Fall 2025 class

Very much a work-in-progress

## `setpath.bash`
The `setpath.bash` sceript is what I use on my systems to quickly add the `neo` emulator and the `llvm-mos` compiler suite to my search path. They assume that llvm-mos is installed in `/opt/llvm-mos` and that the neo6502-firmware is stored in `~/Desktop/MPAD25/forks/neo6502-firmware`. You will probably need to change it.  Use this script by `source setpath.bash`. If you run `bash setpath.bash`, it won't do anything because you're running it in a child `bash` process.

## Getting Started

1. Install firmware and emulator
  1. `git clone https://github.com/paulscottrobson/neo6502-firmware.git`
  2. Follow instructions in `neo6502-firmware/README.md`
  3. Make sure you can run the `neo` emulator; enter a one line BASIC program:
   * `10 PRINT "HELLO"`
   * `RUN` - 
   If it runs, your emulator works.

1. Install llvm-mos-sdk (binaries available for Windows and Linux, Mac compilation was straightforward - ask for help if you're struggling) 

1. Install Andy McCall's C examples from [here](https://github.com/andymccall/neo6502-development.git) and make sure you can follow each and every one of the C examples.

1. Look at and run the examples here. 
