BUILD=./build
INCLUDE=./include
SRC = ./src
MAIN = main.c

all: main.c source
	gcc $(MAIN) $(SRC)/* -I$(INCLUDE) -o $(BUILD)/elf_loader

source: src/elf_def.c src/elf_load.c