# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 34-pixel-life-1dbufs
CC         = mos-neo6502-clang -T custom-link.ld
NEOEMU     = neo
SRCS       = src/main.c src/math.c src/mouse.c

default: all

all:
	@echo "Building project..."
	mkdir -p bin
	$(CC) -Os -o bin/$(NAME).neo $(SRCS)
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
	rm -rf storage
	rm memory.dump
