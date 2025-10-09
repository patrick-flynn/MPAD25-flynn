### 31-hw-asm

This program prints "Hello, World!" to the console of the NEO6502.

#### Building

Perform the following commands to compile the project:

```
username@host:~$ cd neo6502-development/31-hw-asm/c
Building project...
mkdir -p bin
mos-neo6502-clang -Os -o bin/31-hw-asm.neo src/main.c
rm bin/31-hw-asm.neo.elf
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
31-hw-asm.neo          84 bytes.
```

Then run the binary using by loading it and issuing the run command:

```
load"31-hw-asm.neo"
Hello, World!
```
