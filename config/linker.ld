OUTPUT_FORMAT("elf32-i386")
ENTRY(start)
VOFF = 0xC0000000; /* virtual - physical address offset */
SECTIONS
{
	. = 0x100000; /* start at 1MB */

	_kspace_saddr = .;
	.multiboot BLOCK(4K):
	{
		*(.multiboot) 	/* add the multiboot header */
	}

	.ppinit BLOCK(4K):
	{
		*(.ppinit)
	}

	. += VOFF; /* move counter to virtual address */

	.text BLOCK(4K) : AT(ADDR(.text) - VOFF)
	{
		*(.text)	/* add the code */
	}

	.rodata BLOCK(4K) : AT(ADDR(.rodata) - VOFF)
	{
		*(.rodata)		/* read only data */
					/* eg. strings */
	}

	.data BLOCK(4K) : AT(ADDR(.data) - VOFF) /* initialized data */
	{
		*(.data)
	}

	.bss BLOCK(4K) : AT(ADDR(.bss) - VOFF) /* uninitialized data */
	{
		*(COMMON)
		*(.bss)		/* labels, globs */
		*(.comment)
		*(.boot_stack)
	}

	_kspace_eaddr = (. - VOFF);
}
