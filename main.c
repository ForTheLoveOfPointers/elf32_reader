#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "elf_def.h"
#include "elf_load.h"


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

    /*if(is_compat != ELF_OK) {
        return is_compat;
    }
        */


    ElfProgramHeader *elf_ph_arr = NULL;

    if(!load_elf_pheader_array(&elf_ph_arr, elf_header.e_phnum, elf_file)) {
        free(elf_ph_arr);
        fclose(elf_file);
        return ELF_UNK_ERR;
    }
    
    for(uint8_t i = 0; i < elf_header.e_phnum; i++) {
        if(elf_ph_arr[i].p_type == 1) {
            load_pt_load_segment(elf_file, &elf_ph_arr[i]);
            
            print_elf_pheader(&elf_ph_arr[i]);
        }
    }



    Elf32Addr sp = load_stack_mem(); // Stack pointer
    uintptr_t entry_addr = (uintptr_t)elf_header.e_entry;

    load_initial_stack(sp);
    jmp_entry(entry_addr, sp);

    free(elf_ph_arr);
    fclose(elf_file);
    return ELF_OK;
}