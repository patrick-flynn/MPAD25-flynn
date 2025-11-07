# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 24-music
CC         = mos-neo6502-clang
NEOEMU     = neo
SRCS = src/main.c src/sound.c
AUXFILES = xml/SailOnBoys.csv

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
	cp $(AUXFILES) storage
	$(NEOEMU) bin/$(NAME).neo
	rm -rf storage
	rm memory.dump
