# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 01-hello-world
CC         = /opt/llvm-mos-sdk/bin/mos-neo6502-clang
NEOEMU     = /Users/flynn/Desktop/MPAD25/neo6502-firmware/bin/neo

default: all

all:
	@echo "Building project..."
	mkdir -p bin
	$(CC) -Os -o bin/$(NAME).neo src/main.c
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
	$(NEOEMU) bin/$(NAME).neo cold
	rm -rf storage
	rm memory.dump
