# ----------------------------
# Makefile Options
# ----------------------------

NAME       = 22-tunes
CC         = mos-neo6502-clang
NEOEMU     = neo
AUXFILES   = g-minor-bach_converted.csv
SRCFILES   = src/main.c src/math.c src/api-internal.c


default: all

all:
	@echo "Building project..."
	mkdir -p bin
	$(CC) -Os -o bin/$(NAME).neo $(SRCFILES)
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
