CC=m68k-elf-gcc
LIBTOS=/home/simon/dev/smonson-libtos

IDIOTIC_GCC_BULLSHIT=--param=min-pagesize=0
STD_CFLAGS=-m68000 -fomit-frame-pointer -fno-builtin -I. -I$(LIBTOS) -ffreestanding -ffunction-sections $(IDIOTIC_GCC_BULLSHIT)
CFLAGS=-Os -g -Wall $(STD_CFLAGS)

all: acsi.o crt0.o crtstuff.o

clean:
	$(RM) *.o *~
