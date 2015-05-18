MEMORY
{
    rom : ORIGIN = 0x00010000, LENGTH = 0x400
    ram : ORIGIN = 0x00020000, LENGTH = 0x400
}

SECTIONS
{
    .text : { *(.text) } >rom
    .data : { *(.data) } >ram AT>rom
}

