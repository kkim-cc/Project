/* ATmega328p linker script */

MEMORY
{
    flash : ORIGIN = 0x0,    LENGTH = 32K
    ram   : ORIGIN = 0x0100, LENGTH = 2K
}

SECTIONS
{
    .text :
    {
        *( .vectors )
        *( .text )
    } >flash

    .data :
    {
        *(.data)
    } >ram AT>flash
    /* "ram" is VMA (where it will actually live), "flash" is LMA (where it will be loaded) */

    /* symbols needed to relocate .data on init */
    _data_start_lma = LOADADDR(.data);
    _data_start_vma = ADDR(.data);
    _data_size = SIZEOF(.data);

    /* the version of binutils on Ubuntu 12.04LTS (lab computers) is a bit
     * old and has this bug that I need to work around like this...
     */
    /DISCARD/ : { *(.trampolines) }
}

