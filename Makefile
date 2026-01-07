BUILD=./build
INCLUDE=./include
SRC = ./src
MAIN = main.c

all: main.c source
	gcc -m32 -Wall -Wextra $(MAIN) $(SRC)/* -I$(INCLUDE) -o $(BUILD)/elf_loader -g

clean: $(BUILD)/elf_loader
	rm $(BUILD)/elf_loader

source: $(SRC)/elf_def.c $(SRC)/elf_load.c