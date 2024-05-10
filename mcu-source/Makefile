# CPU core selection (for GCC)
MCPU=cortex-m0plus

# CPU architecture selection (for objdump) - might not even matter.
ARCH=armv6-m

FCLK=24000000
FLASH_ADDR=0x8000000

# Device configuration
# TODO: Get the Cortex-M0+ one
FAMILY=STM32F1xx
DEVICE=STM32F103xB
CUBE=STM32Cube_FW_F1_V1.8.0

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

STFLASH=st-flash

INCLUDE_PATHS=-I$(CUBE)/Drivers/CMSIS/Device/ST/$(FAMILY)/Include -I$(CUBE)/Drivers/CMSIS/Include
CFLAGS=-g -Wall -O3 -mcpu=$(MCPU) -mthumb --specs=picolibc.specs -DF_CPU=$(FCLK) -D$(DEVICE) $(INCLUDE_PATHS)
LDFLAGS=-mcpu=$(MCPU) --specs=picolibc.specs -Tlinker.ld
LDLIBS=

TARGET=test
OBJS=debug.o fpga_comm.o serial.o sdcard.o spi.o rtc.o timer0.o

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -j .init -j .text -j .data -O binary $^ $@

$(TARGET).elf: main.o $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

flash: $(TARGET).bin
	$(STFLASH) write $^ $(FLASH_ADDR)

clean:
	$(RM) *.o *.elf $(TARGET).bin
