AVRDUDE_MCPU=avr128da48
LIBC_MCPU=avr128da48
FCLK=20000000

# Don't really need fuses for this project
OSCCFG_FUSE=0x00
SYSCFG0_FUSE=0xC8

# You have to download this from Atmel's website, and then unzip it
ATPACK=atpack

PROGTYPE=jtag2updi
PROGPORT=/dev/ttyUSB0

LIBC=/home/simon/local/avr

OBJS=fpga_comm.o sdcard.o acsi.o modesense.o commands.o serial.o debug.o timer0.o spi.o rtc.o

AVRDUDE=avrdude -C ~/dev/jtag2updi/avrdude.conf -p $(AVRDUDE_MCPU) -c $(PROGTYPE) -P $(PROGPORT) -B 19200

CC=avr-gcc
CFLAGS=-g -Wall -O2 -mmcu=$(LIBC_MCPU) -B$(ATPACK)/gcc/dev/$(LIBC_MCPU) -DF_CPU=$(FCLK) -I$(LIBC)/include
#LDLIBS=-lgcc
LDFLAGS=--sysroot $(LIBC)

TARGET=hdd

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

$(TARGET).elf: main.o $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

flash: $(TARGET).hex
	$(AVRDUDE) -U flash:w:$^:i

fuses:
	$(AVRDUDE) -U osccfg:w:$(OSCCFG_FUSE):m
	$(AVRDUDE) -U syscfg0:w:$(SYSCFG0_FUSE):m

clean:
	$(RM) *.o *.elf *.hex
