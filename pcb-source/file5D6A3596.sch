EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR0108
U 1 1 5D6B4401
P 7450 5100
F 0 "#PWR0108" H 7450 4850 50  0001 C CNN
F 1 "GND" H 7455 4927 50  0000 C CNN
F 2 "" H 7450 5100 50  0001 C CNN
F 3 "" H 7450 5100 50  0001 C CNN
	1    7450 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0109
U 1 1 5D6B440E
P 7350 1900
F 0 "#PWR0109" H 7350 1750 50  0001 C CNN
F 1 "+3.3V" H 7365 2073 50  0000 C CNN
F 2 "" H 7350 1900 50  0001 C CNN
F 3 "" H 7350 1900 50  0001 C CNN
	1    7350 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2100 7350 1950
Wire Wire Line
	7450 2100 7450 1950
Wire Wire Line
	7450 1950 7350 1950
Connection ~ 7350 1950
Wire Wire Line
	7350 1950 7350 1900
Text Label 5900 2600 0    50   ~ 0
SS
Text Label 5900 2700 0    50   ~ 0
MOSI
Text Label 5900 2900 0    50   ~ 0
SCK
Wire Wire Line
	5900 2600 6850 2600
Wire Wire Line
	5900 2700 6850 2700
Wire Wire Line
	5900 2900 6850 2900
Entry Wire Line
	6400 4200 6500 4100
Entry Wire Line
	6400 4300 6500 4200
Entry Wire Line
	6400 4400 6500 4300
Entry Wire Line
	6400 4500 6500 4400
Entry Wire Line
	6400 4600 6500 4500
Entry Wire Line
	6400 4700 6500 4600
Entry Wire Line
	6400 4800 6500 4700
Entry Wire Line
	6400 4900 6500 4800
Wire Wire Line
	6500 4100 6850 4100
Wire Wire Line
	6500 4200 6850 4200
Wire Wire Line
	6500 4300 6850 4300
Wire Wire Line
	6500 4400 6850 4400
Wire Wire Line
	6500 4500 6850 4500
Wire Wire Line
	6500 4600 6850 4600
Wire Wire Line
	6500 4700 6850 4700
Wire Wire Line
	6500 4800 6850 4800
Text Label 6550 4100 0    50   ~ 0
A_D0
Text Label 6550 4200 0    50   ~ 0
A_D1
Text Label 6550 4300 0    50   ~ 0
A_D2
Text Label 6550 4400 0    50   ~ 0
A_D3
Text Label 6550 4500 0    50   ~ 0
A_D4
Text Label 6550 4600 0    50   ~ 0
A_D5
Text Label 6550 4700 0    50   ~ 0
A_D6
Text Label 6550 4800 0    50   ~ 0
A_D7
Wire Bus Line
	6400 5050 5700 5050
Text Label 5800 5050 0    50   ~ 0
A_D[0..7]
Text HLabel 5700 5050 0    50   Input ~ 0
A_D[0..7]
Text HLabel 5900 3000 0    50   Input ~ 0
CLK8
Text HLabel 5700 3400 0    50   Input ~ 0
A_BUS_DIR
Wire Wire Line
	6850 3300 5700 3300
Wire Wire Line
	6850 3500 5700 3500
Text HLabel 5700 3300 0    50   Input ~ 0
A_INT
Text HLabel 5700 3500 0    50   Input ~ 0
A_CS
Text HLabel 5700 3600 0    50   Input ~ 0
A_CMD
Wire Wire Line
	6850 3600 5700 3600
Wire Wire Line
	6850 3400 5700 3400
Wire Wire Line
	6850 3700 5700 3700
Wire Wire Line
	6850 3800 5700 3800
Text HLabel 5700 3700 0    50   Input ~ 0
A_CD
Text HLabel 5700 3800 0    50   Input ~ 0
A_CONT
$Comp
L MCU_Microchip_ATmega:ATmega328-AU U2
U 1 1 5F267B38
P 7450 3600
F 0 "U2" H 6850 2250 50  0000 C CNN
F 1 "ATmega328-AU" H 7000 2100 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 7450 3600 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 7450 3600 50  0001 C CNN
	1    7450 3600
	-1   0    0    -1  
$EndComp
NoConn ~ 8050 2600
NoConn ~ 8050 2700
NoConn ~ 8050 2400
Text HLabel 5900 2700 0    50   Input ~ 0
MOSI
Text HLabel 5900 2900 0    50   Input ~ 0
SCK
Text HLabel 5900 2600 0    50   Input ~ 0
SS
Wire Wire Line
	6850 2500 5900 2500
Text HLabel 5900 2500 0    50   Input ~ 0
A_X
Text HLabel 5700 3900 0    50   Input ~ 0
A_RESET
Wire Wire Line
	5700 3900 6850 3900
Text Label 5900 3900 0    50   ~ 0
RESET
Wire Wire Line
	6850 3000 5900 3000
Wire Wire Line
	6850 3100 5900 3100
Text HLabel 5900 2400 0    50   Input ~ 0
A_Y
Wire Wire Line
	5900 2400 6850 2400
Text HLabel 5900 3100 0    50   Input ~ 0
A_Z
Text Label 4150 2750 0    50   ~ 0
MOSI
Text Label 3150 2650 0    50   ~ 0
MISO
Text Label 3150 2750 0    50   ~ 0
SCK
Text Label 3150 2850 0    50   ~ 0
RESET
Wire Wire Line
	3600 2850 3100 2850
Wire Wire Line
	3600 2750 3100 2750
Wire Wire Line
	3600 2650 3100 2650
Wire Wire Line
	4100 2750 4500 2750
Wire Wire Line
	4100 2850 4500 2850
Wire Wire Line
	4100 2650 4500 2650
$Comp
L power:+3.3V #PWR0114
U 1 1 5D6F4263
P 4500 2650
F 0 "#PWR0114" H 4500 2500 50  0001 C CNN
F 1 "+3.3V" H 4515 2823 50  0000 C CNN
F 2 "" H 4500 2650 50  0001 C CNN
F 3 "" H 4500 2650 50  0001 C CNN
	1    4500 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5D6F425D
P 4500 2850
F 0 "#PWR0113" H 4500 2600 50  0001 C CNN
F 1 "GND" H 4505 2677 50  0000 C CNN
F 2 "" H 4500 2850 50  0001 C CNN
F 3 "" H 4500 2850 50  0001 C CNN
	1    4500 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J4
U 1 1 5D6F4256
P 3800 2750
F 0 "J4" H 3850 3067 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 3850 2976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 3800 2750 50  0001 C CNN
F 3 "~" H 3800 2750 50  0001 C CNN
	1    3800 2750
	1    0    0    -1  
$EndComp
Text HLabel 5900 2800 0    50   Input ~ 0
MISO
Text Label 5900 2800 0    50   ~ 0
MISO
Wire Wire Line
	5900 2800 6850 2800
Wire Bus Line
	6400 4200 6400 5050
$EndSCHEMATC
