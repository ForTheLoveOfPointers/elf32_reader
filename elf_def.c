#include "include/elf_def.h"
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>


const char *elf_machines[ElfMachineNum] = {
    "No Machine",
    "AT&T WE 32100",
    "SPARC",
    "i386",
    "m68",
    "m88",
    "x86_64",
    "MIPS RS3000"
};

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
    printf("\r\n");
}



void print_elf_pheader(ElfProgramHeader *pheader) {
    printf("  Type:                              %u\n", pheader->p_type);
    printf("  Offset:                            %u\n", pheader->p_offset);
    printf("  Virtual Addr:                      %u\n", pheader->p_vaddr);
    printf("  Physicial Addr:                    %u\n", pheader->p_paddr);
    printf("  File size:                         %u\n", pheader->p_filesz);
    printf("  Memory size:                       %u\n", pheader->p_memsz);
    printf("  Flags:                             %u\n", pheader->p_flags);
    printf("  Alignment:                         %u\n", pheader->p_align);
    printf("\r\n");
}


uint8_t check_elf_compat(ElfHeader *header) {
    
    if(header->e_type != 2) {
        printf("Specified elf file is of type '%d', but it must be of type '2'\n", header->e_type);
        return ELF_T_ERR;
    }
    
    struct utsname name;

    const char *machine = elf_machines[header->e_machine];  

    if(uname(&name) == 0) {
        if( memcmp(machine, name.machine, strlen(machine)) != 0) {
            printf("TERMINATE || Incompatible ELF-machine architecture. Expected '%s', but machine is '%s'.\n", machine, name.machine);
            return ELF_ARCH_ERR;
        }  
    } else {
        perror("TERMINATE || Failed fetching system info. Unable to load elf file securely.\n");
        return ELF_SYS_FETCH_ERR;
    }

    return ELF_OK;
}


bool load_elf_pheader_array(ElfProgramHeader **elf_ph_arr, Elf32Half phnum, FILE *elf_file) {
    *elf_ph_arr = (ElfProgramHeader*)malloc(phnum * sizeof(ElfProgramHeader));
    size_t num_read = fread(*elf_ph_arr, sizeof(ElfProgramHeader), phnum, elf_file);
    size_t expected = sizeof(ElfProgramHeader) * (size_t) phnum;
    if(num_read < phnum) {
        printf("ERROR || Could not read the appropiate amount of entries. Expected '%lu' entries, but read '%u'.", expected, phnum);
        return false;
    }
    return true;
}