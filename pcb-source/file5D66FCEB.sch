EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L Regulator_Linear:L7805 U6
U 1 1 5D686A17
P 2450 1300
F 0 "U6" H 2450 1542 50  0000 C CNN
F 1 "L7805" H 2450 1451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 2475 1150 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 2450 1250 50  0001 C CNN
	1    2450 1300
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U7
U 1 1 5D686A25
P 4150 1300
F 0 "U7" H 4150 1542 50  0000 C CNN
F 1 "AMS1117-3.3" H 4150 1451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4150 1500 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4250 1050 50  0001 C CNN
	1    4150 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5D686A2C
P 2450 1700
F 0 "#PWR0115" H 2450 1450 50  0001 C CNN
F 1 "GND" H 2455 1527 50  0000 C CNN
F 2 "" H 2450 1700 50  0001 C CNN
F 3 "" H 2450 1700 50  0001 C CNN
	1    2450 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 5D686A32
P 1500 1700
F 0 "#PWR0116" H 1500 1450 50  0001 C CNN
F 1 "GND" H 1505 1527 50  0000 C CNN
F 2 "" H 1500 1700 50  0001 C CNN
F 3 "" H 1500 1700 50  0001 C CNN
	1    1500 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5D686A38
P 4150 1700
F 0 "#PWR0117" H 4150 1450 50  0001 C CNN
F 1 "GND" H 4155 1527 50  0000 C CNN
F 2 "" H 4150 1700 50  0001 C CNN
F 3 "" H 4150 1700 50  0001 C CNN
	1    4150 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 1600 4150 1700
Wire Wire Line
	2450 1600 2450 1700
Wire Wire Line
	1500 1300 1650 1300
$Comp
L power:+5V #PWR0118
U 1 1 5D686A43
P 3250 1300
F 0 "#PWR0118" H 3250 1150 50  0001 C CNN
F 1 "+5V" H 3265 1473 50  0000 C CNN
F 2 "" H 3250 1300 50  0001 C CNN
F 3 "" H 3250 1300 50  0001 C CNN
	1    3250 1300
	1    0    0    -1  
$EndComp
Connection ~ 3250 1300
Wire Wire Line
	3250 1300 3400 1300
$Comp
L power:+3.3V #PWR0119
U 1 1 5D686A4B
P 5150 1300
F 0 "#PWR0119" H 5150 1150 50  0001 C CNN
F 1 "+3.3V" H 5165 1473 50  0000 C CNN
F 2 "" H 5150 1300 50  0001 C CNN
F 3 "" H 5150 1300 50  0001 C CNN
	1    5150 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C3
U 1 1 5D686A52
P 1950 1450
F 0 "C3" H 2068 1496 50  0000 L CNN
F 1 "4.7uF" H 2068 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 1988 1300 50  0001 C CNN
F 3 "~" H 1950 1450 50  0001 C CNN
	1    1950 1450
	1    0    0    -1  
$EndComp
Connection ~ 1950 1300
Wire Wire Line
	1950 1300 2150 1300
Connection ~ 4150 1600
Connection ~ 2450 1600
Wire Wire Line
	2450 1600 1950 1600
$Comp
L Device:CP C5
U 1 1 5D686A72
P 3400 1450
F 0 "C5" H 3515 1496 50  0000 L CNN
F 1 "4.7uF" H 3515 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 3438 1300 50  0001 C CNN
F 3 "~" H 3400 1450 50  0001 C CNN
	1    3400 1450
	1    0    0    -1  
$EndComp
Connection ~ 3400 1300
Wire Wire Line
	3400 1300 3850 1300
$Comp
L Device:CP C8
U 1 1 5D686A7B
P 5150 1450
F 0 "C8" H 5265 1496 50  0000 L CNN
F 1 "4.7uF" H 5265 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5188 1300 50  0001 C CNN
F 3 "~" H 5150 1450 50  0001 C CNN
	1    5150 1450
	1    0    0    -1  
$EndComp
Connection ~ 5150 1300
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5D686A87
P 1650 1300
F 0 "#FLG0101" H 1650 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 1650 1450 50  0000 C CNN
F 2 "" H 1650 1300 50  0001 C CNN
F 3 "~" H 1650 1300 50  0001 C CNN
	1    1650 1300
	1    0    0    -1  
$EndComp
Connection ~ 1650 1300
Wire Wire Line
	1650 1300 1950 1300
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5D686A8F
P 1650 1600
F 0 "#FLG0102" H 1650 1675 50  0001 C CNN
F 1 "PWR_FLAG" H 1650 1774 50  0000 C CNN
F 2 "" H 1650 1600 50  0001 C CNN
F 3 "~" H 1650 1600 50  0001 C CNN
	1    1650 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 1600 1500 1600
Connection ~ 1500 1600
Wire Wire Line
	1500 1600 1500 1700
$Comp
L power:GND #PWR?
U 1 1 5D686A98
P 4200 2950
AR Path="/5D686A98" Ref="#PWR?"  Part="1" 
AR Path="/5CC8524D/5D686A98" Ref="#PWR?"  Part="1" 
AR Path="/5D66FCEC/5D686A98" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 4200 2700 50  0001 C CNN
F 1 "GND" H 4205 2777 50  0000 C CNN
F 2 "" H 4200 2950 50  0001 C CNN
F 3 "" H 4200 2950 50  0001 C CNN
	1    4200 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C9
U 1 1 5D686A9E
P 5250 2650
F 0 "C9" H 5365 2696 50  0000 L CNN
F 1 "4.7uF" H 5365 2605 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5288 2500 50  0001 C CNN
F 3 "~" H 5250 2650 50  0001 C CNN
	1    5250 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2850 5250 2800
Wire Wire Line
	4200 2950 4200 2850
Wire Wire Line
	5250 2400 5250 2500
$Comp
L power:+3.3V #PWR0121
U 1 1 5D686AB6
P 3300 2400
F 0 "#PWR0121" H 3300 2250 50  0001 C CNN
F 1 "+3.3V" H 3315 2573 50  0000 C CNN
F 2 "" H 3300 2400 50  0001 C CNN
F 3 "" H 3300 2400 50  0001 C CNN
	1    3300 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+1V8 #PWR0122
U 1 1 5D686ABC
P 5250 2400
F 0 "#PWR0122" H 5250 2250 50  0001 C CNN
F 1 "+1V8" H 5265 2573 50  0000 C CNN
F 2 "" H 5250 2400 50  0001 C CNN
F 3 "" H 5250 2400 50  0001 C CNN
	1    5250 2400
	1    0    0    -1  
$EndComp
Connection ~ 5250 2400
$Comp
L Regulator_Linear:AMS1117-1.8 U8
U 1 1 5D686AC3
P 4200 2400
F 0 "U8" H 4200 2642 50  0000 C CNN
F 1 "AMS1117-1.8" H 4200 2551 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 4200 2600 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 4300 2150 50  0001 C CNN
	1    4200 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2400 3900 2400
Wire Wire Line
	4200 2700 4200 2850
Connection ~ 4200 2850
Wire Wire Line
	2750 1300 3250 1300
Wire Wire Line
	2450 1600 3400 1600
Wire Wire Line
	4450 1300 5150 1300
Wire Wire Line
	4150 1600 5150 1600
Wire Wire Line
	4500 2400 5250 2400
Wire Wire Line
	4200 2850 5250 2850
$Comp
L Connector:Conn_01x02_Female J5
U 1 1 5D686A1E
P 1300 1300
F 0 "J5" H 1300 1400 50  0000 C CNN
F 1 "Conn_01x02_Female" V 1400 1250 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 1300 1300 50  0001 C CNN
F 3 "~" H 1300 1300 50  0001 C CNN
	1    1300 1300
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1500 1400 1500 1600
$EndSCHEMATC
