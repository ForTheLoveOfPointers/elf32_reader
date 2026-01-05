# ELF Reader & Executor

A minimal ELF file reader and executor written in C for educational purposes.

## Features

- Parse 32-bit ELF file headers
- Display ELF structure information
- Disassemble and read x86 assembly code
- Manually load and execute ELF binaries

## Usage
```bash
gcc elf_reader.c -o elf_reader
./elf_reader 
```

## Requirements

- GCC with 32-bit support (`gcc-multilib`)
- Linux operating system

## Creating Test ELF Files
```bash
gcc -m32 -no-pie test.c -o test_elf
```

## Project Goals

This project manually implements ELF loading and execution to understand:
- ELF file format structure
- Program headers and sections
- x86 assembly instruction decoding
- Memory mapping and permissions
- Manual program execution

## Status

🚧 Work in progress

## License

MIT