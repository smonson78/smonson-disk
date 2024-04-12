EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
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
L Connector:Conn_01x20_Male J2
U 1 1 5C53EBBC
P 650 2800
F 0 "J2" H 756 3878 50  0000 C CNN
F 1 "ST DMA" H 756 3787 50  0000 C CNN
F 2 "simon:IDC-Header_DB19_P2.54mm_Horizontal" H 650 2800 50  0001 C CNN
F 3 "~" H 650 2800 50  0001 C CNN
	1    650  2800
	1    0    0    -1  
$EndComp
Entry Wire Line
	2100 1900 2200 1800
Entry Wire Line
	2100 2000 2200 1900
Entry Wire Line
	2100 2100 2200 2000
Entry Wire Line
	2100 2200 2200 2100
Entry Wire Line
	2100 2300 2200 2200
Entry Wire Line
	2100 2400 2200 2300
Entry Wire Line
	2100 2500 2200 2400
Entry Wire Line
	2100 2600 2200 2500
Entry Wire Line
	2850 1700 2950 1800
Entry Wire Line
	2850 1800 2950 1900
Entry Wire Line
	2850 1900 2950 2000
Entry Wire Line
	2850 2000 2950 2100
Entry Wire Line
	2850 2100 2950 2200
Entry Wire Line
	2850 2200 2950 2300
Entry Wire Line
	2850 2300 2950 2400
Entry Wire Line
	2850 2400 2950 2500
Text Label 1950 1900 0    50   ~ 0
D0
Text Label 1950 2000 0    50   ~ 0
D1
Text Label 1950 2100 0    50   ~ 0
D2
Text Label 1950 2200 0    50   ~ 0
D3
Text Label 1950 2300 0    50   ~ 0
D4
Text Label 1950 2400 0    50   ~ 0
D5
Text Label 1950 2500 0    50   ~ 0
D6
Text Label 1950 2600 0    50   ~ 0
D7
Text Label 3000 1900 0    50   ~ 0
D0
Text Label 3000 1800 0    50   ~ 0
D1
Text Label 3000 2000 0    50   ~ 0
D2
Text Label 3000 2100 0    50   ~ 0
D3
Text Label 3000 2200 0    50   ~ 0
D4
Text Label 3000 2300 0    50   ~ 0
D5
Text Label 3000 2400 0    50   ~ 0
D6
Text Label 3000 2500 0    50   ~ 0
D7
Text Label 950  2700 0    50   ~ 0
CS
Wire Wire Line
	850  2800 2700 2800
Text Label 950  2800 0    50   ~ 0
IRQ
Wire Wire Line
	850  2900 1400 2900
Wire Wire Line
	1400 2900 1400 3100
Wire Wire Line
	850  3100 1400 3100
Connection ~ 1400 3100
Wire Wire Line
	1400 3100 1400 3300
Wire Wire Line
	850  3300 1400 3300
Connection ~ 1400 3300
Wire Wire Line
	1400 3300 1400 3500
Connection ~ 1400 3500
Wire Wire Line
	1400 3500 1400 4150
Wire Wire Line
	850  3500 1400 3500
$Comp
L power:GND #PWR03
U 1 1 5C541F4E
P 1400 4150
F 0 "#PWR03" H 1400 3900 50  0001 C CNN
F 1 "GND" H 1405 3977 50  0000 C CNN
F 2 "" H 1400 4150 50  0001 C CNN
F 3 "" H 1400 4150 50  0001 C CNN
	1    1400 4150
	1    0    0    -1  
$EndComp
Text Label 950  3000 0    50   ~ 0
RESET
Text Label 950  3200 0    50   ~ 0
ACK
Wire Wire Line
	850  3700 2200 3700
Wire Wire Line
	2200 3700 2200 5000
Text Label 950  3400 0    50   ~ 0
A1
Text Label 950  3600 0    50   ~ 0
RW
Text Label 950  3700 0    50   ~ 0
DRQ
NoConn ~ 850  3800
Wire Bus Line
	2200 1050 2850 1050
$Comp
L power:GND #PWR06
U 1 1 5C548139
P 2900 4300
F 0 "#PWR06" H 2900 4050 50  0001 C CNN
F 1 "GND" H 2905 4127 50  0000 C CNN
F 2 "" H 2900 4300 50  0001 C CNN
F 3 "" H 2900 4300 50  0001 C CNN
	1    2900 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5C5482D4
P 3300 2800
F 0 "#PWR011" H 3300 2550 50  0001 C CNN
F 1 "GND" H 3305 2627 50  0000 C CNN
F 2 "" H 3300 2800 50  0001 C CNN
F 3 "" H 3300 2800 50  0001 C CNN
	1    3300 2800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR05
U 1 1 5C54916C
P 2500 4200
F 0 "#PWR05" H 2500 4050 50  0001 C CNN
F 1 "+3.3V" H 2515 4373 50  0000 C CNN
F 2 "" H 2500 4200 50  0001 C CNN
F 3 "" H 2500 4200 50  0001 C CNN
	1    2500 4200
	1    0    0    -1  
$EndComp
$Comp
L simon:74LVC8T245 U5
U 1 1 5C54BAE7
P 4150 1300
F 0 "U5" H 4475 1365 50  0000 C CNN
F 1 "74LVC8T245" H 4475 1274 50  0000 C CNN
F 2 "Package_SO:TSSOP-24_4.4x7.8mm_P0.65mm" H 4150 1300 50  0001 C CNN
F 3 "" H 4150 1300 50  0001 C CNN
	1    4150 1300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2950 1800 3300 1800
Wire Wire Line
	2950 1900 3300 1900
Wire Wire Line
	2950 2000 3300 2000
Wire Wire Line
	2950 2100 3300 2100
Wire Wire Line
	2950 2200 3300 2200
Wire Wire Line
	2950 2300 3300 2300
Wire Wire Line
	2950 2400 3300 2400
Wire Wire Line
	2950 2500 3300 2500
Wire Wire Line
	3300 2600 3300 2800
Wire Wire Line
	3300 2800 4350 2800
Wire Wire Line
	4350 2800 4350 2600
Connection ~ 3300 2800
Wire Wire Line
	4350 2600 4350 2500
Connection ~ 4350 2600
Wire Wire Line
	3300 1600 3300 1500
Text Label 2250 1050 0    50   ~ 0
D[0..7]
$Comp
L power:+3.3V #PWR013
U 1 1 5C555AF8
P 4350 1300
F 0 "#PWR013" H 4350 1150 50  0001 C CNN
F 1 "+3.3V" H 4365 1473 50  0000 C CNN
F 2 "" H 4350 1300 50  0001 C CNN
F 3 "" H 4350 1300 50  0001 C CNN
	1    4350 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR010
U 1 1 5C555B53
P 3300 1300
F 0 "#PWR010" H 3300 1150 50  0001 C CNN
F 1 "+5V" H 3315 1473 50  0000 C CNN
F 2 "" H 3300 1300 50  0001 C CNN
F 3 "" H 3300 1300 50  0001 C CNN
	1    3300 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1300 3300 1500
Connection ~ 3300 1500
Wire Wire Line
	4350 1300 4350 1500
Wire Wire Line
	2900 4300 3100 4300
Wire Wire Line
	6350 2750 6650 2750
Wire Wire Line
	6350 2850 6650 2850
Wire Wire Line
	6350 2950 6650 2950
Text Label 4400 1800 0    50   ~ 0
F_D0
Text Label 4400 1700 0    50   ~ 0
F_D1
Text Label 4400 1900 0    50   ~ 0
F_D2
Text Label 4400 2000 0    50   ~ 0
F_D3
Text Label 4400 2100 0    50   ~ 0
F_D4
Text Label 4400 2200 0    50   ~ 0
F_D5
Text Label 4400 2300 0    50   ~ 0
F_D6
Text Label 4400 2400 0    50   ~ 0
F_D7
$Comp
L Device:R R1
U 1 1 5C5EBA80
P 1350 1900
F 0 "R1" V 1143 1900 50  0001 C CNN
F 1 "1K" V 1350 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 1900 50  0001 C CNN
F 3 "~" H 1350 1900 50  0001 C CNN
	1    1350 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5C5EBB76
P 1350 2000
F 0 "R2" V 1143 2000 50  0001 C CNN
F 1 "1K" V 1350 2000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2000 50  0001 C CNN
F 3 "~" H 1350 2000 50  0001 C CNN
	1    1350 2000
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5C5EBB9D
P 1350 2100
F 0 "R3" V 1143 2100 50  0001 C CNN
F 1 "1K" V 1350 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2100 50  0001 C CNN
F 3 "~" H 1350 2100 50  0001 C CNN
	1    1350 2100
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5C5EBBC4
P 1350 2200
F 0 "R4" V 1143 2200 50  0001 C CNN
F 1 "1K" V 1350 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2200 50  0001 C CNN
F 3 "~" H 1350 2200 50  0001 C CNN
	1    1350 2200
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5C5EBBEB
P 1350 2300
F 0 "R5" V 1143 2300 50  0001 C CNN
F 1 "1K" V 1350 2300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2300 50  0001 C CNN
F 3 "~" H 1350 2300 50  0001 C CNN
	1    1350 2300
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5C5EBC12
P 1350 2400
F 0 "R6" V 1143 2400 50  0001 C CNN
F 1 "1K" V 1350 2400 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2400 50  0001 C CNN
F 3 "~" H 1350 2400 50  0001 C CNN
	1    1350 2400
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5C5EBC39
P 1350 2500
F 0 "R7" V 1143 2500 50  0001 C CNN
F 1 "1K" V 1350 2500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2500 50  0001 C CNN
F 3 "~" H 1350 2500 50  0001 C CNN
	1    1350 2500
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5C5EBC60
P 1350 2600
F 0 "R8" V 1143 2600 50  0001 C CNN
F 1 "1K" V 1350 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1280 2600 50  0001 C CNN
F 3 "~" H 1350 2600 50  0001 C CNN
	1    1350 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	850  2600 1200 2600
Wire Wire Line
	1500 1900 2100 1900
Wire Wire Line
	1500 2000 2100 2000
Wire Wire Line
	1500 2100 2100 2100
Wire Wire Line
	1500 2200 2100 2200
Wire Wire Line
	1500 2300 2100 2300
Wire Wire Line
	1500 2400 2100 2400
Wire Wire Line
	1500 2500 2100 2500
Wire Wire Line
	1500 2600 2100 2600
Wire Wire Line
	1200 1900 850  1900
Wire Wire Line
	1200 2000 850  2000
Wire Wire Line
	1200 2100 850  2100
Wire Wire Line
	1200 2200 850  2200
Wire Wire Line
	1200 2300 850  2300
Wire Wire Line
	1200 2400 850  2400
Wire Wire Line
	1200 2500 850  2500
$Comp
L power:+3.3V #PWR014
U 1 1 5C65CC08
P 4350 4300
F 0 "#PWR014" H 4350 4150 50  0001 C CNN
F 1 "+3.3V" H 4365 4473 50  0000 C CNN
F 2 "" H 4350 4300 50  0001 C CNN
F 3 "" H 4350 4300 50  0001 C CNN
	1    4350 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 4300 4350 4300
Wire Wire Line
	4000 4200 4100 4200
Text Label 4400 1600 0    50   ~ 0
F_BUS_DIR
Wire Wire Line
	3300 1700 3200 1700
Wire Wire Line
	3200 1700 3200 2600
Wire Wire Line
	3200 2600 3300 2600
Connection ~ 3300 2600
$Sheet
S 9350 3350 1600 2950
U 5D6A3597
F0 "ATMega" 50
F1 "file5D6A3596.sch" 50
F2 "A_D[0..7]" I L 9350 4550 50 
F3 "CLK8" I L 9350 3550 50 
F4 "A_BUS_DIR" I L 9350 4050 50 
F5 "A_INT" I L 9350 3950 50 
F6 "A_CS" I L 9350 3850 50 
F7 "A_CMD" I L 9350 3750 50 
F8 "A_CD" I L 9350 4150 50 
F9 "A_CONT" I L 9350 4250 50 
F10 "MOSI" I L 9350 5700 50 
F11 "MISO" I L 9350 5800 50 
F12 "SCK" I L 9350 5900 50 
F13 "SS" I L 9350 6000 50 
F14 "A_X" I L 9350 4950 50 
F15 "A_RESET" I L 9350 4450 50 
F16 "A_Y" I L 9350 5050 50 
F17 "A_Z" I L 9350 5150 50 
$EndSheet
$Comp
L power:GND #PWR0101
U 1 1 5D674301
P 6650 4650
F 0 "#PWR0101" H 6650 4400 50  0001 C CNN
F 1 "GND" H 6655 4477 50  0000 C CNN
F 2 "" H 6650 4650 50  0001 C CNN
F 3 "" H 6650 4650 50  0001 C CNN
	1    6650 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4650 6650 4550
Wire Wire Line
	6650 4450 6650 4350
Wire Wire Line
	6650 4050 6650 3950
Connection ~ 6650 4050
$Comp
L power:+1V8 #PWR0102
U 1 1 5D674321
P 6450 4400
F 0 "#PWR0102" H 6450 4250 50  0001 C CNN
F 1 "+1V8" H 6465 4573 50  0000 C CNN
F 2 "" H 6450 4400 50  0001 C CNN
F 3 "" H 6450 4400 50  0001 C CNN
	1    6450 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4400 6450 4450
Wire Wire Line
	6450 4450 6650 4450
$Comp
L power:+3.3V #PWR0103
U 1 1 5D67432A
P 6450 4050
F 0 "#PWR0103" H 6450 3900 50  0001 C CNN
F 1 "+3.3V" H 6465 4223 50  0000 C CNN
F 2 "" H 6450 4050 50  0001 C CNN
F 3 "" H 6450 4050 50  0001 C CNN
	1    6450 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4050 6650 4050
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J1
U 1 1 5D67433E
P 7600 5500
F 0 "J1" H 7650 5917 50  0000 C CNN
F 1 "JTAG" H 7650 5826 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 7600 5500 50  0001 C CNN
F 3 "~" H 7600 5500 50  0001 C CNN
	1    7600 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 5300 7050 5300
Wire Wire Line
	7050 5300 7050 4950
Wire Wire Line
	7050 4950 8150 4950
Wire Wire Line
	8150 4950 8150 4450
Wire Wire Line
	8150 4450 7850 4450
Wire Wire Line
	7400 5400 6950 5400
Wire Wire Line
	6950 5400 6950 4850
Wire Wire Line
	6950 4850 8050 4850
Wire Wire Line
	8050 4850 8050 4550
Wire Wire Line
	8050 4550 7850 4550
Wire Wire Line
	7400 5500 6950 5500
Wire Wire Line
	6950 5500 6950 6050
Wire Wire Line
	6950 6050 8350 6050
Wire Wire Line
	8350 6050 8350 4250
Wire Wire Line
	8350 4250 7850 4250
Wire Wire Line
	7050 5700 7050 5950
Wire Wire Line
	7050 5950 8250 5950
Wire Wire Line
	8250 5950 8250 4350
Wire Wire Line
	8250 4350 7850 4350
Wire Wire Line
	7050 5700 7400 5700
$Comp
L power:+3.3V #PWR0104
U 1 1 5D67435A
P 8650 5400
F 0 "#PWR0104" H 8650 5250 50  0001 C CNN
F 1 "+3.3V" H 8665 5573 50  0000 C CNN
F 2 "" H 8650 5400 50  0001 C CNN
F 3 "" H 8650 5400 50  0001 C CNN
	1    8650 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 5400 7900 5400
Wire Wire Line
	7900 5300 8150 5300
Wire Wire Line
	8150 5300 8150 5700
Wire Wire Line
	8150 5700 7900 5700
$Comp
L power:GND #PWR0105
U 1 1 5D674364
P 8150 5700
F 0 "#PWR0105" H 8150 5450 50  0001 C CNN
F 1 "GND" H 8155 5527 50  0000 C CNN
F 2 "" H 8150 5700 50  0001 C CNN
F 3 "" H 8150 5700 50  0001 C CNN
	1    8150 5700
	1    0    0    -1  
$EndComp
Connection ~ 8150 5700
NoConn ~ 7400 5600
NoConn ~ 7900 5600
NoConn ~ 7900 5500
$Comp
L Device:R R11
U 1 1 5D67436E
P 6650 5300
F 0 "R11" V 6443 5300 50  0000 C CNN
F 1 "1K" V 6534 5300 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6580 5300 50  0001 C CNN
F 3 "~" H 6650 5300 50  0001 C CNN
	1    6650 5300
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 5D674375
P 6650 5700
F 0 "R12" V 6443 5700 50  0000 C CNN
F 1 "1K" V 6534 5700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6580 5700 50  0001 C CNN
F 3 "~" H 6650 5700 50  0001 C CNN
	1    6650 5700
	0    1    1    0   
$EndComp
$Comp
L Device:R R13
U 1 1 5D67437C
P 6650 6050
F 0 "R13" V 6443 6050 50  0000 C CNN
F 1 "10K" V 6534 6050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6580 6050 50  0001 C CNN
F 3 "~" H 6650 6050 50  0001 C CNN
	1    6650 6050
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 5300 7050 5300
Connection ~ 7050 5300
Wire Wire Line
	6800 5700 7050 5700
Connection ~ 7050 5700
Wire Wire Line
	6800 6050 6950 6050
Connection ~ 6950 6050
$Comp
L power:+3.3V #PWR0106
U 1 1 5D674389
P 6100 5950
F 0 "#PWR0106" H 6100 5800 50  0001 C CNN
F 1 "+3.3V" H 6115 6123 50  0000 C CNN
F 2 "" H 6100 5950 50  0001 C CNN
F 3 "" H 6100 5950 50  0001 C CNN
	1    6100 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5D67438F
P 6100 5300
F 0 "#PWR0107" H 6100 5050 50  0001 C CNN
F 1 "GND" H 6105 5127 50  0000 C CNN
F 2 "" H 6100 5300 50  0001 C CNN
F 3 "" H 6100 5300 50  0001 C CNN
	1    6100 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 5950 6100 6050
Wire Wire Line
	6100 6050 6350 6050
Wire Wire Line
	6350 6050 6350 5700
Wire Wire Line
	6350 5700 6500 5700
Connection ~ 6350 6050
Wire Wire Line
	6350 6050 6500 6050
Wire Wire Line
	6100 5300 6500 5300
Text Label 7900 2250 0    50   ~ 0
F_D0
Text Label 7900 2350 0    50   ~ 0
F_D1
Text Label 7900 2150 0    50   ~ 0
F_D2
Text Label 7900 2050 0    50   ~ 0
F_D3
Text Label 7900 1950 0    50   ~ 0
F_D4
Text Label 7900 1350 0    50   ~ 0
F_D5
Text Label 7900 1250 0    50   ~ 0
F_D6
Text Label 6300 3750 0    50   ~ 0
F_D7
Entry Wire Line
	8400 2950 8500 3050
Entry Wire Line
	8400 2850 8500 2950
Wire Wire Line
	7850 3150 8400 3150
Wire Wire Line
	7850 3250 8400 3250
Wire Wire Line
	7850 3450 8400 3450
Wire Wire Line
	7850 3650 8400 3650
Wire Wire Line
	7850 3850 8400 3850
Text Label 8900 4550 0    50   ~ 0
A_D[0..7]
Text Label 7900 2950 0    50   ~ 0
A_D0
Text Label 7900 2850 0    50   ~ 0
A_D1
Text Label 7900 2750 0    50   ~ 0
A_D2
Text Label 7900 2650 0    50   ~ 0
A_D3
Text Label 7900 2550 0    50   ~ 0
A_D4
Text Label 6200 1950 0    50   ~ 0
A_D5
Text Label 6200 1650 0    50   ~ 0
A_D6
Text Label 6200 1550 0    50   ~ 0
A_D7
Wire Bus Line
	8500 4550 9350 4550
Wire Wire Line
	9350 4050 8850 4050
Text Label 8900 3750 0    50   ~ 0
A_CMD
Text Label 7900 3450 0    50   ~ 0
A_CMD
Text Label 8900 3850 0    50   ~ 0
A_CS
Text Label 7900 3550 0    50   ~ 0
A_CS
Text Label 8900 4050 0    50   ~ 0
A_BUS_DIR
Text Label 7900 3650 0    50   ~ 0
A_BUS_DIR
Wire Wire Line
	8850 3750 9350 3750
Wire Wire Line
	8850 3850 9350 3850
Wire Wire Line
	8850 3950 9350 3950
Text Label 8900 3950 0    50   ~ 0
A_INT
Text Label 7900 3750 0    50   ~ 0
A_INT
Wire Wire Line
	7850 3050 8400 3050
Wire Wire Line
	8400 2950 7850 2950
Wire Wire Line
	7850 2850 8400 2850
$Comp
L Device:C C18
U 1 1 5DBC6BC6
P 10400 2100
F 0 "C18" H 10515 2146 50  0000 L CNN
F 1 "0.1uF" H 10515 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10438 1950 50  0001 C CNN
F 3 "~" H 10400 2100 50  0001 C CNN
	1    10400 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C15
U 1 1 5DBC6BF6
P 10650 2100
F 0 "C15" H 10765 2146 50  0000 L CNN
F 1 "0.1uF" H 10765 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10688 1950 50  0001 C CNN
F 3 "~" H 10650 2100 50  0001 C CNN
	1    10650 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C16
U 1 1 5DBC6C2C
P 9950 1100
F 0 "C16" H 10065 1146 50  0000 L CNN
F 1 "0.1uF" H 10065 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9988 950 50  0001 C CNN
F 3 "~" H 9950 1100 50  0001 C CNN
	1    9950 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C17
U 1 1 5DBC6CB6
P 10200 1100
F 0 "C17" H 10315 1146 50  0000 L CNN
F 1 "0.1uF" H 10315 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10238 950 50  0001 C CNN
F 3 "~" H 10200 1100 50  0001 C CNN
	1    10200 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5DBC6EDD
P 10200 1400
F 0 "#PWR09" H 10200 1150 50  0001 C CNN
F 1 "GND" H 10205 1227 50  0000 C CNN
F 2 "" H 10200 1400 50  0001 C CNN
F 3 "" H 10200 1400 50  0001 C CNN
	1    10200 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5DBC6F2C
P 10650 2400
F 0 "#PWR02" H 10650 2150 50  0001 C CNN
F 1 "GND" H 10655 2227 50  0000 C CNN
F 2 "" H 10650 2400 50  0001 C CNN
F 3 "" H 10650 2400 50  0001 C CNN
	1    10650 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+1V8 #PWR04
U 1 1 5DBC6FBA
P 10200 800
F 0 "#PWR04" H 10200 650 50  0001 C CNN
F 1 "+1V8" H 10215 973 50  0000 C CNN
F 2 "" H 10200 800 50  0001 C CNN
F 3 "" H 10200 800 50  0001 C CNN
	1    10200 800 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 5DBC7048
P 10650 1800
F 0 "#PWR01" H 10650 1650 50  0001 C CNN
F 1 "+3.3V" H 10665 1973 50  0000 C CNN
F 2 "" H 10650 1800 50  0001 C CNN
F 3 "" H 10650 1800 50  0001 C CNN
	1    10650 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 1800 10650 1950
Connection ~ 10650 1950
Wire Wire Line
	10400 1950 10650 1950
Wire Wire Line
	10400 2250 10650 2250
Wire Wire Line
	10650 2400 10650 2250
Connection ~ 10650 2250
Wire Wire Line
	9950 950  10200 950 
Wire Wire Line
	10200 800  10200 950 
Connection ~ 10200 950 
Wire Wire Line
	9950 1250 10200 1250
Wire Wire Line
	10200 1250 10200 1400
Connection ~ 10200 1250
Wire Wire Line
	6250 3250 6650 3250
Text Label 4150 3400 0    50   ~ 0
F_CS
Text Label 4150 3800 0    50   ~ 0
F_RESET
Text Label 4150 3700 0    50   ~ 0
F_ACK
Text Label 4150 3600 0    50   ~ 0
F_A1
Text Label 4150 3500 0    50   ~ 0
F_RW
Text Label 6300 3650 0    50   ~ 0
F_RESET
Wire Wire Line
	4000 3500 4550 3500
Text Label 6300 3550 0    50   ~ 0
F_ACK
Text Label 6300 3450 0    50   ~ 0
F_A1
Text Label 6300 3350 0    50   ~ 0
F_RW
Wire Wire Line
	4000 3600 4550 3600
Wire Wire Line
	4000 3700 4550 3700
Wire Wire Line
	4000 3800 4550 3800
Text Label 6300 3250 0    50   ~ 0
F_CS
Wire Wire Line
	4000 3400 4550 3400
Wire Wire Line
	6250 3350 6650 3350
Wire Wire Line
	6250 3450 6650 3450
Wire Wire Line
	6250 3550 6650 3550
Wire Wire Line
	6250 3650 6650 3650
Text Label 3500 5600 0    50   ~ 0
F_IRQ
Text Label 3500 5200 0    50   ~ 0
F_DRQ
Connection ~ 10400 2250
Connection ~ 10400 1950
Wire Wire Line
	10150 2250 10400 2250
Wire Wire Line
	9900 2250 10150 2250
Connection ~ 10150 2250
Wire Wire Line
	10150 1950 10400 1950
Wire Wire Line
	9900 1950 10150 1950
Connection ~ 10150 1950
$Comp
L Device:C C14
U 1 1 5DBC6B9A
P 10150 2100
F 0 "C14" H 10265 2146 50  0000 L CNN
F 1 "0.1uF" H 10265 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10188 1950 50  0001 C CNN
F 3 "~" H 10150 2100 50  0001 C CNN
	1    10150 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C13
U 1 1 5DBC64FE
P 9900 2100
F 0 "C13" H 10015 2146 50  0000 L CNN
F 1 "0.1uF" H 10015 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9938 1950 50  0001 C CNN
F 3 "~" H 9900 2100 50  0001 C CNN
	1    9900 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 5DC6DA4A
P 9250 1100
F 0 "C11" H 9365 1146 50  0000 L CNN
F 1 "0.1uF" H 9365 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9288 950 50  0001 C CNN
F 3 "~" H 9250 1100 50  0001 C CNN
	1    9250 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C12
U 1 1 5DC6DA50
P 9500 1100
F 0 "C12" H 9615 1146 50  0000 L CNN
F 1 "0.1uF" H 9615 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9538 950 50  0001 C CNN
F 3 "~" H 9500 1100 50  0001 C CNN
	1    9500 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5DC6DA56
P 9500 1400
F 0 "#PWR017" H 9500 1150 50  0001 C CNN
F 1 "GND" H 9505 1227 50  0000 C CNN
F 2 "" H 9500 1400 50  0001 C CNN
F 3 "" H 9500 1400 50  0001 C CNN
	1    9500 1400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR016
U 1 1 5DC6DA5C
P 9500 800
F 0 "#PWR016" H 9500 650 50  0001 C CNN
F 1 "+3.3V" H 9515 973 50  0000 C CNN
F 2 "" H 9500 800 50  0001 C CNN
F 3 "" H 9500 800 50  0001 C CNN
	1    9500 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 800  9500 950 
Connection ~ 9500 950 
Wire Wire Line
	9250 950  9500 950 
Wire Wire Line
	9250 1250 9500 1250
Wire Wire Line
	9500 1400 9500 1250
Connection ~ 9500 1250
$Comp
L power:GND #PWR015
U 1 1 5DC7A1BA
P 9050 1400
F 0 "#PWR015" H 9050 1150 50  0001 C CNN
F 1 "GND" H 9055 1227 50  0000 C CNN
F 2 "" H 9050 1400 50  0001 C CNN
F 3 "" H 9050 1400 50  0001 C CNN
	1    9050 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 800  9050 950 
Wire Wire Line
	9050 1400 9050 1250
$Comp
L power:+5V #PWR012
U 1 1 5DC86642
P 9050 800
F 0 "#PWR012" H 9050 650 50  0001 C CNN
F 1 "+5V" H 9065 973 50  0000 C CNN
F 2 "" H 9050 800 50  0001 C CNN
F 3 "" H 9050 800 50  0001 C CNN
	1    9050 800 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5DC88602
P 8800 1100
F 0 "C19" H 8915 1146 50  0000 L CNN
F 1 "0.1uF" H 8915 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8838 950 50  0001 C CNN
F 3 "~" H 8800 1100 50  0001 C CNN
	1    8800 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 950  9050 950 
Wire Wire Line
	9050 1250 8800 1250
Wire Wire Line
	3100 4500 3100 4300
Wire Wire Line
	4100 4500 3100 4500
Wire Wire Line
	4100 4200 4100 4500
Wire Wire Line
	7850 2550 8400 2550
Text Label 7900 3350 0    50   ~ 0
A_CD
Wire Wire Line
	7850 2450 8400 2450
Text Label 7900 3150 0    50   ~ 0
A_CONT
Wire Wire Line
	9350 4150 8850 4150
Wire Wire Line
	9350 4250 8850 4250
Text Label 8900 4150 0    50   ~ 0
A_CD
Text Label 8900 4250 0    50   ~ 0
A_CONT
$Comp
L Transistor_BJT:BC847 Q1
U 1 1 5F306733
P 2300 5200
F 0 "Q1" H 2491 5246 50  0000 L CNN
F 1 "BC847" H 2491 5155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2500 5125 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 2300 5200 50  0001 L CNN
	1    2300 5200
	-1   0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC847 Q2
U 1 1 5F30ACA7
P 2800 5600
F 0 "Q2" H 2991 5646 50  0000 L CNN
F 1 "BC847" H 2991 5555 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3000 5525 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 2800 5600 50  0001 L CNN
	1    2800 5600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2700 2800 2700 5400
Wire Wire Line
	2200 5400 2200 5800
Wire Wire Line
	2200 5800 2700 5800
$Comp
L power:GND #PWR0124
U 1 1 5F35C162
P 2200 5800
F 0 "#PWR0124" H 2200 5550 50  0001 C CNN
F 1 "GND" H 2205 5627 50  0000 C CNN
F 2 "" H 2200 5800 50  0001 C CNN
F 3 "" H 2200 5800 50  0001 C CNN
	1    2200 5800
	1    0    0    -1  
$EndComp
Connection ~ 2200 5800
$Comp
L Device:R R9
U 1 1 5F35DD95
P 3250 5200
F 0 "R9" V 3043 5200 50  0000 C CNN
F 1 "R" V 3134 5200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3180 5200 50  0001 C CNN
F 3 "~" H 3250 5200 50  0001 C CNN
	1    3250 5200
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5F35F4C4
P 3250 5600
F 0 "R10" V 3043 5600 50  0000 C CNN
F 1 "R" V 3134 5600 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3180 5600 50  0001 C CNN
F 3 "~" H 3250 5600 50  0001 C CNN
	1    3250 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 5200 2500 5200
Wire Wire Line
	3000 5600 3100 5600
Wire Wire Line
	5650 5200 3400 5200
Wire Wire Line
	5750 5600 3400 5600
Text Notes 3500 5400 0    50   ~ 0
Open-collector outputs
$Comp
L Mechanical:MountingHole H1
U 1 1 5F261B1F
P 10600 650
F 0 "H1" H 10700 696 50  0000 L CNN
F 1 "MountingHole" H 10700 605 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 10600 650 50  0001 C CNN
F 3 "~" H 10600 650 50  0001 C CNN
	1    10600 650 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5F26286F
P 10600 850
F 0 "H2" H 10700 896 50  0000 L CNN
F 1 "MountingHole" H 10700 805 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 10600 850 50  0001 C CNN
F 3 "~" H 10600 850 50  0001 C CNN
	1    10600 850 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5F262A3A
P 10600 1050
F 0 "H3" H 10700 1096 50  0000 L CNN
F 1 "MountingHole" H 10700 1005 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 10600 1050 50  0001 C CNN
F 3 "~" H 10600 1050 50  0001 C CNN
	1    10600 1050
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5F262C52
P 10600 1250
F 0 "H4" H 10700 1296 50  0000 L CNN
F 1 "MountingHole" H 10700 1205 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 10600 1250 50  0001 C CNN
F 3 "~" H 10600 1250 50  0001 C CNN
	1    10600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3600 850  3600
Wire Wire Line
	850  3200 2400 3200
Wire Wire Line
	850  3000 2800 3000
Wire Wire Line
	850  2700 2900 2700
Wire Wire Line
	2500 4200 3100 4200
$Comp
L Device:C C4
U 1 1 5F793837
P 9650 2100
F 0 "C4" H 9765 2146 50  0000 L CNN
F 1 "0.1uF" H 9765 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9688 1950 50  0001 C CNN
F 3 "~" H 9650 2100 50  0001 C CNN
	1    9650 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 1950 9900 1950
Connection ~ 9900 1950
Wire Wire Line
	9650 2250 9900 2250
Connection ~ 9900 2250
Wire Wire Line
	5650 3050 6650 3050
Wire Wire Line
	5650 3050 5650 5200
Wire Wire Line
	5750 3150 6650 3150
Wire Wire Line
	5750 3150 5750 5600
$Comp
L Connector:SD_Card J3
U 1 1 5F8E69B4
P 4500 6650
AR Path="/5F8E69B4" Ref="J3"  Part="1" 
AR Path="/5D6A3597/5F8E69B4" Ref="J?"  Part="1" 
F 0 "J3" H 4500 7315 50  0000 C CNN
F 1 "SD_Card" H 4500 7224 50  0000 C CNN
F 2 "smonson:Amphenol_SD_Card_GSD090012SEU" H 4500 6650 50  0001 C CNN
F 3 "http://portal.fciconnect.com/Comergent//fci/drawing/10067847.pdf" H 4500 6650 50  0001 C CNN
	1    4500 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 5F8E69BA
P 3350 7250
AR Path="/5F8E69BA" Ref="#PWR0111"  Part="1" 
AR Path="/5D6A3597/5F8E69BA" Ref="#PWR?"  Part="1" 
F 0 "#PWR0111" H 3350 7000 50  0001 C CNN
F 1 "GND" H 3355 7077 50  0000 C CNN
F 2 "" H 3350 7250 50  0001 C CNN
F 3 "" H 3350 7250 50  0001 C CNN
	1    3350 7250
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0112
U 1 1 5F8E69C0
P 3450 6050
AR Path="/5F8E69C0" Ref="#PWR0112"  Part="1" 
AR Path="/5D6A3597/5F8E69C0" Ref="#PWR?"  Part="1" 
F 0 "#PWR0112" H 3450 5900 50  0001 C CNN
F 1 "+3.3V" H 3465 6223 50  0000 C CNN
F 2 "" H 3450 6050 50  0001 C CNN
F 3 "" H 3450 6050 50  0001 C CNN
	1    3450 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 6350 2950 6350
Wire Wire Line
	3600 6650 3450 6650
Wire Wire Line
	3450 6650 3450 6050
Wire Wire Line
	3600 6550 3350 6550
Wire Wire Line
	3350 6550 3350 6850
Wire Wire Line
	3600 6850 3350 6850
Connection ~ 3350 6850
Wire Wire Line
	3350 6850 3350 7250
Text Label 3000 6350 0    50   ~ 0
AVR_SS
Wire Wire Line
	3600 6450 2950 6450
Text Label 3000 6450 0    50   ~ 0
MOSI
Wire Wire Line
	3600 6750 2950 6750
Text Label 3000 6750 0    50   ~ 0
SCK
Wire Wire Line
	3600 6950 2950 6950
Text Label 3000 6950 0    50   ~ 0
MISO
NoConn ~ 3600 7050
NoConn ~ 3600 6250
NoConn ~ 5400 6450
NoConn ~ 5400 6550
$Comp
L power:GND #PWR?
U 1 1 5F8E69D9
P 5550 7050
AR Path="/5D6A3597/5F8E69D9" Ref="#PWR?"  Part="1" 
AR Path="/5F8E69D9" Ref="#PWR0123"  Part="1" 
F 0 "#PWR0123" H 5550 6800 50  0001 C CNN
F 1 "GND" H 5555 6877 50  0000 C CNN
F 2 "" H 5550 7050 50  0001 C CNN
F 3 "" H 5550 7050 50  0001 C CNN
	1    5550 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 6750 5550 6750
Wire Wire Line
	5550 6750 5550 6850
Wire Wire Line
	5400 6850 5550 6850
Connection ~ 5550 6850
Wire Wire Line
	5550 6850 5550 7050
Text Label 6200 2250 0    50   ~ 0
MOSI
Wire Wire Line
	9350 6000 8900 6000
Text Label 8900 6000 0    50   ~ 0
AVR_SS
Wire Wire Line
	9350 5900 8900 5900
Wire Wire Line
	9350 5700 8900 5700
Text Label 8900 5700 0    50   ~ 0
AVR_MOSI
Text Label 8150 6300 0    50   ~ 0
AVR_MISO
Text Label 8900 5900 0    50   ~ 0
AVR_SCK
Wire Wire Line
	7850 2050 8400 2050
Wire Wire Line
	7850 2150 8400 2150
Wire Wire Line
	7850 2250 8400 2250
Wire Wire Line
	7850 2350 8400 2350
Wire Wire Line
	7850 2650 8400 2650
Wire Wire Line
	7850 2750 8400 2750
$Comp
L Device:C C6
U 1 1 5F9DC3B0
P 9400 2100
F 0 "C6" H 9515 2146 50  0000 L CNN
F 1 "0.1uF" H 9515 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9438 1950 50  0001 C CNN
F 3 "~" H 9400 2100 50  0001 C CNN
	1    9400 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 1950 9650 1950
Connection ~ 9650 1950
Wire Wire Line
	9400 2250 9650 2250
Connection ~ 9650 2250
Wire Wire Line
	7850 1250 8400 1250
Wire Wire Line
	7850 1350 8400 1350
Wire Wire Line
	7850 1450 8400 1450
Wire Wire Line
	7850 1550 8400 1550
Wire Wire Line
	7850 1650 8400 1650
Wire Wire Line
	7850 1750 8400 1750
Wire Wire Line
	7850 1850 8400 1850
Wire Wire Line
	7850 1950 8400 1950
Wire Wire Line
	9350 4950 8850 4950
Wire Wire Line
	9350 4450 8850 4450
Text Label 8900 4950 0    50   ~ 0
A_X
Text Label 8900 4450 0    50   ~ 0
A_RESET
Text Label 7900 3050 0    50   ~ 0
A_RESET
Text Label 7900 3950 0    50   ~ 0
AVR_MISO
Text Label 6200 1250 0    50   ~ 0
AVR_MOSI
Text Label 7900 3850 0    50   ~ 0
AVR_SCK
Connection ~ 6650 4450
Wire Wire Line
	6650 4050 6650 4150
Wire Wire Line
	6650 4150 6650 4250
Connection ~ 6650 4150
$Comp
L FPGA_Altera:5M40Z-EQFP64 U1
U 1 1 5D650820
P 7450 1100
F 0 "U1" H 7250 1175 50  0000 C CNN
F 1 "5M40Z-EQFP64" H 7250 1084 50  0000 C CNN
F 2 "simon:EQFP-64_7x7mm_P0.4mm-thinner-pads" H 7450 1100 50  0001 C CNN
F 3 "" H 7450 1100 50  0001 C CNN
	1    7450 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 1250 6050 1250
Wire Wire Line
	6650 1350 6050 1350
Wire Wire Line
	6650 1550 6050 1550
Wire Wire Line
	6650 1650 6050 1650
Wire Wire Line
	6650 1950 6050 1950
Wire Wire Line
	6650 2050 6050 2050
Text Label 6200 1350 0    50   ~ 0
A_X
Wire Wire Line
	6650 2150 6050 2150
$Comp
L simon:Oscillator7x5 U4
U 1 1 5F45E393
P 850 850
F 0 "U4" H 1100 975 50  0000 C CNN
F 1 "Oscillator7x5" H 1100 884 50  0000 C CNN
F 2 "simon:XO_7x5" H 850 850 50  0001 C CNN
F 3 "" H 850 850 50  0001 C CNN
	1    850  850 
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0125
U 1 1 5F46305C
P 700 850
F 0 "#PWR0125" H 700 700 50  0001 C CNN
F 1 "+3.3V" H 715 1023 50  0000 C CNN
F 2 "" H 700 850 50  0001 C CNN
F 3 "" H 700 850 50  0001 C CNN
	1    700  850 
	1    0    0    -1  
$EndComp
NoConn ~ 700  1050
$Comp
L power:GND #PWR0126
U 1 1 5F479D71
P 1500 1150
F 0 "#PWR0126" H 1500 900 50  0001 C CNN
F 1 "GND" H 1505 977 50  0000 C CNN
F 2 "" H 1500 1150 50  0001 C CNN
F 3 "" H 1500 1150 50  0001 C CNN
	1    1500 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1050 1500 1150
Wire Wire Line
	700  850  700  950 
Wire Wire Line
	1500 950  1900 950 
Text Label 1600 950  0    50   ~ 0
CLK32
Text Label 7900 1850 0    50   ~ 0
CLK32
$Comp
L Device:C C1
U 1 1 5F44B8BC
P 9150 2100
F 0 "C1" H 9265 2146 50  0000 L CNN
F 1 "0.1uF" H 9265 2055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 9188 1950 50  0001 C CNN
F 3 "~" H 9150 2100 50  0001 C CNN
	1    9150 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 1950 9400 1950
Connection ~ 9400 1950
Wire Wire Line
	9150 2250 9400 2250
Connection ~ 9400 2250
Text Label 6200 2150 0    50   ~ 0
MISO
Text Label 6200 2350 0    50   ~ 0
SCK
Wire Wire Line
	6050 2250 6650 2250
Wire Wire Line
	6050 2350 6650 2350
Wire Wire Line
	6050 2450 6650 2450
Wire Wire Line
	6650 1450 6050 1450
Wire Wire Line
	5800 750  5800 1750
Wire Wire Line
	9350 5050 8850 5050
Wire Wire Line
	9350 5150 8850 5150
Text Label 8900 5050 0    50   ~ 0
A_Y
Text Label 8900 5150 0    50   ~ 0
A_Z
Text Label 6200 1450 0    50   ~ 0
A_Y
NoConn ~ 6650 1750
Text Label 6200 2050 0    50   ~ 0
A_Z
Text Label 6200 2450 0    50   ~ 0
LED1
Text Label 6200 2650 0    50   ~ 0
LED0
Wire Wire Line
	6050 2650 6650 2650
Wire Wire Line
	6050 2550 6650 2550
$Comp
L Device:LED D1
U 1 1 5F97ECC7
P 850 5300
F 0 "D1" V 889 5182 50  0000 R CNN
F 1 "LED" V 798 5182 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 850 5300 50  0001 C CNN
F 3 "~" H 850 5300 50  0001 C CNN
	1    850  5300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R14
U 1 1 5F995E4D
P 850 5000
F 0 "R14" H 920 5046 50  0000 L CNN
F 1 "120" H 920 4955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 780 5000 50  0001 C CNN
F 3 "~" H 850 5000 50  0001 C CNN
	1    850  5000
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5F9972BE
P 1250 5300
F 0 "D2" V 1289 5182 50  0000 R CNN
F 1 "LED" V 1198 5182 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 1250 5300 50  0001 C CNN
F 3 "~" H 1250 5300 50  0001 C CNN
	1    1250 5300
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R15
U 1 1 5F9972C8
P 1250 5000
F 0 "R15" H 1320 5046 50  0000 L CNN
F 1 "120" H 1320 4955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1180 5000 50  0001 C CNN
F 3 "~" H 1250 5000 50  0001 C CNN
	1    1250 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 4850 1250 4750
Wire Wire Line
	1250 4750 1600 4750
Wire Wire Line
	850  4850 850  4650
Wire Wire Line
	850  4650 1600 4650
Wire Wire Line
	850  5450 850  5550
Wire Wire Line
	850  5550 1050 5550
Wire Wire Line
	1250 5450 1250 5550
$Comp
L power:GND #PWR0110
U 1 1 5FA03156
P 1050 5550
F 0 "#PWR0110" H 1050 5300 50  0001 C CNN
F 1 "GND" H 1055 5377 50  0000 C CNN
F 2 "" H 1050 5550 50  0001 C CNN
F 3 "" H 1050 5550 50  0001 C CNN
	1    1050 5550
	1    0    0    -1  
$EndComp
Connection ~ 1050 5550
Wire Wire Line
	1050 5550 1250 5550
Text Label 1300 4650 0    50   ~ 0
LED0
Text Label 1300 4750 0    50   ~ 0
LED1
Wire Wire Line
	8650 750  8650 3550
Wire Wire Line
	8650 3550 9350 3550
Wire Wire Line
	5800 750  8650 750 
Entry Wire Line
	8400 2650 8500 2750
Entry Wire Line
	8400 2750 8500 2850
Wire Bus Line
	8500 850  5950 850 
Entry Wire Line
	5950 1450 6050 1550
Entry Wire Line
	5950 1550 6050 1650
Entry Wire Line
	5950 1850 6050 1950
Wire Wire Line
	6650 1850 6400 1850
Wire Wire Line
	6400 1850 6400 1750
Wire Wire Line
	6400 1750 5800 1750
Entry Wire Line
	8400 2550 8500 2650
Wire Wire Line
	6650 3750 6250 3750
Text Label 7900 2450 0    50   ~ 0
F_BUS_DIR
Wire Wire Line
	7850 3350 8400 3350
Wire Wire Line
	7850 3550 8400 3550
Wire Wire Line
	7850 3750 8400 3750
Wire Wire Line
	7850 3950 8400 3950
Wire Wire Line
	4350 1600 4750 1600
Wire Wire Line
	4350 1700 4750 1700
Wire Wire Line
	4350 1800 4750 1800
Wire Wire Line
	4350 1900 4750 1900
Wire Wire Line
	4350 2000 4750 2000
Wire Wire Line
	4350 2100 4750 2100
Wire Wire Line
	4350 2200 4750 2200
Wire Wire Line
	4350 2300 4750 2300
Wire Wire Line
	4350 2400 4750 2400
$Comp
L Device:R R16
U 1 1 60EF9F3F
P 8750 5800
F 0 "R16" V 8543 5800 50  0000 C CNN
F 1 "1K" V 8634 5800 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8680 5800 50  0001 C CNN
F 3 "~" H 8750 5800 50  0001 C CNN
	1    8750 5800
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 5800 9350 5800
Wire Wire Line
	8600 5800 8600 6300
Wire Wire Line
	8600 6300 8100 6300
$Comp
L AtariHDisk3-rescue:74LS245-simon-AtariHDisk2-rescue U3
U 1 1 5C53ED45
P 3550 3800
F 0 "U3" H 3550 4487 60  0000 C CNN
F 1 "74LVC245" H 3550 4381 60  0000 C CNN
F 2 "Package_SO:TSSOP-20_4.4x6.5mm_P0.65mm" H 4050 4200 60  0001 C CNN
F 3 "" H 4050 4200 60  0001 C CNN
	1    3550 3800
	1    0    0    -1  
$EndComp
Connection ~ 3100 4300
Wire Wire Line
	2900 2700 2900 3400
Wire Wire Line
	2900 3400 3100 3400
Wire Wire Line
	3100 3500 2500 3500
Wire Wire Line
	2500 3500 2500 3600
Wire Wire Line
	3100 3600 2600 3600
Wire Wire Line
	2600 3600 2600 3400
Wire Wire Line
	850  3400 2600 3400
Wire Wire Line
	2400 3200 2400 3700
Wire Wire Line
	2400 3700 3100 3700
Wire Wire Line
	2800 3800 3100 3800
Wire Wire Line
	2800 3000 2800 3800
NoConn ~ 3100 3900
NoConn ~ 3100 4000
NoConn ~ 3100 4100
$Sheet
S 750  6200 1450 1250
U 5D66FCEC
F0 "Power" 50
F1 "file5D66FCEB.sch" 50
$EndSheet
NoConn ~ 6350 2750
NoConn ~ 6350 2850
NoConn ~ 6050 2550
NoConn ~ 6350 2950
NoConn ~ 8400 3250
NoConn ~ 8400 1750
NoConn ~ 8400 1650
NoConn ~ 8400 1550
NoConn ~ 8400 1450
NoConn ~ 4000 3900
NoConn ~ 4000 4000
NoConn ~ 4000 4100
Wire Bus Line
	5950 850  5950 1850
Wire Bus Line
	8500 850  8500 4550
Wire Bus Line
	2850 1050 2850 2400
Wire Bus Line
	2200 1050 2200 2500
$EndSCHEMATC
