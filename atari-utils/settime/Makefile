CC=m68k-elf-gcc
CXX=m68k-elf-g++
OBJCOPY=m68k-elf-objcopy
OBJDUMP=m68k-elf-objdump
# https://github.com/8l/vlink
VLINK=/home/simon/dev/vlink/vlink
# https://github.com/smonson78/smonson-libtos
LIBTOS=/home/simon/dev/smonson-libtos
# This is where I keep my Hatari virtual hard drive
INSTALL=/home/simon/atari-hd

IDIOTIC_GCC_BULLSHIT=--param=min-pagesize=0
STD_CFLAGS=-m68000 -fomit-frame-pointer -fno-builtin -I. -I$(LIBTOS) -ffreestanding -ffunction-sections $(IDIOTIC_GCC_BULLSHIT)
CFLAGS=-Os -g -Wall $(STD_CFLAGS)
CXXFLAGS=-Os -g -Wall -fno-exceptions -fno-rtti $(STD_CFLAGS)

ALLLIBS=crt0.o crtstuff.o
LDLIBS=-lgcc $(LIBTOS)/libtos.a $(LIBGCC)
LDFLAGS=-m68000 -nostdlib -Wl,--relocatable

TARGET=settime
LIBGCC:=$(shell $(CXX) $(CFLAGS) --print-libgcc-file-name)

all: $(TARGET).PRG

$(TARGET).PRG: $(TARGET).elf
	@# Strip out .discard section and make .text writable before passing to vlink
	@#$(OBJCOPY) --remove-section=.discard --writable-text $^ ready.o
	$(VLINK) $^ -b ataritos -o $@
	@#$(RM) ready.o

clean:
	$(RM) $(TARGET) *.o *.elf test.bin
	$(RM) *~

dasm: $(TARGET)
	@dd bs=1 skip=28 if=$(TARGET) of=test.bin
	@m68k-elf-objdump -m m68k -D -b binary test.bin
	@$(RM) test.bin

info: debug
	@$(OBJDUMP) -h info.elf

$(TARGET).elf: $(TARGET).o $(ALLLIBS)
	@# Run linker, generate a relocatable object file of the whole project
	$(CXX) -Tatari.ld $(LDFLAGS) $^ $(LDLIBS) -o $(TARGET)1.elf
	$(OBJCOPY) --set-section-flags .data=ALLOC,READONLY,DATA \
		--set-section-flags .text*=ALLOC,CODE \
		$(TARGET)1.elf $@

debug: $(TARGET).o $(ALLLIBS)
	$(CXX) -Tdebug.ld $(LDFLAGS) $^ $(LDLIBS) -o info.elf

install: $(TARGET).PRG
	cp --target-directory=$(INSTALL) $^
