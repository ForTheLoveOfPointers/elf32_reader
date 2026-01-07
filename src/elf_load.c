#include "elf_load.h"
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

typedef void (*entry_fn_t)(void);

void load_pt_load_segment(FILE *elf_file, ElfProgramHeader *pheader) {
    size_t PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
    uintptr_t page_start = pheader->p_vaddr & ~(PAGE_SIZE - 1);

    uintptr_t offset = pheader->p_vaddr & (PAGE_SIZE-1);

    Elf32Addr page_end = (pheader->p_vaddr + pheader->p_memsz + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    uint32_t map_size = (pheader->p_memsz + offset + PAGE_SIZE-1) & ~(PAGE_SIZE-1); //Not necessarily a single page, but multiple

    int prot = 0;
    if (pheader->p_flags & PF_R) prot |= PROT_READ;
    if (pheader->p_flags & PF_W) prot |= PROT_WRITE;
    if (pheader->p_flags & PF_X) prot |= PROT_EXEC;
    
    void *p_map = mmap(
        (void*)page_start, 
        map_size, 
        PROT_READ | PROT_WRITE , 
        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, 
        -1, 
        0
    );
    printf("Mem reserved: %p\n", p_map);
    if(p_map == MAP_FAILED) {
        perror("Incompatible architecture detected. Could not load segment into memory");
        return;
    }

    uintptr_t start_addr = (uintptr_t)pheader->p_vaddr;

    fseek(elf_file, pheader->p_offset, SEEK_SET);

    size_t bytes_read = fread((void*)start_addr, 1, pheader->p_filesz, elf_file);
    if(bytes_read < pheader->p_filesz) {
        perror("TRUNCATED FILE DETECTED");
    }

    // Zero the unused portion of the .bss, if there is
    if (pheader->p_memsz > pheader->p_filesz) {

        uintptr_t start_bss = (uintptr_t)(pheader->p_vaddr + pheader->p_filesz);
        memset( (void *)start_bss, 0, pheader->p_memsz - pheader->p_filesz ); 
    }


    if (mprotect((void*)page_start, map_size, prot) != 0) {
        perror("mprotect failed");
        exit(1);
    }
}

Elf32Addr load_stack_mem(void) {
    uintptr_t stack = (uintptr_t) mmap(
        NULL,
        STACK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    
    if(stack == (uintptr_t)MAP_FAILED) {
        perror("STACK MAP FAILED");
        exit(-1);
    }

    Elf32Addr stack_top = (Elf32Addr)(stack + STACK_SIZE);

    return stack_top;
}

Elf32Addr load_initial_stack(Elf32Addr stack_top) {
    uintptr_t sp = (uintptr_t)stack_top;

    // Push argv/envp properly
    sp -= strlen("prog")+1;
    char *arg0_ptr = (char*)sp;
    strcpy(arg0_ptr, "prog");

    // Align stack
    sp &= ~0xF;

    // envp
    sp -= 4; *(uint32_t*)sp = 0;

    // argv
    sp -= 4; *(uint32_t*)sp = 0;        // argv[1] = NULL
    sp -= 4; *(uint32_t*)sp = (uint32_t)arg0_ptr; // argv[0]

    // argc
    sp -= 4; *(uint32_t*)sp = 1;
    return (Elf32Addr)sp;

}

__attribute__((noreturn))
_Noreturn void jmp_entry(uintptr_t entry_addr, Elf32Addr sp) {

    entry_fn_t entry_point = (entry_fn_t) entry_addr; 

    printf("entry_point numeric: 0x%x\n", (uintptr_t)entry_point);
    printf("entry_point numeric: 0x%x\n", sp);
    __asm__ volatile (
        "movl %0, %%esp\n"
        "xor %%ebp, %%ebp\n"
        "jmp *%1\n"
        :
        : "r"(sp & ~0xF), "r"(entry_point)
        : "memory"
    );
    while(1) {

    }
}