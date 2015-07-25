ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text : {
        *(multiboot)
        *(.text)
    }
    .data ALIGN(4096) : {
        start_ctors = .;
        *(.ctors)
        *(.init_array)
        end_ctors = .;
        
        start_dtors = .;
        *(.dtors)
        *(.fini_array)
        end_dtors = .;
        
        *(.data)
    }
    .rodata ALIGN(4096) : {
        *(.rodata)
    }
    .bss ALIGN(4096) : {
        *(.bss)
    }
    
    /DISCARD/ : { *(.comment) }
}

