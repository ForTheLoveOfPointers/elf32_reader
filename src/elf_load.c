#include "elf_load.h"
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

void load_pt_load_segment(FILE *elf_file, ElfProgramHeader *pheader) {

    uintptr_t page_start = PAGE_DOWN(pheader->p_vaddr);
    Elf32Addr page_end = PAGE_UP(pheader->p_vaddr + pheader->p_memsz);
    uint32_t seg_size = page_end -page_start; //Not necessarily a single page, but multiple

    Elf32Addr file_offset = PAGE_DOWN(pheader->p_offset);
    Elf32Addr offset_in_page = pheader->p_vaddr -page_start;

    int prot = 0;
    if (pheader->p_flags & PF_X) prot |= PROT_EXEC;
    if (pheader->p_flags & PF_W) prot |= PROT_WRITE;
    if (pheader->p_flags & PF_R) prot |= PROT_READ;
    

    void *p_map = mmap(
        (void*)page_start, 
        seg_size, 
        0, 
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 
        -1, 
        0
    );
    if(p_map == MAP_FAILED) {
        perror("Incompatible architecture detected. Could not load segment into memory");
        return;
    }

    uintptr_t start_addr = (uintptr_t)pheader->p_vaddr;

    fseek(elf_file, file_offset, SEEK_SET);
    size_t bytes_read = fread((void*)start_addr, 1, pheader->p_filesz, elf_file);

    if(bytes_read < pheader->p_filesz) {
        perror("TRUNCATED FILE DETECTED");
    }

    // Zero the unused portion of the .bss, if there is
    if (pheader->p_memsz > pheader->p_filesz) {

        uintptr_t start_bss = (uintptr_t)(pheader->p_vaddr + pheader->p_filesz);
        memset( (void *)start_bss, 0, pheader->p_memsz - pheader->p_filesz ); 
    }

}