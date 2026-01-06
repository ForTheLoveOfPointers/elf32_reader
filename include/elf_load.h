#pragma once
#include "elf_def.h"
#include <stdio.h>

/**
 * This definitions serve the purpose of page alignment.
 * The load_pt_load_segment function should use mmap() to manage permissions properly,
 * but mmap() requires data to be page aligned. Because ELF segments may very well start
 * in the middle of a page (aka 'not being aligned'), functions are necessary to set a valid addr before loading into RAM
 */
#define PAGE_SIZE 4096
#define PAGE_DOWN(x) ((x) & ~(PAGE_SIZE - 1))
#define PAGE_UP(x)   (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))


void load_pt_load_segment(FILE *elf_file, ElfProgramHeader *pheader);