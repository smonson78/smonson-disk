# Smonson Disk
SD card (2x) to ACSI hard drive interface.

This project aims to develop a reliable and useful hard drive solution for Atari ST with two SD card sockets and a
battery-backed clock, satisfying the ACSI protocol, voltage, and timing requirements as given in available specifications.

Features:
- FPGA-controlled ACSI interface to ensure timing conforms to the specification
- One front-mounted removable SD card socket
- One internally-mounted microSD card socket
- SD cards running on a 10MHz SPI bus (1.25MB/s throughput)
- Battery-backed real-time clock
- GUI configuration from ST (once someone writes it...) and stored in EEPROM
- Open-source, so if you want a feature all you have to do is write it and fit it into the 110KB of code space that is currently left!

## How to build and upload:

### Building the microcontroller firmware:
#### Prerequisites
You will need to install:
- A version of GCC that includes support for `avr128da` microcontrollers, cross-compiled for AVR (e.g. 5.4.0).
- A very recent version of avr-libc that has support for `avr128da` microcontrollers - https://www.nongnu.org/avr-libc/. You may need to get this from their github repo.
- A copy of the avr128da "atpack" from Microchip - go to https://packs.download.microchip.com/ and search for `avr128da`; "
Microchip AVR-Dx Series Device Support (2.4.286)" will pop open. Download this file, then change the file extension to `.zip` 
and unzip it into the directory `mcu-source/atpack`.
- (if you are using an Arduino as a programmer) A copy of jtag2updi from the githup repo here: https://github.com/ElTangas/jtag2updi. You will already have this on your system from when you programmed the device.

#### Building
1. Inside `mcu-source`:
1. Update the `Makefile` and set up `LIBC` variable to reflect the path to `avr-libc` on your system
1. Update the `AVRDUDE` variable to reflect your programmer setup (probably just the `avrdude.conf` path)
1. With the UPDI programmer connected, build the source and flash the microcontroller with `make flash`

### Building the FPGA configuration ROM image:

#### Prerequisites
You will need to install Quartus Lite which is a free download from Intel, although you will need to create an account.
You will also need a "USB-Blaster" or compatible programmer.
If using Linux, you will very likely need to install a `udev` "rules" file for the programmer before it'll work.

#### Building
Just open the project in Quartus, go to "start compilation", then after it's compiled, open the programmer tool and program the chip. It should auto-detect the settings. If it doesn't find the USB Blaster hardware then it's probably udev's fault.




