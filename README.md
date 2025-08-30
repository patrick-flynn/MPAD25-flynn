# MPAD25-flynn
Flynn's repo for the CSE 40679 Fall 2025 class

Very much a work-in-progress

## `setpath.bash`
The `setpath.bash` script is what I use on my systems to quickly add the `neo` emulator and the `llvm-mos` compiler suite to my search path. It assumes that `llvm-mos` is installed in `/opt/llvm-mos` and that the `neo6502-firmware` tree (which contains the `neo` emulator in its `bin` directory) is stored in `~/Desktop/MPAD25/forks/neo6502-firmware`.

*You will probably need to change `setpath.bash` for your environment*.  Suggestion: copy it to your class repo, change it to make it work, and call it good.  Use this script by running `source setpath.bash`. If you run `bash setpath.bash`, it won't do anything because you're running it in a child `bash` process. If you use a shell other than bash, you'll need to translate it for your shell environment (and the mere fact that you don't run bash means you probably know how to do that...).

## Getting Started

1.  Install firmware and emulator
    1. `git clone https://github.com/paulscottrobson/neo6502-firmware.git`
    2. Follow instructions in `neo6502-firmware/README.md`
    3. Make sure you can run the `neo` emulator; enter a one line BASIC program:
       * `10 PRINT "HELLO"`
       * `RUN`
       If it runs, your emulator works.
     
1. Install llvm-mos-sdk from [here](https://github.com/llvm-mos/llvm-mos-sdk) (binaries available for Windows and Linux, Mac compilation was straightforward - ask for help if you're struggling)
1. Install Andy McCall's C examples from [here](https://github.com/andymccall/neo6502-development.git) and make sure you can follow each and every one of the C examples.
1. Look at and run the example programs hosted here and described below.

### Starting your own project

1. Choose a project name (_e.g._, `assignment987`)
2. `bash tools/new_c_project.sh assignment987`
3. (you can copy the `assignment987` directory just created anywhere you want - for example, _your repo_)
4. Make sure your PATH is set up for Neo development (use your version of `setpath.sh`)
5. Before modifying the source code, make sure that `make` and `make run` work.

### Prof's demos
  * `01-helloworld`: ye olde hello worlde, the ur-program from the dawn of computer time. This is what you get if you run the `tools/new_c_project` bash script, specifying `01-helloworld` as the project name.
  * `02-charlife-c`: [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life), where cells are character positions on the screen. Side note: Life has been extensively studied in both math (cellular automata) and in CS (algorithms, data structures, _etc._). If you're curious about Life, this [site](https://playgameoflife.com/) and this [site](https://conwaylife.com/) are good places to visit.
  * `03-estimatepi`: estimate Pi by throwing darts!  Will be placed in the repo after Assignment 1 is due :) .
  * `04-stripes`: draw some stripey stripes
  * `05-dots`: draw some dotty dots
  * `06-twinkledots`: draw some _twinkling_ dots
  * `07-diagcolorstripes`: manipulate the 8-bit color palette and draw (one pixel at a time) colored lines
  * `08-bouncingsprite`: animate a random sprite moving across the screen. A companion Python script `makegfx.py` creates a graphics _asset file_ named `sprite.gfx` that is read into the Neo at runtime. Also: shows frame synchronization (if you turn off the `wait_frame()` function, the sprite will speed up to the point that the illusion of movement is lost (try it!). Final note: There are some extra bits in the `Makefile` to handle the generation and location of the `sprite.gfx` asset file.
  * `09-beeps-and-boops`: Bouncing sprite, with amusing (?) sound generation. In addition to sound generation (it's a one-liner to play the stored sound effects), the code waits until a sound is finished playing before playing another one. For long sounds, the effect is that the sprite pauses at one of the corners until the new sound starts.
  * `10-tilemap`: display (and then randomly update) a _tilemap_ - one of the canonical gaming "background" design approaches. Tiles here are 1 bit deep (black and white) and are 16x16 images, converted from a tile set I found [here](https://opengameart.org/content/tileset-1bit-color) to a `tileset.gfx` graphics asset using a python script.  This tileset is read in at runtime and the screen is represented as a small grid of indices (a _tilemap_) into the tile set.  After rendering all of the tiles, the program loops forever, randomly changing one of the indices in the tilemap to a different value and updating the tilemap.
  * `11-controller`: print out the state of any attached game controllers (including the default controller, which is the WASD + OPKL keys on the keyboard, or the arrow keys and the ZXCV keys on the keyboard).
  * `12-steerbox`: use the controller to steer a changeable shape around the screen. If no controllers are plugged in, used the arrow keys to steer the shape and ZXCV to change the shape. If you've plugged in a DS4 controller, use the DPAD and the 
shape keys (circle, square, cross, triangle) to change the shape.  Much of the complexity in the code handles the erasure of the prior shape when it needs to be redrawn.
 * `13-thing`: a demo from the how-to-compile video available on Canvas.
 * `14-spaceship`: not working as of this update.
