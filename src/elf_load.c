#include "elf_load.h"

void load_pt_load_segment(FILE *elf_file, ElfProgramHeader *pheader) {

    Elf32Addr page_start = PAGE_DOWN(pheader->p_vaddr);
    Elf32Addr page_end = PAGE_UP(pheader->p_vaddr + pheader->p_memsz);
    uint32_t seg_size = page_end -page_start; //Not necessarily a single page, but multiple

    

}