# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 18-framerate
CC         = mos-neo6502-clang
NEOEMU     = neo

SRCS = main.c

default: all

# fix up python for gfx file!


all:
	@echo "Building project..."
	mkdir -p bin
	(cd src; $(CC) -Os -o ../bin/$(NAME).neo $(SRCS); cd ..)
	rm bin/$(NAME).neo.elf

clean:
	@echo "Cleaning project..."
	rm -rf bin
	rm -rf storage
	rm -rf src/*.o
	rm -rf includes/*.o
	if [ -f memory.dump ]; then rm memory.dump; fi

run: all
	@echo "Launching emulator..."
	mkdir -p storage
	cp bin/$(NAME).neo storage
	$(NEOEMU) bin/$(NAME).neo
	# rm -rf storage
	rm memory.dump
