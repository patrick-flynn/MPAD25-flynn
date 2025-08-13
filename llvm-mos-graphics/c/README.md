### llvm-mos-graphics

This program prints "Hello, World!" to the console of the NEO6502.

#### Building

Perform the following commands to compile the project:

```
username@host:~$ cd neo6502-development/llvm-mos-graphics/c
Building project...
mkdir -p bin
mos-neo6502-clang -Os -o bin/llvm-mos-graphics.neo src/main.c
rm bin/llvm-mos-graphics.neo.elf
```

#### Running the program

The newly built program can be run within the neo emulator using the following commands:

```
username@host:~$ make run
```

This will launch the neo emulator.

Once the emulator has started check the binary is accessible to the emulator by using the dir command:

```
cat
llvm-mos-graphics.neo          84 bytes.
```

Then run the binary using by loading it and issuing the run command:

```
load"llvm-mos-graphics.neo"
Hello, World!
```
