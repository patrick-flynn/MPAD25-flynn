# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 30-hello-world-redux
CC         = mos-neo6502-clang
NEOEMU     = neo

default: all

all:
	@echo "Building project..."
	mkdir -p bin
	$(CC) -Os -o bin/$(NAME).neo src/main.c
	rm bin/$(NAME).neo.elf

asm:
	$(CC) -O0 -Wl,--lto-emit-asm  -o main.s src/main.c

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
	rm -rf storage
	rm memory.dump
