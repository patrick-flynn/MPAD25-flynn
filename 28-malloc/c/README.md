[Source: Andy McCall's [neo6502-development](https://github.com/andymccall/neo6502-development) repo

### 28-malloc

A memory allocator tester.

#### Building

Perform the following commands to compile the project:

```
username@host:~$ cd neo6502-development/28-malloc/c
Building project...
mkdir -p bin
mos-neo6502-clang -Os -o bin/28-malloc.neo src/main.c
rm bin/28-malloc.neo.elf
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
28-malloc.neo          84 bytes.
```

Then run the binary using by loading it and issuing the run command:

```
load"28-malloc.neo"
Hello, World!
```
