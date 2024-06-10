# CPU core selection (for GCC)
MCPU=cortex-m0plus

# CPU architecture selection (for objdump) - might not even matter.
ARCH=armv6-m

FCLK=48000000
HCLK=48000000
PCLK=48000000
FLASH_ADDR=0x8000000

# Device configuration for STM32C031, ARM Cortex M0+
FAMILY=STM32C0xx
DEVICE=STM32C031xx
CUBE=STM32CubeC0

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

STFLASH=st-flash --freq=1000k

INCLUDE_PATHS=-I$(CUBE)/Drivers/CMSIS/Device/ST/$(FAMILY)/Include -I$(CUBE)/Drivers/CMSIS/Include
CFLAGS=-g -Wall -mcpu=$(MCPU) -mthumb --specs=picolibc.specs -DF_CLK=$(FCLK) \
	-DF_PCLK=$(PCLK) -DF_HCLK=$(HCLK) -D$(DEVICE) $(INCLUDE_PATHS)
LDFLAGS=-mcpu=$(MCPU) --specs=picolibc.specs -Tlinker.ld
LDLIBS=

TARGET=hdd
OBJS=debug.o fpga_comm.o acsi.o commands.o modesense.o serial.o sdcard.o spi.o rtc.o timer0.o int.o

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -j .init -j .text -j .data -j .rodata -O binary $^ $@

$(TARGET).elf: main.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

flash: $(TARGET).bin
	$(STFLASH) write $^ $(FLASH_ADDR)

clean:
	$(RM) *.o *.elf $(TARGET).bin

dasm: $(TARGET).elf
	$(OBJDUMP) -S $(TARGET).elf
