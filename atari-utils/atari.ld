SECTIONS {
	. = 0;
	.text : {
		crt0.o (.entry)
		/* Normal .text */
		*(.text)
		/* C++ static methods seem to go into their own segment */
		*(.text.*)
		/* _etext = .; */
	}
	.data : {
		*(.data*)
		*(.rodata*)
		*(.init_array)
		. = ALIGN(4);
		__CTOR_LIST__ = .;
		crtstuff.o(.ctors)
		*(.ctors)
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		*(.dtors)
		crtstuff.o(.dtors)
		__DTOR_END__ = .;
		/* _edata = .; */
	}
	.bss : {
		*(.bss)
		. = ALIGN(2);
		/* _end = .; */
	}
	/* Allow a 32KB stack */
	. = . + 0x8000;
	stack = ALIGN(2);

	/DISCARD/ : {
		*(.group)
		*(.comment)
		*(.debug*)
		*(.note.*)
	}
}
