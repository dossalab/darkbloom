MEMORY
{
	rom(rx) : ORIGIN = 0x08000000, LENGTH = 16K
	ram(rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

__stack__ = ORIGIN(ram) + LENGTH(ram);

ENTRY(reset_handler)

SECTIONS
{
	. = ORIGIN(rom);

	.text : {
		*(.isr_vectors)
		. = ALIGN(4);
		*(.text*)
		. = ALIGN(4);
		*(.rodata*)
		. = ALIGN(4);
		__text_end__ = .;
	} >rom

	. = ORIGIN(ram);

	__data_load__ = LOADADDR(.data);
	.data : {
		__data_start__ = .;
		*(.data*)
		. = ALIGN(4);
		__data_end__ = .;
	} >ram AT >rom

	.bss : {
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	} >ram AT >rom

	. = ALIGN(4);
	end = .;
}

