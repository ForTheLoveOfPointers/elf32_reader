#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "include/elf_def.h"


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

    size_t n_read = fread(&elf_header, sizeof(elf_header), 1, elf_file);

    if(n_read <= 0) {
        perror("Unable to read file");
        return -1;
    }

    print_elf_header(&elf_header);
    uint8_t is_compat = check_elf_compat(&elf_header);

    if(is_compat != ELF_OK && is_compat != ELF_ARCH_ERR) {
        return is_compat;
    }

    fclose(elf_file);
    return 0;
}