EESchema Schematic File Version 4
LIBS:picobaloon-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Deadbadger Picoballoon probe"
Date ""
Rev "0.3"
Comp "Jakub Kaderka"
Comment1 "Probe for Hvězdárna a planetárium Brno balloon challange"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:D_Schottky D1
U 1 1 5C478BA9
P 1250 1250
F 0 "D1" H 1250 1034 50  0000 C CNN
F 1 "BAT20J" H 1250 1125 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323" H 1250 1250 50  0001 C CNN
F 3 "~" H 1250 1250 50  0001 C CNN
	1    1250 1250
	-1   0    0    1   
$EndComp
$Comp
L Device:CP C2
U 1 1 5C478CDB
P 1500 1550
F 0 "C2" H 1618 1596 50  0000 L CNN
F 1 "3F3/3V" H 1618 1505 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 1538 1400 50  0001 C CNN
F 3 "~" H 1500 1550 50  0001 C CNN
	1    1500 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 1250 1500 1250
Wire Wire Line
	1500 1250 1500 1400
$Comp
L power:GND #PWR02
U 1 1 5C478D9B
P 1500 1900
F 0 "#PWR02" H 1500 1650 50  0001 C CNN
F 1 "GND" H 1505 1727 50  0000 C CNN
F 2 "" H 1500 1900 50  0001 C CNN
F 3 "" H 1500 1900 50  0001 C CNN
	1    1500 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 1900 1500 1850
Connection ~ 1500 1850
Wire Wire Line
	1500 1850 1500 1700
$Comp
L Device:D_Zener D3
U 1 1 5C478EEE
P 2000 1550
F 0 "D3" V 1954 1629 50  0000 L CNN
F 1 "3v" V 2045 1629 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-323" H 2000 1550 50  0001 C CNN
F 3 "~" H 2000 1550 50  0001 C CNN
	1    2000 1550
	0    1    1    0   
$EndComp
Text Notes 1350 800  0    50   ~ 0
C = 2*(power*duration / (Vmax^2-Vmin^2))
Text Notes 650  4350 0    50   ~ 0
GPS 17 mA acquisition, 150 uA sleep \nLoRa 1,8-3,7V, 10mA rx, 120mA tx\nMCU 200uA sleep, 27 mA 48 MHz, 4,5 mA 8 MHz
$Comp
L RF_AM_FM:RFM95W-868S2 U4
U 1 1 5C482108
P 9600 1900
F 0 "U4" H 9700 2450 50  0000 C CNN
F 1 "HPD13 (SX1276)" H 9950 2350 50  0000 C CNN
F 2 "RF_Module:HOPERF_RFM9XW_SMD" H 6300 3550 50  0001 C CNN
F 3 "http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf" H 6300 3550 50  0001 C CNN
	1    9600 1900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR028
U 1 1 5C5321CE
P 9600 750
F 0 "#PWR028" H 9600 600 50  0001 C CNN
F 1 "+3V3" H 9615 923 50  0000 C CNN
F 2 "" H 9600 750 50  0001 C CNN
F 3 "" H 9600 750 50  0001 C CNN
	1    9600 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 750  9600 850 
$Comp
L Device:C C13
U 1 1 5C532455
P 9250 1250
F 0 "C13" V 8998 1250 50  0000 C CNN
F 1 "100n" V 9089 1250 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9288 1100 50  0001 C CNN
F 3 "~" H 9250 1250 50  0001 C CNN
	1    9250 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	9400 1250 9600 1250
Connection ~ 9600 1250
Wire Wire Line
	9600 1250 9600 1400
$Comp
L Device:C C12
U 1 1 5C5329DE
P 9250 850
F 0 "C12" V 8998 850 50  0000 C CNN
F 1 "1u" V 9089 850 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9288 700 50  0001 C CNN
F 3 "~" H 9250 850 50  0001 C CNN
	1    9250 850 
	0    1    1    0   
$EndComp
Wire Wire Line
	9400 850  9600 850 
Connection ~ 9600 850 
Wire Wire Line
	9600 850  9600 1250
Wire Wire Line
	9100 850  9050 850 
Wire Wire Line
	9050 850  9050 1250
Wire Wire Line
	9050 1250 9100 1250
$Comp
L power:GND #PWR026
U 1 1 5C533708
P 9000 1250
F 0 "#PWR026" H 9000 1000 50  0001 C CNN
F 1 "GND" V 9005 1122 50  0000 R CNN
F 2 "" H 9000 1250 50  0001 C CNN
F 3 "" H 9000 1250 50  0001 C CNN
	1    9000 1250
	0    1    1    0   
$EndComp
Wire Wire Line
	9000 1250 9050 1250
Connection ~ 9050 1250
Wire Wire Line
	9500 2500 9500 2550
Wire Wire Line
	9500 2550 9600 2550
Wire Wire Line
	9700 2550 9700 2500
Wire Wire Line
	9600 2500 9600 2550
Connection ~ 9600 2550
Wire Wire Line
	9600 2550 9700 2550
$Comp
L power:GND #PWR029
U 1 1 5C534440
P 9600 2600
F 0 "#PWR029" H 9600 2350 50  0001 C CNN
F 1 "GND" H 9605 2427 50  0000 C CNN
F 2 "" H 9600 2600 50  0001 C CNN
F 3 "" H 9600 2600 50  0001 C CNN
	1    9600 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2600 9600 2550
Wire Wire Line
	10100 2300 10450 2300
Text Label 10300 2300 0    50   ~ 0
DIO0
Wire Wire Line
	10100 2200 10450 2200
Text Label 10300 2200 0    50   ~ 0
DIO1
NoConn ~ 10100 2000
NoConn ~ 10100 1900
NoConn ~ 10100 1800
$Comp
L Regulator_Switching:MCP1640CH U2
U 1 1 5C537A1F
P 3850 1450
F 0 "U2" H 3850 1917 50  0000 C CNN
F 1 "MCP1640CH" H 3850 1826 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 3900 1200 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20002234D.pdf" H 3600 1900 50  0001 C CNN
	1    3850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 1250 3400 1250
Connection ~ 1500 1250
$Comp
L Device:R R6
U 1 1 5C53887F
P 4450 1550
F 0 "R6" H 4520 1596 50  0000 L CNN
F 1 "270k" H 4520 1505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4380 1550 50  0001 C CNN
F 3 "~" H 4450 1550 50  0001 C CNN
	1    4450 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5C5388CA
P 4450 1950
F 0 "R7" H 4520 1996 50  0000 L CNN
F 1 "470k" H 4520 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4380 1950 50  0001 C CNN
F 3 "~" H 4450 1950 50  0001 C CNN
	1    4450 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1700 4450 1750
$Comp
L power:GND #PWR011
U 1 1 5C538D59
P 4450 2150
F 0 "#PWR011" H 4450 1900 50  0001 C CNN
F 1 "GND" H 4455 1977 50  0000 C CNN
F 2 "" H 4450 2150 50  0001 C CNN
F 3 "" H 4450 2150 50  0001 C CNN
	1    4450 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 2150 4450 2100
$Comp
L Device:C C3
U 1 1 5C53963F
P 2650 1550
F 0 "C3" H 2765 1596 50  0000 L CNN
F 1 "100n" H 2765 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2688 1400 50  0001 C CNN
F 3 "~" H 2650 1550 50  0001 C CNN
	1    2650 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1400 2650 1250
Connection ~ 2650 1250
Wire Wire Line
	2650 1250 2000 1250
Wire Wire Line
	2650 1700 2650 1850
Wire Wire Line
	2650 1850 2000 1850
$Comp
L Device:L L1
U 1 1 5C53AB88
P 3900 800
F 0 "L1" V 4090 800 50  0000 C CNN
F 1 "SRN2510F-4R7M" V 3999 800 50  0000 C CNN
F 2 "Inductor_SMD:L_Abracon_ASPI-3012S" H 3900 800 50  0001 C CNN
F 3 "~" H 3900 800 50  0001 C CNN
	1    3900 800 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3750 800  3400 800 
Wire Wire Line
	3400 800  3400 1250
Connection ~ 3400 1250
Wire Wire Line
	3400 1250 3000 1250
Wire Wire Line
	4050 800  4350 800 
Wire Wire Line
	4350 800  4350 1250
Wire Wire Line
	4350 1250 4250 1250
Wire Wire Line
	4250 1350 4450 1350
Wire Wire Line
	4450 1350 4450 1400
Wire Wire Line
	4450 1750 4300 1750
Wire Wire Line
	4300 1750 4300 1550
Wire Wire Line
	4300 1550 4250 1550
Connection ~ 4450 1750
Wire Wire Line
	4450 1750 4450 1800
$Comp
L Device:C C4
U 1 1 5C53E2F2
P 4900 1550
F 0 "C4" H 5015 1596 50  0000 L CNN
F 1 "10n" H 5015 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4938 1400 50  0001 C CNN
F 3 "~" H 4900 1550 50  0001 C CNN
	1    4900 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1350 4900 1350
Wire Wire Line
	5350 1350 5350 1400
Connection ~ 4450 1350
Wire Wire Line
	4900 1400 4900 1350
Connection ~ 4900 1350
Wire Wire Line
	4900 1350 5350 1350
$Comp
L power:GND #PWR016
U 1 1 5C53FA75
P 5350 1800
F 0 "#PWR016" H 5350 1550 50  0001 C CNN
F 1 "GND" H 5355 1627 50  0000 C CNN
F 2 "" H 5350 1800 50  0001 C CNN
F 3 "" H 5350 1800 50  0001 C CNN
	1    5350 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 1800 5350 1750
Wire Wire Line
	5350 1750 4900 1750
Wire Wire Line
	4900 1750 4900 1700
Connection ~ 5350 1750
Wire Wire Line
	5350 1750 5350 1700
$Comp
L Device:CP C7
U 1 1 5C541634
P 5350 1550
F 0 "C7" H 5468 1596 50  0000 L CNN
F 1 "10u" H 5468 1505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5388 1400 50  0001 C CNN
F 3 "~" H 5350 1550 50  0001 C CNN
	1    5350 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1400 2000 1250
Connection ~ 2000 1250
Wire Wire Line
	2000 1250 1500 1250
Wire Wire Line
	2000 1700 2000 1850
Connection ~ 2000 1850
Wire Wire Line
	2000 1850 1500 1850
Wire Wire Line
	950  1250 1100 1250
$Comp
L power:+3V3 #PWR015
U 1 1 5C5465E4
P 5350 1300
F 0 "#PWR015" H 5350 1150 50  0001 C CNN
F 1 "+3V3" H 5365 1473 50  0000 C CNN
F 2 "" H 5350 1300 50  0001 C CNN
F 3 "" H 5350 1300 50  0001 C CNN
	1    5350 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 1300 5350 1350
Connection ~ 5350 1350
$Comp
L Device:R R1
U 1 1 5C547B2A
P 1750 2250
F 0 "R1" H 1820 2296 50  0000 L CNN
F 1 "100k" H 1820 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1680 2250 50  0001 C CNN
F 3 "~" H 1750 2250 50  0001 C CNN
	1    1750 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5C547BAA
P 2450 2550
F 0 "R3" V 2243 2550 50  0000 C CNN
F 1 "5k6" V 2334 2550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2380 2550 50  0001 C CNN
F 3 "~" H 2450 2550 50  0001 C CNN
	1    2450 2550
	0    1    1    0   
$EndComp
$Comp
L Transistor_BJT:BC857 Q2
U 1 1 5C547EB5
P 2900 2550
F 0 "Q2" H 3091 2504 50  0000 L CNN
F 1 "BC857" H 3091 2595 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3100 2475 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC856.pdf" H 2900 2550 50  0001 L CNN
	1    2900 2550
	1    0    0    1   
$EndComp
$Comp
L Device:R R5
U 1 1 5C547FF4
P 3000 3050
F 0 "R5" H 3070 3096 50  0000 L CNN
F 1 "100k" H 3070 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2930 3050 50  0001 C CNN
F 3 "~" H 3000 3050 50  0001 C CNN
	1    3000 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2900 3000 2850
$Comp
L power:GND #PWR08
U 1 1 5C5490CD
P 3000 3300
F 0 "#PWR08" H 3000 3050 50  0001 C CNN
F 1 "GND" H 3005 3127 50  0000 C CNN
F 2 "" H 3000 3300 50  0001 C CNN
F 3 "" H 3000 3300 50  0001 C CNN
	1    3000 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 2550 2600 2550
Wire Wire Line
	2300 2550 2100 2550
Wire Wire Line
	1750 2550 1750 2400
Connection ~ 1750 2550
Wire Wire Line
	1750 2100 1750 2000
Wire Wire Line
	1750 2000 3000 2000
Wire Wire Line
	3000 2000 3000 2350
Wire Wire Line
	3000 2000 3000 1250
Connection ~ 3000 2000
Connection ~ 3000 1250
Wire Wire Line
	3000 1250 2650 1250
Wire Wire Line
	3000 2850 3450 2850
Wire Wire Line
	3450 2850 3450 1450
Connection ~ 3000 2850
Wire Wire Line
	3000 2850 3000 2750
$Comp
L Device:D_Zener D2
U 1 1 5C56D960
P 1750 2800
F 0 "D2" V 1704 2879 50  0000 L CNN
F 1 "2v" V 1795 2879 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-323" H 1750 2800 50  0001 C CNN
F 3 "~" H 1750 2800 50  0001 C CNN
	1    1750 2800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5C56DA75
P 1750 3050
F 0 "#PWR05" H 1750 2800 50  0001 C CNN
F 1 "GND" H 1755 2877 50  0000 C CNN
F 2 "" H 1750 3050 50  0001 C CNN
F 3 "" H 1750 3050 50  0001 C CNN
	1    1750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 3050 1750 2950
Wire Wire Line
	1750 2550 1750 2650
Wire Wire Line
	3000 3200 3000 3300
$Comp
L Transistor_BJT:BC847 Q1
U 1 1 5C575428
P 2200 2850
F 0 "Q1" H 2100 3050 50  0000 L CNN
F 1 "BC847" H 1850 2950 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2400 2775 50  0001 L CIN
F 3 "http://www.infineon.com/dgdl/Infineon-BC847SERIES_BC848SERIES_BC849SERIES_BC850SERIES-DS-v01_01-en.pdf?fileId=db3a304314dca389011541d4630a1657" H 2200 2850 50  0001 L CNN
	1    2200 2850
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5C57DE0B
P 2700 2850
F 0 "R4" V 2493 2850 50  0000 C CNN
F 1 "100k" V 2584 2850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2630 2850 50  0001 C CNN
F 3 "~" H 2700 2850 50  0001 C CNN
	1    2700 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 2850 3000 2850
Wire Wire Line
	2550 2850 2500 2850
Wire Wire Line
	2100 2650 2100 2550
Connection ~ 2100 2550
Wire Wire Line
	2100 2550 1750 2550
$Comp
L power:GND #PWR06
U 1 1 5C583483
P 2100 3100
F 0 "#PWR06" H 2100 2850 50  0001 C CNN
F 1 "GND" H 2105 2927 50  0000 C CNN
F 2 "" H 2100 3100 50  0001 C CNN
F 3 "" H 2100 3100 50  0001 C CNN
	1    2100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3100 2100 3050
$Comp
L Device:R R2
U 1 1 5C58958D
P 2250 3550
F 0 "R2" V 2043 3550 50  0000 C CNN
F 1 "10k" V 2134 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2180 3550 50  0001 C CNN
F 3 "~" H 2250 3550 50  0001 C CNN
	1    2250 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 3550 2500 3550
Wire Wire Line
	2500 3550 2500 2850
Connection ~ 2500 2850
Wire Wire Line
	2500 2850 2400 2850
Wire Wire Line
	2100 3550 1600 3550
Text Label 1600 3550 0    50   ~ 0
LATCH_RESET
Text Notes 650  4000 0    50   ~ 0
When voltage on supercap reaches about Vzener + 0,6\nPNP opens and the voltage on en input goes to vss,\nNPN latches the state, it keeps PNP open until mcu\npulls latch_reset to 0.
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 5C59D03A
P 10850 1600
F 0 "J5" H 10930 1642 50  0000 L CNN
F 1 "ANT" H 10930 1551 50  0000 L CNN
F 2 "Connector_Wire:SolderWirePad_1x01_Drill1.2mm" H 10850 1600 50  0001 C CNN
F 3 "~" H 10850 1600 50  0001 C CNN
	1    10850 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 1600 10600 1600
$Comp
L MCU_ST_STM32F0:STM32F070CBTx U3
U 1 1 5C5432D2
P 6550 4500
F 0 "U3" H 6850 5950 50  0000 C CNN
F 1 "STM32F070CBTx" H 5950 5950 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 5950 3100 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00141386.pdf" H 6550 4500 50  0001 C CNN
	1    6550 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR020
U 1 1 5C54586E
P 6500 2800
F 0 "#PWR020" H 6500 2650 50  0001 C CNN
F 1 "+3V3" H 6515 2973 50  0000 C CNN
F 2 "" H 6500 2800 50  0001 C CNN
F 3 "" H 6500 2800 50  0001 C CNN
	1    6500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2900 6350 2900
Wire Wire Line
	6350 2900 6350 3000
Wire Wire Line
	6450 2900 6500 2900
Wire Wire Line
	6650 2900 6650 3000
Connection ~ 6450 2900
Wire Wire Line
	6550 3000 6550 2900
Connection ~ 6550 2900
Wire Wire Line
	6550 2900 6650 2900
Wire Wire Line
	6450 3000 6450 2900
$Comp
L Device:L L2
U 1 1 5C551466
P 7500 2650
F 0 "L2" V 7690 2650 50  0000 C CNN
F 1 "BLM21AG601SN1D" V 7599 2650 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric" H 7500 2650 50  0001 C CNN
F 3 "~" H 7500 2650 50  0001 C CNN
	1    7500 2650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7350 2650 7250 2650
Wire Wire Line
	6750 2650 6750 3000
$Comp
L Device:C C11
U 1 1 5C55649C
P 7250 2850
F 0 "C11" H 7365 2896 50  0000 L CNN
F 1 "100n" H 7365 2805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7288 2700 50  0001 C CNN
F 3 "~" H 7250 2850 50  0001 C CNN
	1    7250 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5C558E68
P 7250 3100
F 0 "#PWR022" H 7250 2850 50  0001 C CNN
F 1 "GND" H 7255 2927 50  0000 C CNN
F 2 "" H 7250 3100 50  0001 C CNN
F 3 "" H 7250 3100 50  0001 C CNN
	1    7250 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3100 7250 3000
Wire Wire Line
	7250 2700 7250 2650
Connection ~ 7250 2650
Wire Wire Line
	7250 2650 6750 2650
$Comp
L power:+3V3 #PWR024
U 1 1 5C55DFC5
P 7750 2650
F 0 "#PWR024" H 7750 2500 50  0001 C CNN
F 1 "+3V3" V 7765 2778 50  0000 L CNN
F 2 "" H 7750 2650 50  0001 C CNN
F 3 "" H 7750 2650 50  0001 C CNN
	1    7750 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	7750 2650 7650 2650
Wire Wire Line
	6350 6000 6350 6050
Wire Wire Line
	6350 6050 6450 6050
Wire Wire Line
	6650 6050 6650 6000
Wire Wire Line
	6550 6000 6550 6050
Connection ~ 6550 6050
Wire Wire Line
	6550 6050 6650 6050
Wire Wire Line
	6450 6000 6450 6050
Connection ~ 6450 6050
Wire Wire Line
	6450 6050 6500 6050
$Comp
L power:GND #PWR021
U 1 1 5C56949F
P 6500 6150
F 0 "#PWR021" H 6500 5900 50  0001 C CNN
F 1 "GND" H 6505 5977 50  0000 C CNN
F 2 "" H 6500 6150 50  0001 C CNN
F 3 "" H 6500 6150 50  0001 C CNN
	1    6500 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 6150 6500 6050
Connection ~ 6500 6050
Wire Wire Line
	6500 6050 6550 6050
Wire Wire Line
	6500 2800 6500 2900
Connection ~ 6500 2900
Wire Wire Line
	6500 2900 6550 2900
$Comp
L Device:C C8
U 1 1 5C56FA14
P 5650 7300
F 0 "C8" H 5765 7346 50  0000 L CNN
F 1 "100n" H 5765 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5688 7150 50  0001 C CNN
F 3 "~" H 5650 7300 50  0001 C CNN
	1    5650 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 5C56FB22
P 6100 7300
F 0 "C9" H 6215 7346 50  0000 L CNN
F 1 "100n" H 6215 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6138 7150 50  0001 C CNN
F 3 "~" H 6100 7300 50  0001 C CNN
	1    6100 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C10
U 1 1 5C56FBCC
P 6550 7300
F 0 "C10" H 6665 7346 50  0000 L CNN
F 1 "100n" H 6665 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 6588 7150 50  0001 C CNN
F 3 "~" H 6550 7300 50  0001 C CNN
	1    6550 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 5C56FD9D
P 5200 7300
F 0 "C6" H 5315 7346 50  0000 L CNN
F 1 "1u" H 5315 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5238 7150 50  0001 C CNN
F 3 "~" H 5200 7300 50  0001 C CNN
	1    5200 7300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5C5733DA
P 5900 7550
F 0 "#PWR019" H 5900 7300 50  0001 C CNN
F 1 "GND" H 5905 7377 50  0000 C CNN
F 2 "" H 5900 7550 50  0001 C CNN
F 3 "" H 5900 7550 50  0001 C CNN
	1    5900 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 7450 5200 7500
Wire Wire Line
	5200 7500 5650 7500
Wire Wire Line
	6550 7500 6550 7450
Wire Wire Line
	5900 7550 5900 7500
Connection ~ 5900 7500
Wire Wire Line
	5900 7500 6100 7500
Wire Wire Line
	6100 7450 6100 7500
Connection ~ 6100 7500
Wire Wire Line
	6100 7500 6550 7500
Wire Wire Line
	5650 7450 5650 7500
Connection ~ 5650 7500
Wire Wire Line
	5650 7500 5900 7500
$Comp
L power:+3V3 #PWR018
U 1 1 5C580FB9
P 5900 7050
F 0 "#PWR018" H 5900 6900 50  0001 C CNN
F 1 "+3V3" H 5915 7223 50  0000 C CNN
F 2 "" H 5900 7050 50  0001 C CNN
F 3 "" H 5900 7050 50  0001 C CNN
	1    5900 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 7050 5900 7100
Wire Wire Line
	5900 7100 5650 7100
Wire Wire Line
	5200 7100 5200 7150
Wire Wire Line
	5650 7150 5650 7100
Connection ~ 5650 7100
Wire Wire Line
	5650 7100 5200 7100
Wire Wire Line
	5900 7100 6100 7100
Wire Wire Line
	6550 7100 6550 7150
Connection ~ 5900 7100
Wire Wire Line
	6100 7150 6100 7100
Connection ~ 6100 7100
Wire Wire Line
	6100 7100 6550 7100
Wire Wire Line
	9100 1600 8750 1600
Text Label 8750 1600 0    50   ~ 0
SCK
Wire Wire Line
	9100 1700 8750 1700
Text Label 8750 1700 0    50   ~ 0
MOSI
Wire Wire Line
	9100 1800 8750 1800
Text Label 8750 1800 0    50   ~ 0
MISO
Wire Wire Line
	9100 1900 8750 1900
Text Label 8750 1900 0    50   ~ 0
RF_CS
Wire Wire Line
	9100 2100 8750 2100
Text Label 8750 2100 0    50   ~ 0
RF_RESET
Wire Wire Line
	7150 5600 7700 5600
Text Label 7450 5600 0    50   ~ 0
SWDIO
Wire Wire Line
	7150 5700 7700 5700
Text Label 7450 5700 0    50   ~ 0
SWCLK
Wire Wire Line
	5300 4600 5850 4600
Text Label 5300 4600 0    50   ~ 0
SCK
Wire Wire Line
	5300 4700 5850 4700
Text Label 5300 4700 0    50   ~ 0
MISO
Wire Wire Line
	5300 4800 5850 4800
Text Label 5300 4800 0    50   ~ 0
MOSI
Wire Wire Line
	7150 4500 7700 4500
Text Label 7400 4500 0    50   ~ 0
GPS_TX
Wire Wire Line
	7150 4600 7700 4600
Text Label 7400 4600 0    50   ~ 0
GPS_RX
Wire Wire Line
	5300 4900 5850 4900
Text Label 5300 4900 0    50   ~ 0
RF_CS
Wire Wire Line
	5300 5000 5850 5000
Text Label 5300 5000 0    50   ~ 0
RF_RESET
Wire Wire Line
	7150 5100 7700 5100
Text Label 7500 5100 0    50   ~ 0
DIO0
Wire Wire Line
	7150 5800 7700 5800
Text Label 7500 5800 0    50   ~ 0
DIO1
Wire Wire Line
	950  1250 950  950 
Wire Wire Line
	950  950  1400 950 
Connection ~ 950  1250
Text Label 950  950  0    50   ~ 0
SOLAR_VOLT
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 5C650AA2
P 4700 5400
F 0 "J3" H 4619 5075 50  0000 C CNN
F 1 "DEBUG" H 4619 5166 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4700 5400 50  0001 C CNN
F 3 "~" H 4700 5400 50  0001 C CNN
	1    4700 5400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5C656E18
P 4950 5550
F 0 "#PWR012" H 4950 5300 50  0001 C CNN
F 1 "GND" H 4955 5377 50  0000 C CNN
F 2 "" H 4950 5550 50  0001 C CNN
F 3 "" H 4950 5550 50  0001 C CNN
	1    4950 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4950 5550 4950 5500
Wire Wire Line
	4950 5500 4900 5500
Text Label 5300 5400 2    50   ~ 0
DEBUG_RX
Text Label 5300 5300 2    50   ~ 0
DEBUG_TX
Wire Wire Line
	4450 7100 4350 7100
Wire Wire Line
	4350 7100 4350 7050
$Comp
L power:+3V3 #PWR010
U 1 1 5C69753D
P 4350 7050
F 0 "#PWR010" H 4350 6900 50  0001 C CNN
F 1 "+3V3" H 4365 7223 50  0000 C CNN
F 2 "" H 4350 7050 50  0001 C CNN
F 3 "" H 4350 7050 50  0001 C CNN
	1    4350 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 7200 4100 7200
Text Label 4100 7200 0    50   ~ 0
SWCLK
$Comp
L power:GND #PWR09
U 1 1 5C69E0EC
P 4100 7300
F 0 "#PWR09" H 4100 7050 50  0001 C CNN
F 1 "GND" V 4105 7172 50  0000 R CNN
F 2 "" H 4100 7300 50  0001 C CNN
F 3 "" H 4100 7300 50  0001 C CNN
	1    4100 7300
	0    1    1    0   
$EndComp
Wire Wire Line
	4100 7300 4450 7300
Wire Wire Line
	4450 7400 4100 7400
Text Label 4100 7400 0    50   ~ 0
SWDIO
Wire Wire Line
	4900 5300 5850 5300
Wire Wire Line
	4900 5400 5850 5400
$Comp
L Device:R R9
U 1 1 5C6CF24F
P 7400 4050
F 0 "R9" V 7193 4050 50  0000 C CNN
F 1 "10k" V 7284 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7330 4050 50  0001 C CNN
F 3 "~" H 7400 4050 50  0001 C CNN
	1    7400 4050
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5C6CF3A9
P 7400 4350
F 0 "R10" V 7193 4350 50  0000 C CNN
F 1 "10k" V 7284 4350 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7330 4350 50  0001 C CNN
F 3 "~" H 7400 4350 50  0001 C CNN
	1    7400 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	7250 4050 7200 4050
Wire Wire Line
	7200 4050 7200 4300
Wire Wire Line
	7200 4300 7150 4300
Wire Wire Line
	7250 4350 7250 4400
Wire Wire Line
	7250 4400 7150 4400
Wire Wire Line
	7550 4350 8100 4350
Text Label 7600 4350 0    50   ~ 0
SOLAR_VOLT
$Comp
L Device:R R8
U 1 1 5C6EC394
P 5150 2950
F 0 "R8" H 5220 2996 50  0000 L CNN
F 1 "10k" H 5220 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5080 2950 50  0001 C CNN
F 3 "~" H 5150 2950 50  0001 C CNN
	1    5150 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5C6EC459
P 5150 3400
F 0 "C5" H 5265 3446 50  0000 L CNN
F 1 "100n" H 5265 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 5188 3250 50  0001 C CNN
F 3 "~" H 5150 3400 50  0001 C CNN
	1    5150 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3200 5150 3200
Wire Wire Line
	5150 3200 5150 3250
Wire Wire Line
	5150 3200 5150 3100
Connection ~ 5150 3200
$Comp
L power:GND #PWR014
U 1 1 5C702C6F
P 5150 3600
F 0 "#PWR014" H 5150 3350 50  0001 C CNN
F 1 "GND" H 5155 3427 50  0000 C CNN
F 2 "" H 5150 3600 50  0001 C CNN
F 3 "" H 5150 3600 50  0001 C CNN
	1    5150 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3600 5150 3550
$Comp
L power:+3V3 #PWR013
U 1 1 5C70A69E
P 5150 2750
F 0 "#PWR013" H 5150 2600 50  0001 C CNN
F 1 "+3V3" H 5165 2923 50  0000 C CNN
F 2 "" H 5150 2750 50  0001 C CNN
F 3 "" H 5150 2750 50  0001 C CNN
	1    5150 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 2750 5150 2800
Wire Wire Line
	5150 3200 4800 3200
Text Label 4800 3200 0    50   ~ 0
RESET
Wire Wire Line
	5300 5100 5850 5100
Text Label 5300 5100 0    50   ~ 0
LATCH_RESET
$Comp
L Sensor_Temperature:LMT87DCK U1
U 1 1 5C743042
P 1350 7150
F 0 "U1" H 1020 7196 50  0000 R CNN
F 1 "LMT87DCK" H 1020 7105 50  0000 R CNN
F 2 "Package_TO_SOT_SMD:SOT-353_SC-70-5" H 1350 6750 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmt87-q1.pdf" H 1350 7150 50  0001 C CNN
	1    1350 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 6750 1350 6750
Wire Wire Line
	1450 6750 1450 6850
Wire Wire Line
	1350 6850 1350 6750
Connection ~ 1350 6750
Wire Wire Line
	1350 6750 1450 6750
Wire Wire Line
	1250 6850 1250 6750
$Comp
L power:GND #PWR04
U 1 1 5C75C82D
P 1350 7500
F 0 "#PWR04" H 1350 7250 50  0001 C CNN
F 1 "GND" H 1355 7327 50  0000 C CNN
F 2 "" H 1350 7500 50  0001 C CNN
F 3 "" H 1350 7500 50  0001 C CNN
	1    1350 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 7500 1350 7450
$Comp
L power:+3V3 #PWR03
U 1 1 5C76567C
P 1350 6650
F 0 "#PWR03" H 1350 6500 50  0001 C CNN
F 1 "+3V3" H 1365 6823 50  0000 C CNN
F 2 "" H 1350 6650 50  0001 C CNN
F 3 "" H 1350 6650 50  0001 C CNN
	1    1350 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 6650 1350 6750
$Comp
L Device:C C1
U 1 1 5C76E4B8
P 1050 6750
F 0 "C1" V 798 6750 50  0000 C CNN
F 1 "100n" V 889 6750 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1088 6600 50  0001 C CNN
F 3 "~" H 1050 6750 50  0001 C CNN
	1    1050 6750
	0    1    1    0   
$EndComp
Wire Wire Line
	1200 6750 1250 6750
Connection ~ 1250 6750
$Comp
L power:GND #PWR01
U 1 1 5C7775CF
P 850 6750
F 0 "#PWR01" H 850 6500 50  0001 C CNN
F 1 "GND" V 855 6622 50  0000 R CNN
F 2 "" H 850 6750 50  0001 C CNN
F 3 "" H 850 6750 50  0001 C CNN
	1    850  6750
	0    1    1    0   
$EndComp
Wire Wire Line
	850  6750 900  6750
Wire Wire Line
	1750 7150 2150 7150
Text Label 1800 7150 0    50   ~ 0
TEMP_AD
Wire Wire Line
	7150 4800 7700 4800
Text Label 7350 4800 0    50   ~ 0
TEMP_AD
$Comp
L power:VCC #PWR07
U 1 1 5C7B1779
P 2650 1150
F 0 "#PWR07" H 2650 1000 50  0001 C CNN
F 1 "VCC" H 2667 1323 50  0000 C CNN
F 2 "" H 2650 1150 50  0001 C CNN
F 3 "" H 2650 1150 50  0001 C CNN
	1    2650 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1150 2650 1250
$Comp
L power:VCC #PWR023
U 1 1 5C7BB363
P 7650 4050
F 0 "#PWR023" H 7650 3900 50  0001 C CNN
F 1 "VCC" V 7667 4178 50  0000 L CNN
F 2 "" H 7650 4050 50  0001 C CNN
F 3 "" H 7650 4050 50  0001 C CNN
	1    7650 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 4050 7550 4050
$Comp
L RF_GPS:SIM28ML U5
U 1 1 5C54C1BC
P 9850 4500
F 0 "U5" H 10100 5150 50  0000 L CNN
F 1 "SIM28ML" H 10100 5050 50  0000 L CNN
F 2 "RF_Module:SIM28ML" H 9850 4500 50  0001 C CNN
F 3 "https://simcom.ee/documents/SIM28ML/SIM28ML_Hardware%20Design_V1.01.pdf" H 9850 4500 50  0001 C CNN
	1    9850 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR030
U 1 1 5C555C12
P 9750 3250
F 0 "#PWR030" H 9750 3100 50  0001 C CNN
F 1 "+3V3" H 9765 3423 50  0000 C CNN
F 2 "" H 9750 3250 50  0001 C CNN
F 3 "" H 9750 3250 50  0001 C CNN
	1    9750 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 3250 9750 3400
$Comp
L Device:C C15
U 1 1 5C57AC49
P 9500 3800
F 0 "C15" V 9248 3800 50  0000 C CNN
F 1 "100n" V 9339 3800 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9538 3650 50  0001 C CNN
F 3 "~" H 9500 3800 50  0001 C CNN
	1    9500 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	9650 3800 9750 3800
Connection ~ 9750 3800
Wire Wire Line
	9750 3800 9750 3900
$Comp
L power:GND #PWR027
U 1 1 5C584171
P 9200 3800
F 0 "#PWR027" H 9200 3550 50  0001 C CNN
F 1 "GND" V 9205 3672 50  0000 R CNN
F 2 "" H 9200 3800 50  0001 C CNN
F 3 "" H 9200 3800 50  0001 C CNN
	1    9200 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	9200 3800 9300 3800
NoConn ~ 10050 3900
NoConn ~ 10550 4300
NoConn ~ 10550 4600
Wire Wire Line
	9150 4200 8800 4200
Text Label 8800 4200 0    50   ~ 0
GPS_RX
Wire Wire Line
	9150 4300 8800 4300
Text Label 8800 4300 0    50   ~ 0
GPS_TX
NoConn ~ 9150 4400
NoConn ~ 9150 4500
NoConn ~ 9150 4900
$Comp
L Device:Battery_Cell BT1
U 1 1 5C5EDAE9
P 10150 3650
F 0 "BT1" V 10405 3700 50  0000 C CNN
F 1 "3V" V 10314 3700 50  0000 C CNN
F 2 "Battery:BatteryHolder_Keystone_3001_1x12mm" V 10150 3710 50  0001 C CNN
F 3 "~" V 10150 3710 50  0001 C CNN
	1    10150 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9950 3650 9850 3650
Wire Wire Line
	9850 3650 9850 3900
$Comp
L power:GND #PWR032
U 1 1 5C6016B8
P 10300 3650
F 0 "#PWR032" H 10300 3400 50  0001 C CNN
F 1 "GND" V 10305 3522 50  0000 R CNN
F 2 "" H 10300 3650 50  0001 C CNN
F 3 "" H 10300 3650 50  0001 C CNN
	1    10300 3650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10300 3650 10250 3650
$Comp
L Device:C C14
U 1 1 5C62AA24
P 9500 3400
F 0 "C14" V 9248 3400 50  0000 C CNN
F 1 "1u" V 9339 3400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 9538 3250 50  0001 C CNN
F 3 "~" H 9500 3400 50  0001 C CNN
	1    9500 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	9650 3400 9750 3400
Connection ~ 9750 3400
Wire Wire Line
	9750 3400 9750 3800
Wire Wire Line
	9350 3400 9300 3400
Wire Wire Line
	9300 3400 9300 3800
Connection ~ 9300 3800
Wire Wire Line
	9300 3800 9350 3800
$Comp
L Device:R R11
U 1 1 5C654D77
P 8600 4600
F 0 "R11" H 8670 4646 50  0000 L CNN
F 1 "10k" H 8670 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8530 4600 50  0001 C CNN
F 3 "~" H 8600 4600 50  0001 C CNN
	1    8600 4600
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR025
U 1 1 5C654E63
P 8600 4400
F 0 "#PWR025" H 8600 4250 50  0001 C CNN
F 1 "+3V3" H 8615 4573 50  0000 C CNN
F 2 "" H 8600 4400 50  0001 C CNN
F 3 "" H 8600 4400 50  0001 C CNN
	1    8600 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 4400 8600 4450
Wire Wire Line
	8600 4750 8600 4800
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5C6D2475
P 11100 4400
F 0 "J4" H 11020 4175 50  0000 C CNN
F 1 "GPS_ANT" H 11020 4266 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_Drill1.2mm" H 11100 4400 50  0001 C CNN
F 3 "~" H 11100 4400 50  0001 C CNN
	1    11100 4400
	1    0    0    1   
$EndComp
NoConn ~ 7150 4900
NoConn ~ 7150 5000
NoConn ~ 7150 5400
NoConn ~ 7150 5500
NoConn ~ 5850 5500
NoConn ~ 5850 4300
NoConn ~ 5850 4400
NoConn ~ 5850 4500
NoConn ~ 5850 4100
NoConn ~ 5850 4000
NoConn ~ 5850 3600
NoConn ~ 5850 3700
$Comp
L power:GND #PWR017
U 1 1 5C7C6DE7
P 5800 3400
F 0 "#PWR017" H 5800 3150 50  0001 C CNN
F 1 "GND" V 5805 3272 50  0000 R CNN
F 2 "" H 5800 3400 50  0001 C CNN
F 3 "" H 5800 3400 50  0001 C CNN
	1    5800 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	5800 3400 5850 3400
Wire Wire Line
	10900 4400 10850 4400
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5C81060F
P 700 1250
F 0 "J1" H 620 1025 50  0000 C CNN
F 1 "SOLAR+" H 620 1116 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_Drill1.2mm" H 700 1250 50  0001 C CNN
F 3 "~" H 700 1250 50  0001 C CNN
	1    700  1250
	-1   0    0    1   
$EndComp
Wire Wire Line
	900  1250 950  1250
$Comp
L Connector_Generic:Conn_01x01 J6
U 1 1 5C81C229
P 700 1850
F 0 "J6" H 620 1625 50  0000 C CNN
F 1 "SOLAR-" H 620 1716 50  0000 C CNN
F 2 "Connector_Wire:SolderWirePad_1x01_Drill1.2mm" H 700 1850 50  0001 C CNN
F 3 "~" H 700 1850 50  0001 C CNN
	1    700  1850
	-1   0    0    1   
$EndComp
Wire Wire Line
	900  1850 1500 1850
$Comp
L power:GND #PWR0101
U 1 1 5C828248
P 3850 1850
F 0 "#PWR0101" H 3850 1600 50  0001 C CNN
F 1 "GND" H 3855 1677 50  0000 C CNN
F 2 "" H 3850 1850 50  0001 C CNN
F 3 "" H 3850 1850 50  0001 C CNN
	1    3850 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 1850 3850 1750
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5C833E92
P 4750 800
F 0 "#FLG0101" H 4750 875 50  0001 C CNN
F 1 "PWR_FLAG" H 4750 974 50  0000 C CNN
F 2 "" H 4750 800 50  0001 C CNN
F 3 "~" H 4750 800 50  0001 C CNN
	1    4750 800 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5C833F7A
P 4750 850
F 0 "#PWR0102" H 4750 600 50  0001 C CNN
F 1 "GND" H 4755 677 50  0000 C CNN
F 2 "" H 4750 850 50  0001 C CNN
F 3 "" H 4750 850 50  0001 C CNN
	1    4750 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 850  4750 800 
$Comp
L power:VCC #PWR0103
U 1 1 5C83FA73
P 5250 850
F 0 "#PWR0103" H 5250 700 50  0001 C CNN
F 1 "VCC" H 5268 1023 50  0000 C CNN
F 2 "" H 5250 850 50  0001 C CNN
F 3 "" H 5250 850 50  0001 C CNN
	1    5250 850 
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5C83FBB5
P 5250 800
F 0 "#FLG0102" H 5250 875 50  0001 C CNN
F 1 "PWR_FLAG" H 5250 974 50  0000 C CNN
F 2 "" H 5250 800 50  0001 C CNN
F 3 "~" H 5250 800 50  0001 C CNN
	1    5250 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 800  5250 850 
NoConn ~ 7150 5200
NoConn ~ 7150 5300
NoConn ~ 5850 5200
NoConn ~ 5850 3900
NoConn ~ 10100 2100
$Comp
L power:GND #PWR0104
U 1 1 5C5BD69D
P 9850 5150
F 0 "#PWR0104" H 9850 4900 50  0001 C CNN
F 1 "GND" H 9855 4977 50  0000 C CNN
F 2 "" H 9850 5150 50  0001 C CNN
F 3 "" H 9850 5150 50  0001 C CNN
	1    9850 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 5150 9850 5100
Wire Wire Line
	8600 4800 9150 4800
NoConn ~ 7150 4700
Text Label 5000 6200 0    50   ~ 0
SCL
$Comp
L power:GND #PWR037
U 1 1 5C5D4DF8
P 1050 5800
F 0 "#PWR037" H 1050 5550 50  0001 C CNN
F 1 "GND" H 1055 5627 50  0000 C CNN
F 2 "" H 1050 5800 50  0001 C CNN
F 3 "" H 1050 5800 50  0001 C CNN
	1    1050 5800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1050 5800 1050 5750
$Comp
L power:+3V3 #PWR036
U 1 1 5C5F5633
P 1050 4850
F 0 "#PWR036" H 1050 4700 50  0001 C CNN
F 1 "+3V3" H 1065 5023 50  0000 C CNN
F 2 "" H 1050 4850 50  0001 C CNN
F 3 "" H 1050 4850 50  0001 C CNN
	1    1050 4850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1050 4850 1050 4900
$Comp
L power:GND #PWR035
U 1 1 5C600539
P 1500 4900
F 0 "#PWR035" H 1500 4650 50  0001 C CNN
F 1 "GND" V 1505 4772 50  0000 R CNN
F 2 "" H 1500 4900 50  0001 C CNN
F 3 "" H 1500 4900 50  0001 C CNN
	1    1500 4900
	0    -1   1    0   
$EndComp
$Comp
L Device:C C16
U 1 1 5C600628
P 1250 4900
F 0 "C16" V 998 4900 50  0000 C CNN
F 1 "100n" V 1089 4900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1288 4750 50  0001 C CNN
F 3 "~" H 1250 4900 50  0001 C CNN
	1    1250 4900
	0    -1   1    0   
$EndComp
Wire Wire Line
	1100 4900 1050 4900
Connection ~ 1050 4900
Wire Wire Line
	1050 4900 1050 4950
Wire Wire Line
	1400 4900 1500 4900
$Comp
L power:+3V3 #PWR033
U 1 1 5C616C76
P 1550 5150
F 0 "#PWR033" H 1550 5000 50  0001 C CNN
F 1 "+3V3" V 1565 5278 50  0000 L CNN
F 2 "" H 1550 5150 50  0001 C CNN
F 3 "" H 1550 5150 50  0001 C CNN
	1    1550 5150
	0    1    -1   0   
$EndComp
Wire Wire Line
	1550 5150 1450 5150
$Comp
L power:GND #PWR034
U 1 1 5C622118
P 1550 5600
F 0 "#PWR034" H 1550 5350 50  0001 C CNN
F 1 "GND" H 1555 5427 50  0000 C CNN
F 2 "" H 1550 5600 50  0001 C CNN
F 3 "" H 1550 5600 50  0001 C CNN
	1    1550 5600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1550 5600 1550 5550
Wire Wire Line
	1550 5550 1450 5550
Wire Wire Line
	1450 5350 1700 5350
Text Label 1700 5350 2    50   ~ 0
SDA
Wire Wire Line
	1450 5450 1700 5450
Text Label 1700 5450 2    50   ~ 0
SCL
$Comp
L Device:R R12
U 1 1 5C65C0DC
P 5200 5900
F 0 "R12" H 5270 5946 50  0000 L CNN
F 1 "10k" H 5270 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5130 5900 50  0001 C CNN
F 3 "~" H 5200 5900 50  0001 C CNN
	1    5200 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5C65C1BB
P 5500 5900
F 0 "R13" H 5570 5946 50  0000 L CNN
F 1 "10k" H 5570 5855 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5430 5900 50  0001 C CNN
F 3 "~" H 5500 5900 50  0001 C CNN
	1    5500 5900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR031
U 1 1 5C6736F9
P 5350 5650
F 0 "#PWR031" H 5350 5500 50  0001 C CNN
F 1 "+3V3" H 5365 5823 50  0000 C CNN
F 2 "" H 5350 5650 50  0001 C CNN
F 3 "" H 5350 5650 50  0001 C CNN
	1    5350 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 5750 5500 5700
Wire Wire Line
	5500 5700 5350 5700
Wire Wire Line
	5200 5700 5200 5750
Wire Wire Line
	5350 5700 5350 5650
Connection ~ 5350 5700
Wire Wire Line
	5350 5700 5200 5700
Wire Wire Line
	5200 6200 5200 6050
Connection ~ 5200 6200
Wire Wire Line
	5200 6200 5000 6200
Wire Wire Line
	5500 6300 5500 6050
Wire Wire Line
	5200 6200 5750 6200
Wire Wire Line
	5800 6300 5500 6300
NoConn ~ 1450 5250
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5C700548
P 4650 7200
F 0 "J2" H 4730 7192 50  0000 L CNN
F 1 "SWD" H 4730 7101 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 4650 7200 50  0001 C CNN
F 3 "~" H 4650 7200 50  0001 C CNN
	1    4650 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 6300 5500 6300
Connection ~ 5500 6300
Wire Wire Line
	5750 5600 5850 5600
Wire Wire Line
	5750 5600 5750 6200
Wire Wire Line
	5850 5700 5800 5700
Wire Wire Line
	5800 5700 5800 6300
NoConn ~ 5850 5800
Text Label 5000 6300 0    50   ~ 0
SDA
$Comp
L Device:R R15
U 1 1 5C5DCD1F
P 10600 1850
F 0 "R15" H 10670 1896 50  0000 L CNN
F 1 "1M" H 10670 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10530 1850 50  0001 C CNN
F 3 "~" H 10600 1850 50  0001 C CNN
	1    10600 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR039
U 1 1 5C5DCE1E
P 10600 2050
F 0 "#PWR039" H 10600 1800 50  0001 C CNN
F 1 "GND" H 10605 1877 50  0000 C CNN
F 2 "" H 10600 2050 50  0001 C CNN
F 3 "" H 10600 2050 50  0001 C CNN
	1    10600 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10600 2050 10600 2000
Wire Wire Line
	10600 1700 10600 1600
Connection ~ 10600 1600
Wire Wire Line
	10600 1600 10100 1600
$Comp
L Device:R R14
U 1 1 5C67EF28
P 10850 4650
F 0 "R14" H 10920 4696 50  0000 L CNN
F 1 "1M" H 10920 4605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 10780 4650 50  0001 C CNN
F 3 "~" H 10850 4650 50  0001 C CNN
	1    10850 4650
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR038
U 1 1 5C67F0A4
P 10850 4850
F 0 "#PWR038" H 10850 4600 50  0001 C CNN
F 1 "GND" H 10855 4677 50  0000 C CNN
F 2 "" H 10850 4850 50  0001 C CNN
F 3 "" H 10850 4850 50  0001 C CNN
	1    10850 4850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10850 4850 10850 4800
Wire Wire Line
	10850 4500 10850 4400
Connection ~ 10850 4400
Wire Wire Line
	10850 4400 10550 4400
Text Notes 9200 5850 0    50   ~ 0
Resistor on antennas are added as a simple ESD\nprotection as the air in high altitudes is dry\nand some charge might build up
$Comp
L Sensor_Pressure:MS5607-02BA U6
U 1 1 5C6A08B7
P 1050 5350
F 0 "U6" H 1250 5700 50  0000 L CNN
F 1 "MS5607-02BA" V 1400 5100 50  0000 L CNN
F 2 "Package_LGA:LGA-8_3x5mm_P1.25mm" H 1050 5350 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5607-02BA03%7FB2%7Fpdf%7FEnglish%7FENG_DS_MS5607-02BA03_B2.pdf%7FCAT-BLPS0035" H 1050 5350 50  0001 C CNN
	1    1050 5350
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
