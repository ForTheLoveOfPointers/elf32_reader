#pragma once
#include "elf_def.h"
#include <stdio.h>

/**
 * This definitions serve the purpose of page alignment.
 * The load_pt_load_segment function should use mmap() to manage permissions properly,
 * but mmap() requires data to be page aligned. Because ELF segments may very well start
 * in the middle of a page (aka 'not being aligned'), functions are necessary to set a valid addr before loading into RAM
 */
#define STACK_SIZE 1048576 // 1 MiB

enum ELF_PPROTOFLAGS {
    PF_X = 1,
    PF_W = 2,
    PF_R = 4
};

void load_pt_load_segment(FILE *elf_file, ElfProgramHeader *pheader);
Elf32Addr load_stack_mem(void);
Elf32Addr load_initial_stack(Elf32Addr stack_top);

_Noreturn void jmp_entry(uintptr_t entry_addr, Elf32Addr sp);