#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EIDENT 16

#define Elf32Addr uint32_t
#define Elf32Half uint16_t
#define Elf32Off uint32_t
#define Elf32Sword int32_t
#define Elf32Word uint32_t
#define Elf32UChar uint8_t

#define ElfMachineNum 8

enum ELF_VALID {
    ELF_OK,
    ELF_SYS_FETCH_ERR,
    ELF_T_ERR,
    ELF_ARCH_ERR,
    ELF_UNK_ERR
};

typedef struct 
{
    Elf32UChar  e_ident[EIDENT];
    Elf32Half   e_type;
    Elf32Half   e_machine;
    Elf32Word   e_version;
    Elf32Addr   e_entry;
    Elf32Off    e_phoff;
    Elf32Off    e_shoff;
    Elf32Word   e_flags;
    Elf32Half   e_ehsize;
    Elf32Half   e_phentsize;
    Elf32Half   e_phnum;
    Elf32Half   e_shentsize;
    Elf32Half   e_shnum;
    Elf32Half   e_shstrndx;

} __attribute__((packed)) ElfHeader;


typedef struct 
{
    Elf32Word   p_type;
    Elf32Off    p_offset;
    Elf32Addr   p_vaddr;
    Elf32Addr   p_paddr;
    Elf32Word   p_filesz;
    Elf32Word   p_memsz;
    Elf32Word   p_flags;
    Elf32Word   p_align;

} __attribute__((packed)) ElfProgramHeader;


void print_elf_header(ElfHeader *header);
void print_elf_pheader(ElfProgramHeader *pheader);
bool load_elf_pheader_array(ElfProgramHeader **elf_ph_arr, Elf32Half phnum, FILE *elf_file);
uint8_t check_elf_compat(ElfHeader *header);