### 14-spaceship

### Assets

 * sprite sheet from [here](https://www.kindpng.com/imgv/iwbwoih_spaceship-sprite-sheet-png-transparent-png/)

![](kindpng_3021848.png)

#### Building

Perform the following commands to compile the project:

```
username@host:~$ cd neo6502-development/14-spaceship/c
Building project...
mkdir -p bin
mos-neo6502-clang -Os -o bin/14-spaceship.neo src/main.c
rm bin/14-spaceship.neo.elf
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
14-spaceship.neo          84 bytes.
```

Then run the binary using by loading it and issuing the run command:

```
load"14-spaceship.neo"
Hello, World!
```
