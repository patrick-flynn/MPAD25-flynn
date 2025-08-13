# MPAD25-flynn
Flynn's repo for the CSE 40679 Fall 2025 class

Very much a work-in-progress

## Getting Started

1. Install firmware and emulator
 1. `git clone https://github.com/paulscottrobson/neo6502-firmware.git`
 2. Follow instructions in `neo6502-firmware/README.md`
  * If you're a Mac person, I recommend installing a Linux VM using `multipass` (at least 16GB of disk) and using it for development
  * if you're a Linux person, make sure you do `pip install git-python`
  * Hopefully the Windows instructions work because I have no way to test them.
 3. Make sure you can run the `neo` emulator; enter a oneline BASIC program:
  * `10 PRINT "HELLO"`
  * `RUN`
  If it runs, your emulator works.

1. Install Andy McCall's C examples: `git clone https://github.com/andymccall/neo6502-development.git`
1. Make sure you can follow each and every one of the C examples.

1. Look at Flynn's examples also.
