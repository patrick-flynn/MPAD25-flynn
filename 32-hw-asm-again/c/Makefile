# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 32-hw-asm-again
CC         = mos-neo6502-clang
NEOEMU     = neo

default: all

all:
	@echo "Building project..."
	mkdir -p bin
	$(CC) -Os -o bin/$(NAME).neo src/main.s
	rm bin/$(NAME).neo.elf

obj:
	$(CC) -Os -c src/main.s

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
