# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 36-pixlife-more
CC         = mos-neo6502-clang -T custom-link.ld -Wl,-Map,$(NAME).map
NEOEMU     = neo
SRCS       = src/main.c src/math.c src/mouse.c src/blitter.c

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
