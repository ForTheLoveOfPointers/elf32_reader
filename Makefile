BUILD=./build

all: main.c
	gcc main.c -o $(BUILD)/elf_loader