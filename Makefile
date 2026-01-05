BUILD=./build

all: main.c
	gcc main.c elf_def.c -o $(BUILD)/elf_loader