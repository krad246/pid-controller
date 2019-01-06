/*
 *  ======== CC3220SF_LAUNCHXL.cmd ========
 */

/*
 * The starting address of the application.  Normally the interrupt vectors
 * must be located at the beginning of the application.
 */
#define SRAM_BASE   0x20000000
#define FLASH_BASE  0x01000800

--stack_size=1024   /* C stack is also used for ISR stack */

HEAPSIZE = 0x8000;  /* Size of heap buffer used by HeapMem */

MEMORY
{
    /* Bootloader uses FLASH_HDR during initialization */
    FLASH_HDR (RX)  : origin = 0x01000000, length = 0x7FF      /* 2 KB */
    FLASH     (RX)  : origin = 0x01000800, length = 0x0FF800   /* 1022KB */
    SRAM      (RWX) : origin = 0x20000000, length = 0x00040000 /* 256KB */
}

/* Section allocation in memory */

SECTIONS
{
    .dbghdr     : > FLASH_HDR
    .text       : > FLASH
    .TI.ramfunc : {} load=FLASH, run=SRAM, table(BINIT)
    .const      : > FLASH
    .cinit      : > FLASH
    .pinit      : > FLASH
    .init_array : > FLASH

    .data       : > SRAM
    .bss        : > SRAM
    .sysmem     : > SRAM

    /* Heap buffer used by HeapMem */
    .priheap   : {
        __primary_heap_start__ = .;
        . += HEAPSIZE;
        __primary_heap_end__ = .;
    } > SRAM align 8

    .stack      : > SRAM(HIGH)
}
