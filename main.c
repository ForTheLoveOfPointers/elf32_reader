#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define EIDENT 16

#define Elf32Addr uint32_t
#define Elf32Half uint16_t
#define Elf32Off uint32_t
#define Elf32Sword int32_t
#define Elf32Word uint32_t
#define Elf32UChar uint8_t

typedef struct 
{
    Elf32UChar e_ident[EIDENT];
    Elf32Half e_type;
    Elf32Half e_machine;
    Elf32Word e_version;
    Elf32Addr e_entry;
    Elf32Off e_phoff;
    Elf32Off e_shoff;
    Elf32Word e_flags;
    Elf32Half e_ehsize;
    Elf32Half e_phentsize;
    Elf32Half e_phnum;
    Elf32Half e_shentsize;
    Elf32Half e_shnum;
    Elf32Half e_shstrndx;

} __attribute__((packed)) ElfHeader;



void print_elf_header(ElfHeader *header) {
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for(int i = 0; i < EIDENT; i++) {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n");
    printf("  Class:                             %s\n", header->e_ident[4] == 1 ? "ELF32" : "ELF64");
    printf("  Data:                              %s\n",  header->e_ident[5] == 1 ? "Little endian" : "Big endian");
    printf("  Type:                              %u\n", header->e_type);
    printf("  Machine:                           %u\n", header->e_machine);
    printf("  Version:                           %u\n", header->e_version);
    printf("  Entry point address:               0x%x\n", header->e_entry);
    printf("  Program header offset:             %u (bytes)\n", header->e_phoff);
    printf("  Section header offset:             %u (bytes)\n", header->e_shoff);
    printf("  Flags:                             0x%x\n", header->e_flags);
    printf("  ELF header size:                   %u (bytes)\n", header->e_ehsize);
    printf("  Program header entry size:         %u (bytes)\n", header->e_phentsize);
    printf("  Program header entry count:        %u\n", header->e_phnum);
    printf("  Section header entry size:         %u (bytes)\n", header->e_shentsize);
    printf("  Section header entry count:        %u\n", header->e_shnum);
    printf("  Section header string table index: %u\n", header->e_shstrndx);
}


/**
 * 
 * MAIN SECTION
 * 
 */

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Mode of use is: %s <elf_file_name>\n", argv[0]);
        return -1;
    }

    FILE *elf_file = fopen(argv[1], "r");
    if(!elf_file) {
        printf("Could not open file... Make sure the path is correct.\n");
        return -1;
    }
    
    ElfHeader elf_header = {0};

    fread(&elf_header, sizeof(elf_header), 1, elf_file);

    print_elf_header(&elf_header);


    if(elf_header.e_type != 2) {
        printf("Specified elf file is of type '%d', but it must be of type '2'\n", elf_header.e_type);
        return -1;
    }

    fclose(elf_file);
    return 0;
}