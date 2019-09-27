

;*************************************************************************
;* A P P L I C A T I O N   N O T E   F O R   T H E   A V R   F A M I L Y
;* 
;* Number            : AVR000
;* File Name         : ATxmega128A1def.inc
;* Title             : Register/Bit Definitions for the ATxmega128A1
;* Date              : Jan 01 2008
;* Version           : 1.00
;* Support E-mail    : avr@atmel.com
;* Target MCU        : ATxmega128A1
;* 
;* DESCRIPTION
;* When including this file in the assembly program file, all I/O register 
;* names and I/O register bit names appearing in the data book can be used.
;* In addition, the six registers forming the three data pointers X, Y and 
;* Z have been assigned names XL - ZH. Highest RAM address for Internal 
;* SRAM is also defined 
;* 
;*************************************************************************

#ifndef _ATxmega128A1DEF_INC_
#define _ATxmega128A1DEF_INC_


#pragma partinc 0

; ***** SPECIFY DEVICE ***************************************************
.device ATxmega128A1


.equ	SIGNATURE_000 = 0x1E
.equ	SIGNATURE_001 = 0x97
.equ	SIGNATURE_002 = 0x4C

#pragma AVRPART ADMIN PART_NAME ATxmega128A1
#pragma AVRPART CORE CORE_VERSION V3X


; ***** ABSOLUTE I/O REGISTER LOCATIONS **********************************


;***************************************************************************
;** GPIO - General Purpose IO Registers
;***************************************************************************
	
.equ GPIO_GPIO0 = 0		// General Purpose IO Register 0
.equ GPIO_GPIO1 = 1		// General Purpose IO Register 1
.equ GPIO_GPIO2 = 2		// General Purpose IO Register 2
.equ GPIO_GPIO3 = 3		// General Purpose IO Register 3
.equ GPIO_GPIO4 = 4		// General Purpose IO Register 4
.equ GPIO_GPIO5 = 5		// General Purpose IO Register 5
.equ GPIO_GPIO6 = 6		// General Purpose IO Register 6
.equ GPIO_GPIO7 = 7		// General Purpose IO Register 7
.equ GPIO_GPIO8 = 8		// General Purpose IO Register 8
.equ GPIO_GPIO9 = 9		// General Purpose IO Register 9
.equ GPIO_GPIOA = 10		// General Purpose IO Register 10
.equ GPIO_GPIOB = 11		// General Purpose IO Register 11
.equ GPIO_GPIOC = 12		// General Purpose IO Register 12
.equ GPIO_GPIOD = 13		// General Purpose IO Register 13
.equ GPIO_GPIOE = 14		// General Purpose IO Register 14
.equ GPIO_GPIOF = 15		// General Purpose IO Register 15

;***************************************************************************
;** OCD - On-Chip Debug System
;***************************************************************************
	
.equ OCD_OCDR0 = 46		// OCD Register 0
.equ OCD_OCDR1 = 47		// OCD Register 1

;***************************************************************************
;** CPU - CPU Registers
;***************************************************************************
	
.equ CPU_CCP = 52		// Configuration Change Protection
.equ CPU_RAMPD = 56		// Ramp D
.equ CPU_RAMPX = 57		// Ramp X
.equ CPU_RAMPY = 58		// Ramp Y
.equ CPU_RAMPZ = 59		// Ramp Z
.equ CPU_EIND = 60		// Extended Indirect Jump
.equ CPU_SPL = 61		// Stack Pointer Low
.equ CPU_SPH = 62		// Stack Pointer High
.equ CPU_SREG = 63		// Status Register

;***************************************************************************
;** CLK - Clock System
;***************************************************************************
	
.equ CLK_CTRL = 64		// Control Register
.equ CLK_PSCTRL = 65		// Prescaler Control Register
.equ CLK_LOCK = 66		// Lock register
.equ CLK_RTCCTRL = 67		// RTC Control Register

;***************************************************************************
;** SLEEP - Sleep Controller
;***************************************************************************
	
.equ SLEEP_CTRL = 72		// Control Register

;***************************************************************************
;** OSC - Oscillator Control
;***************************************************************************
	
.equ OSC_CTRL = 80		// Control Register
.equ OSC_STATUS = 81		// Status Register
.equ OSC_XOSCCTRL = 82		// External Oscillator Control Register
.equ OSC_XOSCFAIL = 83		// External Oscillator Failure Detection Register
.equ OSC_RC32KCAL = 84		// 32kHz Internal Oscillator Calibration Register
.equ OSC_PLLCTRL = 85		// PLL Control REgister
.equ OSC_DFLLCTRL = 86		// DFLL Control Register

;***************************************************************************
;** DFLLRC32M - DFLL for 32MHz RC Oscillator
;***************************************************************************
	
.equ DFLLRC32M_CTRL = 96		// Control Register
.equ DFLLRC32M_CALA = 98		// Calibration Register A
.equ DFLLRC32M_CALB = 99		// Calibration Register B
.equ DFLLRC32M_OSCCNT0 = 100		// Oscillator Counter Register 0
.equ DFLLRC32M_OSCCNT1 = 101		// Oscillator Counter Register 1
.equ DFLLRC32M_OSCCNT2 = 102		// Oscillator Counter Register 2

;***************************************************************************
;** DFLLRC2M - DFLL for 2MHz RC Oscillator
;***************************************************************************
	
.equ DFLLRC2M_CTRL = 104		// Control Register
.equ DFLLRC2M_CALA = 106		// Calibration Register A
.equ DFLLRC2M_CALB = 107		// Calibration Register B
.equ DFLLRC2M_OSCCNT0 = 108		// Oscillator Counter Register 0
.equ DFLLRC2M_OSCCNT1 = 109		// Oscillator Counter Register 1
.equ DFLLRC2M_OSCCNT2 = 110		// Oscillator Counter Register 2

;***************************************************************************
;** PR - Power Reduction
;***************************************************************************
	
.equ PR_PR = 112		// General Power Reduction
.equ PR_PRPA = 113		// Power Reduction Port A
.equ PR_PRPB = 114		// Power Reduction Port B
.equ PR_PRPC = 115		// Power Reduction Port C
.equ PR_PRPD = 116		// Power Reduction Port D
.equ PR_PRPE = 117		// Power Reduction Port E
.equ PR_PRPF = 118		// Power Reduction Port F

;***************************************************************************
;** RST - Reset Controller
;***************************************************************************
	
.equ RST_STATUS = 120		// Status Register
.equ RST_CTRL = 121		// Control Register

;***************************************************************************
;** WDT - Watch-Dog Timer
;***************************************************************************
	
.equ WDT_CTRL = 128		// Control
.equ WDT_WINCTRL = 129		// Windowed Mode Control
.equ WDT_STATUS = 130		// Status

;***************************************************************************
;** MCU - MCU Control
;***************************************************************************
	
.equ MCU_DEVID0 = 144		// Device ID byte 0
.equ MCU_DEVID1 = 145		// Device ID byte 1
.equ MCU_DEVID2 = 146		// Device ID byte 2
.equ MCU_REVID = 147		// Revision ID
.equ MCU_JTAGUID = 148		// JTAG User ID
.equ MCU_MCUCR = 150		// MCU Control
.equ MCU_EVSYSLOCK = 152		// Event System Lock
.equ MCU_AWEXLOCK = 153		// AWEX Lock

;***************************************************************************
;** PMIC - Programmable Interrupt Controller
;***************************************************************************
	
.equ PMIC_STATUS = 160		// Status Register
.equ PMIC_INTPRI = 161		// Interrupt Priority
.equ PMIC_CTRL = 162		// Control Register

;***************************************************************************
;** DMA - DMA Controller
;***************************************************************************
	
.equ DMA_CTRL = 256		// Control
.equ DMA_INTFLAGS = 259		// Transfer Interrupt Status
.equ DMA_STATUS = 260		// Status
.equ DMA_TEMP = 262		// Temporary Register For 16/24-bit Access
.equ DMA_CH0_CTRLA = 272		// Channel Control
.equ DMA_CH0_CTRLB = 273		// Channel Control
.equ DMA_CH0_ADDRCTRL = 274		// Address Control
.equ DMA_CH0_TRIGSRC = 275		// Channel Trigger Source
.equ DMA_CH0_TRFCNT = 276		// Channel Block Transfer Count
.equ DMA_CH0_REPCNT = 278		// Channel Repeat Count
.equ DMA_CH0_SRCADDR0 = 280		// Channel Source Address 0
.equ DMA_CH0_SRCADDR1 = 281		// Channel Source Address 1
.equ DMA_CH0_SRCADDR2 = 282		// Channel Source Address 2
.equ DMA_CH0_DESTADDR0 = 284		// Channel Destination Address 0
.equ DMA_CH0_DESTADDR1 = 285		// Channel Destination Address 1
.equ DMA_CH0_DESTADDR2 = 286		// Channel Destination Address 2
.equ DMA_CH1_CTRLA = 288		// Channel Control
.equ DMA_CH1_CTRLB = 289		// Channel Control
.equ DMA_CH1_ADDRCTRL = 290		// Address Control
.equ DMA_CH1_TRIGSRC = 291		// Channel Trigger Source
.equ DMA_CH1_TRFCNT = 292		// Channel Block Transfer Count
.equ DMA_CH1_REPCNT = 294		// Channel Repeat Count
.equ DMA_CH1_SRCADDR0 = 296		// Channel Source Address 0
.equ DMA_CH1_SRCADDR1 = 297		// Channel Source Address 1
.equ DMA_CH1_SRCADDR2 = 298		// Channel Source Address 2
.equ DMA_CH1_DESTADDR0 = 300		// Channel Destination Address 0
.equ DMA_CH1_DESTADDR1 = 301		// Channel Destination Address 1
.equ DMA_CH1_DESTADDR2 = 302		// Channel Destination Address 2
.equ DMA_CH2_CTRLA = 304		// Channel Control
.equ DMA_CH2_CTRLB = 305		// Channel Control
.equ DMA_CH2_ADDRCTRL = 306		// Address Control
.equ DMA_CH2_TRIGSRC = 307		// Channel Trigger Source
.equ DMA_CH2_TRFCNT = 308		// Channel Block Transfer Count
.equ DMA_CH2_REPCNT = 310		// Channel Repeat Count
.equ DMA_CH2_SRCADDR0 = 312		// Channel Source Address 0
.equ DMA_CH2_SRCADDR1 = 313		// Channel Source Address 1
.equ DMA_CH2_SRCADDR2 = 314		// Channel Source Address 2
.equ DMA_CH2_DESTADDR0 = 316		// Channel Destination Address 0
.equ DMA_CH2_DESTADDR1 = 317		// Channel Destination Address 1
.equ DMA_CH2_DESTADDR2 = 318		// Channel Destination Address 2
.equ DMA_CH3_CTRLA = 320		// Channel Control
.equ DMA_CH3_CTRLB = 321		// Channel Control
.equ DMA_CH3_ADDRCTRL = 322		// Address Control
.equ DMA_CH3_TRIGSRC = 323		// Channel Trigger Source
.equ DMA_CH3_TRFCNT = 324		// Channel Block Transfer Count
.equ DMA_CH3_REPCNT = 326		// Channel Repeat Count
.equ DMA_CH3_SRCADDR0 = 328		// Channel Source Address 0
.equ DMA_CH3_SRCADDR1 = 329		// Channel Source Address 1
.equ DMA_CH3_SRCADDR2 = 330		// Channel Source Address 2
.equ DMA_CH3_DESTADDR0 = 332		// Channel Destination Address 0
.equ DMA_CH3_DESTADDR1 = 333		// Channel Destination Address 1
.equ DMA_CH3_DESTADDR2 = 334		// Channel Destination Address 2

;***************************************************************************
;** EVSYS - Event System
;***************************************************************************
	
.equ EVSYS_CH0MUX = 384		// Event Channel 0 Multiplexer
.equ EVSYS_CH1MUX = 385		// Event Channel 1 Multiplexer
.equ EVSYS_CH2MUX = 386		// Event Channel 2 Multiplexer
.equ EVSYS_CH3MUX = 387		// Event Channel 3 Multiplexer
.equ EVSYS_CH4MUX = 388		// Event Channel 4 Multiplexer
.equ EVSYS_CH5MUX = 389		// Event Channel 5 Multiplexer
.equ EVSYS_CH6MUX = 390		// Event Channel 6 Multiplexer
.equ EVSYS_CH7MUX = 391		// Event Channel 7 Multiplexer
.equ EVSYS_CH0CTRL = 392		// Channel 0 Control Register
.equ EVSYS_CH1CTRL = 393		// Channel 1 Control Register
.equ EVSYS_CH2CTRL = 394		// Channel 2 Control Register
.equ EVSYS_CH3CTRL = 395		// Channel 3 Control Register
.equ EVSYS_CH4CTRL = 396		// Channel 4 Control Register
.equ EVSYS_CH5CTRL = 397		// Channel 5 Control Register
.equ EVSYS_CH6CTRL = 398		// Channel 6 Control Register
.equ EVSYS_CH7CTRL = 399		// Channel 7 Control Register
.equ EVSYS_STROBE = 400		// Event Strobe
.equ EVSYS_DATA = 401		// Event Data

;***************************************************************************
;** NVM - Non Volatile Memory
;***************************************************************************
	
.equ NVM_ADDR0 = 448		// Address Register 0
.equ NVM_ADDR1 = 449		// Address Register 1
.equ NVM_ADDR2 = 450		// Address Register 2
.equ NVM_DATA0 = 452		// Data Register 0
.equ NVM_DATA1 = 453		// Data Register 1
.equ NVM_DATA2 = 454		// Data Register 2
.equ NVM_CMD = 458		// Command
.equ NVM_CTRLA = 459		// Control Register A
.equ NVM_CTRLB = 460		// Control Register B
.equ NVM_INTCTRL = 461		// Interrupt Control
.equ NVM_STATUS = 463		// Status
.equ NVM_LOCKBITS = 464		// Lock Bits

;***************************************************************************
;** ACA - Analog Comparator A
;***************************************************************************
	
.equ ACA_AC0CTRL = 896		// Comparator 0 Control
.equ ACA_AC1CTRL = 897		// Comparator 1 Control
.equ ACA_AC0MUXCTRL = 898		// Comparator 0 MUX Control
.equ ACA_AC1MUXCTRL = 899		// Comparator 1 MUX Control
.equ ACA_CTRLA = 900		// Control Register A
.equ ACA_CTRLB = 901		// Control Register B
.equ ACA_WINCTRL = 902		// Window Mode Control
.equ ACA_STATUS = 903		// Status

;***************************************************************************
;** ACB - Analog Comparator B
;***************************************************************************
	
.equ ACB_AC0CTRL = 912		// Comparator 0 Control
.equ ACB_AC1CTRL = 913		// Comparator 1 Control
.equ ACB_AC0MUXCTRL = 914		// Comparator 0 MUX Control
.equ ACB_AC1MUXCTRL = 915		// Comparator 1 MUX Control
.equ ACB_CTRLA = 916		// Control Register A
.equ ACB_CTRLB = 917		// Control Register B
.equ ACB_WINCTRL = 918		// Window Mode Control
.equ ACB_STATUS = 919		// Status

;***************************************************************************
;** ADCA - Analog to Digital Converter A
;***************************************************************************
	
.equ ADCA_CTRLA = 512		// Control Register A
.equ ADCA_CTRLB = 513		// Control Register B
.equ ADCA_REFCTRL = 514		// Reference Control
.equ ADCA_EVCTRL = 515		// Event Control
.equ ADCA_PRESCALER = 516		// Clock Prescaler
.equ ADCA_CALCTRL = 517		// Calibration Control Register
.equ ADCA_INTFLAGS = 518		// Interrupt Flags
.equ ADCA_CALIB = 524		// Calibration Value
.equ ADCA_CH0RES = 528		// Channel 0 Result
.equ ADCA_CH1RES = 530		// Channel 1 Result
.equ ADCA_CH2RES = 532		// Channel 2 Result
.equ ADCA_CH3RES = 534		// Channel 3 Result
.equ ADCA_CMP = 536		// Compare Value
.equ ADCA_CH0_CTRL = 544		// Control Register
.equ ADCA_CH0_MUXCTRL = 545		// MUX Control
.equ ADCA_CH0_INTCTRL = 546		// Channel Interrupt Control
.equ ADCA_CH0_INTFLAGS = 547		// Interrupt Flags
.equ ADCA_CH0_RES = 548		// Channel Result
.equ ADCA_CH1_CTRL = 552		// Control Register
.equ ADCA_CH1_MUXCTRL = 553		// MUX Control
.equ ADCA_CH1_INTCTRL = 554		// Channel Interrupt Control
.equ ADCA_CH1_INTFLAGS = 555		// Interrupt Flags
.equ ADCA_CH1_RES = 556		// Channel Result
.equ ADCA_CH2_CTRL = 560		// Control Register
.equ ADCA_CH2_MUXCTRL = 561		// MUX Control
.equ ADCA_CH2_INTCTRL = 562		// Channel Interrupt Control
.equ ADCA_CH2_INTFLAGS = 563		// Interrupt Flags
.equ ADCA_CH2_RES = 564		// Channel Result
.equ ADCA_CH3_CTRL = 568		// Control Register
.equ ADCA_CH3_MUXCTRL = 569		// MUX Control
.equ ADCA_CH3_INTCTRL = 570		// Channel Interrupt Control
.equ ADCA_CH3_INTFLAGS = 571		// Interrupt Flags
.equ ADCA_CH3_RES = 572		// Channel Result

;***************************************************************************
;** ADCB - Analog to Digital Converter B
;***************************************************************************
	
.equ ADCB_CTRLA = 576		// Control Register A
.equ ADCB_CTRLB = 577		// Control Register B
.equ ADCB_REFCTRL = 578		// Reference Control
.equ ADCB_EVCTRL = 579		// Event Control
.equ ADCB_PRESCALER = 580		// Clock Prescaler
.equ ADCB_CALCTRL = 581		// Calibration Control Register
.equ ADCB_INTFLAGS = 582		// Interrupt Flags
.equ ADCB_CALIB = 588		// Calibration Value
.equ ADCB_CH0RES = 592		// Channel 0 Result
.equ ADCB_CH1RES = 594		// Channel 1 Result
.equ ADCB_CH2RES = 596		// Channel 2 Result
.equ ADCB_CH3RES = 598		// Channel 3 Result
.equ ADCB_CMP = 600		// Compare Value
.equ ADCB_CH0_CTRL = 608		// Control Register
.equ ADCB_CH0_MUXCTRL = 609		// MUX Control
.equ ADCB_CH0_INTCTRL = 610		// Channel Interrupt Control
.equ ADCB_CH0_INTFLAGS = 611		// Interrupt Flags
.equ ADCB_CH0_RES = 612		// Channel Result
.equ ADCB_CH1_CTRL = 616		// Control Register
.equ ADCB_CH1_MUXCTRL = 617		// MUX Control
.equ ADCB_CH1_INTCTRL = 618		// Channel Interrupt Control
.equ ADCB_CH1_INTFLAGS = 619		// Interrupt Flags
.equ ADCB_CH1_RES = 620		// Channel Result
.equ ADCB_CH2_CTRL = 624		// Control Register
.equ ADCB_CH2_MUXCTRL = 625		// MUX Control
.equ ADCB_CH2_INTCTRL = 626		// Channel Interrupt Control
.equ ADCB_CH2_INTFLAGS = 627		// Interrupt Flags
.equ ADCB_CH2_RES = 628		// Channel Result
.equ ADCB_CH3_CTRL = 632		// Control Register
.equ ADCB_CH3_MUXCTRL = 633		// MUX Control
.equ ADCB_CH3_INTCTRL = 634		// Channel Interrupt Control
.equ ADCB_CH3_INTFLAGS = 635		// Interrupt Flags
.equ ADCB_CH3_RES = 636		// Channel Result

;***************************************************************************
;** DACA - Digital to Analog Converter A
;***************************************************************************
	
.equ DACA_CTRLA = 768		// Control Register A
.equ DACA_CTRLB = 769		// Control Register B
.equ DACA_CTRLC = 770		// Control Register C
.equ DACA_EVCTRL = 771		// Event Input Control
.equ DACA_TIMCTRL = 772		// Timing Control
.equ DACA_STATUS = 773		// Status
.equ DACA_GAINCAL = 776		// Gain Calibration
.equ DACA_OFFSETCAL = 777		// Offset Calibration
.equ DACA_CH0DATA = 792		// Channel 0 Data
.equ DACA_CH1DATA = 794		// Channel 1 Data

;***************************************************************************
;** DACB - Digital to Analog Converter B
;***************************************************************************
	
.equ DACB_CTRLA = 800		// Control Register A
.equ DACB_CTRLB = 801		// Control Register B
.equ DACB_CTRLC = 802		// Control Register C
.equ DACB_EVCTRL = 803		// Event Input Control
.equ DACB_TIMCTRL = 804		// Timing Control
.equ DACB_STATUS = 805		// Status
.equ DACB_GAINCAL = 808		// Gain Calibration
.equ DACB_OFFSETCAL = 809		// Offset Calibration
.equ DACB_CH0DATA = 824		// Channel 0 Data
.equ DACB_CH1DATA = 826		// Channel 1 Data

;***************************************************************************
;** RTC - Real-Time Counter
;***************************************************************************
	
.equ RTC_CTRL = 1024		// Control Register
.equ RTC_STATUS = 1025		// Status Register
.equ RTC_INTCTRL = 1026		// Interrupt Control Register
.equ RTC_INTFLAGS = 1027		// Interrupt Flags
.equ RTC_TEMP = 1028		// Temporary register
.equ RTC_CNT = 1032		// Count Register
.equ RTC_PER = 1034		// Period Register
.equ RTC_COMP = 1036		// Compare Register

;***************************************************************************
;** EBI - External Bus Interface
;***************************************************************************
	
.equ EBI_CTRL = 1088		// Control
.equ EBI_SDRAMCTRLA = 1089		// SDRAM Control Register A
.equ EBI_REFRESH = 1092		// SDRAM Refresh Period
.equ EBI_INITDLY = 1094		// SDRAM Initialization Delay
.equ EBI_SDRAMCTRLB = 1096		// SDRAM Control Register B
.equ EBI_SDRAMCTRLC = 1097		// SDRAM Control Register C
.equ EBI_CS0_CTRLA = 1104		// Chip Select Control Register A
.equ EBI_CS0_CTRLB = 1105		// Chip Select Control Register B
.equ EBI_CS0_BASEADDR = 1106		// Chip Select Base Address
.equ EBI_CS1_CTRLA = 1108		// Chip Select Control Register A
.equ EBI_CS1_CTRLB = 1109		// Chip Select Control Register B
.equ EBI_CS1_BASEADDR = 1110		// Chip Select Base Address
.equ EBI_CS2_CTRLA = 1112		// Chip Select Control Register A
.equ EBI_CS2_CTRLB = 1113		// Chip Select Control Register B
.equ EBI_CS2_BASEADDR = 1114		// Chip Select Base Address
.equ EBI_CS3_CTRLA = 1116		// Chip Select Control Register A
.equ EBI_CS3_CTRLB = 1117		// Chip Select Control Register B
.equ EBI_CS3_BASEADDR = 1118		// Chip Select Base Address

;***************************************************************************
;** TWIC - Two-Wire Interface C
;***************************************************************************
	
.equ TWIC_CTRL = 1152		// TWI Common Control Register
.equ TWIC_MASTER_CTRLA = 1153		// Control Register A
.equ TWIC_MASTER_CTRLB = 1154		// Control Register B
.equ TWIC_MASTER_CTRLC = 1155		// Control Register C
.equ TWIC_MASTER_STATUS = 1156		// Status Register
.equ TWIC_MASTER_BAUD = 1157		// Baurd Rate Control Register
.equ TWIC_MASTER_ADDR = 1158		// Address Register
.equ TWIC_MASTER_DATA = 1159		// Data Register
.equ TWIC_SLAVE_CTRLA = 1160		// Control Register A
.equ TWIC_SLAVE_CTRLB = 1161		// Control Register B
.equ TWIC_SLAVE_STATUS = 1162		// Status Register
.equ TWIC_SLAVE_ADDR = 1163		// Address Register
.equ TWIC_SLAVE_DATA = 1164		// Data Register

;***************************************************************************
;** TWID - Two-Wire Interface D
;***************************************************************************
	
.equ TWID_CTRL = 1168		// TWI Common Control Register
.equ TWID_MASTER_CTRLA = 1169		// Control Register A
.equ TWID_MASTER_CTRLB = 1170		// Control Register B
.equ TWID_MASTER_CTRLC = 1171		// Control Register C
.equ TWID_MASTER_STATUS = 1172		// Status Register
.equ TWID_MASTER_BAUD = 1173		// Baurd Rate Control Register
.equ TWID_MASTER_ADDR = 1174		// Address Register
.equ TWID_MASTER_DATA = 1175		// Data Register
.equ TWID_SLAVE_CTRLA = 1176		// Control Register A
.equ TWID_SLAVE_CTRLB = 1177		// Control Register B
.equ TWID_SLAVE_STATUS = 1178		// Status Register
.equ TWID_SLAVE_ADDR = 1179		// Address Register
.equ TWID_SLAVE_DATA = 1180		// Data Register

;***************************************************************************
;** TWIE - Two-Wire Interface E
;***************************************************************************
	
.equ TWIE_CTRL = 1184		// TWI Common Control Register
.equ TWIE_MASTER_CTRLA = 1185		// Control Register A
.equ TWIE_MASTER_CTRLB = 1186		// Control Register B
.equ TWIE_MASTER_CTRLC = 1187		// Control Register C
.equ TWIE_MASTER_STATUS = 1188		// Status Register
.equ TWIE_MASTER_BAUD = 1189		// Baurd Rate Control Register
.equ TWIE_MASTER_ADDR = 1190		// Address Register
.equ TWIE_MASTER_DATA = 1191		// Data Register
.equ TWIE_SLAVE_CTRLA = 1192		// Control Register A
.equ TWIE_SLAVE_CTRLB = 1193		// Control Register B
.equ TWIE_SLAVE_STATUS = 1194		// Status Register
.equ TWIE_SLAVE_ADDR = 1195		// Address Register
.equ TWIE_SLAVE_DATA = 1196		// Data Register

;***************************************************************************
;** TWIF - Two-Wire Interface F
;***************************************************************************
	
.equ TWIF_CTRL = 1200		// TWI Common Control Register
.equ TWIF_MASTER_CTRLA = 1201		// Control Register A
.equ TWIF_MASTER_CTRLB = 1202		// Control Register B
.equ TWIF_MASTER_CTRLC = 1203		// Control Register C
.equ TWIF_MASTER_STATUS = 1204		// Status Register
.equ TWIF_MASTER_BAUD = 1205		// Baurd Rate Control Register
.equ TWIF_MASTER_ADDR = 1206		// Address Register
.equ TWIF_MASTER_DATA = 1207		// Data Register
.equ TWIF_SLAVE_CTRLA = 1208		// Control Register A
.equ TWIF_SLAVE_CTRLB = 1209		// Control Register B
.equ TWIF_SLAVE_STATUS = 1210		// Status Register
.equ TWIF_SLAVE_ADDR = 1211		// Address Register
.equ TWIF_SLAVE_DATA = 1212		// Data Register

;***************************************************************************
;** PORT_CFG - Port Configuration
;***************************************************************************
	
.equ PORTCFG_MPCMASK = 176		// Multi-pin Configuration Mask
.equ PORTCFG_VPCTRLA = 178		// Virtual Port Control Register A
.equ PORTCFG_VPCTRLB = 179		// Virtual Port Control Register B
.equ PORTCFG_CLKEVOUT = 180		// Clock and Event Out Register

;***************************************************************************
;** VPORT0 - Virtual Port 0
;***************************************************************************
	
.equ VPORT0_DIR = 16		// I/O Port Data Direction
.equ VPORT0_OUT = 17		// I/O Port Output
.equ VPORT0_IN = 18		// I/O Port Input
.equ VPORT0_INTFLAGS = 19		// Interrupt Flag Register

;***************************************************************************
;** VPORT1 - Virtual Port 1
;***************************************************************************
	
.equ VPORT1_DIR = 20		// I/O Port Data Direction
.equ VPORT1_OUT = 21		// I/O Port Output
.equ VPORT1_IN = 22		// I/O Port Input
.equ VPORT1_INTFLAGS = 23		// Interrupt Flag Register

;***************************************************************************
;** VPORT2 - Virtual Port 2
;***************************************************************************
	
.equ VPORT2_DIR = 24		// I/O Port Data Direction
.equ VPORT2_OUT = 25		// I/O Port Output
.equ VPORT2_IN = 26		// I/O Port Input
.equ VPORT2_INTFLAGS = 27		// Interrupt Flag Register

;***************************************************************************
;** VPORT3 - Virtual Port 3
;***************************************************************************
	
.equ VPORT3_DIR = 28		// I/O Port Data Direction
.equ VPORT3_OUT = 29		// I/O Port Output
.equ VPORT3_IN = 30		// I/O Port Input
.equ VPORT3_INTFLAGS = 31		// Interrupt Flag Register

;***************************************************************************
;** PORTA - Port A
;***************************************************************************
	
.equ PORTA_DIR = 1536		// I/O Port Data Direction
.equ PORTA_DIRSET = 1537		// I/O Port Data Direction Set
.equ PORTA_DIRCLR = 1538		// I/O Port Data Direction Clear
.equ PORTA_DIRTGL = 1539		// I/O Port Data Direction Toggle
.equ PORTA_OUT = 1540		// I/O Port Output
.equ PORTA_OUTSET = 1541		// I/O Port Output Set
.equ PORTA_OUTCLR = 1542		// I/O Port Output Clear
.equ PORTA_OUTTGL = 1543		// I/O Port Output Toggle
.equ PORTA_IN = 1544		// I/O port Input
.equ PORTA_INTCTRL = 1545		// Interrupt Control Register
.equ PORTA_INT0MASK = 1546		// Port Interrupt 0 Mask
.equ PORTA_INT1MASK = 1547		// Port Interrupt 1 Mask
.equ PORTA_INTFLAGS = 1548		// Interrupt Flag Register
.equ PORTA_PIN0CTRL = 1552		// Pin 0 Control Register
.equ PORTA_PIN1CTRL = 1553		// Pin 1 Control Register
.equ PORTA_PIN2CTRL = 1554		// Pin 2 Control Register
.equ PORTA_PIN3CTRL = 1555		// Pin 3 Control Register
.equ PORTA_PIN4CTRL = 1556		// Pin 4 Control Register
.equ PORTA_PIN5CTRL = 1557		// Pin 5 Control Register
.equ PORTA_PIN6CTRL = 1558		// Pin 6 Control Register
.equ PORTA_PIN7CTRL = 1559		// Pin 7 Control Register

;***************************************************************************
;** PORTB - Port B
;***************************************************************************
	
.equ PORTB_DIR = 1568		// I/O Port Data Direction
.equ PORTB_DIRSET = 1569		// I/O Port Data Direction Set
.equ PORTB_DIRCLR = 1570		// I/O Port Data Direction Clear
.equ PORTB_DIRTGL = 1571		// I/O Port Data Direction Toggle
.equ PORTB_OUT = 1572		// I/O Port Output
.equ PORTB_OUTSET = 1573		// I/O Port Output Set
.equ PORTB_OUTCLR = 1574		// I/O Port Output Clear
.equ PORTB_OUTTGL = 1575		// I/O Port Output Toggle
.equ PORTB_IN = 1576		// I/O port Input
.equ PORTB_INTCTRL = 1577		// Interrupt Control Register
.equ PORTB_INT0MASK = 1578		// Port Interrupt 0 Mask
.equ PORTB_INT1MASK = 1579		// Port Interrupt 1 Mask
.equ PORTB_INTFLAGS = 1580		// Interrupt Flag Register
.equ PORTB_PIN0CTRL = 1584		// Pin 0 Control Register
.equ PORTB_PIN1CTRL = 1585		// Pin 1 Control Register
.equ PORTB_PIN2CTRL = 1586		// Pin 2 Control Register
.equ PORTB_PIN3CTRL = 1587		// Pin 3 Control Register
.equ PORTB_PIN4CTRL = 1588		// Pin 4 Control Register
.equ PORTB_PIN5CTRL = 1589		// Pin 5 Control Register
.equ PORTB_PIN6CTRL = 1590		// Pin 6 Control Register
.equ PORTB_PIN7CTRL = 1591		// Pin 7 Control Register

;***************************************************************************
;** PORTC - Port C
;***************************************************************************
	
.equ PORTC_DIR = 1600		// I/O Port Data Direction
.equ PORTC_DIRSET = 1601		// I/O Port Data Direction Set
.equ PORTC_DIRCLR = 1602		// I/O Port Data Direction Clear
.equ PORTC_DIRTGL = 1603		// I/O Port Data Direction Toggle
.equ PORTC_OUT = 1604		// I/O Port Output
.equ PORTC_OUTSET = 1605		// I/O Port Output Set
.equ PORTC_OUTCLR = 1606		// I/O Port Output Clear
.equ PORTC_OUTTGL = 1607		// I/O Port Output Toggle
.equ PORTC_IN = 1608		// I/O port Input
.equ PORTC_INTCTRL = 1609		// Interrupt Control Register
.equ PORTC_INT0MASK = 1610		// Port Interrupt 0 Mask
.equ PORTC_INT1MASK = 1611		// Port Interrupt 1 Mask
.equ PORTC_INTFLAGS = 1612		// Interrupt Flag Register
.equ PORTC_PIN0CTRL = 1616		// Pin 0 Control Register
.equ PORTC_PIN1CTRL = 1617		// Pin 1 Control Register
.equ PORTC_PIN2CTRL = 1618		// Pin 2 Control Register
.equ PORTC_PIN3CTRL = 1619		// Pin 3 Control Register
.equ PORTC_PIN4CTRL = 1620		// Pin 4 Control Register
.equ PORTC_PIN5CTRL = 1621		// Pin 5 Control Register
.equ PORTC_PIN6CTRL = 1622		// Pin 6 Control Register
.equ PORTC_PIN7CTRL = 1623		// Pin 7 Control Register

;***************************************************************************
;** PORTD - Port D
;***************************************************************************
	
.equ PORTD_DIR = 1632		// I/O Port Data Direction
.equ PORTD_DIRSET = 1633		// I/O Port Data Direction Set
.equ PORTD_DIRCLR = 1634		// I/O Port Data Direction Clear
.equ PORTD_DIRTGL = 1635		// I/O Port Data Direction Toggle
.equ PORTD_OUT = 1636		// I/O Port Output
.equ PORTD_OUTSET = 1637		// I/O Port Output Set
.equ PORTD_OUTCLR = 1638		// I/O Port Output Clear
.equ PORTD_OUTTGL = 1639		// I/O Port Output Toggle
.equ PORTD_IN = 1640		// I/O port Input
.equ PORTD_INTCTRL = 1641		// Interrupt Control Register
.equ PORTD_INT0MASK = 1642		// Port Interrupt 0 Mask
.equ PORTD_INT1MASK = 1643		// Port Interrupt 1 Mask
.equ PORTD_INTFLAGS = 1644		// Interrupt Flag Register
.equ PORTD_PIN0CTRL = 1648		// Pin 0 Control Register
.equ PORTD_PIN1CTRL = 1649		// Pin 1 Control Register
.equ PORTD_PIN2CTRL = 1650		// Pin 2 Control Register
.equ PORTD_PIN3CTRL = 1651		// Pin 3 Control Register
.equ PORTD_PIN4CTRL = 1652		// Pin 4 Control Register
.equ PORTD_PIN5CTRL = 1653		// Pin 5 Control Register
.equ PORTD_PIN6CTRL = 1654		// Pin 6 Control Register
.equ PORTD_PIN7CTRL = 1655		// Pin 7 Control Register

;***************************************************************************
;** PORTE - Port E
;***************************************************************************
	
.equ PORTE_DIR = 1664		// I/O Port Data Direction
.equ PORTE_DIRSET = 1665		// I/O Port Data Direction Set
.equ PORTE_DIRCLR = 1666		// I/O Port Data Direction Clear
.equ PORTE_DIRTGL = 1667		// I/O Port Data Direction Toggle
.equ PORTE_OUT = 1668		// I/O Port Output
.equ PORTE_OUTSET = 1669		// I/O Port Output Set
.equ PORTE_OUTCLR = 1670		// I/O Port Output Clear
.equ PORTE_OUTTGL = 1671		// I/O Port Output Toggle
.equ PORTE_IN = 1672		// I/O port Input
.equ PORTE_INTCTRL = 1673		// Interrupt Control Register
.equ PORTE_INT0MASK = 1674		// Port Interrupt 0 Mask
.equ PORTE_INT1MASK = 1675		// Port Interrupt 1 Mask
.equ PORTE_INTFLAGS = 1676		// Interrupt Flag Register
.equ PORTE_PIN0CTRL = 1680		// Pin 0 Control Register
.equ PORTE_PIN1CTRL = 1681		// Pin 1 Control Register
.equ PORTE_PIN2CTRL = 1682		// Pin 2 Control Register
.equ PORTE_PIN3CTRL = 1683		// Pin 3 Control Register
.equ PORTE_PIN4CTRL = 1684		// Pin 4 Control Register
.equ PORTE_PIN5CTRL = 1685		// Pin 5 Control Register
.equ PORTE_PIN6CTRL = 1686		// Pin 6 Control Register
.equ PORTE_PIN7CTRL = 1687		// Pin 7 Control Register

;***************************************************************************
;** PORTF - Port F
;***************************************************************************
	
.equ PORTF_DIR = 1696		// I/O Port Data Direction
.equ PORTF_DIRSET = 1697		// I/O Port Data Direction Set
.equ PORTF_DIRCLR = 1698		// I/O Port Data Direction Clear
.equ PORTF_DIRTGL = 1699		// I/O Port Data Direction Toggle
.equ PORTF_OUT = 1700		// I/O Port Output
.equ PORTF_OUTSET = 1701		// I/O Port Output Set
.equ PORTF_OUTCLR = 1702		// I/O Port Output Clear
.equ PORTF_OUTTGL = 1703		// I/O Port Output Toggle
.equ PORTF_IN = 1704		// I/O port Input
.equ PORTF_INTCTRL = 1705		// Interrupt Control Register
.equ PORTF_INT0MASK = 1706		// Port Interrupt 0 Mask
.equ PORTF_INT1MASK = 1707		// Port Interrupt 1 Mask
.equ PORTF_INTFLAGS = 1708		// Interrupt Flag Register
.equ PORTF_PIN0CTRL = 1712		// Pin 0 Control Register
.equ PORTF_PIN1CTRL = 1713		// Pin 1 Control Register
.equ PORTF_PIN2CTRL = 1714		// Pin 2 Control Register
.equ PORTF_PIN3CTRL = 1715		// Pin 3 Control Register
.equ PORTF_PIN4CTRL = 1716		// Pin 4 Control Register
.equ PORTF_PIN5CTRL = 1717		// Pin 5 Control Register
.equ PORTF_PIN6CTRL = 1718		// Pin 6 Control Register
.equ PORTF_PIN7CTRL = 1719		// Pin 7 Control Register

;***************************************************************************
;** PORTH - Port H
;***************************************************************************
	
.equ PORTH_DIR = 1760		// I/O Port Data Direction
.equ PORTH_DIRSET = 1761		// I/O Port Data Direction Set
.equ PORTH_DIRCLR = 1762		// I/O Port Data Direction Clear
.equ PORTH_DIRTGL = 1763		// I/O Port Data Direction Toggle
.equ PORTH_OUT = 1764		// I/O Port Output
.equ PORTH_OUTSET = 1765		// I/O Port Output Set
.equ PORTH_OUTCLR = 1766		// I/O Port Output Clear
.equ PORTH_OUTTGL = 1767		// I/O Port Output Toggle
.equ PORTH_IN = 1768		// I/O port Input
.equ PORTH_INTCTRL = 1769		// Interrupt Control Register
.equ PORTH_INT0MASK = 1770		// Port Interrupt 0 Mask
.equ PORTH_INT1MASK = 1771		// Port Interrupt 1 Mask
.equ PORTH_INTFLAGS = 1772		// Interrupt Flag Register
.equ PORTH_PIN0CTRL = 1776		// Pin 0 Control Register
.equ PORTH_PIN1CTRL = 1777		// Pin 1 Control Register
.equ PORTH_PIN2CTRL = 1778		// Pin 2 Control Register
.equ PORTH_PIN3CTRL = 1779		// Pin 3 Control Register
.equ PORTH_PIN4CTRL = 1780		// Pin 4 Control Register
.equ PORTH_PIN5CTRL = 1781		// Pin 5 Control Register
.equ PORTH_PIN6CTRL = 1782		// Pin 6 Control Register
.equ PORTH_PIN7CTRL = 1783		// Pin 7 Control Register

;***************************************************************************
;** PORTJ - Port J
;***************************************************************************
	
.equ PORTJ_DIR = 1792		// I/O Port Data Direction
.equ PORTJ_DIRSET = 1793		// I/O Port Data Direction Set
.equ PORTJ_DIRCLR = 1794		// I/O Port Data Direction Clear
.equ PORTJ_DIRTGL = 1795		// I/O Port Data Direction Toggle
.equ PORTJ_OUT = 1796		// I/O Port Output
.equ PORTJ_OUTSET = 1797		// I/O Port Output Set
.equ PORTJ_OUTCLR = 1798		// I/O Port Output Clear
.equ PORTJ_OUTTGL = 1799		// I/O Port Output Toggle
.equ PORTJ_IN = 1800		// I/O port Input
.equ PORTJ_INTCTRL = 1801		// Interrupt Control Register
.equ PORTJ_INT0MASK = 1802		// Port Interrupt 0 Mask
.equ PORTJ_INT1MASK = 1803		// Port Interrupt 1 Mask
.equ PORTJ_INTFLAGS = 1804		// Interrupt Flag Register
.equ PORTJ_PIN0CTRL = 1808		// Pin 0 Control Register
.equ PORTJ_PIN1CTRL = 1809		// Pin 1 Control Register
.equ PORTJ_PIN2CTRL = 1810		// Pin 2 Control Register
.equ PORTJ_PIN3CTRL = 1811		// Pin 3 Control Register
.equ PORTJ_PIN4CTRL = 1812		// Pin 4 Control Register
.equ PORTJ_PIN5CTRL = 1813		// Pin 5 Control Register
.equ PORTJ_PIN6CTRL = 1814		// Pin 6 Control Register
.equ PORTJ_PIN7CTRL = 1815		// Pin 7 Control Register

;***************************************************************************
;** PORTK - Port K
;***************************************************************************
	
.equ PORTK_DIR = 1824		// I/O Port Data Direction
.equ PORTK_DIRSET = 1825		// I/O Port Data Direction Set
.equ PORTK_DIRCLR = 1826		// I/O Port Data Direction Clear
.equ PORTK_DIRTGL = 1827		// I/O Port Data Direction Toggle
.equ PORTK_OUT = 1828		// I/O Port Output
.equ PORTK_OUTSET = 1829		// I/O Port Output Set
.equ PORTK_OUTCLR = 1830		// I/O Port Output Clear
.equ PORTK_OUTTGL = 1831		// I/O Port Output Toggle
.equ PORTK_IN = 1832		// I/O port Input
.equ PORTK_INTCTRL = 1833		// Interrupt Control Register
.equ PORTK_INT0MASK = 1834		// Port Interrupt 0 Mask
.equ PORTK_INT1MASK = 1835		// Port Interrupt 1 Mask
.equ PORTK_INTFLAGS = 1836		// Interrupt Flag Register
.equ PORTK_PIN0CTRL = 1840		// Pin 0 Control Register
.equ PORTK_PIN1CTRL = 1841		// Pin 1 Control Register
.equ PORTK_PIN2CTRL = 1842		// Pin 2 Control Register
.equ PORTK_PIN3CTRL = 1843		// Pin 3 Control Register
.equ PORTK_PIN4CTRL = 1844		// Pin 4 Control Register
.equ PORTK_PIN5CTRL = 1845		// Pin 5 Control Register
.equ PORTK_PIN6CTRL = 1846		// Pin 6 Control Register
.equ PORTK_PIN7CTRL = 1847		// Pin 7 Control Register

;***************************************************************************
;** PORTQ - Port Q
;***************************************************************************
	
.equ PORTQ_DIR = 1984		// I/O Port Data Direction
.equ PORTQ_DIRSET = 1985		// I/O Port Data Direction Set
.equ PORTQ_DIRCLR = 1986		// I/O Port Data Direction Clear
.equ PORTQ_DIRTGL = 1987		// I/O Port Data Direction Toggle
.equ PORTQ_OUT = 1988		// I/O Port Output
.equ PORTQ_OUTSET = 1989		// I/O Port Output Set
.equ PORTQ_OUTCLR = 1990		// I/O Port Output Clear
.equ PORTQ_OUTTGL = 1991		// I/O Port Output Toggle
.equ PORTQ_IN = 1992		// I/O port Input
.equ PORTQ_INTCTRL = 1993		// Interrupt Control Register
.equ PORTQ_INT0MASK = 1994		// Port Interrupt 0 Mask
.equ PORTQ_INT1MASK = 1995		// Port Interrupt 1 Mask
.equ PORTQ_INTFLAGS = 1996		// Interrupt Flag Register
.equ PORTQ_PIN0CTRL = 2000		// Pin 0 Control Register
.equ PORTQ_PIN1CTRL = 2001		// Pin 1 Control Register
.equ PORTQ_PIN2CTRL = 2002		// Pin 2 Control Register
.equ PORTQ_PIN3CTRL = 2003		// Pin 3 Control Register
.equ PORTQ_PIN4CTRL = 2004		// Pin 4 Control Register
.equ PORTQ_PIN5CTRL = 2005		// Pin 5 Control Register
.equ PORTQ_PIN6CTRL = 2006		// Pin 6 Control Register
.equ PORTQ_PIN7CTRL = 2007		// Pin 7 Control Register

;***************************************************************************
;** PORTR - Port R
;***************************************************************************
	
.equ PORTR_DIR = 2016		// I/O Port Data Direction
.equ PORTR_DIRSET = 2017		// I/O Port Data Direction Set
.equ PORTR_DIRCLR = 2018		// I/O Port Data Direction Clear
.equ PORTR_DIRTGL = 2019		// I/O Port Data Direction Toggle
.equ PORTR_OUT = 2020		// I/O Port Output
.equ PORTR_OUTSET = 2021		// I/O Port Output Set
.equ PORTR_OUTCLR = 2022		// I/O Port Output Clear
.equ PORTR_OUTTGL = 2023		// I/O Port Output Toggle
.equ PORTR_IN = 2024		// I/O port Input
.equ PORTR_INTCTRL = 2025		// Interrupt Control Register
.equ PORTR_INT0MASK = 2026		// Port Interrupt 0 Mask
.equ PORTR_INT1MASK = 2027		// Port Interrupt 1 Mask
.equ PORTR_INTFLAGS = 2028		// Interrupt Flag Register
.equ PORTR_PIN0CTRL = 2032		// Pin 0 Control Register
.equ PORTR_PIN1CTRL = 2033		// Pin 1 Control Register
.equ PORTR_PIN2CTRL = 2034		// Pin 2 Control Register
.equ PORTR_PIN3CTRL = 2035		// Pin 3 Control Register
.equ PORTR_PIN4CTRL = 2036		// Pin 4 Control Register
.equ PORTR_PIN5CTRL = 2037		// Pin 5 Control Register
.equ PORTR_PIN6CTRL = 2038		// Pin 6 Control Register
.equ PORTR_PIN7CTRL = 2039		// Pin 7 Control Register

;***************************************************************************
;** TCC0 - Timer/Counter C0
;***************************************************************************
	
.equ TCC0_CTRLA = 2048		// Control  Register A
.equ TCC0_CTRLB = 2049		// Control Register B
.equ TCC0_CTRLC = 2050		// Control register C
.equ TCC0_CTRLD = 2051		// Control Register D
.equ TCC0_CTRLE = 2052		// Control Register E
.equ TCC0_INTCTRLA = 2054		// Interrupt Control Register A
.equ TCC0_INTCTRLB = 2055		// Interrupt Control Register B
.equ TCC0_CTRLFCLR = 2056		// Control Register F Clear
.equ TCC0_CTRLFSET = 2057		// Control Register F Set
.equ TCC0_CTRLGCLR = 2058		// Control Register G Clear
.equ TCC0_CTRLGSET = 2059		// Control Register G Set
.equ TCC0_INTFLAGS = 2060		// Interrupt Flag Register
.equ TCC0_TEMP = 2063		// Temporary Register For 16-bit Access
.equ TCC0_CNT = 2080		// Count
.equ TCC0_PER = 2086		// Period
.equ TCC0_CCA = 2088		// Compare or Capture A
.equ TCC0_CCB = 2090		// Compare or Capture B
.equ TCC0_CCC = 2092		// Compare or Capture C
.equ TCC0_CCD = 2094		// Compare or Capture D
.equ TCC0_PERBUF = 2102		// Period Buffer
.equ TCC0_CCABUF = 2104		// Compare Or Capture A Buffer
.equ TCC0_CCBBUF = 2106		// Compare Or Capture B Buffer
.equ TCC0_CCCBUF = 2108		// Compare Or Capture C Buffer
.equ TCC0_CCDBUF = 2110		// Compare Or Capture D Buffer

;***************************************************************************
;** TCC1 - Timer/Counter C1
;***************************************************************************
	
.equ TCC1_CTRLA = 2112		// Control  Register A
.equ TCC1_CTRLB = 2113		// Control Register B
.equ TCC1_CTRLC = 2114		// Control register C
.equ TCC1_CTRLD = 2115		// Control Register D
.equ TCC1_CTRLE = 2116		// Control Register E
.equ TCC1_INTCTRLA = 2118		// Interrupt Control Register A
.equ TCC1_INTCTRLB = 2119		// Interrupt Control Register B
.equ TCC1_CTRLFCLR = 2120		// Control Register F Clear
.equ TCC1_CTRLFSET = 2121		// Control Register F Set
.equ TCC1_CTRLGCLR = 2122		// Control Register G Clear
.equ TCC1_CTRLGSET = 2123		// Control Register G Set
.equ TCC1_INTFLAGS = 2124		// Interrupt Flag Register
.equ TCC1_TEMP = 2127		// Temporary Register For 16-bit Access
.equ TCC1_CNT = 2144		// Count
.equ TCC1_PER = 2150		// Period
.equ TCC1_CCA = 2152		// Compare or Capture A
.equ TCC1_CCB = 2154		// Compare or Capture B
.equ TCC1_PERBUF = 2166		// Period Buffer
.equ TCC1_CCABUF = 2168		// Compare Or Capture A Buffer
.equ TCC1_CCBBUF = 2170		// Compare Or Capture B Buffer

;***************************************************************************
;** AWEXC - Advanced Waveform Extension C
;***************************************************************************
	
.equ AWEXC_CTRL = 2176		// Control Register
.equ AWEXC_FDEVMASK = 2178		// Fault Detection Event Mask
.equ AWEXC_FDCTRL = 2179		// Fault Detection Control Register
.equ AWEXC_STATUS = 2180		// Status Register
.equ AWEXC_DTBOTH = 2182		// Dead Time Both Sides
.equ AWEXC_DTBOTHBUF = 2183		// Dead Time Both Sides Buffer
.equ AWEXC_DTLS = 2184		// Dead Time Low Side
.equ AWEXC_DTHS = 2185		// Dead Time High Side
.equ AWEXC_DTLSBUF = 2186		// Dead Time Low Side Buffer
.equ AWEXC_DTHSBUF = 2187		// Dead Time High Side Buffer
.equ AWEXC_OUTOVEN = 2188		// Output Override Enable

;***************************************************************************
;** HIRESC - High-Resolution Extension C
;***************************************************************************
	
.equ HIRESC_CTRL = 2192		// Control Register

;***************************************************************************
;** USARTC0 - Universal Asynchronous Receiver-Transmitter C0
;***************************************************************************
	
.equ USARTC0_DATA = 2208		// Data Register
.equ USARTC0_STATUS = 2209		// Status Register
.equ USARTC0_CTRLA = 2211		// Control Register A
.equ USARTC0_CTRLB = 2212		// Control Register B
.equ USARTC0_CTRLC = 2213		// Control Register C
.equ USARTC0_BAUDCTRLA = 2214		// Baud Rate Control Register A
.equ USARTC0_BAUDCTRLB = 2215		// Baud Rate Control Register B

;***************************************************************************
;** USARTC1 - Universal Asynchronous Receiver-Transmitter C1
;***************************************************************************
	
.equ USARTC1_DATA = 2224		// Data Register
.equ USARTC1_STATUS = 2225		// Status Register
.equ USARTC1_CTRLA = 2227		// Control Register A
.equ USARTC1_CTRLB = 2228		// Control Register B
.equ USARTC1_CTRLC = 2229		// Control Register C
.equ USARTC1_BAUDCTRLA = 2230		// Baud Rate Control Register A
.equ USARTC1_BAUDCTRLB = 2231		// Baud Rate Control Register B

;***************************************************************************
;** SPIC - Serial Peripheral Interface C
;***************************************************************************
	
.equ SPIC_CTRL = 2240		// Control Register
.equ SPIC_INTCTRL = 2241		// Interrupt Control Register
.equ SPIC_STATUS = 2242		// Status Register
.equ SPIC_DATA = 2243		// Data Register

;***************************************************************************
;** TCD0 - Timer/Counter D0
;***************************************************************************
	
.equ TCD0_CTRLA = 2304		// Control  Register A
.equ TCD0_CTRLB = 2305		// Control Register B
.equ TCD0_CTRLC = 2306		// Control register C
.equ TCD0_CTRLD = 2307		// Control Register D
.equ TCD0_CTRLE = 2308		// Control Register E
.equ TCD0_INTCTRLA = 2310		// Interrupt Control Register A
.equ TCD0_INTCTRLB = 2311		// Interrupt Control Register B
.equ TCD0_CTRLFCLR = 2312		// Control Register F Clear
.equ TCD0_CTRLFSET = 2313		// Control Register F Set
.equ TCD0_CTRLGCLR = 2314		// Control Register G Clear
.equ TCD0_CTRLGSET = 2315		// Control Register G Set
.equ TCD0_INTFLAGS = 2316		// Interrupt Flag Register
.equ TCD0_TEMP = 2319		// Temporary Register For 16-bit Access
.equ TCD0_CNT = 2336		// Count
.equ TCD0_PER = 2342		// Period
.equ TCD0_CCA = 2344		// Compare or Capture A
.equ TCD0_CCB = 2346		// Compare or Capture B
.equ TCD0_CCC = 2348		// Compare or Capture C
.equ TCD0_CCD = 2350		// Compare or Capture D
.equ TCD0_PERBUF = 2358		// Period Buffer
.equ TCD0_CCABUF = 2360		// Compare Or Capture A Buffer
.equ TCD0_CCBBUF = 2362		// Compare Or Capture B Buffer
.equ TCD0_CCCBUF = 2364		// Compare Or Capture C Buffer
.equ TCD0_CCDBUF = 2366		// Compare Or Capture D Buffer

;***************************************************************************
;** TCD1 - Timer/Counter D1
;***************************************************************************
	
.equ TCD1_CTRLA = 2368		// Control  Register A
.equ TCD1_CTRLB = 2369		// Control Register B
.equ TCD1_CTRLC = 2370		// Control register C
.equ TCD1_CTRLD = 2371		// Control Register D
.equ TCD1_CTRLE = 2372		// Control Register E
.equ TCD1_INTCTRLA = 2374		// Interrupt Control Register A
.equ TCD1_INTCTRLB = 2375		// Interrupt Control Register B
.equ TCD1_CTRLFCLR = 2376		// Control Register F Clear
.equ TCD1_CTRLFSET = 2377		// Control Register F Set
.equ TCD1_CTRLGCLR = 2378		// Control Register G Clear
.equ TCD1_CTRLGSET = 2379		// Control Register G Set
.equ TCD1_INTFLAGS = 2380		// Interrupt Flag Register
.equ TCD1_TEMP = 2383		// Temporary Register For 16-bit Access
.equ TCD1_CNT = 2400		// Count
.equ TCD1_PER = 2406		// Period
.equ TCD1_CCA = 2408		// Compare or Capture A
.equ TCD1_CCB = 2410		// Compare or Capture B
.equ TCD1_PERBUF = 2422		// Period Buffer
.equ TCD1_CCABUF = 2424		// Compare Or Capture A Buffer
.equ TCD1_CCBBUF = 2426		// Compare Or Capture B Buffer

;***************************************************************************
;** HIRESD - High-Resolution Extension D
;***************************************************************************
	
.equ HIRESD_CTRL = 2448		// Control Register

;***************************************************************************
;** USARTD0 - Universal Asynchronous Receiver-Transmitter D0
;***************************************************************************
	
.equ USARTD0_DATA = 2464		// Data Register
.equ USARTD0_STATUS = 2465		// Status Register
.equ USARTD0_CTRLA = 2467		// Control Register A
.equ USARTD0_CTRLB = 2468		// Control Register B
.equ USARTD0_CTRLC = 2469		// Control Register C
.equ USARTD0_BAUDCTRLA = 2470		// Baud Rate Control Register A
.equ USARTD0_BAUDCTRLB = 2471		// Baud Rate Control Register B

;***************************************************************************
;** USARTD1 - Universal Asynchronous Receiver-Transmitter D1
;***************************************************************************
	
.equ USARTD1_DATA = 2480		// Data Register
.equ USARTD1_STATUS = 2481		// Status Register
.equ USARTD1_CTRLA = 2483		// Control Register A
.equ USARTD1_CTRLB = 2484		// Control Register B
.equ USARTD1_CTRLC = 2485		// Control Register C
.equ USARTD1_BAUDCTRLA = 2486		// Baud Rate Control Register A
.equ USARTD1_BAUDCTRLB = 2487		// Baud Rate Control Register B

;***************************************************************************
;** SPID - Serial Peripheral Interface D
;***************************************************************************
	
.equ SPID_CTRL = 2496		// Control Register
.equ SPID_INTCTRL = 2497		// Interrupt Control Register
.equ SPID_STATUS = 2498		// Status Register
.equ SPID_DATA = 2499		// Data Register

;***************************************************************************
;** TCE0 - Timer/Counter E0
;***************************************************************************
	
.equ TCE0_CTRLA = 2560		// Control  Register A
.equ TCE0_CTRLB = 2561		// Control Register B
.equ TCE0_CTRLC = 2562		// Control register C
.equ TCE0_CTRLD = 2563		// Control Register D
.equ TCE0_CTRLE = 2564		// Control Register E
.equ TCE0_INTCTRLA = 2566		// Interrupt Control Register A
.equ TCE0_INTCTRLB = 2567		// Interrupt Control Register B
.equ TCE0_CTRLFCLR = 2568		// Control Register F Clear
.equ TCE0_CTRLFSET = 2569		// Control Register F Set
.equ TCE0_CTRLGCLR = 2570		// Control Register G Clear
.equ TCE0_CTRLGSET = 2571		// Control Register G Set
.equ TCE0_INTFLAGS = 2572		// Interrupt Flag Register
.equ TCE0_TEMP = 2575		// Temporary Register For 16-bit Access
.equ TCE0_CNT = 2592		// Count
.equ TCE0_PER = 2598		// Period
.equ TCE0_CCA = 2600		// Compare or Capture A
.equ TCE0_CCB = 2602		// Compare or Capture B
.equ TCE0_CCC = 2604		// Compare or Capture C
.equ TCE0_CCD = 2606		// Compare or Capture D
.equ TCE0_PERBUF = 2614		// Period Buffer
.equ TCE0_CCABUF = 2616		// Compare Or Capture A Buffer
.equ TCE0_CCBBUF = 2618		// Compare Or Capture B Buffer
.equ TCE0_CCCBUF = 2620		// Compare Or Capture C Buffer
.equ TCE0_CCDBUF = 2622		// Compare Or Capture D Buffer

;***************************************************************************
;** TCE1 - Timer/Counter E1
;***************************************************************************
	
.equ TCE1_CTRLA = 2624		// Control  Register A
.equ TCE1_CTRLB = 2625		// Control Register B
.equ TCE1_CTRLC = 2626		// Control register C
.equ TCE1_CTRLD = 2627		// Control Register D
.equ TCE1_CTRLE = 2628		// Control Register E
.equ TCE1_INTCTRLA = 2630		// Interrupt Control Register A
.equ TCE1_INTCTRLB = 2631		// Interrupt Control Register B
.equ TCE1_CTRLFCLR = 2632		// Control Register F Clear
.equ TCE1_CTRLFSET = 2633		// Control Register F Set
.equ TCE1_CTRLGCLR = 2634		// Control Register G Clear
.equ TCE1_CTRLGSET = 2635		// Control Register G Set
.equ TCE1_INTFLAGS = 2636		// Interrupt Flag Register
.equ TCE1_TEMP = 2639		// Temporary Register For 16-bit Access
.equ TCE1_CNT = 2656		// Count
.equ TCE1_PER = 2662		// Period
.equ TCE1_CCA = 2664		// Compare or Capture A
.equ TCE1_CCB = 2666		// Compare or Capture B
.equ TCE1_PERBUF = 2678		// Period Buffer
.equ TCE1_CCABUF = 2680		// Compare Or Capture A Buffer
.equ TCE1_CCBBUF = 2682		// Compare Or Capture B Buffer

;***************************************************************************
;** AWEXE - Advanced Waveform Extension E
;***************************************************************************
	
.equ AWEXE_CTRL = 2688		// Control Register
.equ AWEXE_FDEVMASK = 2690		// Fault Detection Event Mask
.equ AWEXE_FDCTRL = 2691		// Fault Detection Control Register
.equ AWEXE_STATUS = 2692		// Status Register
.equ AWEXE_DTBOTH = 2694		// Dead Time Both Sides
.equ AWEXE_DTBOTHBUF = 2695		// Dead Time Both Sides Buffer
.equ AWEXE_DTLS = 2696		// Dead Time Low Side
.equ AWEXE_DTHS = 2697		// Dead Time High Side
.equ AWEXE_DTLSBUF = 2698		// Dead Time Low Side Buffer
.equ AWEXE_DTHSBUF = 2699		// Dead Time High Side Buffer
.equ AWEXE_OUTOVEN = 2700		// Output Override Enable

;***************************************************************************
;** HIRESE - High-Resolution Extension E
;***************************************************************************
	
.equ HIRESE_CTRL = 2704		// Control Register

;***************************************************************************
;** USARTE0 - Universal Asynchronous Receiver-Transmitter E0
;***************************************************************************
	
.equ USARTE0_DATA = 2720		// Data Register
.equ USARTE0_STATUS = 2721		// Status Register
.equ USARTE0_CTRLA = 2723		// Control Register A
.equ USARTE0_CTRLB = 2724		// Control Register B
.equ USARTE0_CTRLC = 2725		// Control Register C
.equ USARTE0_BAUDCTRLA = 2726		// Baud Rate Control Register A
.equ USARTE0_BAUDCTRLB = 2727		// Baud Rate Control Register B

;***************************************************************************
;** USARTE1 - Universal Asynchronous Receiver-Transmitter E1
;***************************************************************************
	
.equ USARTE1_DATA = 2736		// Data Register
.equ USARTE1_STATUS = 2737		// Status Register
.equ USARTE1_CTRLA = 2739		// Control Register A
.equ USARTE1_CTRLB = 2740		// Control Register B
.equ USARTE1_CTRLC = 2741		// Control Register C
.equ USARTE1_BAUDCTRLA = 2742		// Baud Rate Control Register A
.equ USARTE1_BAUDCTRLB = 2743		// Baud Rate Control Register B

;***************************************************************************
;** SPIE - Serial Peripheral Interface E
;***************************************************************************
	
.equ SPIE_CTRL = 2752		// Control Register
.equ SPIE_INTCTRL = 2753		// Interrupt Control Register
.equ SPIE_STATUS = 2754		// Status Register
.equ SPIE_DATA = 2755		// Data Register

;***************************************************************************
;** TCF0 - Timer/Counter F0
;***************************************************************************
	
.equ TCF0_CTRLA = 2816		// Control  Register A
.equ TCF0_CTRLB = 2817		// Control Register B
.equ TCF0_CTRLC = 2818		// Control register C
.equ TCF0_CTRLD = 2819		// Control Register D
.equ TCF0_CTRLE = 2820		// Control Register E
.equ TCF0_INTCTRLA = 2822		// Interrupt Control Register A
.equ TCF0_INTCTRLB = 2823		// Interrupt Control Register B
.equ TCF0_CTRLFCLR = 2824		// Control Register F Clear
.equ TCF0_CTRLFSET = 2825		// Control Register F Set
.equ TCF0_CTRLGCLR = 2826		// Control Register G Clear
.equ TCF0_CTRLGSET = 2827		// Control Register G Set
.equ TCF0_INTFLAGS = 2828		// Interrupt Flag Register
.equ TCF0_TEMP = 2831		// Temporary Register For 16-bit Access
.equ TCF0_CNT = 2848		// Count
.equ TCF0_PER = 2854		// Period
.equ TCF0_CCA = 2856		// Compare or Capture A
.equ TCF0_CCB = 2858		// Compare or Capture B
.equ TCF0_CCC = 2860		// Compare or Capture C
.equ TCF0_CCD = 2862		// Compare or Capture D
.equ TCF0_PERBUF = 2870		// Period Buffer
.equ TCF0_CCABUF = 2872		// Compare Or Capture A Buffer
.equ TCF0_CCBBUF = 2874		// Compare Or Capture B Buffer
.equ TCF0_CCCBUF = 2876		// Compare Or Capture C Buffer
.equ TCF0_CCDBUF = 2878		// Compare Or Capture D Buffer

;***************************************************************************
;** TCF1 - Timer/Counter F1
;***************************************************************************
	
.equ TCF1_CTRLA = 2880		// Control  Register A
.equ TCF1_CTRLB = 2881		// Control Register B
.equ TCF1_CTRLC = 2882		// Control register C
.equ TCF1_CTRLD = 2883		// Control Register D
.equ TCF1_CTRLE = 2884		// Control Register E
.equ TCF1_INTCTRLA = 2886		// Interrupt Control Register A
.equ TCF1_INTCTRLB = 2887		// Interrupt Control Register B
.equ TCF1_CTRLFCLR = 2888		// Control Register F Clear
.equ TCF1_CTRLFSET = 2889		// Control Register F Set
.equ TCF1_CTRLGCLR = 2890		// Control Register G Clear
.equ TCF1_CTRLGSET = 2891		// Control Register G Set
.equ TCF1_INTFLAGS = 2892		// Interrupt Flag Register
.equ TCF1_TEMP = 2895		// Temporary Register For 16-bit Access
.equ TCF1_CNT = 2912		// Count
.equ TCF1_PER = 2918		// Period
.equ TCF1_CCA = 2920		// Compare or Capture A
.equ TCF1_CCB = 2922		// Compare or Capture B
.equ TCF1_PERBUF = 2934		// Period Buffer
.equ TCF1_CCABUF = 2936		// Compare Or Capture A Buffer
.equ TCF1_CCBBUF = 2938		// Compare Or Capture B Buffer

;***************************************************************************
;** HIRESF - High-Resolution Extension F
;***************************************************************************
	
.equ HIRESF_CTRL = 2960		// Control Register

;***************************************************************************
;** USARTF0 - Universal Asynchronous Receiver-Transmitter F0
;***************************************************************************
	
.equ USARTF0_DATA = 2976		// Data Register
.equ USARTF0_STATUS = 2977		// Status Register
.equ USARTF0_CTRLA = 2979		// Control Register A
.equ USARTF0_CTRLB = 2980		// Control Register B
.equ USARTF0_CTRLC = 2981		// Control Register C
.equ USARTF0_BAUDCTRLA = 2982		// Baud Rate Control Register A
.equ USARTF0_BAUDCTRLB = 2983		// Baud Rate Control Register B

;***************************************************************************
;** USARTF1 - Universal Asynchronous Receiver-Transmitter F1
;***************************************************************************
	
.equ USARTF1_DATA = 2992		// Data Register
.equ USARTF1_STATUS = 2993		// Status Register
.equ USARTF1_CTRLA = 2995		// Control Register A
.equ USARTF1_CTRLB = 2996		// Control Register B
.equ USARTF1_CTRLC = 2997		// Control Register C
.equ USARTF1_BAUDCTRLA = 2998		// Baud Rate Control Register A
.equ USARTF1_BAUDCTRLB = 2999		// Baud Rate Control Register B

;***************************************************************************
;** SPIF - Serial Peripheral Interface F
;***************************************************************************
	
.equ SPIF_CTRL = 3008		// Control Register
.equ SPIF_INTCTRL = 3009		// Interrupt Control Register
.equ SPIF_STATUS = 3010		// Status Register
.equ SPIF_DATA = 3011		// Data Register

;***************************************************************************
;** IRCOM - IR Communication Module
;***************************************************************************
	
.equ IRCOM_CTRL = 2296		// Control Register
.equ IRCOM_TXPLCTRL = 2297		// IrDA Transmitter Pulse Length Control Register
.equ IRCOM_RXPLCTRL = 2298		// IrDA Receiver Pulse Length Control Register

;***************************************************************************
;** AES - AES Crypto Module
;***************************************************************************
	
.equ AES_CTRL = 192		// AES Control Register
.equ AES_STATUS = 193		// AES Status Register
.equ AES_STATE = 194		// AES State Register
.equ AES_KEY = 195		// AES Key Register
.equ AES_INTCTRL = 196		// AES Interrupt Control Register

; ***** ALL MODULE BASE ADRESSES *****************************************

.equ GPIO_base = 0x0000		// General Purpose IO Registers
.equ OCD_base = 0x002E		// On-Chip Debug System
.equ CPU_base = 0x0030		// CPU Registers
.equ CLK_base = 0x0040		// Clock System
.equ SLEEP_base = 0x0048		// Sleep Controller
.equ OSC_base = 0x0050		// Oscillator Control
.equ DFLLRC32M_base = 0x0060		// DFLL for 32MHz RC Oscillator
.equ DFLLRC2M_base = 0x0068		// DFLL for 2MHz RC Oscillator
.equ PR_base = 0x0070		// Power Reduction
.equ RST_base = 0x0078		// Reset Controller
.equ WDT_base = 0x0080		// Watch-Dog Timer
.equ MCU_base = 0x0090		// MCU Control
.equ PMIC_base = 0x00A0		// Programmable Interrupt Controller
.equ DMA_base = 0x0100		// DMA Controller
.equ EVSYS_base = 0x0180		// Event System
.equ NVM_base = 0x01C0		// Non Volatile Memory Controller
.equ ACA_base = 0x0380		// Analog Comparator A
.equ ACB_base = 0x0390		// Analog Comparator B
.equ ADCA_base = 0x0200		// Analog to Digital Converter A
.equ ADCB_base = 0x0240		// Analog to Digital Converter B
.equ DACA_base = 0x0300		// Digital to Analog Converter A
.equ DACB_base = 0x0320		// Digital to Analog Converter B
.equ RTC_base = 0x0400		// Real-Time Counter
.equ EBI_base = 0x0440		// External Bus Interface
.equ TWIC_base = 0x480		// Two-Wire Interface C
.equ TWID_base = 0x490		// Two-Wire Interface D
.equ TWIE_base = 0x4A0		// Two-Wire Interface E
.equ TWIF_base = 0x4B0		// Two-Wire Interface F
.equ PORTCFG_base = 0x00B0		// Port Configuration
.equ VPORT0_base = 0x0010		// Virtual Port 0
.equ VPORT1_base = 0x0014		// Virtual Port 1
.equ VPORT2_base = 0x0018		// Virtual Port 2
.equ VPORT3_base = 0x001C		// Virtual Port 3
.equ PORTA_base = 0x0600		// Port A
.equ PORTB_base = 0x0620		// Port B
.equ PORTC_base = 0x0640		// Port C
.equ PORTD_base = 0x0660		// Port D
.equ PORTE_base = 0x0680		// Port E
.equ PORTF_base = 0x06A0		// Port F
.equ PORTH_base = 0x06E0		// Port H
.equ PORTJ_base = 0x0700		// Port J
.equ PORTK_base = 0x0720		// Port K
.equ PORTQ_base = 0x07C0		// Port Q
.equ PORTR_base = 0x07E0		// Port R
.equ TCC0_base = 0x800		// Timer/Counter C0
.equ TCC1_base = 0x840		// Timer/Counter C1
.equ AWEXC_base = 0x880		// Advanced Waveform Extension C
.equ HIRESC_base = 0x890		// High-Resolution Extension C
.equ USARTC0_base = 0x8A0		// Universal Asynchronous Receiver-Transmitter C0
.equ USARTC1_base = 0x8B0		// Universal Asynchronous Receiver-Transmitter C1
.equ SPIC_base = 0x8C0		// Serial Peripheral Interface C
.equ TCD0_base = 0x900		// Timer/Counter D0
.equ TCD1_base = 0x940		// Timer/Counter D1
.equ HIRESD_base = 0x990		// High-Resolution Extension D
.equ USARTD0_base = 0x9A0		// Universal Asynchronous Receiver-Transmitter D0
.equ USARTD1_base = 0x9B0		// Universal Asynchronous Receiver-Transmitter D1
.equ SPID_base = 0x9C0		// Serial Peripheral Interface D
.equ TCE0_base = 0xA00		// Timer/Counter E0
.equ TCE1_base = 0xA40		// Timer/Counter E1
.equ AWEXE_base = 0xA80		// Advanced Waveform Extension E
.equ HIRESE_base = 0xA90		// High-Resolution Extension E
.equ USARTE0_base = 0xAA0		// Universal Asynchronous Receiver-Transmitter E0
.equ USARTE1_base = 0xAB0		// Universal Asynchronous Receiver-Transmitter E1
.equ SPIE_base = 0xAC0		// Serial Peripheral Interface E
.equ TCF0_base = 0xB00		// Timer/Counter F0
.equ TCF1_base = 0xB40		// Timer/Counter F1
.equ HIRESF_base = 0xB90		// High-Resolution Extension F
.equ USARTF0_base = 0xBA0		// Universal Asynchronous Receiver-Transmitter F0
.equ USARTF1_base = 0xBB0		// Universal Asynchronous Receiver-Transmitter F1
.equ SPIF_base = 0xBC0		// Serial Peripheral Interface F
.equ IRCOM_base = 0x8F8		// IR Communication Module
.equ AES_base = 0x0C0		// AES Crypto Module

; ***** IO REGISTER OFFSETS **********************************************


;***************************************************************************
;**  - General Purpose IO
;***************************************************************************
	

;***************************************************************************
;**  - On-Chip Debug System
;***************************************************************************
	

;***************************************************************************
;**  - CPU
;***************************************************************************
	

;***************************************************************************
;**  - Clock System
;***************************************************************************
	

;***************************************************************************
;**  - Sleep Controller
;***************************************************************************
	

;***************************************************************************
;**  - Oscillator
;***************************************************************************
	

;***************************************************************************
;**  - DFLL
;***************************************************************************
	

;***************************************************************************
;**  - Reset
;***************************************************************************
	

;***************************************************************************
;**  - Watch-Dog Timer
;***************************************************************************
	

;***************************************************************************
;**  - MCU Control
;***************************************************************************
	

;***************************************************************************
;**  - Programmable Multi-level Interrupt Controller
;***************************************************************************
	

;***************************************************************************
;**  - DMA Controller
;***************************************************************************
	

;***************************************************************************
;**  - Event System
;***************************************************************************
	

;***************************************************************************
;**  - Non Volatile Memory Controller
;***************************************************************************
	

;***************************************************************************
;**  - Analog Comparator
;***************************************************************************
	

;***************************************************************************
;**  - Analog/Digital Converter
;***************************************************************************
	

;***************************************************************************
;**  - Digital/Analog Converter
;***************************************************************************
	

;***************************************************************************
;**  - Real-Time Clounter
;***************************************************************************
	

;***************************************************************************
;**  - External Bus Interface
;***************************************************************************
	

;***************************************************************************
;**  - Two-Wire Interface
;***************************************************************************
	

;***************************************************************************
;**  - Port Configuration
;***************************************************************************
	

;***************************************************************************
;**  - 16-bit Timer/Counter With PWM
;***************************************************************************
	

;***************************************************************************
;**  - Universal Asynchronous Receiver-Transmitter
;***************************************************************************
	

;***************************************************************************
;**  - Serial Peripheral Interface
;***************************************************************************
	

;***************************************************************************
;**  - IR Communication Module
;***************************************************************************
	

;***************************************************************************
;**  - AES Module
;***************************************************************************
	

; ***** LOCKBIT REGISTER LOCATIONS ***************************************


;***************************************************************************
;** LOCKBIT - Lockbits
;***************************************************************************
	
.equ LOCKBIT_LOCKBITS = 0		// Lock Bits


; ***** FUSE REGISTER LOCATIONS ******************************************


;***************************************************************************
;** FUSE - Fuses
;***************************************************************************
	
.equ FUSE_FUSEBYTE0 = 0		// JTAG User ID
.equ FUSE_FUSEBYTE1 = 1		// Watchdog Configuration
.equ FUSE_FUSEBYTE2 = 2		// Reset Configuration
.equ FUSE_FUSEBYTE4 = 4		// Start-up Configuration
.equ FUSE_FUSEBYTE5 = 5		// EESAVE and BOD Level


; ***** BIT AND VALUE DEFINITIONS ****************************************


;***************************************************************************
;** GPIO - General Purpose IO
;***************************************************************************/
 

;***************************************************************************
;** XOCD - On-Chip Debug System
;***************************************************************************/

; OCD_OCDR1 masks
.equ OCD_OCDRD_bm = 0x01 ; OCDR Dirty
 

;***************************************************************************
;** CPU - CPU
;***************************************************************************/

; CPU_CCP masks
.equ CPU_CCP_gm = 0xFF ; CCP signature
.equ CPU_CCP0_bm = (1<<0) ; CCP signature bit 0
.equ CPU_CCP1_bm = (1<<1) ; CCP signature bit 1
.equ CPU_CCP2_bm = (1<<2) ; CCP signature bit 2
.equ CPU_CCP3_bm = (1<<3) ; CCP signature bit 3
.equ CPU_CCP4_bm = (1<<4) ; CCP signature bit 4
.equ CPU_CCP5_bm = (1<<5) ; CCP signature bit 5
.equ CPU_CCP6_bm = (1<<6) ; CCP signature bit 6
.equ CPU_CCP7_bm = (1<<7) ; CCP signature bit 7

; CPU_SREG masks
.equ CPU_I_bm = 0x80 ; Global Interrupt Enable Flag
.equ CPU_T_bm = 0x40 ; Transfer Bit
.equ CPU_H_bm = 0x20 ; Half Carry Flag
.equ CPU_S_bm = 0x10 ; N Exclusive Or V Flag
.equ CPU_V_bm = 0x08 ; Two's Complement Overflow Flag
.equ CPU_N_bm = 0x04 ; Negative Flag
.equ CPU_Z_bm = 0x02 ; Zero Flag
.equ CPU_C_bm = 0x01 ; Carry Flag
 
; CCP signatures
.equ CCP_SPM_gc = (0x9D<<0) ; SPM Instruction Protection
.equ CCP_IOREG_gc = (0xD8<<0) ; IO Register Protection


;***************************************************************************
;** CLK - Clock System
;***************************************************************************/

; CLK_CTRL masks
.equ CLK_SCLKSEL_gm = 0x07 ; System Clock Selection
.equ CLK_SCLKSEL0_bm = (1<<0) ; System Clock Selection bit 0
.equ CLK_SCLKSEL1_bm = (1<<1) ; System Clock Selection bit 1
.equ CLK_SCLKSEL2_bm = (1<<2) ; System Clock Selection bit 2

; CLK_PSCTRL masks
.equ CLK_PSADIV_gm = 0x7C ; Prescaler A Division Factor
.equ CLK_PSADIV0_bm = (1<<2) ; Prescaler A Division Factor bit 0
.equ CLK_PSADIV1_bm = (1<<3) ; Prescaler A Division Factor bit 1
.equ CLK_PSADIV2_bm = (1<<4) ; Prescaler A Division Factor bit 2
.equ CLK_PSADIV3_bm = (1<<5) ; Prescaler A Division Factor bit 3
.equ CLK_PSADIV4_bm = (1<<6) ; Prescaler A Division Factor bit 4
.equ CLK_PSBCDIV_gm = 0x03 ; Prescaler B and C Division factor
.equ CLK_PSBCDIV0_bm = (1<<0) ; Prescaler B and C Division factor bit 0
.equ CLK_PSBCDIV1_bm = (1<<1) ; Prescaler B and C Division factor bit 1

; CLK_LOCK masks
.equ CLK_LOCK_bm = 0x01 ; Clock System Lock

; CLK_RTCCTRL masks
.equ CLK_RTCSRC_gm = 0x0E ; Clock Source
.equ CLK_RTCSRC0_bm = (1<<1) ; Clock Source bit 0
.equ CLK_RTCSRC1_bm = (1<<2) ; Clock Source bit 1
.equ CLK_RTCSRC2_bm = (1<<3) ; Clock Source bit 2
.equ CLK_RTCEN_bm = 0x01 ; RTC Clock Source Enable

; PR_PR masks
.equ PR_EBI_bm = 0x08 ; External Bus Interface
.equ PR_RTC_bm = 0x04 ; Real-time Counter
.equ PR_EVSYS_bm = 0x02 ; Event System
.equ PR_DMA_bm = 0x01 ; DMA-Controller

; PR_PRPA masks
.equ PR_DAC_bm = 0x04 ; Port A DAC
.equ PR_ADC_bm = 0x02 ; Port A ADC
.equ PR_AC_bm = 0x01 ; Port A Analog Comparator

; PR_PRPB masks
; Masks for DAC aready defined
; Masks for ADC aready defined
; Masks for AC aready defined

; PR_PRPC masks
.equ PR_TWI_bm = 0x40 ; Port C Two-wire Interface
.equ PR_USART1_bm = 0x20 ; Port C USART1
.equ PR_USART0_bm = 0x10 ; Port C USART0
.equ PR_SPI_bm = 0x08 ; Port C SPI
.equ PR_HIRES_bm = 0x04 ; Port C AWEX
.equ PR_TC1_bm = 0x02 ; Port C Timer/Counter1
.equ PR_TC0_bm = 0x01 ; Port C Timer/Counter0

; PR_PRPD masks
; Masks for TWI aready defined
; Masks for USART1 aready defined
; Masks for USART0 aready defined
; Masks for SPI aready defined
; Masks for HIRES aready defined
; Masks for TC1 aready defined
; Masks for TC0 aready defined

; PR_PRPE masks
; Masks for TWI aready defined
; Masks for USART1 aready defined
; Masks for USART0 aready defined
; Masks for SPI aready defined
; Masks for HIRES aready defined
; Masks for TC1 aready defined
; Masks for TC0 aready defined

; PR_PRPF masks
; Masks for TWI aready defined
; Masks for USART1 aready defined
; Masks for USART0 aready defined
; Masks for SPI aready defined
; Masks for HIRES aready defined
; Masks for TC1 aready defined
; Masks for TC0 aready defined
 
; System Clock Selection
.equ CLK_SCLKSEL_RC2M_gc = (0x00<<0) ; Internal 2MHz RC Oscillator
.equ CLK_SCLKSEL_RC32M_gc = (0x01<<0) ; Internal 32MHz RC Oscillator
.equ CLK_SCLKSEL_RC32K_gc = (0x02<<0) ; Internal 32kHz RC Oscillator
.equ CLK_SCLKSEL_XOSC_gc = (0x03<<0) ; External Crystal Oscillator or Clock
.equ CLK_SCLKSEL_PLL_gc = (0x04<<0) ; Phase Locked Loop

; Prescaler A Division Factor
.equ CLK_PSADIV_1_gc = (0x00<<2) ; Divide by 1
.equ CLK_PSADIV_2_gc = (0x01<<2) ; Divide by 2
.equ CLK_PSADIV_4_gc = (0x03<<2) ; Divide by 4
.equ CLK_PSADIV_8_gc = (0x05<<2) ; Divide by 8
.equ CLK_PSADIV_16_gc = (0x07<<2) ; Divide by 16
.equ CLK_PSADIV_32_gc = (0x09<<2) ; Divide by 32
.equ CLK_PSADIV_64_gc = (0x0B<<2) ; Divide by 64
.equ CLK_PSADIV_128_gc = (0x0D<<2) ; Divide by 128
.equ CLK_PSADIV_256_gc = (0x0F<<2) ; Divide by 256
.equ CLK_PSADIV_512_gc = (0x11<<2) ; Divide by 512

; Prescaler B and C Division Factor
.equ CLK_PSBCDIV_1_1_gc = (0x00<<0) ; Divide B by 1 and C by 1
.equ CLK_PSBCDIV_1_2_gc = (0x01<<0) ; Divide B by 1 and C by 2
.equ CLK_PSBCDIV_4_1_gc = (0x02<<0) ; Divide B by 4 and C by 1
.equ CLK_PSBCDIV_2_2_gc = (0x03<<0) ; Divide B by 2 and C by 2

; RTC Clock Source
.equ CLK_RTCSRC_ULP_gc = (0x00<<1) ; 1kHz from internal 32kHz ULP
.equ CLK_RTCSRC_TOSC_gc = (0x01<<1) ; 1kHz from 32kHz crystal oscillator on TOSC
.equ CLK_RTCSRC_RCOSC_gc = (0x02<<1) ; 1kHz from internal 32kHz RC oscillator
.equ CLK_RTCSRC_TOSC32_gc = (0x05<<1) ; 32kHz from 32kHz crystal oscillator on TOSC


;***************************************************************************
;** SLEEP - Sleep Controller
;***************************************************************************/

; SLEEP_CTRL masks
.equ SLEEP_SMODE_gm = 0x0E ; Sleep Mode
.equ SLEEP_SMODE0_bm = (1<<1) ; Sleep Mode bit 0
.equ SLEEP_SMODE1_bm = (1<<2) ; Sleep Mode bit 1
.equ SLEEP_SMODE2_bm = (1<<3) ; Sleep Mode bit 2
.equ SLEEP_SEN_bm = 0x01 ; Sleep Enable
 
; Sleep Mode
.equ SLEEP_SMODE_IDLE_gc = (0x00<<1) ; Idle mode
.equ SLEEP_SMODE_PDOWN_gc = (0x02<<1) ; Power-down Mode
.equ SLEEP_SMODE_PSAVE_gc = (0x03<<1) ; Power-save Mode
.equ SLEEP_SMODE_STDBY_gc = (0x06<<1) ; Standby Mode
.equ SLEEP_SMODE_ESTDBY_gc = (0x07<<1) ; Extended Standby Mode


;***************************************************************************
;** OSC - Oscillator
;***************************************************************************/

; OSC_CTRL masks
.equ OSC_PLLEN_bm = 0x10 ; PLL Enable
.equ OSC_XOSCEN_bm = 0x08 ; External Oscillator Enable
.equ OSC_RC32KEN_bm = 0x04 ; Internal 32kHz RC Oscillator Enable
.equ OSC_RC32MEN_bm = 0x02 ; Internal 32MHz RC Oscillator Enable
.equ OSC_RC2MEN_bm = 0x01 ; Internal 2MHz RC Oscillator Enable

; OSC_STATUS masks
.equ OSC_PLLRDY_bm = 0x10 ; PLL Ready
.equ OSC_XOSCRDY_bm = 0x08 ; External Oscillator Ready
.equ OSC_RC32KRDY_bm = 0x04 ; Internal 32kHz RC Oscillator Ready
.equ OSC_RC32MRDY_bm = 0x02 ; Internal 32MHz RC Oscillator Ready
.equ OSC_RC2MRDY_bm = 0x01 ; Internal 2MHz RC Oscillator Ready

; OSC_XOSCCTRL masks
.equ OSC_FRQRANGE_gm = 0xC0 ; Frequency Range
.equ OSC_FRQRANGE0_bm = (1<<6) ; Frequency Range bit 0
.equ OSC_FRQRANGE1_bm = (1<<7) ; Frequency Range bit 1
.equ OSC_X32KLPM_bm = 0x20 ; 32kHz XTAL OSC Low-power Mode
.equ OSC_XOSCSEL_gm = 0x0F ; External Oscillator Selection and Startup Time
.equ OSC_XOSCSEL0_bm = (1<<0) ; External Oscillator Selection and Startup Time bit 0
.equ OSC_XOSCSEL1_bm = (1<<1) ; External Oscillator Selection and Startup Time bit 1
.equ OSC_XOSCSEL2_bm = (1<<2) ; External Oscillator Selection and Startup Time bit 2
.equ OSC_XOSCSEL3_bm = (1<<3) ; External Oscillator Selection and Startup Time bit 3

; OSC_XOSCFAIL masks
.equ OSC_XOSCFDIF_bm = 0x02 ; Failure Detection Interrupt Flag
.equ OSC_XOSCFDEN_bm = 0x01 ; Failure Detection Enable

; OSC_PLLCTRL masks
.equ OSC_PLLSRC_gm = 0xC0 ; Clock Source
.equ OSC_PLLSRC0_bm = (1<<6) ; Clock Source bit 0
.equ OSC_PLLSRC1_bm = (1<<7) ; Clock Source bit 1
.equ OSC_PLLFAC_gm = 0x1F ; Multiplication Factor
.equ OSC_PLLFAC0_bm = (1<<0) ; Multiplication Factor bit 0
.equ OSC_PLLFAC1_bm = (1<<1) ; Multiplication Factor bit 1
.equ OSC_PLLFAC2_bm = (1<<2) ; Multiplication Factor bit 2
.equ OSC_PLLFAC3_bm = (1<<3) ; Multiplication Factor bit 3
.equ OSC_PLLFAC4_bm = (1<<4) ; Multiplication Factor bit 4

; OSC_DFLLCTRL masks
.equ OSC_RC32MCREF_bm = 0x02 ; 32MHz Calibration Reference
.equ OSC_RC2MCREF_bm = 0x01 ; 2MHz Calibration Reference
 
; Oscillator Frequency Range
.equ OSC_FRQRANGE_04TO2_gc = (0x00<<6) ; 0.4 - 2 MHz
.equ OSC_FRQRANGE_2TO9_gc = (0x01<<6) ; 2 - 9 MHz
.equ OSC_FRQRANGE_9TO12_gc = (0x02<<6) ; 9 - 12 MHz
.equ OSC_FRQRANGE_12TO16_gc = (0x03<<6) ; 12 - 16 MHz

; External Oscillator Selection and Startup Time
.equ OSC_XOSCSEL_EXTCLK_gc = (0x00<<0) ; External Clock - 6 CLK
.equ OSC_XOSCSEL_32KHz_gc = (0x02<<0) ; 32kHz TOSC - 32K CLK
.equ OSC_XOSCSEL_XTAL_256CLK_gc = (0x03<<0) ; 0.4-16MHz XTAL - 256 CLK
.equ OSC_XOSCSEL_XTAL_1KCLK_gc = (0x07<<0) ; 0.4-16MHz XTAL - 1K CLK
.equ OSC_XOSCSEL_XTAL_16KCLK_gc = (0x0B<<0) ; 0.4-16MHz XTAL - 16K CLK

; PLL Clock Source
.equ OSC_PLLSRC_RC2M_gc = (0x00<<6) ; Internal 2MHz RC Oscillator
.equ OSC_PLLSRC_RC32M_gc = (0x02<<6) ; Internal 32MHz RC Oscillator
.equ OSC_PLLSRC_XOSC_gc = (0x03<<6) ; External Oscillator


;***************************************************************************
;** DFLL - DFLL
;***************************************************************************/

; DFLL_CTRL masks
.equ DFLL_ENABLE_bm = 0x01 ; DFLL Enable

; DFLL_CALA masks
.equ DFLL_CALL_gm = 0x7F ; DFLL Calibration bits [6:0]
.equ DFLL_CALL0_bm = (1<<0) ; DFLL Calibration bits [6:0] bit 0
.equ DFLL_CALL1_bm = (1<<1) ; DFLL Calibration bits [6:0] bit 1
.equ DFLL_CALL2_bm = (1<<2) ; DFLL Calibration bits [6:0] bit 2
.equ DFLL_CALL3_bm = (1<<3) ; DFLL Calibration bits [6:0] bit 3
.equ DFLL_CALL4_bm = (1<<4) ; DFLL Calibration bits [6:0] bit 4
.equ DFLL_CALL5_bm = (1<<5) ; DFLL Calibration bits [6:0] bit 5
.equ DFLL_CALL6_bm = (1<<6) ; DFLL Calibration bits [6:0] bit 6

; DFLL_CALB masks
.equ DFLL_CALH_gm = 0x3F ; DFLL Calibration bits [12:7]
.equ DFLL_CALH0_bm = (1<<0) ; DFLL Calibration bits [12:7] bit 0
.equ DFLL_CALH1_bm = (1<<1) ; DFLL Calibration bits [12:7] bit 1
.equ DFLL_CALH2_bm = (1<<2) ; DFLL Calibration bits [12:7] bit 2
.equ DFLL_CALH3_bm = (1<<3) ; DFLL Calibration bits [12:7] bit 3
.equ DFLL_CALH4_bm = (1<<4) ; DFLL Calibration bits [12:7] bit 4
.equ DFLL_CALH5_bm = (1<<5) ; DFLL Calibration bits [12:7] bit 5
 

;***************************************************************************
;** RST - Reset
;***************************************************************************/

; RST_STATUS masks
.equ RST_SDRF_bm = 0x40 ; Spike Detection Reset Flag
.equ RST_SRF_bm = 0x20 ; Software Reset Flag
.equ RST_PDIRF_bm = 0x10 ; Programming and Debug Interface Interface Reset Flag
.equ RST_WDRF_bm = 0x08 ; Watchdog Reset Flag
.equ RST_BORF_bm = 0x04 ; Brown-out Reset Flag
.equ RST_EXTRF_bm = 0x02 ; External Reset Flag
.equ RST_PORF_bm = 0x01 ; Power-on Reset Flag

; RST_CTRL masks
.equ RST_SWRST_bm = 0x01 ; Software Reset
 

;***************************************************************************
;** WDT - Watch-Dog Timer
;***************************************************************************/

; WDT_CTRL masks
.equ WDT_PER_gm = 0x3C ; Period
.equ WDT_PER0_bm = (1<<2) ; Period bit 0
.equ WDT_PER1_bm = (1<<3) ; Period bit 1
.equ WDT_PER2_bm = (1<<4) ; Period bit 2
.equ WDT_PER3_bm = (1<<5) ; Period bit 3
.equ WDT_ENABLE_bm = 0x02 ; Enable
.equ WDT_CEN_bm = 0x01 ; Change Enable

; WDT_WINCTRL masks
.equ WDT_WPER_gm = 0x3C ; Windowed Mode Period
.equ WDT_WPER0_bm = (1<<2) ; Windowed Mode Period bit 0
.equ WDT_WPER1_bm = (1<<3) ; Windowed Mode Period bit 1
.equ WDT_WPER2_bm = (1<<4) ; Windowed Mode Period bit 2
.equ WDT_WPER3_bm = (1<<5) ; Windowed Mode Period bit 3
.equ WDT_WEN_bm = 0x02 ; Windowed Mode Enable
.equ WDT_WCEN_bm = 0x01 ; Windowed Mode Change Enable

; WDT_STATUS masks
.equ WDT_SYNCBUSY_bm = 0x01 ; Syncronization busy
 
; Period setting
.equ WDT_PER_8CLK_gc = (0x00<<2) ; 8 cycles (8ms @ 3.3V)
.equ WDT_PER_16CLK_gc = (0x01<<2) ; 16 cycles (16ms @ 3.3V)
.equ WDT_PER_32CLK_gc = (0x02<<2) ; 32 cycles (32ms @ 3.3V)
.equ WDT_PER_64CLK_gc = (0x03<<2) ; 64 cycles (64ms @ 3.3V)
.equ WDT_PER_128CLK_gc = (0x04<<2) ; 128 cycles (0.125s @ 3.3V)
.equ WDT_PER_256CLK_gc = (0x05<<2) ; 256 cycles (0.25s @ 3.3V)
.equ WDT_PER_512CLK_gc = (0x06<<2) ; 512 cycles (0.5s @ 3.3V)
.equ WDT_PER_1KCLK_gc = (0x07<<2) ; 1K cycles (1s @ 3.3V)
.equ WDT_PER_2KCLK_gc = (0x08<<2) ; 2K cycles (2s @ 3.3V)
.equ WDT_PER_4KCLK_gc = (0x09<<2) ; 4K cycles (4s @ 3.3V)
.equ WDT_PER_8KCLK_gc = (0x0A<<2) ; 8K cycles (8s @ 3.3V)

; Closed window period
.equ WDT_WPER_8CLK_gc = (0x00<<2) ; 8 cycles (8ms @ 3.3V)
.equ WDT_WPER_16CLK_gc = (0x01<<2) ; 16 cycles (16ms @ 3.3V)
.equ WDT_WPER_32CLK_gc = (0x02<<2) ; 32 cycles (32ms @ 3.3V)
.equ WDT_WPER_64CLK_gc = (0x03<<2) ; 64 cycles (64ms @ 3.3V)
.equ WDT_WPER_128CLK_gc = (0x04<<2) ; 128 cycles (0.125s @ 3.3V)
.equ WDT_WPER_256CLK_gc = (0x05<<2) ; 256 cycles (0.25s @ 3.3V)
.equ WDT_WPER_512CLK_gc = (0x06<<2) ; 512 cycles (0.5s @ 3.3V)
.equ WDT_WPER_1KCLK_gc = (0x07<<2) ; 1K cycles (1s @ 3.3V)
.equ WDT_WPER_2KCLK_gc = (0x08<<2) ; 2K cycles (2s @ 3.3V)
.equ WDT_WPER_4KCLK_gc = (0x09<<2) ; 4K cycles (4s @ 3.3V)
.equ WDT_WPER_8KCLK_gc = (0x0A<<2) ; 8K cycles (8s @ 3.3V)


;***************************************************************************
;** MCU - MCU Control
;***************************************************************************/

; MCU_MCUCR masks
.equ MCU_JTAGD_bm = 0x01 ; JTAG Disable

; MCU_EVSYSLOCK masks
.equ MCU_EVSYS1LOCK_bm = 0x10 ; Event Channel 4-7 Lock
.equ MCU_EVSYS0LOCK_bm = 0x01 ; Event Channel 0-3 Lock

; MCU_AWEXLOCK masks
.equ MCU_AWEXELOCK_bm = 0x04 ; AWeX on T/C E0 Lock
.equ MCU_AWEXCLOCK_bm = 0x01 ; AWeX on T/C C0 Lock
 

;***************************************************************************
;** PMIC - Programmable Multi-level Interrupt Controller
;***************************************************************************/

; PMIC_STATUS masks
.equ PMIC_NMIEX_bm = 0x80 ; Non-maskable Interrupt Executing
.equ PMIC_HILVLEX_bm = 0x04 ; High Level Interrupt Executing
.equ PMIC_MEDLVLEX_bm = 0x02 ; Medium Level Interrupt Executing
.equ PMIC_LOLVLEX_bm = 0x01 ; Low Level Interrupt Executing

; PMIC_CTRL masks
.equ PMIC_RREN_bm = 0x80 ; Round-Robin Priority Enable
.equ PMIC_IVSEL_bm = 0x40 ; Interrupt Vector Select
.equ PMIC_HILVLEN_bm = 0x04 ; High Level Enable
.equ PMIC_MEDLVLEN_bm = 0x02 ; Medium Level Enable
.equ PMIC_LOLVLEN_bm = 0x01 ; Low Level Enable
 

;***************************************************************************
;** DMA - DMA Controller
;***************************************************************************/

; DMA_CH_CTRLA masks
.equ DMA_CH_ENABLE_bm = 0x80 ; Channel Enable
.equ DMA_CH_RESET_bm = 0x40 ; Channel Software Reset
.equ DMA_CH_REPEAT_bm = 0x20 ; Channel Repeat Mode
.equ DMA_CH_TRFREQ_bm = 0x10 ; Channel Transfer Request
.equ DMA_CH_SINGLE_bm = 0x04 ; Channel Single Shot Data Transfer
.equ DMA_CH_BURSTLEN_gm = 0x03 ; Channel Transfer Mode
.equ DMA_CH_BURSTLEN0_bm = (1<<0) ; Channel Transfer Mode bit 0
.equ DMA_CH_BURSTLEN1_bm = (1<<1) ; Channel Transfer Mode bit 1

; DMA_CH_CTRLB masks
.equ DMA_CH_CHBUSY_bm = 0x80 ; Block Transfer Busy
.equ DMA_CH_CHPEND_bm = 0x40 ; Block Transfer Pending
.equ DMA_CH_ERRIF_bm = 0x20 ; Block Transfer Error Interrupt Flag
.equ DMA_CH_TRNIF_bm = 0x10 ; Transaction Complete Interrup Flag
.equ DMA_CH_ERRINTLVL_gm = 0x0C ; Transfer Error Interrupt Level
.equ DMA_CH_ERRINTLVL0_bm = (1<<2) ; Transfer Error Interrupt Level bit 0
.equ DMA_CH_ERRINTLVL1_bm = (1<<3) ; Transfer Error Interrupt Level bit 1
.equ DMA_CH_TRNINTLVL_gm = 0x03 ; Transaction Complete Interrupt Level
.equ DMA_CH_TRNINTLVL0_bm = (1<<0) ; Transaction Complete Interrupt Level bit 0
.equ DMA_CH_TRNINTLVL1_bm = (1<<1) ; Transaction Complete Interrupt Level bit 1

; DMA_CH_ADDRCTRL masks
.equ DMA_CH_SRCRELOAD_gm = 0xC0 ; Channel Source Address Reload
.equ DMA_CH_SRCRELOAD0_bm = (1<<6) ; Channel Source Address Reload bit 0
.equ DMA_CH_SRCRELOAD1_bm = (1<<7) ; Channel Source Address Reload bit 1
.equ DMA_CH_SRCDIR_gm = 0x30 ; Channel Source Address Mode
.equ DMA_CH_SRCDIR0_bm = (1<<4) ; Channel Source Address Mode bit 0
.equ DMA_CH_SRCDIR1_bm = (1<<5) ; Channel Source Address Mode bit 1
.equ DMA_CH_DESTRELOAD_gm = 0x0C ; Channel Destination Address Reload
.equ DMA_CH_DESTRELOAD0_bm = (1<<2) ; Channel Destination Address Reload bit 0
.equ DMA_CH_DESTRELOAD1_bm = (1<<3) ; Channel Destination Address Reload bit 1
.equ DMA_CH_DESTDIR_gm = 0x03 ; Channel Destination Address Mode
.equ DMA_CH_DESTDIR0_bm = (1<<0) ; Channel Destination Address Mode bit 0
.equ DMA_CH_DESTDIR1_bm = (1<<1) ; Channel Destination Address Mode bit 1

; DMA_CH_TRIGSRC masks
.equ DMA_CH_TRIGSRC_gm = 0xFF ; Channel Trigger Source
.equ DMA_CH_TRIGSRC0_bm = (1<<0) ; Channel Trigger Source bit 0
.equ DMA_CH_TRIGSRC1_bm = (1<<1) ; Channel Trigger Source bit 1
.equ DMA_CH_TRIGSRC2_bm = (1<<2) ; Channel Trigger Source bit 2
.equ DMA_CH_TRIGSRC3_bm = (1<<3) ; Channel Trigger Source bit 3
.equ DMA_CH_TRIGSRC4_bm = (1<<4) ; Channel Trigger Source bit 4
.equ DMA_CH_TRIGSRC5_bm = (1<<5) ; Channel Trigger Source bit 5
.equ DMA_CH_TRIGSRC6_bm = (1<<6) ; Channel Trigger Source bit 6
.equ DMA_CH_TRIGSRC7_bm = (1<<7) ; Channel Trigger Source bit 7

; DMA_CTRL masks
.equ DMA_ENABLE_bm = 0x80 ; Enable
.equ DMA_RESET_bm = 0x40 ; Software Reset
.equ DMA_DBUFMODE_gm = 0x0C ; Double Buffering Mode
.equ DMA_DBUFMODE0_bm = (1<<2) ; Double Buffering Mode bit 0
.equ DMA_DBUFMODE1_bm = (1<<3) ; Double Buffering Mode bit 1
.equ DMA_PRIMODE_gm = 0x03 ; Channel Priority Mode
.equ DMA_PRIMODE0_bm = (1<<0) ; Channel Priority Mode bit 0
.equ DMA_PRIMODE1_bm = (1<<1) ; Channel Priority Mode bit 1

; DMA_INTFLAGS masks
.equ DMA_CH3ERRIF_bm = 0x80 ; Channel 3 Block Transfer Error Interrupt Flag
.equ DMA_CH2ERRIF_bm = 0x40 ; Channel 2 Block Transfer Error Interrupt Flag
.equ DMA_CH1ERRIF_bm = 0x20 ; Channel 1 Block Transfer Error Interrupt Flag
.equ DMA_CH0ERRIF_bm = 0x10 ; Channel 0 Block Transfer Error Interrupt Flag
.equ DMA_CH3TRNIF_bm = 0x08 ; Channel 3 Transaction Complete Interrupt Flag
.equ DMA_CH2TRNIF_bm = 0x04 ; Channel 2 Transaction Complete Interrupt Flag
.equ DMA_CH1TRNIF_bm = 0x02 ; Channel 1 Transaction Complete Interrupt Flag
.equ DMA_CH0TRNIF_bm = 0x01 ; Channel 0 Transaction Complete Interrupt Flag

; DMA_STATUS masks
.equ DMA_CH3BUSY_bm = 0x80 ; Channel 3 Block Transfer Busy
.equ DMA_CH2BUSY_bm = 0x40 ; Channel 2 Block Transfer Busy
.equ DMA_CH1BUSY_bm = 0x20 ; Channel 1 Block Transfer Busy
.equ DMA_CH0BUSY_bm = 0x10 ; Channel 0 Block Transfer Busy
.equ DMA_CH3PEND_bm = 0x08 ; Channel 3 Block Transfer Pending
.equ DMA_CH2PEND_bm = 0x04 ; Channel 2 Block Transfer Pending
.equ DMA_CH1PEND_bm = 0x02 ; Channel 1 Block Transfer Pending
.equ DMA_CH0PEND_bm = 0x01 ; Channel 0 Block Transfer Pending
 
; Burst mode
.equ DMA_CH_BURSTLEN_1BYTE_gc = (0x00<<0) ; 1-byte burst mode
.equ DMA_CH_BURSTLEN_2BYTE_gc = (0x01<<0) ; 2-byte burst mode
.equ DMA_CH_BURSTLEN_4BYTE_gc = (0x02<<0) ; 4-byte burst mode
.equ DMA_CH_BURSTLEN_8BYTE_gc = (0x03<<0) ; 8-byte burst mode

; Source address reload mode
.equ DMA_CH_SRCRELOAD_NONE_gc = (0x00<<6) ; No reload
.equ DMA_CH_SRCRELOAD_BLOCK_gc = (0x01<<6) ; Reload at end of block
.equ DMA_CH_SRCRELOAD_BURST_gc = (0x02<<6) ; Reload at end of burst
.equ DMA_CH_SRCRELOAD_TRANSACTION_gc = (0x03<<6) ; Reload at end of transaction

; Source addressing mode
.equ DMA_CH_SRCDIR_FIXED_gc = (0x00<<4) ; Fixed
.equ DMA_CH_SRCDIR_INC_gc = (0x01<<4) ; Increment
.equ DMA_CH_SRCDIR_DEC_gc = (0x02<<4) ; Decrement

; Destination adress reload mode
.equ DMA_CH_DESTRELOAD_NONE_gc = (0x00<<2) ; No reload
.equ DMA_CH_DESTRELOAD_BLOCK_gc = (0x01<<2) ; Reload at end of block
.equ DMA_CH_DESTRELOAD_BURST_gc = (0x02<<2) ; Reload at end of burst
.equ DMA_CH_DESTRELOAD_TRANSACTION_gc = (0x03<<2) ; Reload at end of transaction

; Destination adressing mode
.equ DMA_CH_DESTDIR_FIXED_gc = (0x00<<0) ; Fixed
.equ DMA_CH_DESTDIR_INC_gc = (0x01<<0) ; Increment
.equ DMA_CH_DESTDIR_DEC_gc = (0x02<<0) ; Decrement

; Transfer trigger source
.equ DMA_CH_TRIGSRC_OFF_gc = (0x00<<0) ; Off software triggers only
.equ DMA_CH_TRIGSRC_EVSYS_CH0_gc = (0x01<<0) ; Event System Channel 0
.equ DMA_CH_TRIGSRC_EVSYS_CH1_gc = (0x02<<0) ; Event System Channel 1
.equ DMA_CH_TRIGSRC_EVSYS_CH2_gc = (0x03<<0) ; Event System Channel 2
.equ DMA_CH_TRIGSRC_ADCA_CH0_gc = (0x10<<0) ; ADCA Channel 0
.equ DMA_CH_TRIGSRC_ADCA_CH1_gc = (0x11<<0) ; ADCA Channel 1
.equ DMA_CH_TRIGSRC_ADCA_CH2_gc = (0x12<<0) ; ADCA Channel 2
.equ DMA_CH_TRIGSRC_ADCA_CH3_gc = (0x13<<0) ; ADCA Channel 3
.equ DMA_CH_TRIGSRC_ADCA_CH4_gc = (0x14<<0) ; ADCA Channel 0,1,2,3 combined
.equ DMA_CH_TRIGSRC_DACA_CH0_gc = (0x15<<0) ; DACA Channel 0
.equ DMA_CH_TRIGSRC_DACA_CH1_gc = (0x16<<0) ; DACA Channel 1
.equ DMA_CH_TRIGSRC_ADCB_CH0_gc = (0x20<<0) ; ADCB Channel 0
.equ DMA_CH_TRIGSRC_ADCB_CH1_gc = (0x21<<0) ; ADCB Channel 1
.equ DMA_CH_TRIGSRC_ADCB_CH2_gc = (0x22<<0) ; ADCB Channel 2
.equ DMA_CH_TRIGSRC_ADCB_CH3_gc = (0x23<<0) ; ADCB Channel 3
.equ DMA_CH_TRIGSRC_ADCB_CH4_gc = (0x24<<0) ; ADCB Channel 0,1,2,3 combined
.equ DMA_CH_TRIGSRC_DACB_CH0_gc = (0x25<<0) ; DACB Channel 0
.equ DMA_CH_TRIGSRC_DACB_CH1_gc = (0x26<<0) ; DACB Channel 1
.equ DMA_CH_TRIGSRC_TCC0_OVF_gc = (0x40<<0) ; Timer/Counter C0 Overflow
.equ DMA_CH_TRIGSRC_TCC0_ERR_gc = (0x41<<0) ; Timer/Counter C0 Error
.equ DMA_CH_TRIGSRC_TCC0_CCA_gc = (0x42<<0) ; Timer/Counter C0 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCC0_CCB_gc = (0x43<<0) ; Timer/Counter C0 Compare or Capture B
.equ DMA_CH_TRIGSRC_TCC0_CCC_gc = (0x44<<0) ; Timer/Counter C0 Compare or Capture C
.equ DMA_CH_TRIGSRC_TCC0_CCD_gc = (0x45<<0) ; Timer/Counter C0 Compare or Capture D
.equ DMA_CH_TRIGSRC_TCC1_OVF_gc = (0x46<<0) ; Timer/Counter C1 Overflow
.equ DMA_CH_TRIGSRC_TCC1_ERR_gc = (0x47<<0) ; Timer/Counter C1 Error
.equ DMA_CH_TRIGSRC_TCC1_CCA_gc = (0x48<<0) ; Timer/Counter C1 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCC1_CCB_gc = (0x49<<0) ; Timer/Counter C1 Compare or Capture B
.equ DMA_CH_TRIGSRC_SPIC_gc = (0x4A<<0) ; SPI C Transfer Complete
.equ DMA_CH_TRIGSRC_USARTC0_RXC_gc = (0x4B<<0) ; USART C0 Receive Complete
.equ DMA_CH_TRIGSRC_USARTC0_DRE_gc = (0x4C<<0) ; USART C0 Data Register Empty
.equ DMA_CH_TRIGSRC_USARTC1_RXC_gc = (0x4E<<0) ; USART C1 Receive Complete
.equ DMA_CH_TRIGSRC_USARTC1_DRE_gc = (0x4F<<0) ; USART C1 Data Register Empty
.equ DMA_CH_TRIGSRC_TCD0_OVF_gc = (0x60<<0) ; Timer/Counter D0 Overflow
.equ DMA_CH_TRIGSRC_TCD0_ERR_gc = (0x61<<0) ; Timer/Counter D0 Error
.equ DMA_CH_TRIGSRC_TCD0_CCA_gc = (0x62<<0) ; Timer/Counter D0 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCD0_CCB_gc = (0x63<<0) ; Timer/Counter D0 Compare or Capture B
.equ DMA_CH_TRIGSRC_TCD0_CCC_gc = (0x64<<0) ; Timer/Counter D0 Compare or Capture C
.equ DMA_CH_TRIGSRC_TCD0_CCD_gc = (0x65<<0) ; Timer/Counter D0 Compare or Capture D
.equ DMA_CH_TRIGSRC_TCD1_OVF_gc = (0x66<<0) ; Timer/Counter D1 Overflow
.equ DMA_CH_TRIGSRC_TCD1_ERR_gc = (0x67<<0) ; Timer/Counter D1 Error
.equ DMA_CH_TRIGSRC_TCD1_CCA_gc = (0x68<<0) ; Timer/Counter D1 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCD1_CCB_gc = (0x69<<0) ; Timer/Counter D1 Compare or Capture B
.equ DMA_CH_TRIGSRC_SPID_gc = (0x6A<<0) ; SPI D Transfer Complete
.equ DMA_CH_TRIGSRC_USARTD0_RXC_gc = (0x6B<<0) ; USART D0 Receive Complete
.equ DMA_CH_TRIGSRC_USARTD0_DRE_gc = (0x6C<<0) ; USART D0 Data Register Empty
.equ DMA_CH_TRIGSRC_USARTD1_RXC_gc = (0x6E<<0) ; USART D1 Receive Complete
.equ DMA_CH_TRIGSRC_USARTD1_DRE_gc = (0x6F<<0) ; USART D1 Data Register Empty
.equ DMA_CH_TRIGSRC_TCE0_OVF_gc = (0x80<<0) ; Timer/Counter E0 Overflow
.equ DMA_CH_TRIGSRC_TCE0_ERR_gc = (0x81<<0) ; Timer/Counter E0 Error
.equ DMA_CH_TRIGSRC_TCE0_CCA_gc = (0x82<<0) ; Timer/Counter E0 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCE0_CCB_gc = (0x83<<0) ; Timer/Counter E0 Compare or Capture B
.equ DMA_CH_TRIGSRC_TCE0_CCC_gc = (0x84<<0) ; Timer/Counter E0 Compare or Capture C
.equ DMA_CH_TRIGSRC_TCE0_CCD_gc = (0x85<<0) ; Timer/Counter E0 Compare or Capture D
.equ DMA_CH_TRIGSRC_TCE1_OVF_gc = (0x86<<0) ; Timer/Counter E1 Overflow
.equ DMA_CH_TRIGSRC_TCE1_ERR_gc = (0x87<<0) ; Timer/Counter E1 Error
.equ DMA_CH_TRIGSRC_TCE1_CCA_gc = (0x88<<0) ; Timer/Counter E1 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCE1_CCB_gc = (0x89<<0) ; Timer/Counter E1 Compare or Capture B
.equ DMA_CH_TRIGSRC_SPIE_gc = (0x8A<<0) ; SPI E Transfer Complete
.equ DMA_CH_TRIGSRC_USARTE0_RXC_gc = (0x8B<<0) ; USART E0 Receive Complete
.equ DMA_CH_TRIGSRC_USARTE0_DRE_gc = (0x8C<<0) ; USART E0 Data Register Empty
.equ DMA_CH_TRIGSRC_USARTE1_RXC_gc = (0x8E<<0) ; USART E1 Receive Complete
.equ DMA_CH_TRIGSRC_USARTE1_DRE_gc = (0x8F<<0) ; USART E1 Data Register Empty
.equ DMA_CH_TRIGSRC_TCF0_OVF_gc = (0xA0<<0) ; Timer/Counter F0 Overflow
.equ DMA_CH_TRIGSRC_TCF0_ERR_gc = (0xA1<<0) ; Timer/Counter F0 Error
.equ DMA_CH_TRIGSRC_TCF0_CCA_gc = (0xA2<<0) ; Timer/Counter F0 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCF0_CCB_gc = (0xA3<<0) ; Timer/Counter F0 Compare or Capture B
.equ DMA_CH_TRIGSRC_TCF0_CCC_gc = (0xA4<<0) ; Timer/Counter F0 Compare or Capture C
.equ DMA_CH_TRIGSRC_TCF0_CCD_gc = (0xA5<<0) ; Timer/Counter F0 Compare or Capture D
.equ DMA_CH_TRIGSRC_TCF1_OVF_gc = (0xA6<<0) ; Timer/Counter F1 Overflow
.equ DMA_CH_TRIGSRC_TCF1_ERR_gc = (0xA7<<0) ; Timer/Counter F1 Error
.equ DMA_CH_TRIGSRC_TCF1_CCA_gc = (0xA8<<0) ; Timer/Counter F1 Compare or Capture A
.equ DMA_CH_TRIGSRC_TCF1_CCB_gc = (0xA9<<0) ; Timer/Counter F1 Compare or Capture B
.equ DMA_CH_TRIGSRC_SPIF_gc = (0xAA<<0) ; SPI F Transfer Complete
.equ DMA_CH_TRIGSRC_USARTF0_RXC_gc = (0xAB<<0) ; USART F0 Receive Complete
.equ DMA_CH_TRIGSRC_USARTF0_DRE_gc = (0xAC<<0) ; USART F0 Data Register Empty
.equ DMA_CH_TRIGSRC_USARTF1_RXC_gc = (0xAE<<0) ; USART F1 Receive Complete
.equ DMA_CH_TRIGSRC_USARTF1_DRE_gc = (0xAF<<0) ; USART F1 Data Register Empty

; Double buffering mode
.equ DMA_DBUFMODE_DISABLED_gc = (0x00<<2) ; Double buffering disabled
.equ DMA_DBUFMODE_CH01_gc = (0x01<<2) ; Double buffering enabled on channel 0/1
.equ DMA_DBUFMODE_CH23_gc = (0x02<<2) ; Double buffering enabled on channel 2/3
.equ DMA_DBUFMODE_CH01CH23_gc = (0x03<<2) ; Double buffering enabled on ch. 0/1 and ch. 2/3

; Priority mode
.equ DMA_PRIMODE_RR0123_gc = (0x00<<0) ; Round Robin
.equ DMA_PRIMODE_CH0RR123_gc = (0x01<<0) ; Channel 0 > Round Robin on channel 1/2/3
.equ DMA_PRIMODE_CH01RR23_gc = (0x02<<0) ; Channel 0 > channel 1 > Round Robin on channel 2/3
.equ DMA_PRIMODE_CH0123_gc = (0x03<<0) ; Channel 0 > channel 1 > channel 2 > channel 3

; Interrupt level
.equ DMA_CH_ERRINTLVL_OFF_gc = (0x00<<2) ; Interrupt disabled
.equ DMA_CH_ERRINTLVL_LO_gc = (0x01<<2) ; Low level
.equ DMA_CH_ERRINTLVL_MED_gc = (0x02<<2) ; Medium level
.equ DMA_CH_ERRINTLVL_HI_gc = (0x03<<2) ; High level

; Interrupt level
.equ DMA_CH_TRNINTLVL_OFF_gc = (0x00<<0) ; Interrupt disabled
.equ DMA_CH_TRNINTLVL_LO_gc = (0x01<<0) ; Low level
.equ DMA_CH_TRNINTLVL_MED_gc = (0x02<<0) ; Medium level
.equ DMA_CH_TRNINTLVL_HI_gc = (0x03<<0) ; High level


;***************************************************************************
;** EVSYS - Event System
;***************************************************************************/

; EVSYS_CH0MUX masks
.equ EVSYS_CHMUX_gm = 0xFF ; Event Channel 0 Multiplexer
.equ EVSYS_CHMUX0_bm = (1<<0) ; Event Channel 0 Multiplexer bit 0
.equ EVSYS_CHMUX1_bm = (1<<1) ; Event Channel 0 Multiplexer bit 1
.equ EVSYS_CHMUX2_bm = (1<<2) ; Event Channel 0 Multiplexer bit 2
.equ EVSYS_CHMUX3_bm = (1<<3) ; Event Channel 0 Multiplexer bit 3
.equ EVSYS_CHMUX4_bm = (1<<4) ; Event Channel 0 Multiplexer bit 4
.equ EVSYS_CHMUX5_bm = (1<<5) ; Event Channel 0 Multiplexer bit 5
.equ EVSYS_CHMUX6_bm = (1<<6) ; Event Channel 0 Multiplexer bit 6
.equ EVSYS_CHMUX7_bm = (1<<7) ; Event Channel 0 Multiplexer bit 7

; EVSYS_CH1MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH2MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH3MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH4MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH5MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH6MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH7MUX masks
; Masks for CHMUX aready defined

; EVSYS_CH0CTRL masks
.equ EVSYS_QDIRM_gm = 0x60 ; Quadrature Decoder Index Recognition Mode
.equ EVSYS_QDIRM0_bm = (1<<5) ; Quadrature Decoder Index Recognition Mode bit 0
.equ EVSYS_QDIRM1_bm = (1<<6) ; Quadrature Decoder Index Recognition Mode bit 1
.equ EVSYS_QDIEN_bm = 0x10 ; Quadrature Decoder Index Enable
.equ EVSYS_QDEN_bm = 0x08 ; Quadrature Decoder Enable
.equ EVSYS_DIGFILT_gm = 0x07 ; Digital Filter
.equ EVSYS_DIGFILT0_bm = (1<<0) ; Digital Filter bit 0
.equ EVSYS_DIGFILT1_bm = (1<<1) ; Digital Filter bit 1
.equ EVSYS_DIGFILT2_bm = (1<<2) ; Digital Filter bit 2

; EVSYS_CH1CTRL masks
; Masks for DIGFILT aready defined

; EVSYS_CH2CTRL masks
; Masks for QDIRM aready defined
; Masks for QDIEN aready defined
; Masks for QDEN aready defined
; Masks for DIGFILT aready defined

; EVSYS_CH3CTRL masks
; Masks for DIGFILT aready defined

; EVSYS_CH4CTRL masks
; Masks for QDIRM aready defined
; Masks for QDIEN aready defined
; Masks for QDEN aready defined
; Masks for DIGFILT aready defined

; EVSYS_CH5CTRL masks
; Masks for DIGFILT aready defined

; EVSYS_CH6CTRL masks
; Masks for DIGFILT aready defined

; EVSYS_CH7CTRL masks
; Masks for DIGFILT aready defined
 
; Quadrature Decoder Index Recognition Mode
.equ EVSYS_QDIRM_00_gc = (0x00<<5) ; QDPH0 = 0, QDPH90 = 0
.equ EVSYS_QDIRM_01_gc = (0x01<<5) ; QDPH0 = 0, QDPH90 = 1
.equ EVSYS_QDIRM_10_gc = (0x02<<5) ; QDPH0 = 1, QDPH90 = 0
.equ EVSYS_QDIRM_11_gc = (0x03<<5) ; QDPH0 = 1, QDPH90 = 1

; Digital filter coefficient
.equ EVSYS_DIGFILT_1SAMPLE_gc = (0x00<<0) ; 1 SAMPLE
.equ EVSYS_DIGFILT_2SAMPLES_gc = (0x01<<0) ; 2 SAMPLES
.equ EVSYS_DIGFILT_3SAMPLES_gc = (0x02<<0) ; 3 SAMPLES
.equ EVSYS_DIGFILT_4SAMPLES_gc = (0x03<<0) ; 4 SAMPLES
.equ EVSYS_DIGFILT_5SAMPLES_gc = (0x04<<0) ; 5 SAMPLES
.equ EVSYS_DIGFILT_6SAMPLES_gc = (0x05<<0) ; 6 SAMPLES
.equ EVSYS_DIGFILT_7SAMPLES_gc = (0x06<<0) ; 7 SAMPLES
.equ EVSYS_DIGFILT_8SAMPLES_gc = (0x07<<0) ; 8 SAMPLES

; Event Channel multiplexer input selection
.equ EVSYS_CHMUX_OFF_gc = (0x00<<0) ; Off
.equ EVSYS_CHMUX_RTC_OVF_gc = (0x08<<0) ; RTC Overflow
.equ EVSYS_CHMUX_RTC_CMP_gc = (0x09<<0) ; RTC Compare Match
.equ EVSYS_CHMUX_ACA_CH0_gc = (0x10<<0) ; Analog Comparator A Channel 0
.equ EVSYS_CHMUX_ACA_CH1_gc = (0x11<<0) ; Analog Comparator A Channel 1
.equ EVSYS_CHMUX_ACA_WIN_gc = (0x12<<0) ; Analog Comparator A Window
.equ EVSYS_CHMUX_ACB_CH0_gc = (0x13<<0) ; Analog Comparator B Channel 0
.equ EVSYS_CHMUX_ACB_CH1_gc = (0x14<<0) ; Analog Comparator B Channel 1
.equ EVSYS_CHMUX_ACB_WIN_gc = (0x15<<0) ; Analog Comparator B Window
.equ EVSYS_CHMUX_ADCA_CH0_gc = (0x20<<0) ; ADC A Channel 0
.equ EVSYS_CHMUX_ADCA_CH1_gc = (0x21<<0) ; ADC A Channel 1
.equ EVSYS_CHMUX_ADCA_CH2_gc = (0x22<<0) ; ADC A Channel 2
.equ EVSYS_CHMUX_ADCA_CH3_gc = (0x23<<0) ; ADC A Channel 3
.equ EVSYS_CHMUX_ADCB_CH0_gc = (0x24<<0) ; ADC B Channel 0
.equ EVSYS_CHMUX_ADCB_CH1_gc = (0x25<<0) ; ADC B Channel 1
.equ EVSYS_CHMUX_ADCB_CH2_gc = (0x26<<0) ; ADC B Channel 2
.equ EVSYS_CHMUX_ADCB_CH3_gc = (0x27<<0) ; ADC B Channel 3
.equ EVSYS_CHMUX_PORTA_PIN0_gc = (0x50<<0) ; Port A, Pin0
.equ EVSYS_CHMUX_PORTA_PIN1_gc = (0x51<<0) ; Port A, Pin1
.equ EVSYS_CHMUX_PORTA_PIN2_gc = (0x52<<0) ; Port A, Pin2
.equ EVSYS_CHMUX_PORTA_PIN3_gc = (0x53<<0) ; Port A, Pin3
.equ EVSYS_CHMUX_PORTA_PIN4_gc = (0x54<<0) ; Port A, Pin4
.equ EVSYS_CHMUX_PORTA_PIN5_gc = (0x55<<0) ; Port A, Pin5
.equ EVSYS_CHMUX_PORTA_PIN6_gc = (0x56<<0) ; Port A, Pin6
.equ EVSYS_CHMUX_PORTA_PIN7_gc = (0x57<<0) ; Port A, Pin7
.equ EVSYS_CHMUX_PORTB_PIN0_gc = (0x58<<0) ; Port B, Pin0
.equ EVSYS_CHMUX_PORTB_PIN1_gc = (0x59<<0) ; Port B, Pin1
.equ EVSYS_CHMUX_PORTB_PIN2_gc = (0x5A<<0) ; Port B, Pin2
.equ EVSYS_CHMUX_PORTB_PIN3_gc = (0x5B<<0) ; Port B, Pin3
.equ EVSYS_CHMUX_PORTB_PIN4_gc = (0x5C<<0) ; Port B, Pin4
.equ EVSYS_CHMUX_PORTB_PIN5_gc = (0x5D<<0) ; Port B, Pin5
.equ EVSYS_CHMUX_PORTB_PIN6_gc = (0x5E<<0) ; Port B, Pin6
.equ EVSYS_CHMUX_PORTB_PIN7_gc = (0x5F<<0) ; Port B, Pin7
.equ EVSYS_CHMUX_PORTC_PIN0_gc = (0x60<<0) ; Port C, Pin0
.equ EVSYS_CHMUX_PORTC_PIN1_gc = (0x61<<0) ; Port C, Pin1
.equ EVSYS_CHMUX_PORTC_PIN2_gc = (0x62<<0) ; Port C, Pin2
.equ EVSYS_CHMUX_PORTC_PIN3_gc = (0x63<<0) ; Port C, Pin3
.equ EVSYS_CHMUX_PORTC_PIN4_gc = (0x64<<0) ; Port C, Pin4
.equ EVSYS_CHMUX_PORTC_PIN5_gc = (0x65<<0) ; Port C, Pin5
.equ EVSYS_CHMUX_PORTC_PIN6_gc = (0x66<<0) ; Port C, Pin6
.equ EVSYS_CHMUX_PORTC_PIN7_gc = (0x67<<0) ; Port C, Pin7
.equ EVSYS_CHMUX_PORTD_PIN0_gc = (0x68<<0) ; Port D, Pin0
.equ EVSYS_CHMUX_PORTD_PIN1_gc = (0x69<<0) ; Port D, Pin1
.equ EVSYS_CHMUX_PORTD_PIN2_gc = (0x6A<<0) ; Port D, Pin2
.equ EVSYS_CHMUX_PORTD_PIN3_gc = (0x6B<<0) ; Port D, Pin3
.equ EVSYS_CHMUX_PORTD_PIN4_gc = (0x6C<<0) ; Port D, Pin4
.equ EVSYS_CHMUX_PORTD_PIN5_gc = (0x6D<<0) ; Port D, Pin5
.equ EVSYS_CHMUX_PORTD_PIN6_gc = (0x6E<<0) ; Port D, Pin6
.equ EVSYS_CHMUX_PORTD_PIN7_gc = (0x6F<<0) ; Port D, Pin7
.equ EVSYS_CHMUX_PORTE_PIN0_gc = (0x70<<0) ; Port E, Pin0
.equ EVSYS_CHMUX_PORTE_PIN1_gc = (0x71<<0) ; Port E, Pin1
.equ EVSYS_CHMUX_PORTE_PIN2_gc = (0x72<<0) ; Port E, Pin2
.equ EVSYS_CHMUX_PORTE_PIN3_gc = (0x73<<0) ; Port E, Pin3
.equ EVSYS_CHMUX_PORTE_PIN4_gc = (0x74<<0) ; Port E, Pin4
.equ EVSYS_CHMUX_PORTE_PIN5_gc = (0x75<<0) ; Port E, Pin5
.equ EVSYS_CHMUX_PORTE_PIN6_gc = (0x76<<0) ; Port E, Pin6
.equ EVSYS_CHMUX_PORTE_PIN7_gc = (0x77<<0) ; Port E, Pin7
.equ EVSYS_CHMUX_PORTF_PIN0_gc = (0x78<<0) ; Port F, Pin0
.equ EVSYS_CHMUX_PORTF_PIN1_gc = (0x79<<0) ; Port F, Pin1
.equ EVSYS_CHMUX_PORTF_PIN2_gc = (0x7A<<0) ; Port F, Pin2
.equ EVSYS_CHMUX_PORTF_PIN3_gc = (0x7B<<0) ; Port F, Pin3
.equ EVSYS_CHMUX_PORTF_PIN4_gc = (0x7C<<0) ; Port F, Pin4
.equ EVSYS_CHMUX_PORTF_PIN5_gc = (0x7D<<0) ; Port F, Pin5
.equ EVSYS_CHMUX_PORTF_PIN6_gc = (0x7E<<0) ; Port F, Pin6
.equ EVSYS_CHMUX_PORTF_PIN7_gc = (0x7F<<0) ; Port F, Pin7
.equ EVSYS_CHMUX_PRESCALER_1_gc = (0x80<<0) ; Prescaler, divide by 1
.equ EVSYS_CHMUX_PRESCALER_2_gc = (0x81<<0) ; Prescaler, divide by 2
.equ EVSYS_CHMUX_PRESCALER_4_gc = (0x82<<0) ; Prescaler, divide by 4
.equ EVSYS_CHMUX_PRESCALER_8_gc = (0x83<<0) ; Prescaler, divide by 8
.equ EVSYS_CHMUX_PRESCALER_16_gc = (0x84<<0) ; Prescaler, divide by 16
.equ EVSYS_CHMUX_PRESCALER_32_gc = (0x85<<0) ; Prescaler, divide by 32
.equ EVSYS_CHMUX_PRESCALER_64_gc = (0x86<<0) ; Prescaler, divide by 64
.equ EVSYS_CHMUX_PRESCALER_128_gc = (0x87<<0) ; Prescaler, divide by 128
.equ EVSYS_CHMUX_PRESCALER_256_gc = (0x88<<0) ; Prescaler, divide by 256
.equ EVSYS_CHMUX_PRESCALER_512_gc = (0x89<<0) ; Prescaler, divide by 512
.equ EVSYS_CHMUX_PRESCALER_1024_gc = (0x8A<<0) ; Prescaler, divide by 1024
.equ EVSYS_CHMUX_PRESCALER_2048_gc = (0x8B<<0) ; Prescaler, divide by 2048
.equ EVSYS_CHMUX_PRESCALER_4096_gc = (0x8C<<0) ; Prescaler, divide by 4096
.equ EVSYS_CHMUX_PRESCALER_8192_gc = (0x8D<<0) ; Prescaler, divide by 8192
.equ EVSYS_CHMUX_PRESCALER_16384_gc = (0x8E<<0) ; Prescaler, divide by 16384
.equ EVSYS_CHMUX_PRESCALER_32768_gc = (0x8F<<0) ; Prescaler, divide by 32768
.equ EVSYS_CHMUX_TCC0_OVF_gc = (0xC0<<0) ; Timer/Counter C0 Overflow
.equ EVSYS_CHMUX_TCC0_ERR_gc = (0xC1<<0) ; Timer/Counter C0 Error
.equ EVSYS_CHMUX_TCC0_CCA_gc = (0xC4<<0) ; Timer/Counter C0 Compare or Capture A
.equ EVSYS_CHMUX_TCC0_CCB_gc = (0xC5<<0) ; Timer/Counter C0 Compare or Capture B
.equ EVSYS_CHMUX_TCC0_CCC_gc = (0xC6<<0) ; Timer/Counter C0 Compare or Capture C
.equ EVSYS_CHMUX_TCC0_CCD_gc = (0xC7<<0) ; Timer/Counter C0 Compare or Capture D
.equ EVSYS_CHMUX_TCC1_OVF_gc = (0xC8<<0) ; Timer/Counter C1 Overflow
.equ EVSYS_CHMUX_TCC1_ERR_gc = (0xC9<<0) ; Timer/Counter C1 Error
.equ EVSYS_CHMUX_TCC1_CCA_gc = (0xCC<<0) ; Timer/Counter C1 Compare or Capture A
.equ EVSYS_CHMUX_TCC1_CCB_gc = (0xCD<<0) ; Timer/Counter C1 Compare or Capture B
.equ EVSYS_CHMUX_TCD0_OVF_gc = (0xD0<<0) ; Timer/Counter D0 Overflow
.equ EVSYS_CHMUX_TCD0_ERR_gc = (0xD1<<0) ; Timer/Counter D0 Error
.equ EVSYS_CHMUX_TCD0_CCA_gc = (0xD4<<0) ; Timer/Counter D0 Compare or Capture A
.equ EVSYS_CHMUX_TCD0_CCB_gc = (0xD5<<0) ; Timer/Counter D0 Compare or Capture B
.equ EVSYS_CHMUX_TCD0_CCC_gc = (0xD6<<0) ; Timer/Counter D0 Compare or Capture C
.equ EVSYS_CHMUX_TCD0_CCD_gc = (0xD7<<0) ; Timer/Counter D0 Compare or Capture D
.equ EVSYS_CHMUX_TCD1_OVF_gc = (0xD8<<0) ; Timer/Counter D1 Overflow
.equ EVSYS_CHMUX_TCD1_ERR_gc = (0xD9<<0) ; Timer/Counter D1 Error
.equ EVSYS_CHMUX_TCD1_CCA_gc = (0xDC<<0) ; Timer/Counter D1 Compare or Capture A
.equ EVSYS_CHMUX_TCD1_CCB_gc = (0xDD<<0) ; Timer/Counter D1 Compare or Capture B
.equ EVSYS_CHMUX_TCE0_OVF_gc = (0xE0<<0) ; Timer/Counter E0 Overflow
.equ EVSYS_CHMUX_TCE0_ERR_gc = (0xE1<<0) ; Timer/Counter E0 Error
.equ EVSYS_CHMUX_TCE0_CCA_gc = (0xE4<<0) ; Timer/Counter E0 Compare or Capture A
.equ EVSYS_CHMUX_TCE0_CCB_gc = (0xE5<<0) ; Timer/Counter E0 Compare or Capture B
.equ EVSYS_CHMUX_TCE0_CCC_gc = (0xE6<<0) ; Timer/Counter E0 Compare or Capture C
.equ EVSYS_CHMUX_TCE0_CCD_gc = (0xE7<<0) ; Timer/Counter E0 Compare or Capture D
.equ EVSYS_CHMUX_TCE1_OVF_gc = (0xE8<<0) ; Timer/Counter E1 Overflow
.equ EVSYS_CHMUX_TCE1_ERR_gc = (0xE9<<0) ; Timer/Counter E1 Error
.equ EVSYS_CHMUX_TCE1_CCA_gc = (0xEC<<0) ; Timer/Counter E1 Compare or Capture A
.equ EVSYS_CHMUX_TCE1_CCB_gc = (0xED<<0) ; Timer/Counter E1 Compare or Capture B
.equ EVSYS_CHMUX_TCF0_OVF_gc = (0xF0<<0) ; Timer/Counter F0 Overflow
.equ EVSYS_CHMUX_TCF0_ERR_gc = (0xF1<<0) ; Timer/Counter F0 Error
.equ EVSYS_CHMUX_TCF0_CCA_gc = (0xF4<<0) ; Timer/Counter F0 Compare or Capture A
.equ EVSYS_CHMUX_TCF0_CCB_gc = (0xF5<<0) ; Timer/Counter F0 Compare or Capture B
.equ EVSYS_CHMUX_TCF0_CCC_gc = (0xF6<<0) ; Timer/Counter F0 Compare or Capture C
.equ EVSYS_CHMUX_TCF0_CCD_gc = (0xF7<<0) ; Timer/Counter F0 Compare or Capture D
.equ EVSYS_CHMUX_TCF1_OVF_gc = (0xF8<<0) ; Timer/Counter F1 Overflow
.equ EVSYS_CHMUX_TCF1_ERR_gc = (0xF9<<0) ; Timer/Counter F1 Error
.equ EVSYS_CHMUX_TCF1_CCA_gc = (0xFC<<0) ; Timer/Counter F1 Compare or Capture A
.equ EVSYS_CHMUX_TCF1_CCB_gc = (0xFD<<0) ; Timer/Counter F1 Compare or Capture B


;***************************************************************************
;** NVM - Non Volatile Memory Controller
;***************************************************************************/

; NVM_CMD masks
.equ NVM_CMD_gm = 0xFF ; Command
.equ NVM_CMD0_bm = (1<<0) ; Command bit 0
.equ NVM_CMD1_bm = (1<<1) ; Command bit 1
.equ NVM_CMD2_bm = (1<<2) ; Command bit 2
.equ NVM_CMD3_bm = (1<<3) ; Command bit 3
.equ NVM_CMD4_bm = (1<<4) ; Command bit 4
.equ NVM_CMD5_bm = (1<<5) ; Command bit 5
.equ NVM_CMD6_bm = (1<<6) ; Command bit 6
.equ NVM_CMD7_bm = (1<<7) ; Command bit 7

; NVM_CTRLA masks
.equ NVM_CMDEX_bm = 0x01 ; Command Execute

; NVM_CTRLB masks
.equ NVM_EEMAPEN_bm = 0x08 ; EEPROM Mapping Enable
.equ NVM_FPRM_bm = 0x04 ; Flash Power Reduction Enable
.equ NVM_EPRM_bm = 0x02 ; EEPROM Power Reduction Enable
.equ NVM_SPMLOCK_bm = 0x01 ; SPM Lock

; NVM_INTCTRL masks
.equ NVM_SPMLVL_gm = 0x0C ; SPM Interrupt Level
.equ NVM_SPMLVL0_bm = (1<<2) ; SPM Interrupt Level bit 0
.equ NVM_SPMLVL1_bm = (1<<3) ; SPM Interrupt Level bit 1
.equ NVM_EELVL_gm = 0x03 ; EEPROM Interrupt Level
.equ NVM_EELVL0_bm = (1<<0) ; EEPROM Interrupt Level bit 0
.equ NVM_EELVL1_bm = (1<<1) ; EEPROM Interrupt Level bit 1

; NVM_STATUS masks
.equ NVM_NVMBUSY_bm = 0x80 ; Non-volatile Memory Busy
.equ NVM_FBUSY_bm = 0x40 ; Flash Memory Busy
.equ NVM_EELOAD_bm = 0x02 ; EEPROM Page Buffer Active Loading
.equ NVM_FLOAD_bm = 0x01 ; Flash Page Buffer Active Loading

; NVM_LOCKBITS masks
.equ NVM_BLBB_gm = 0xC0 ; Boot Lock Bits - Boot Section
.equ NVM_BLBB0_bm = (1<<6) ; Boot Lock Bits - Boot Section bit 0
.equ NVM_BLBB1_bm = (1<<7) ; Boot Lock Bits - Boot Section bit 1
.equ NVM_BLBA_gm = 0x30 ; Boot Lock Bits - Application Section
.equ NVM_BLBA0_bm = (1<<4) ; Boot Lock Bits - Application Section bit 0
.equ NVM_BLBA1_bm = (1<<5) ; Boot Lock Bits - Application Section bit 1
.equ NVM_BLBAT_gm = 0x0C ; Boot Lock Bits - Application Table
.equ NVM_BLBAT0_bm = (1<<2) ; Boot Lock Bits - Application Table bit 0
.equ NVM_BLBAT1_bm = (1<<3) ; Boot Lock Bits - Application Table bit 1
.equ NVM_LB_gm = 0x03 ; Lock Bits
.equ NVM_LB0_bm = (1<<0) ; Lock Bits bit 0
.equ NVM_LB1_bm = (1<<1) ; Lock Bits bit 1

; NVM_LOCKBITS_LOCKBITS masks
.equ NVM_LOCKBITS_BLBB_gm = 0xC0 ; Boot Lock Bits - Boot Section
.equ NVM_LOCKBITS_BLBB0_bm = (1<<6) ; Boot Lock Bits - Boot Section bit 0
.equ NVM_LOCKBITS_BLBB1_bm = (1<<7) ; Boot Lock Bits - Boot Section bit 1
.equ NVM_LOCKBITS_BLBA_gm = 0x30 ; Boot Lock Bits - Application Section
.equ NVM_LOCKBITS_BLBA0_bm = (1<<4) ; Boot Lock Bits - Application Section bit 0
.equ NVM_LOCKBITS_BLBA1_bm = (1<<5) ; Boot Lock Bits - Application Section bit 1
.equ NVM_LOCKBITS_BLBAT_gm = 0x0C ; Boot Lock Bits - Application Table
.equ NVM_LOCKBITS_BLBAT0_bm = (1<<2) ; Boot Lock Bits - Application Table bit 0
.equ NVM_LOCKBITS_BLBAT1_bm = (1<<3) ; Boot Lock Bits - Application Table bit 1
.equ NVM_LOCKBITS_LB_gm = 0x03 ; Lock Bits
.equ NVM_LOCKBITS_LB0_bm = (1<<0) ; Lock Bits bit 0
.equ NVM_LOCKBITS_LB1_bm = (1<<1) ; Lock Bits bit 1

; NVM_FUSES_FUSEBYTE0 masks
.equ NVM_FUSES_JTAGUSERID_gm = 0xFF ; JTAG User ID
.equ NVM_FUSES_JTAGUSERID0_bm = (1<<0) ; JTAG User ID bit 0
.equ NVM_FUSES_JTAGUSERID1_bm = (1<<1) ; JTAG User ID bit 1
.equ NVM_FUSES_JTAGUSERID2_bm = (1<<2) ; JTAG User ID bit 2
.equ NVM_FUSES_JTAGUSERID3_bm = (1<<3) ; JTAG User ID bit 3
.equ NVM_FUSES_JTAGUSERID4_bm = (1<<4) ; JTAG User ID bit 4
.equ NVM_FUSES_JTAGUSERID5_bm = (1<<5) ; JTAG User ID bit 5
.equ NVM_FUSES_JTAGUSERID6_bm = (1<<6) ; JTAG User ID bit 6
.equ NVM_FUSES_JTAGUSERID7_bm = (1<<7) ; JTAG User ID bit 7

; NVM_FUSES_FUSEBYTE1 masks
.equ NVM_FUSES_WDWP_gm = 0xF0 ; Watchdog Window Timeout Period
.equ NVM_FUSES_WDWP0_bm = (1<<4) ; Watchdog Window Timeout Period bit 0
.equ NVM_FUSES_WDWP1_bm = (1<<5) ; Watchdog Window Timeout Period bit 1
.equ NVM_FUSES_WDWP2_bm = (1<<6) ; Watchdog Window Timeout Period bit 2
.equ NVM_FUSES_WDWP3_bm = (1<<7) ; Watchdog Window Timeout Period bit 3
.equ NVM_FUSES_WDP_gm = 0x0F ; Watchdog Timeout Period
.equ NVM_FUSES_WDP0_bm = (1<<0) ; Watchdog Timeout Period bit 0
.equ NVM_FUSES_WDP1_bm = (1<<1) ; Watchdog Timeout Period bit 1
.equ NVM_FUSES_WDP2_bm = (1<<2) ; Watchdog Timeout Period bit 2
.equ NVM_FUSES_WDP3_bm = (1<<3) ; Watchdog Timeout Period bit 3

; NVM_FUSES_FUSEBYTE2 masks
.equ NVM_FUSES_DVSDON_bm = 0x80 ; Spike Detector Enable
.equ NVM_FUSES_BOOTRST_bm = 0x40 ; Boot Loader Section Reset Vector
.equ NVM_FUSES_BODACT_gm = 0x0C ; BOD Operation in Active Mode
.equ NVM_FUSES_BODACT0_bm = (1<<2) ; BOD Operation in Active Mode bit 0
.equ NVM_FUSES_BODACT1_bm = (1<<3) ; BOD Operation in Active Mode bit 1
.equ NVM_FUSES_BODPD_gm = 0x03 ; BOD Operation in Power-Down Mode
.equ NVM_FUSES_BODPD0_bm = (1<<0) ; BOD Operation in Power-Down Mode bit 0
.equ NVM_FUSES_BODPD1_bm = (1<<1) ; BOD Operation in Power-Down Mode bit 1

; NVM_FUSES_FUSEBYTE4 masks
.equ NVM_FUSES_SUT_gm = 0x0C ; Start-up Time
.equ NVM_FUSES_SUT0_bm = (1<<2) ; Start-up Time bit 0
.equ NVM_FUSES_SUT1_bm = (1<<3) ; Start-up Time bit 1
.equ NVM_FUSES_WDLOCK_bm = 0x02 ; Watchdog Timer Lock
.equ NVM_FUSES_JTAGEN_bm = 0x01 ; JTAG Interface Enable

; NVM_FUSES_FUSEBYTE5 masks
.equ NVM_FUSES_EESAVE_bm = 0x08 ; Preserve EEPROM Through Chip Erase
.equ NVM_FUSES_BODLVL_gm = 0x07 ; Brown Out Detection Voltage Level
.equ NVM_FUSES_BODLVL0_bm = (1<<0) ; Brown Out Detection Voltage Level bit 0
.equ NVM_FUSES_BODLVL1_bm = (1<<1) ; Brown Out Detection Voltage Level bit 1
.equ NVM_FUSES_BODLVL2_bm = (1<<2) ; Brown Out Detection Voltage Level bit 2
 
; NVM Command
.equ NVM_CMD_NO_OPERATION_gc = (0x00<<0) ; Noop/Ordinary LPM
.equ NVM_CMD_READ_CALIB_ROW_gc = (0x02<<0) ; Read calibration row
.equ NVM_CMD_READ_USER_SIG_ROW_gc = (0x01<<0) ; Read user signature row
.equ NVM_CMD_READ_EEPROM_gc = (0x06<<0) ; Read EEPROM
.equ NVM_CMD_READ_FUSES_gc = (0x07<<0) ; Read fuse byte
.equ NVM_CMD_WRITE_LOCK_BITS_gc = (0x08<<0) ; Write lock bits
.equ NVM_CMD_ERASE_USER_SIG_ROW_gc = (0x18<<0) ; Erase user signature row
.equ NVM_CMD_WRITE_USER_SIG_ROW_gc = (0x1A<<0) ; Write user signature row
.equ NVM_CMD_ERASE_APP_gc = (0x20<<0) ; Erase Application Section
.equ NVM_CMD_ERASE_APP_PAGE_gc = (0x22<<0) ; Erase Application Section page
.equ NVM_CMD_LOAD_FLASH_BUFFER_gc = (0x23<<0) ; Load Flash page buffer
.equ NVM_CMD_WRITE_APP_PAGE_gc = (0x24<<0) ; Write Application Section page
.equ NVM_CMD_ERASE_WRITE_APP_PAGE_gc = (0x25<<0) ; Erase-and-write Application Section page
.equ NVM_CMD_ERASE_FLASH_BUFFER_gc = (0x26<<0) ; Erase/flush Flash page buffer
.equ NVM_CMD_ERASE_BOOT_PAGE_gc = (0x2A<<0) ; Erase Boot Section page
.equ NVM_CMD_WRITE_BOOT_PAGE_gc = (0x2C<<0) ; Write Boot Section page
.equ NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc = (0x2D<<0) ; Erase-and-write Boot Section page
.equ NVM_CMD_ERASE_EEPROM_gc = (0x30<<0) ; Erase EEPROM
.equ NVM_CMD_ERASE_EEPROM_PAGE_gc = (0x32<<0) ; Erase EEPROM page
.equ NVM_CMD_LOAD_EEPROM_BUFFER_gc = (0x33<<0) ; Load EEPROM page buffer
.equ NVM_CMD_WRITE_EEPROM_PAGE_gc = (0x34<<0) ; Write EEPROM page
.equ NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc = (0x35<<0) ; Erase-and-write EEPROM page
.equ NVM_CMD_ERASE_EEPROM_BUFFER_gc = (0x36<<0) ; Erase/flush EEPROM page buffer
.equ NVM_CMD_APP_CRC_gc = (0x38<<0) ; Generate Application section CRC
.equ NVM_CMD_BOOT_CRC_gc = (0x39<<0) ; Generate Boot Section CRC
.equ NVM_CMD_FLASH_RANGE_CRC_gc = (0x3A<<0) ; Generate Flash Range CRC

; SPM ready interrupt level
.equ NVM_SPMLVL_OFF_gc = (0x00<<2) ; Interrupt disabled
.equ NVM_SPMLVL_LO_gc = (0x01<<2) ; Low level
.equ NVM_SPMLVL_MED_gc = (0x02<<2) ; Medium level
.equ NVM_SPMLVL_HI_gc = (0x03<<2) ; High level

; EEPROM ready interrupt level
.equ NVM_EELVL_OFF_gc = (0x00<<0) ; Interrupt disabled
.equ NVM_EELVL_LO_gc = (0x01<<0) ; Low level
.equ NVM_EELVL_MED_gc = (0x02<<0) ; Medium level
.equ NVM_EELVL_HI_gc = (0x03<<0) ; High level

; Boot lock bits - boot setcion
.equ NVM_BLBB_NOLOCK_gc = (0x03<<6) ; No locks
.equ NVM_BLBB_WLOCK_gc = (0x02<<6) ; Write not allowed
.equ NVM_BLBB_RLOCK_gc = (0x01<<6) ; Read not allowed
.equ NVM_BLBB_RWLOCK_gc = (0x00<<6) ; Read and write not allowed

; Boot lock bits - application section
.equ NVM_BLBA_NOLOCK_gc = (0x03<<4) ; No locks
.equ NVM_BLBA_WLOCK_gc = (0x02<<4) ; Write not allowed
.equ NVM_BLBA_RLOCK_gc = (0x01<<4) ; Read not allowed
.equ NVM_BLBA_RWLOCK_gc = (0x00<<4) ; Read and write not allowed

; Boot lock bits - application table section
.equ NVM_BLBAT_NOLOCK_gc = (0x03<<2) ; No locks
.equ NVM_BLBAT_WLOCK_gc = (0x02<<2) ; Write not allowed
.equ NVM_BLBAT_RLOCK_gc = (0x01<<2) ; Read not allowed
.equ NVM_BLBAT_RWLOCK_gc = (0x00<<2) ; Read and write not allowed

; Lock bits
.equ NVM_LB_NOLOCK_gc = (0x03<<0) ; No locks
.equ NVM_LB_WLOCK_gc = (0x02<<0) ; Write not allowed
.equ NVM_LB_RWLOCK_gc = (0x00<<0) ; Read and write not allowed

; Boot Loader Section Reset Vector
.equ BOOTRST_BOOTLDR_gc = (0x00<<6) ; Boot Loader Reset
.equ BOOTRST_APPLICATION_gc = (0x01<<6) ; Application Reset

; BOD operation
.equ BOD_INSAMPLEDMODE_gc = (0x01<<2) ; BOD enabled in sampled mode
.equ BOD_CONTINOUSLY_gc = (0x02<<2) ; BOD enabled continuously
.equ BOD_DISABLED_gc = (0x03<<2) ; BOD Disabled

; Watchdog (Window) Timeout Period
.equ WD_8CLK_gc = (0x00<<4) ; 8 cycles (8ms @ 3.3V)
.equ WD_16CLK_gc = (0x01<<4) ; 16 cycles (16ms @ 3.3V)
.equ WD_32CLK_gc = (0x02<<4) ; 32 cycles (32ms @ 3.3V)
.equ WD_64CLK_gc = (0x03<<4) ; 64 cycles (64ms @ 3.3V)
.equ WD_128CLK_gc = (0x04<<4) ; 128 cycles (0.125s @ 3.3V)
.equ WD_256CLK_gc = (0x05<<4) ; 256 cycles (0.25s @ 3.3V)
.equ WD_512CLK_gc = (0x06<<4) ; 512 cycles (0.5s @ 3.3V)
.equ WD_1KCLK_gc = (0x07<<4) ; 1K cycles (1s @ 3.3V)
.equ WD_2KCLK_gc = (0x08<<4) ; 2K cycles (2s @ 3.3V)
.equ WD_4KCLK_gc = (0x09<<4) ; 4K cycles (4s @ 3.3V)
.equ WD_8KCLK_gc = (0x0A<<4) ; 8K cycles (8s @ 3.3V)

; Start-up Time
.equ SUT_0MS_gc = (0x03<<2) ; 0 ms
.equ SUT_4MS_gc = (0x01<<2) ; 4 ms
.equ SUT_64MS_gc = (0x00<<2) ; 64 ms

; Brown Out Detection Voltage Level
.equ BODLVL_1V6_gc = (0x07<<0) ; 1.6 V
.equ BODLVL_1V8_gc = (0x06<<0) ; 1.8 V
.equ BODLVL_2V0_gc = (0x05<<0) ; 2.0 V
.equ BODLVL_2V2_gc = (0x04<<0) ; 2.2 V
.equ BODLVL_2V4_gc = (0x03<<0) ; 2.4 V
.equ BODLVL_2V7_gc = (0x02<<0) ; 2.7 V
.equ BODLVL_2V9_gc = (0x01<<0) ; 2.9 V
.equ BODLVL_3V2_gc = (0x00<<0) ; 3.2 V


;***************************************************************************
;** AC - Analog Comparator
;***************************************************************************/

; AC_AC0CTRL masks
.equ AC_INTMODE_gm = 0xC0 ; Interrupt Mode
.equ AC_INTMODE0_bm = (1<<6) ; Interrupt Mode bit 0
.equ AC_INTMODE1_bm = (1<<7) ; Interrupt Mode bit 1
.equ AC_INTLVL_gm = 0x30 ; Interrupt Level
.equ AC_INTLVL0_bm = (1<<4) ; Interrupt Level bit 0
.equ AC_INTLVL1_bm = (1<<5) ; Interrupt Level bit 1
.equ AC_HSMODE_bm = 0x08 ; High-speed Mode
.equ AC_HYSMODE_gm = 0x06 ; Hysteresis Mode
.equ AC_HYSMODE0_bm = (1<<1) ; Hysteresis Mode bit 0
.equ AC_HYSMODE1_bm = (1<<2) ; Hysteresis Mode bit 1
.equ AC_ENABLE_bm = 0x01 ; Enable

; AC_AC1CTRL masks
; Masks for INTMODE aready defined
; Masks for INTLVL aready defined
; Masks for HSMODE aready defined
; Masks for HYSMODE aready defined
; Masks for ENABLE aready defined

; AC_AC0MUXCTRL masks
.equ AC_MUXPOS_gm = 0x38 ; MUX Positive Input
.equ AC_MUXPOS0_bm = (1<<3) ; MUX Positive Input bit 0
.equ AC_MUXPOS1_bm = (1<<4) ; MUX Positive Input bit 1
.equ AC_MUXPOS2_bm = (1<<5) ; MUX Positive Input bit 2
.equ AC_MUXNEG_gm = 0x07 ; MUX Negative Input
.equ AC_MUXNEG0_bm = (1<<0) ; MUX Negative Input bit 0
.equ AC_MUXNEG1_bm = (1<<1) ; MUX Negative Input bit 1
.equ AC_MUXNEG2_bm = (1<<2) ; MUX Negative Input bit 2

; AC_AC1MUXCTRL masks
; Masks for MUXPOS aready defined
; Masks for MUXNEG aready defined

; AC_CTRLA masks
.equ AC_AC0OUT_bm = 0x01 ; Comparator 0 Output Enable

; AC_CTRLB masks
.equ AC_SCALEFAC_gm = 0x3F ; VCC Voltage Scaler Factor
.equ AC_SCALEFAC0_bm = (1<<0) ; VCC Voltage Scaler Factor bit 0
.equ AC_SCALEFAC1_bm = (1<<1) ; VCC Voltage Scaler Factor bit 1
.equ AC_SCALEFAC2_bm = (1<<2) ; VCC Voltage Scaler Factor bit 2
.equ AC_SCALEFAC3_bm = (1<<3) ; VCC Voltage Scaler Factor bit 3
.equ AC_SCALEFAC4_bm = (1<<4) ; VCC Voltage Scaler Factor bit 4
.equ AC_SCALEFAC5_bm = (1<<5) ; VCC Voltage Scaler Factor bit 5

; AC_WINCTRL masks
.equ AC_WEN_bm = 0x10 ; Window Mode Enable
.equ AC_WINTMODE_gm = 0x0C ; Window Interrupt Mode
.equ AC_WINTMODE0_bm = (1<<2) ; Window Interrupt Mode bit 0
.equ AC_WINTMODE1_bm = (1<<3) ; Window Interrupt Mode bit 1
.equ AC_WINTLVL_gm = 0x03 ; Window Interrupt Level
.equ AC_WINTLVL0_bm = (1<<0) ; Window Interrupt Level bit 0
.equ AC_WINTLVL1_bm = (1<<1) ; Window Interrupt Level bit 1

; AC_STATUS masks
.equ AC_WSTATE_gm = 0xC0 ; Window Mode State
.equ AC_WSTATE0_bm = (1<<6) ; Window Mode State bit 0
.equ AC_WSTATE1_bm = (1<<7) ; Window Mode State bit 1
.equ AC_AC1STATE_bm = 0x20 ; Comparator 1 State
.equ AC_AC0STATE_bm = 0x10 ; Comparator 0 State
.equ AC_WIF_bm = 0x04 ; Window Mode Interrupt Flag
.equ AC_AC1IF_bm = 0x02 ; Comparator 1 Interrupt Flag
.equ AC_AC0IF_bm = 0x01 ; Comparator 0 Interrupt Flag
 
; Interrupt mode
.equ AC_INTMODE_BOTHEDGES_gc = (0x00<<6) ; Interrupt on both edges
.equ AC_INTMODE_FALLING_gc = (0x02<<6) ; Interrupt on falling edge
.equ AC_INTMODE_RISING_gc = (0x03<<6) ; Interrupt on rising edge

; Interrupt level
.equ AC_INTLVL_OFF_gc = (0x00<<4) ; Interrupt disabled
.equ AC_INTLVL_LO_gc = (0x01<<4) ; Low level
.equ AC_INTLVL_MED_gc = (0x02<<4) ; Medium level
.equ AC_INTLVL_HI_gc = (0x03<<4) ; High level

; Hysteresis mode selection
.equ AC_HYSMODE_NO_gc = (0x00<<1) ; No hysteresis
.equ AC_HYSMODE_SMALL_gc = (0x01<<1) ; Small hysteresis
.equ AC_HYSMODE_LARGE_gc = (0x02<<1) ; Large hysteresis

; Positive input multiplexer selection
.equ AC_MUXPOS_PIN0_gc = (0x00<<3) ; Pin 0
.equ AC_MUXPOS_PIN1_gc = (0x01<<3) ; Pin 1
.equ AC_MUXPOS_PIN2_gc = (0x02<<3) ; Pin 2
.equ AC_MUXPOS_PIN3_gc = (0x03<<3) ; Pin 3
.equ AC_MUXPOS_PIN4_gc = (0x04<<3) ; Pin 4
.equ AC_MUXPOS_PIN5_gc = (0x05<<3) ; Pin 5
.equ AC_MUXPOS_PIN6_gc = (0x06<<3) ; Pin 6
.equ AC_MUXPOS_DAC_gc = (0x07<<3) ; DAC output

; Negative input multiplexer selection
.equ AC_MUXNEG_PIN0_gc = (0x00<<0) ; Pin 0
.equ AC_MUXNEG_PIN1_gc = (0x01<<0) ; Pin 1
.equ AC_MUXNEG_PIN3_gc = (0x02<<0) ; Pin 3
.equ AC_MUXNEG_PIN5_gc = (0x03<<0) ; Pin 5
.equ AC_MUXNEG_PIN7_gc = (0x04<<0) ; Pin 7
.equ AC_MUXNEG_DAC_gc = (0x05<<0) ; DAC output
.equ AC_MUXNEG_BANDGAP_gc = (0x06<<0) ; Bandgap Reference
.equ AC_MUXNEG_SCALER_gc = (0x07<<0) ; Internal voltage scaler

; Windows interrupt mode
.equ AC_WINTMODE_ABOVE_gc = (0x00<<2) ; Interrupt on above window
.equ AC_WINTMODE_INSIDE_gc = (0x01<<2) ; Interrupt on inside window
.equ AC_WINTMODE_BELOW_gc = (0x02<<2) ; Interrupt on below window
.equ AC_WINTMODE_OUTSIDE_gc = (0x03<<2) ; Interrupt on outside window

; Window interrupt level
.equ AC_WINTLVL_OFF_gc = (0x00<<0) ; Interrupt disabled
.equ AC_WINTLVL_LO_gc = (0x01<<0) ; Low priority
.equ AC_WINTLVL_MED_gc = (0x02<<0) ; Medium priority
.equ AC_WINTLVL_HI_gc = (0x03<<0) ; High priority

; Window mode state
.equ AC_WSTATE_ABOVE_gc = (0x00<<6) ; Signal above window
.equ AC_WSTATE_INSIDE_gc = (0x01<<6) ; Signal inside window
.equ AC_WSTATE_BELOW_gc = (0x02<<6) ; Signal below window


;***************************************************************************
;** ADC - Analog/Digital Converter
;***************************************************************************/

; ADC_CH_CTRL masks
.equ ADC_CH_START_bm = 0x80 ; Channel Start Conversion
.equ ADC_CH_GAINFAC_gm = 0x1C ; Gain Factor
.equ ADC_CH_GAINFAC0_bm = (1<<2) ; Gain Factor bit 0
.equ ADC_CH_GAINFAC1_bm = (1<<3) ; Gain Factor bit 1
.equ ADC_CH_GAINFAC2_bm = (1<<4) ; Gain Factor bit 2
.equ ADC_CH_INPUTMODE_gm = 0x03 ; Input Mode Select
.equ ADC_CH_INPUTMODE0_bm = (1<<0) ; Input Mode Select bit 0
.equ ADC_CH_INPUTMODE1_bm = (1<<1) ; Input Mode Select bit 1

; ADC_CH_MUXCTRL masks
.equ ADC_CH_MUXPOS_gm = 0x78 ; Positive Input Select
.equ ADC_CH_MUXPOS0_bm = (1<<3) ; Positive Input Select bit 0
.equ ADC_CH_MUXPOS1_bm = (1<<4) ; Positive Input Select bit 1
.equ ADC_CH_MUXPOS2_bm = (1<<5) ; Positive Input Select bit 2
.equ ADC_CH_MUXPOS3_bm = (1<<6) ; Positive Input Select bit 3
.equ ADC_CH_MUXINT_gm = 0x78 ; Internal Input Select
.equ ADC_CH_MUXINT0_bm = (1<<3) ; Internal Input Select bit 0
.equ ADC_CH_MUXINT1_bm = (1<<4) ; Internal Input Select bit 1
.equ ADC_CH_MUXINT2_bm = (1<<5) ; Internal Input Select bit 2
.equ ADC_CH_MUXINT3_bm = (1<<6) ; Internal Input Select bit 3
.equ ADC_CH_MUXNEG_gm = 0x03 ; Negative Input Select
.equ ADC_CH_MUXNEG0_bm = (1<<0) ; Negative Input Select bit 0
.equ ADC_CH_MUXNEG1_bm = (1<<1) ; Negative Input Select bit 1

; ADC_CH_INTCTRL masks
.equ ADC_CH_INTMODE_gm = 0x0C ; Interrupt Mode
.equ ADC_CH_INTMODE0_bm = (1<<2) ; Interrupt Mode bit 0
.equ ADC_CH_INTMODE1_bm = (1<<3) ; Interrupt Mode bit 1
.equ ADC_CH_INTLVL_gm = 0x03 ; Interrupt Level
.equ ADC_CH_INTLVL0_bm = (1<<0) ; Interrupt Level bit 0
.equ ADC_CH_INTLVL1_bm = (1<<1) ; Interrupt Level bit 1

; ADC_CH_INTFLAGS masks
.equ ADC_CH_CHIF_bm = 0x01 ; Channel Interrupt Flag

; ADC_CTRLA masks
.equ ADC_DMASEL_gm = 0xE0 ; DMA Selection
.equ ADC_DMASEL0_bm = (1<<5) ; DMA Selection bit 0
.equ ADC_DMASEL1_bm = (1<<6) ; DMA Selection bit 1
.equ ADC_DMASEL2_bm = (1<<7) ; DMA Selection bit 2
.equ ADC_CH3START_bm = 0x20 ; Channel 3 Start Conversion
.equ ADC_CH2START_bm = 0x10 ; Channel 2 Start Conversion
.equ ADC_CH1START_bm = 0x08 ; Channel 1 Start Conversion
.equ ADC_CH0START_bm = 0x04 ; Channel 0 Start Conversion
.equ ADC_FLUSH_bm = 0x02 ; Flush Pipeline
.equ ADC_ENABLE_bm = 0x01 ; Enable ADC

; ADC_CTRLB masks
.equ ADC_CONMODE_bm = 0x10 ; Conversion Mode
.equ ADC_FREERUN_bm = 0x08 ; Free Running Mode Enable
.equ ADC_RESOLUTION_gm = 0x06 ; Result Resolution
.equ ADC_RESOLUTION0_bm = (1<<1) ; Result Resolution bit 0
.equ ADC_RESOLUTION1_bm = (1<<2) ; Result Resolution bit 1

; ADC_REFCTRL masks
.equ ADC_REFSEL_gm = 0x30 ; Reference Selection
.equ ADC_REFSEL0_bm = (1<<4) ; Reference Selection bit 0
.equ ADC_REFSEL1_bm = (1<<5) ; Reference Selection bit 1
.equ ADC_BANDGAP_bm = 0x02 ; Bandgap enable
.equ ADC_TEMPREF_bm = 0x01 ; Temperature Reference Enable

; ADC_EVCTRL masks
.equ ADC_SWEEP_gm = 0xC0 ; Channel Sweep Selection
.equ ADC_SWEEP0_bm = (1<<6) ; Channel Sweep Selection bit 0
.equ ADC_SWEEP1_bm = (1<<7) ; Channel Sweep Selection bit 1
.equ ADC_EVSEL_gm = 0x38 ; Event Input Select
.equ ADC_EVSEL0_bm = (1<<3) ; Event Input Select bit 0
.equ ADC_EVSEL1_bm = (1<<4) ; Event Input Select bit 1
.equ ADC_EVSEL2_bm = (1<<5) ; Event Input Select bit 2
.equ ADC_EVACT_gm = 0x07 ; Event Action Select
.equ ADC_EVACT0_bm = (1<<0) ; Event Action Select bit 0
.equ ADC_EVACT1_bm = (1<<1) ; Event Action Select bit 1
.equ ADC_EVACT2_bm = (1<<2) ; Event Action Select bit 2

; ADC_PRESCALER masks
.equ ADC_PRESCALER_gm = 0x07 ; Clock Prescaler Selection
.equ ADC_PRESCALER0_bm = (1<<0) ; Clock Prescaler Selection bit 0
.equ ADC_PRESCALER1_bm = (1<<1) ; Clock Prescaler Selection bit 1
.equ ADC_PRESCALER2_bm = (1<<2) ; Clock Prescaler Selection bit 2

; ADC_CALCTRL masks
.equ ADC_CAL_bm = 0x01 ; ADC Calibration Start

; ADC_INTFLAGS masks
.equ ADC_CH3IF_bm = 0x08 ; Channel 3 Interrupt Flag
.equ ADC_CH2IF_bm = 0x04 ; Channel 2 Interrupt Flag
.equ ADC_CH1IF_bm = 0x02 ; Channel 1 Interrupt Flag
.equ ADC_CH0IF_bm = 0x01 ; Channel 0 Interrupt Flag
 
; Positive input multiplexer selection
.equ ADC_CH_MUXPOS_PIN0_gc = (0x00<<3) ; Input pin 0
.equ ADC_CH_MUXPOS_PIN1_gc = (0x01<<3) ; Input pin 1
.equ ADC_CH_MUXPOS_PIN2_gc = (0x02<<3) ; Input pin 2
.equ ADC_CH_MUXPOS_PIN3_gc = (0x03<<3) ; Input pin 3
.equ ADC_CH_MUXPOS_PIN4_gc = (0x04<<3) ; Input pin 4
.equ ADC_CH_MUXPOS_PIN5_gc = (0x05<<3) ; Input pin 5
.equ ADC_CH_MUXPOS_PIN6_gc = (0x06<<3) ; Input pin 6
.equ ADC_CH_MUXPOS_PIN7_gc = (0x07<<3) ; Input pin 7

; Internal input multiplexer selections
.equ ADC_CH_MUXINT_TEMP_gc = (0x00<<3) ; Temperature Reference
.equ ADC_CH_MUXINT_BANDGAP_gc = (0x01<<3) ; Bandgap Reference
.equ ADC_CH_MUXINT_SCALEDVCC_gc = (0x02<<3) ; 1/10 scaled VCC
.equ ADC_CH_MUXINT_DAC_gc = (0x03<<3) ; DAC output

; Negative input multiplexer selection
.equ ADC_CH_MUXNEG_PIN0_gc = (0x00<<0) ; Input pin 0
.equ ADC_CH_MUXNEG_PIN1_gc = (0x01<<0) ; Input pin 1
.equ ADC_CH_MUXNEG_PIN2_gc = (0x02<<0) ; Input pin 2
.equ ADC_CH_MUXNEG_PIN3_gc = (0x03<<0) ; Input pin 3
.equ ADC_CH_MUXNEG_PIN4_gc = (0x04<<0) ; Input pin 4
.equ ADC_CH_MUXNEG_PIN5_gc = (0x05<<0) ; Input pin 5
.equ ADC_CH_MUXNEG_PIN6_gc = (0x06<<0) ; Input pin 6
.equ ADC_CH_MUXNEG_PIN7_gc = (0x07<<0) ; Input pin 7

; Input mode
.equ ADC_CH_INPUTMODE_INTERNAL_gc = (0x00<<0) ; Internal inputs, no gain
.equ ADC_CH_INPUTMODE_SINGLEENDED_gc = (0x01<<0) ; Single-ended input, no gain
.equ ADC_CH_INPUTMODE_DIFF_gc = (0x02<<0) ; Differential input, no gain
.equ ADC_CH_INPUTMODE_DIFFWGAIN_gc = (0x03<<0) ; Differential input, with gain

; Gain factor
.equ ADC_CH_GAIN_1X_gc = (0x00<<2) ; 1x gain
.equ ADC_CH_GAIN_2X_gc = (0x01<<2) ; 2x gain
.equ ADC_CH_GAIN_4X_gc = (0x02<<2) ; 4x gain
.equ ADC_CH_GAIN_8X_gc = (0x03<<2) ; 8x gain
.equ ADC_CH_GAIN_16X_gc = (0x04<<2) ; 16x gain
.equ ADC_CH_GAIN_32X_gc = (0x05<<2) ; 32x gain
.equ ADC_CH_GAIN_64X_gc = (0x06<<2) ; 64x gain

; Conversion result resolution
.equ ADC_RESOLUTION_12BIT_gc = (0x00<<1) ; 12-bit right-adjusted result
.equ ADC_RESOLUTION_8BIT_gc = (0x02<<1) ; 8-bit right-adjusted result
.equ ADC_RESOLUTION_LEFT12BIT_gc = (0x03<<1) ; 12-bit left-adjusted result

; Voltage reference selection
.equ ADC_REFSEL_INT1V_gc = (0x00<<4) ; Internal 1V
.equ ADC_REFSEL_VCC_gc = (0x01<<4) ; Internal VCC / 1.6V
.equ ADC_REFSEL_AREFA_gc = (0x02<<4) ; External reference on PORT A
.equ ADC_REFSEL_AREFB_gc = (0x03<<4) ; External reference on PORT B

; Channel sweep selection
.equ ADC_SWEEP_0_gc = (0x00<<6) ; ADC Channel 0
.equ ADC_SWEEP_01_gc = (0x01<<6) ; ADC Channel 0,1
.equ ADC_SWEEP_012_gc = (0x02<<6) ; ADC Channel 0,1,2
.equ ADC_SWEEP_0123_gc = (0x03<<6) ; ADC Channel 0,1,2,3

; Event channel input selection
.equ ADC_EVSEL_0123_gc = (0x00<<3) ; Event Channel 0,1,2,3
.equ ADC_EVSEL_1234_gc = (0x01<<3) ; Event Channel 1,2,3,4
.equ ADC_EVSEL_2345_gc = (0x02<<3) ; Event Channel 2,3,4,5
.equ ADC_EVSEL_3456_gc = (0x03<<3) ; Event Channel 3,4,5,6
.equ ADC_EVSEL_4567_gc = (0x04<<3) ; Event Channel 4,5,6,7
.equ ADC_EVSEL_567_gc = (0x05<<3) ; Event Channel 5,6,7
.equ ADC_EVSEL_67_gc = (0x06<<3) ; Event Channel 6,7
.equ ADC_EVSEL_7_gc = (0x07<<3) ; Event Channel 7

; Event action selection
.equ ADC_EVACT_NONE_gc = (0x00<<0) ; No event action
.equ ADC_EVACT_CH0_gc = (0x01<<0) ; First event triggers channel 0
.equ ADC_EVACT_CH01_gc = (0x02<<0) ; First two events trigger channel 0,1
.equ ADC_EVACT_CH012_gc = (0x03<<0) ; First three events trigger channel 0,1,2
.equ ADC_EVACT_CH0123_gc = (0x04<<0) ; Events trigger channel 0,1,2,3
.equ ADC_EVACT_SWEEP_gc = (0x05<<0) ; First event triggers sweep
.equ ADC_EVACT_SYNCHSWEEP_gc = (0x06<<0) ; First event triggers synchronized sweep

; Interupt mode
.equ ADC_CH_INTMODE_COMPLETE_gc = (0x00<<2) ; Interrupt on conversion complete
.equ ADC_CH_INTMODE_BELOW_gc = (0x01<<2) ; Interrupt on result below compare value
.equ ADC_CH_INTMODE_ABOVE_gc = (0x03<<2) ; Interrupt on result above compare value

; Interrupt level
.equ ADC_CH_INTLVL_OFF_gc = (0x00<<0) ; Interrupt disabled
.equ ADC_CH_INTLVL_LO_gc = (0x01<<0) ; Low level
.equ ADC_CH_INTLVL_MED_gc = (0x02<<0) ; Medium level
.equ ADC_CH_INTLVL_HI_gc = (0x03<<0) ; High level

; DMA request selection
.equ ADC_DMASEL_OFF_gc = (0x00<<5) ; Combined DMA request OFF
.equ ADC_DMASEL_CH01_gc = (0x01<<5) ; ADC Channel 0 or 1
.equ ADC_DMASEL_CH012_gc = (0x02<<5) ; ADC Channel 0 or 1 or 2
.equ ADC_DMASEL_CH0123_gc = (0x03<<5) ; ADC Channel 0 or 1 or 2 or 3

; Clock prescaler
.equ ADC_PRESCALER_DIV4_gc = (0x00<<0) ; Divide clock by 4
.equ ADC_PRESCALER_DIV8_gc = (0x01<<0) ; Divide clock by 8
.equ ADC_PRESCALER_DIV16_gc = (0x02<<0) ; Divide clock by 16
.equ ADC_PRESCALER_DIV32_gc = (0x03<<0) ; Divide clock by 32
.equ ADC_PRESCALER_DIV64_gc = (0x04<<0) ; Divide clock by 64
.equ ADC_PRESCALER_DIV128_gc = (0x05<<0) ; Divide clock by 128
.equ ADC_PRESCALER_DIV256_gc = (0x06<<0) ; Divide clock by 256
.equ ADC_PRESCALER_DIV512_gc = (0x07<<0) ; Divide clock by 512


;***************************************************************************
;** DAC - Digital/Analog Converter
;***************************************************************************/

; DAC_CTRLA masks
.equ DAC_IDOEN_bm = 0x10 ; Internal Output Enable
.equ DAC_CH1EN_bm = 0x08 ; Channel 1 Output Enable
.equ DAC_CH0EN_bm = 0x04 ; Channel 0 Output Enable
.equ DAC_LPMODE_bm = 0x02 ; Low Power Mode
.equ DAC_ENABLE_bm = 0x01 ; Enable

; DAC_CTRLB masks
.equ DAC_CHSEL_gm = 0x60 ; Channel Select
.equ DAC_CHSEL0_bm = (1<<5) ; Channel Select bit 0
.equ DAC_CHSEL1_bm = (1<<6) ; Channel Select bit 1
.equ DAC_CH1TRIG_bm = 0x02 ; Channel 1 Event Trig Enable
.equ DAC_CH0TRIG_bm = 0x01 ; Channel 0 Event Trig Enable

; DAC_CTRLC masks
.equ DAC_REFSEL_gm = 0x18 ; Reference Select
.equ DAC_REFSEL0_bm = (1<<3) ; Reference Select bit 0
.equ DAC_REFSEL1_bm = (1<<4) ; Reference Select bit 1
.equ DAC_LEFTADJ_bm = 0x01 ; Left-adjust Result

; DAC_EVCTRL masks
.equ DAC_EVSEL_gm = 0x07 ; Event Input Selection
.equ DAC_EVSEL0_bm = (1<<0) ; Event Input Selection bit 0
.equ DAC_EVSEL1_bm = (1<<1) ; Event Input Selection bit 1
.equ DAC_EVSEL2_bm = (1<<2) ; Event Input Selection bit 2

; DAC_TIMCTRL masks
.equ DAC_CONINTVAL_gm = 0x70 ; Conversion Intercal
.equ DAC_CONINTVAL0_bm = (1<<4) ; Conversion Intercal bit 0
.equ DAC_CONINTVAL1_bm = (1<<5) ; Conversion Intercal bit 1
.equ DAC_CONINTVAL2_bm = (1<<6) ; Conversion Intercal bit 2
.equ DAC_REFRESH_gm = 0x0F ; Refresh Timing Control
.equ DAC_REFRESH0_bm = (1<<0) ; Refresh Timing Control bit 0
.equ DAC_REFRESH1_bm = (1<<1) ; Refresh Timing Control bit 1
.equ DAC_REFRESH2_bm = (1<<2) ; Refresh Timing Control bit 2
.equ DAC_REFRESH3_bm = (1<<3) ; Refresh Timing Control bit 3

; DAC_STATUS masks
.equ DAC_CH1DRE_bm = 0x02 ; Channel 1 Data Register Empty
.equ DAC_CH0DRE_bm = 0x01 ; Channel 0 Data Register Empty
 
; Output channel selection
.equ DAC_CHSEL_SINGLE_gc = (0x00<<5) ; Single channel operation (Channel A only)
.equ DAC_CHSEL_DUAL_gc = (0x02<<5) ; Dual channel operation (S/H on both channels)

; Reference voltage selection
.equ DAC_REFSEL_INT1V_gc = (0x00<<3) ; Internal 1V 
.equ DAC_REFSEL_AVCC_gc = (0x01<<3) ; Analog supply voltage
.equ DAC_REFSEL_AREFA_gc = (0x02<<3) ; External reference on AREF on PORTA
.equ DAC_REFSEL_AREFB_gc = (0x03<<3) ; External reference on AREF on PORTB

; Event channel selection
.equ DAC_EVSEL_0_gc = (0x00<<0) ; Event Channel 0
.equ DAC_EVSEL_1_gc = (0x01<<0) ; Event Channel 1
.equ DAC_EVSEL_2_gc = (0x02<<0) ; Event Channel 2
.equ DAC_EVSEL_3_gc = (0x03<<0) ; Event Channel 3
.equ DAC_EVSEL_4_gc = (0x04<<0) ; Event Channel 4
.equ DAC_EVSEL_5_gc = (0x05<<0) ; Event Channel 5
.equ DAC_EVSEL_6_gc = (0x06<<0) ; Event Channel 6
.equ DAC_EVSEL_7_gc = (0x07<<0) ; Event Channel 7

; Conversion interval
.equ DAC_CONINTVAL_1CLK_gc = (0x00<<4) ; 1 CLK / 2 CLK in S/H mode
.equ DAC_CONINTVAL_2CLK_gc = (0x01<<4) ; 2 CLK / 3 CLK in S/H mode
.equ DAC_CONINTVAL_4CLK_gc = (0x02<<4) ; 4 CLK / 6 CLK in S/H mode
.equ DAC_CONINTVAL_8CLK_gc = (0x03<<4) ; 8 CLK / 12 CLK in S/H mode
.equ DAC_CONINTVAL_16CLK_gc = (0x04<<4) ; 16 CLK / 24 CLK in S/H mode
.equ DAC_CONINTVAL_32CLK_gc = (0x05<<4) ; 32 CLK / 48 CLK in S/H mode
.equ DAC_CONINTVAL_64CLK_gc = (0x06<<4) ; 64 CLK / 96 CLK in S/H mode
.equ DAC_CONINTVAL_128CLK_gc = (0x07<<4) ; 128 CLK / 192 CLK in S/H mode

; Refresh rate
.equ DAC_REFRESH_16CLK_gc = (0x00<<0) ; 16 CLK
.equ DAC_REFRESH_32CLK_gc = (0x01<<0) ; 32 CLK
.equ DAC_REFRESH_64CLK_gc = (0x02<<0) ; 64 CLK
.equ DAC_REFRESH_128CLK_gc = (0x03<<0) ; 128 CLK
.equ DAC_REFRESH_256CLK_gc = (0x04<<0) ; 256 CLK
.equ DAC_REFRESH_512CLK_gc = (0x05<<0) ; 512 CLK
.equ DAC_REFRESH_1024CLK_gc = (0x06<<0) ; 1024 CLK
.equ DAC_REFRESH_2048CLK_gc = (0x07<<0) ; 2048 CLK
.equ DAC_REFRESH_4086CLK_gc = (0x08<<0) ; 4096 CLK
.equ DAC_REFRESH_8192CLK_gc = (0x09<<0) ; 8192 CLK
.equ DAC_REFRESH_16384CLK_gc = (0x0A<<0) ; 16384 CLK
.equ DAC_REFRESH_32768CLK_gc = (0x0B<<0) ; 32768 CLK
.equ DAC_REFRESH_65536CLK_gc = (0x0C<<0) ; 65536 CLK
.equ DAC_REFRESH_OFF_gc = (0x0F<<0) ; Auto refresh OFF


;***************************************************************************
;** RTC - Real-Time Clounter
;***************************************************************************/

; RTC_CTRL masks
.equ RTC_PRESCALER_gm = 0x07 ; Prescaling Factor
.equ RTC_PRESCALER0_bm = (1<<0) ; Prescaling Factor bit 0
.equ RTC_PRESCALER1_bm = (1<<1) ; Prescaling Factor bit 1
.equ RTC_PRESCALER2_bm = (1<<2) ; Prescaling Factor bit 2

; RTC_STATUS masks
.equ RTC_SYNCBUSY_bm = 0x01 ; Synchronization Busy Flag

; RTC_INTCTRL masks
.equ RTC_COMPINTLVL_gm = 0x0C ; Compare Match Interrupt Level
.equ RTC_COMPINTLVL0_bm = (1<<2) ; Compare Match Interrupt Level bit 0
.equ RTC_COMPINTLVL1_bm = (1<<3) ; Compare Match Interrupt Level bit 1
.equ RTC_OVFINTLVL_gm = 0x03 ; Overflow Interrupt Level
.equ RTC_OVFINTLVL0_bm = (1<<0) ; Overflow Interrupt Level bit 0
.equ RTC_OVFINTLVL1_bm = (1<<1) ; Overflow Interrupt Level bit 1

; RTC_INTFLAGS masks
.equ RTC_COMPIF_bm = 0x02 ; Compare Match Interrupt Flag
.equ RTC_OVFIF_bm = 0x01 ; Overflow Interrupt Flag
 
; Prescaler Factor
.equ RTC_PRESCALER_OFF_gc = (0x00<<0) ; RTC Off
.equ RTC_PRESCALER_DIV1_gc = (0x01<<0) ; RTC Clock
.equ RTC_PRESCALER_DIV2_gc = (0x02<<0) ; RTC Clock / 2
.equ RTC_PRESCALER_DIV8_gc = (0x03<<0) ; RTC Clock / 8
.equ RTC_PRESCALER_DIV16_gc = (0x04<<0) ; RTC Clock / 16
.equ RTC_PRESCALER_DIV64_gc = (0x05<<0) ; RTC Clock / 64
.equ RTC_PRESCALER_DIV256_gc = (0x06<<0) ; RTC Clock / 256
.equ RTC_PRESCALER_DIV1024_gc = (0x07<<0) ; RTC Clock / 1024

; Compare Interrupt level
.equ RTC_COMPINTLVL_OFF_gc = (0x00<<2) ; Interrupt Disabled
.equ RTC_COMPINTLVL_LO_gc = (0x01<<2) ; Low Level
.equ RTC_COMPINTLVL_MED_gc = (0x02<<2) ; Medium Level
.equ RTC_COMPINTLVL_HI_gc = (0x03<<2) ; High Level

; Overflow Interrupt level
.equ RTC_OVFINTLVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ RTC_OVFINTLVL_LO_gc = (0x01<<0) ; Low Level
.equ RTC_OVFINTLVL_MED_gc = (0x02<<0) ; Medium Level
.equ RTC_OVFINTLVL_HI_gc = (0x03<<0) ; High Level


;***************************************************************************
;** EBI - External Bus Interface
;***************************************************************************/

; EBI_CS_CTRLA masks
.equ EBI_CS_ASPACE_gm = 0x7C ; Address Space
.equ EBI_CS_ASPACE0_bm = (1<<2) ; Address Space bit 0
.equ EBI_CS_ASPACE1_bm = (1<<3) ; Address Space bit 1
.equ EBI_CS_ASPACE2_bm = (1<<4) ; Address Space bit 2
.equ EBI_CS_ASPACE3_bm = (1<<5) ; Address Space bit 3
.equ EBI_CS_ASPACE4_bm = (1<<6) ; Address Space bit 4
.equ EBI_CS_MODE_gm = 0x03 ; Memory Mode
.equ EBI_CS_MODE0_bm = (1<<0) ; Memory Mode bit 0
.equ EBI_CS_MODE1_bm = (1<<1) ; Memory Mode bit 1

; EBI_CS_CTRLB masks
.equ EBI_CS_SRWS_gm = 0x07 ; SRAM Wait State Cycles
.equ EBI_CS_SRWS0_bm = (1<<0) ; SRAM Wait State Cycles bit 0
.equ EBI_CS_SRWS1_bm = (1<<1) ; SRAM Wait State Cycles bit 1
.equ EBI_CS_SRWS2_bm = (1<<2) ; SRAM Wait State Cycles bit 2
.equ EBI_CS_SDINITDONE_bm = 0x80 ; SDRAM Initialization Done
.equ EBI_CS_SDSREN_bm = 0x04 ; SDRAM Self-refresh Enable
.equ EBI_CS_SDMODE_gm = 0x03 ; SDRAM Mode
.equ EBI_CS_SDMODE0_bm = (1<<0) ; SDRAM Mode bit 0
.equ EBI_CS_SDMODE1_bm = (1<<1) ; SDRAM Mode bit 1

; EBI_CTRL masks
.equ EBI_SDDATAW_gm = 0xC0 ; SDRAM Data Width Setting
.equ EBI_SDDATAW0_bm = (1<<6) ; SDRAM Data Width Setting bit 0
.equ EBI_SDDATAW1_bm = (1<<7) ; SDRAM Data Width Setting bit 1
.equ EBI_LPCMODE_gm = 0x30 ; SRAM LPC Mode
.equ EBI_LPCMODE0_bm = (1<<4) ; SRAM LPC Mode bit 0
.equ EBI_LPCMODE1_bm = (1<<5) ; SRAM LPC Mode bit 1
.equ EBI_SRMODE_gm = 0x0C ; SRAM Mode
.equ EBI_SRMODE0_bm = (1<<2) ; SRAM Mode bit 0
.equ EBI_SRMODE1_bm = (1<<3) ; SRAM Mode bit 1
.equ EBI_IFMODE_gm = 0x03 ; Interface Mode
.equ EBI_IFMODE0_bm = (1<<0) ; Interface Mode bit 0
.equ EBI_IFMODE1_bm = (1<<1) ; Interface Mode bit 1

; EBI_SDRAMCTRLA masks
.equ EBI_SDCAS_bm = 0x08 ; SDRAM CAS Latency Setting
.equ EBI_SDROW_bm = 0x04 ; SDRAM ROW Bits Setting
.equ EBI_SDCOL_gm = 0x03 ; SDRAM Column Bits Setting
.equ EBI_SDCOL0_bm = (1<<0) ; SDRAM Column Bits Setting bit 0
.equ EBI_SDCOL1_bm = (1<<1) ; SDRAM Column Bits Setting bit 1

; EBI_SDRAMCTRLB masks
.equ EBI_MRDLY_gm = 0xC0 ; SDRAM Mode Register Delay
.equ EBI_MRDLY0_bm = (1<<6) ; SDRAM Mode Register Delay bit 0
.equ EBI_MRDLY1_bm = (1<<7) ; SDRAM Mode Register Delay bit 1
.equ EBI_ROWCYCDLY_gm = 0x38 ; SDRAM Row Cycle Delay
.equ EBI_ROWCYCDLY0_bm = (1<<3) ; SDRAM Row Cycle Delay bit 0
.equ EBI_ROWCYCDLY1_bm = (1<<4) ; SDRAM Row Cycle Delay bit 1
.equ EBI_ROWCYCDLY2_bm = (1<<5) ; SDRAM Row Cycle Delay bit 2
.equ EBI_RPDLY_gm = 0x07 ; SDRAM Row-to-Precharge Delay
.equ EBI_RPDLY0_bm = (1<<0) ; SDRAM Row-to-Precharge Delay bit 0
.equ EBI_RPDLY1_bm = (1<<1) ; SDRAM Row-to-Precharge Delay bit 1
.equ EBI_RPDLY2_bm = (1<<2) ; SDRAM Row-to-Precharge Delay bit 2

; EBI_SDRAMCTRLC masks
.equ EBI_WRDLY_gm = 0xC0 ; SDRAM Write Recovery Delay
.equ EBI_WRDLY0_bm = (1<<6) ; SDRAM Write Recovery Delay bit 0
.equ EBI_WRDLY1_bm = (1<<7) ; SDRAM Write Recovery Delay bit 1
.equ EBI_ESRDLY_gm = 0x38 ; SDRAM Exit-Self-refresh-to-Active Delay
.equ EBI_ESRDLY0_bm = (1<<3) ; SDRAM Exit-Self-refresh-to-Active Delay bit 0
.equ EBI_ESRDLY1_bm = (1<<4) ; SDRAM Exit-Self-refresh-to-Active Delay bit 1
.equ EBI_ESRDLY2_bm = (1<<5) ; SDRAM Exit-Self-refresh-to-Active Delay bit 2
.equ EBI_ROWCOLDLY_gm = 0x07 ; SDRAM Row-to-Column Delay
.equ EBI_ROWCOLDLY0_bm = (1<<0) ; SDRAM Row-to-Column Delay bit 0
.equ EBI_ROWCOLDLY1_bm = (1<<1) ; SDRAM Row-to-Column Delay bit 1
.equ EBI_ROWCOLDLY2_bm = (1<<2) ; SDRAM Row-to-Column Delay bit 2
 
; Chip Select adress space
.equ EBI_CS_ASPACE_256B_gc = (0x00<<2) ; 256 bytes
.equ EBI_CS_ASPACE_512B_gc = (0x01<<2) ; 512 bytes
.equ EBI_CS_ASPACE_1KB_gc = (0x02<<2) ; 1K bytes
.equ EBI_CS_ASPACE_2KB_gc = (0x03<<2) ; 2K bytes
.equ EBI_CS_ASPACE_4KB_gc = (0x04<<2) ; 4K bytes
.equ EBI_CS_ASPACE_8KB_gc = (0x05<<2) ; 8K bytes
.equ EBI_CS_ASPACE_16KB_gc = (0x06<<2) ; 16K bytes
.equ EBI_CS_ASPACE_32KB_gc = (0x07<<2) ; 32K bytes
.equ EBI_CS_ASPACE_64KB_gc = (0x08<<2) ; 64K bytes
.equ EBI_CS_ASPACE_128KB_gc = (0x09<<2) ; 128K bytes
.equ EBI_CS_ASPACE_256KB_gc = (0x0A<<2) ; 256K bytes
.equ EBI_CS_ASPACE_512KB_gc = (0x0B<<2) ; 512K bytes
.equ EBI_CS_ASPACE_1MB_gc = (0x0C<<2) ; 1M bytes
.equ EBI_CS_ASPACE_2MB_gc = (0x0D<<2) ; 2M bytes
.equ EBI_CS_ASPACE_4MB_gc = (0x0E<<2) ; 4M bytes
.equ EBI_CS_ASPACE_8MB_gc = (0x0F<<2) ; 8M bytes
.equ EBI_CS_ASPACE_16M_gc = (0x10<<2) ; 16M bytes

; 
.equ EBI_CS_SRWS_0CLK_gc = (0x00<<0) ; 0 cycles
.equ EBI_CS_SRWS_1CLK_gc = (0x01<<0) ; 1 cycle
.equ EBI_CS_SRWS_2CLK_gc = (0x02<<0) ; 2 cycles
.equ EBI_CS_SRWS_3CLK_gc = (0x03<<0) ; 3 cycles
.equ EBI_CS_SRWS_4CLK_gc = (0x04<<0) ; 4 cycles
.equ EBI_CS_SRWS_5CLK_gc = (0x05<<0) ; 5 cycle
.equ EBI_CS_SRWS_6CLK_gc = (0x06<<0) ; 6 cycles
.equ EBI_CS_SRWS_7CLK_gc = (0x07<<0) ; 7 cycles

; Chip Select address mode
.equ EBI_CS_MODE_DISABLED_gc = (0x00<<0) ; Chip Select Disabled
.equ EBI_CS_MODE_SRAM_gc = (0x01<<0) ; Chip Select in SRAM mode
.equ EBI_CS_MODE_LPC_gc = (0x02<<0) ; Chip Select in SRAM LPC mode
.equ EBI_CS_MODE_SDRAM_gc = (0x03<<0) ; Chip Select in SDRAM mode

; Chip Select SDRAM mode
.equ EBI_CS_SDMODE_NORMAL_gc = (0x00<<0) ; Normal mode
.equ EBI_CS_SDMODE_LOAD_gc = (0x01<<0) ; Load Mode Register command mode

; 
.equ EBI_SDDATAW_4BIT_gc = (0x00<<6) ; 4-bit data bus
.equ EBI_SDDATAW_8BIT_gc = (0x01<<6) ; 8-bit data bus

; 
.equ EBI_LPCMODE_ALE1_gc = (0x00<<4) ; Data muxed with addr byte 0
.equ EBI_LPCMODE_ALE12_gc = (0x02<<4) ; Data muxed with addr byte 0 and 1

; 
.equ EBI_SRMODE_ALE1_gc = (0x00<<2) ; Addr byte 0 muxed with 1
.equ EBI_SRMODE_ALE2_gc = (0x01<<2) ; Addr byte 0 muxed with 2
.equ EBI_SRMODE_ALE12_gc = (0x02<<2) ; Addr byte 0 muxed with 1 and 2
.equ EBI_SRMODE_NOALE_gc = (0x03<<2) ; No addr muxing

; 
.equ EBI_IFMODE_DISABLED_gc = (0x00<<0) ; EBI Disabled
.equ EBI_IFMODE_3PORT_gc = (0x01<<0) ; 3-port mode
.equ EBI_IFMODE_4PORT_gc = (0x02<<0) ; 4-port mode
.equ EBI_IFMODE_2PORT_gc = (0x03<<0) ; 2-port mode

; 
.equ EBI_SDCOL_8BIT_gc = (0x00<<0) ; 8 column bits
.equ EBI_SDCOL_9BIT_gc = (0x01<<0) ; 9 column bits
.equ EBI_SDCOL_10BIT_gc = (0x02<<0) ; 10 column bits
.equ EBI_SDCOL_11BIT_gc = (0x03<<0) ; 11 column bits

; 
.equ EBI_MRDLY_0CLK_gc = (0x00<<6) ; 0 cycles
.equ EBI_MRDLY_1CLK_gc = (0x01<<6) ; 1 cycle
.equ EBI_MRDLY_2CLK_gc = (0x02<<6) ; 2 cycles
.equ EBI_MRDLY_3CLK_gc = (0x03<<6) ; 3 cycles

; 
.equ EBI_ROWCYCDLY_0CLK_gc = (0x00<<3) ; 0 cycles
.equ EBI_ROWCYCDLY_1CLK_gc = (0x01<<3) ; 1 cycle
.equ EBI_ROWCYCDLY_2CLK_gc = (0x02<<3) ; 2 cycles
.equ EBI_ROWCYCDLY_3CLK_gc = (0x03<<3) ; 3 cycles
.equ EBI_ROWCYCDLY_4CLK_gc = (0x04<<3) ; 4 cycles
.equ EBI_ROWCYCDLY_5CLK_gc = (0x05<<3) ; 5 cycle
.equ EBI_ROWCYCDLY_6CLK_gc = (0x06<<3) ; 6 cycles
.equ EBI_ROWCYCDLY_7CLK_gc = (0x07<<3) ; 7 cycles

; 
.equ EBI_RPDLY_0CLK_gc = (0x00<<0) ; 0 cycles
.equ EBI_RPDLY_1CLK_gc = (0x01<<0) ; 1 cycle
.equ EBI_RPDLY_2CLK_gc = (0x02<<0) ; 2 cycles
.equ EBI_RPDLY_3CLK_gc = (0x03<<0) ; 3 cycles
.equ EBI_RPDLY_4CLK_gc = (0x04<<0) ; 4 cycles
.equ EBI_RPDLY_5CLK_gc = (0x05<<0) ; 5 cycle
.equ EBI_RPDLY_6CLK_gc = (0x06<<0) ; 6 cycles
.equ EBI_RPDLY_7CLK_gc = (0x07<<0) ; 7 cycles

; 
.equ EBI_WRDLY_0CLK_gc = (0x00<<6) ; 0 cycles
.equ EBI_WRDLY_1CLK_gc = (0x01<<6) ; 1 cycle
.equ EBI_WRDLY_2CLK_gc = (0x02<<6) ; 2 cycles
.equ EBI_WRDLY_3CLK_gc = (0x03<<6) ; 3 cycles

; 
.equ EBI_ESRDLY_0CLK_gc = (0x00<<3) ; 0 cycles
.equ EBI_ESRDLY_1CLK_gc = (0x01<<3) ; 1 cycle
.equ EBI_ESRDLY_2CLK_gc = (0x02<<3) ; 2 cycles
.equ EBI_ESRDLY_3CLK_gc = (0x03<<3) ; 3 cycles
.equ EBI_ESRDLY_4CLK_gc = (0x04<<3) ; 4 cycles
.equ EBI_ESRDLY_5CLK_gc = (0x05<<3) ; 5 cycle
.equ EBI_ESRDLY_6CLK_gc = (0x06<<3) ; 6 cycles
.equ EBI_ESRDLY_7CLK_gc = (0x07<<3) ; 7 cycles

; 
.equ EBI_ROWCOLDLY_0CLK_gc = (0x00<<0) ; 0 cycles
.equ EBI_ROWCOLDLY_1CLK_gc = (0x01<<0) ; 1 cycle
.equ EBI_ROWCOLDLY_2CLK_gc = (0x02<<0) ; 2 cycles
.equ EBI_ROWCOLDLY_3CLK_gc = (0x03<<0) ; 3 cycles
.equ EBI_ROWCOLDLY_4CLK_gc = (0x04<<0) ; 4 cycles
.equ EBI_ROWCOLDLY_5CLK_gc = (0x05<<0) ; 5 cycle
.equ EBI_ROWCOLDLY_6CLK_gc = (0x06<<0) ; 6 cycles
.equ EBI_ROWCOLDLY_7CLK_gc = (0x07<<0) ; 7 cycles


;***************************************************************************
;** TWI - Two-Wire Interface
;***************************************************************************/

; TWI_MASTER_CTRLA masks
.equ TWI_MASTER_INTLVL_gm = 0xC0 ; Interrupt Level
.equ TWI_MASTER_INTLVL0_bm = (1<<6) ; Interrupt Level bit 0
.equ TWI_MASTER_INTLVL1_bm = (1<<7) ; Interrupt Level bit 1
.equ TWI_MASTER_RIEN_bm = 0x20 ; Read Interrupt Enable
.equ TWI_MASTER_WIEN_bm = 0x10 ; Write Interrupt Enable
.equ TWI_MASTER_ENABLE_bm = 0x08 ; Enable TWI Master

; TWI_MASTER_CTRLB masks
.equ TWI_MASTER_TIMEOUT_gm = 0x0C ; Inactive Bus Timeout
.equ TWI_MASTER_TIMEOUT0_bm = (1<<2) ; Inactive Bus Timeout bit 0
.equ TWI_MASTER_TIMEOUT1_bm = (1<<3) ; Inactive Bus Timeout bit 1
.equ TWI_MASTER_QCEN_bm = 0x02 ; Quick Command Enable
.equ TWI_MASTER_SMEN_bm = 0x01 ; Smart Mode Enable

; TWI_MASTER_CTRLC masks
.equ TWI_MASTER_ACKACT_bm = 0x04 ; Acknowledge Action
.equ TWI_MASTER_CMD_gm = 0x03 ; Command
.equ TWI_MASTER_CMD0_bm = (1<<0) ; Command bit 0
.equ TWI_MASTER_CMD1_bm = (1<<1) ; Command bit 1

; TWI_MASTER_STATUS masks
.equ TWI_MASTER_RIF_bm = 0x80 ; Read Interrupt Flag
.equ TWI_MASTER_WIF_bm = 0x40 ; Write Interrupt Flag
.equ TWI_MASTER_CLKHOLD_bm = 0x20 ; Clock Hold
.equ TWI_MASTER_RXACK_bm = 0x10 ; Received Acknowledge
.equ TWI_MASTER_ARBLOST_bm = 0x08 ; Arbitration Lost
.equ TWI_MASTER_BUSERR_bm = 0x04 ; Bus Error
.equ TWI_MASTER_BUSSTATE_gm = 0x03 ; Bus State
.equ TWI_MASTER_BUSSTATE0_bm = (1<<0) ; Bus State bit 0
.equ TWI_MASTER_BUSSTATE1_bm = (1<<1) ; Bus State bit 1

; TWI_SLAVE_CTRLA masks
.equ TWI_SLAVE_INTLVL_gm = 0xC0 ; Interrupt Level
.equ TWI_SLAVE_INTLVL0_bm = (1<<6) ; Interrupt Level bit 0
.equ TWI_SLAVE_INTLVL1_bm = (1<<7) ; Interrupt Level bit 1
.equ TWI_SLAVE_DIEN_bm = 0x20 ; Data Interrupt Enable
.equ TWI_SLAVE_APIEN_bm = 0x10 ; Address/Stop Interrupt Enable
.equ TWI_SLAVE_ENABLE_bm = 0x08 ; Enable TWI Slave
.equ TWI_SLAVE_PIEN_bm = 0x04 ; Stop Interrupt Enable
.equ TWI_SLAVE_PMEN_bm = 0x02 ; Promiscuous Mode Enable
.equ TWI_SLAVE_SMEN_bm = 0x01 ; Smart Mode Enable

; TWI_SLAVE_CTRLB masks
.equ TWI_SLAVE_ACKACT_bm = 0x04 ; Acknowledge Action
.equ TWI_SLAVE_CMD_gm = 0x03 ; Command
.equ TWI_SLAVE_CMD0_bm = (1<<0) ; Command bit 0
.equ TWI_SLAVE_CMD1_bm = (1<<1) ; Command bit 1

; TWI_SLAVE_STATUS masks
.equ TWI_SLAVE_DIF_bm = 0x80 ; Data Interrupt Flag
.equ TWI_SLAVE_APIF_bm = 0x40 ; Address/Stop Interrupt Flag
.equ TWI_SLAVE_CLKHOLD_bm = 0x20 ; Clock Hold
.equ TWI_SLAVE_RXACK_bm = 0x10 ; Received Acknowledge
.equ TWI_SLAVE_COLL_bm = 0x08 ; Collision
.equ TWI_SLAVE_BUSERR_bm = 0x04 ; Bus Error
.equ TWI_SLAVE_DIR_bm = 0x02 ; Read/Write Direction
.equ TWI_SLAVE_AP_bm = 0x01 ; Slave Address or Stop

; TWI_CTRL masks
.equ TWI_SDAHOLD_bm = 0x02 ; SDA Hold Time Enable
.equ TWI_EDIEN_bm = 0x01 ; External Driver Interface Enable
 
; Master Interrupt Level
.equ TWI_MASTER_INTLVL_OFF_gc = (0x00<<6) ; Interrupt Disabled
.equ TWI_MASTER_INTLVL_LO_gc = (0x01<<6) ; Low Level
.equ TWI_MASTER_INTLVL_MED_gc = (0x02<<6) ; Medium Level
.equ TWI_MASTER_INTLVL_HI_gc = (0x03<<6) ; High Level

; Inactive Timeout
.equ TWI_MASTER_TIMEOUT_DISABLED_gc = (0x00<<2) ; Bus Timeout Disabled
.equ TWI_MASTER_TIMEOUT_50US_gc = (0x01<<2) ; 50 Microseconds
.equ TWI_MASTER_TIMEOUT_100US_gc = (0x02<<2) ; 100 Microseconds
.equ TWI_MASTER_TIMEOUT_200US_gc = (0x03<<2) ; 200 Microseconds

; Master Command
.equ TWI_MASTER_CMD_NOACT_gc = (0x00<<0) ; No Action
.equ TWI_MASTER_CMD_REPSTART_gc = (0x01<<0) ; Issue Repeated Start Condition
.equ TWI_MASTER_CMD_RECVTRANS_gc = (0x02<<0) ; Receive or Transmit Data
.equ TWI_MASTER_CMD_STOP_gc = (0x03<<0) ; Issue Stop Condition

; Master Bus State
.equ TWI_MASTER_BUSSTATE_UNKNOWN_gc = (0x00<<0) ; Unknown Bus State
.equ TWI_MASTER_BUSSTATE_IDLE_gc = (0x01<<0) ; Bus is Idle
.equ TWI_MASTER_BUSSTATE_OWNER_gc = (0x02<<0) ; This Module Controls The Bus
.equ TWI_MASTER_BUSSTATE_BUSY_gc = (0x03<<0) ; The Bus is Busy

; Slave Interrupt Level
.equ TWI_SLAVE_INTLVL_OFF_gc = (0x00<<6) ; Interrupt Disabled
.equ TWI_SLAVE_INTLVL_LO_gc = (0x01<<6) ; Low Level
.equ TWI_SLAVE_INTLVL_MED_gc = (0x02<<6) ; Medium Level
.equ TWI_SLAVE_INTLVL_HI_gc = (0x03<<6) ; High Level

; Slave Command
.equ TWI_SLAVE_CMD_NOACT_gc = (0x00<<0) ; No Action
.equ TWI_SLAVE_CMD_COMPTRANS_gc = (0x02<<0) ; Used To Complete a Transaction
.equ TWI_SLAVE_CMD_RESPONSE_gc = (0x03<<0) ; Used in Response to Address/Data Interrupt


;***************************************************************************
;** PORT - Port Configuration
;***************************************************************************/

; PORTCFG_VPCTRLA masks
.equ PORTCFG_VP1MAP_gm = 0xF0 ; Virtual Port 1 Mapping
.equ PORTCFG_VP1MAP0_bm = (1<<4) ; Virtual Port 1 Mapping bit 0
.equ PORTCFG_VP1MAP1_bm = (1<<5) ; Virtual Port 1 Mapping bit 1
.equ PORTCFG_VP1MAP2_bm = (1<<6) ; Virtual Port 1 Mapping bit 2
.equ PORTCFG_VP1MAP3_bm = (1<<7) ; Virtual Port 1 Mapping bit 3
.equ PORTCFG_VP0MAP_gm = 0x0F ; Virtual Port 0 Mapping
.equ PORTCFG_VP0MAP0_bm = (1<<0) ; Virtual Port 0 Mapping bit 0
.equ PORTCFG_VP0MAP1_bm = (1<<1) ; Virtual Port 0 Mapping bit 1
.equ PORTCFG_VP0MAP2_bm = (1<<2) ; Virtual Port 0 Mapping bit 2
.equ PORTCFG_VP0MAP3_bm = (1<<3) ; Virtual Port 0 Mapping bit 3

; PORTCFG_VPCTRLB masks
.equ PORTCFG_VP3MAP_gm = 0xF0 ; Virtual Port 3 Mapping
.equ PORTCFG_VP3MAP0_bm = (1<<4) ; Virtual Port 3 Mapping bit 0
.equ PORTCFG_VP3MAP1_bm = (1<<5) ; Virtual Port 3 Mapping bit 1
.equ PORTCFG_VP3MAP2_bm = (1<<6) ; Virtual Port 3 Mapping bit 2
.equ PORTCFG_VP3MAP3_bm = (1<<7) ; Virtual Port 3 Mapping bit 3
.equ PORTCFG_VP2MAP_gm = 0x0F ; Virtual Port 2 Mapping
.equ PORTCFG_VP2MAP0_bm = (1<<0) ; Virtual Port 2 Mapping bit 0
.equ PORTCFG_VP2MAP1_bm = (1<<1) ; Virtual Port 2 Mapping bit 1
.equ PORTCFG_VP2MAP2_bm = (1<<2) ; Virtual Port 2 Mapping bit 2
.equ PORTCFG_VP2MAP3_bm = (1<<3) ; Virtual Port 2 Mapping bit 3

; PORTCFG_CLKEVOUT masks
.equ PORTCFG_CLKOUT_gm = 0x03 ; Clock Output Port
.equ PORTCFG_CLKOUT0_bm = (1<<0) ; Clock Output Port bit 0
.equ PORTCFG_CLKOUT1_bm = (1<<1) ; Clock Output Port bit 1
.equ PORTCFG_EVOUT_gm = 0x30 ; Event Output Port
.equ PORTCFG_EVOUT0_bm = (1<<4) ; Event Output Port bit 0
.equ PORTCFG_EVOUT1_bm = (1<<5) ; Event Output Port bit 1

; VPORT_INTFLAGS masks
.equ VPORT_INT1IF_bm = 0x02 ; Port Interrupt 1 Flag
.equ VPORT_INT0IF_bm = 0x01 ; Port Interrupt 0 Flag

; PORT_INTCTRL masks
.equ PORT_INT1LVL_gm = 0x0C ; Port Interrupt 1 Level
.equ PORT_INT1LVL0_bm = (1<<2) ; Port Interrupt 1 Level bit 0
.equ PORT_INT1LVL1_bm = (1<<3) ; Port Interrupt 1 Level bit 1
.equ PORT_INT0LVL_gm = 0x03 ; Port Interrupt 0 Level
.equ PORT_INT0LVL0_bm = (1<<0) ; Port Interrupt 0 Level bit 0
.equ PORT_INT0LVL1_bm = (1<<1) ; Port Interrupt 0 Level bit 1

; PORT_INTFLAGS masks
.equ PORT_INT1IF_bm = 0x02 ; Port Interrupt 1 Flag
.equ PORT_INT0IF_bm = 0x01 ; Port Interrupt 0 Flag

; PORT_PIN0CTRL masks
.equ PORT_SRLEN_bm = 0x80 ; Slew Rate Enable
.equ PORT_INVEN_bm = 0x40 ; Inverted I/O Enable
.equ PORT_OPC_gm = 0x38 ; Output/Pull Configuration
.equ PORT_OPC0_bm = (1<<3) ; Output/Pull Configuration bit 0
.equ PORT_OPC1_bm = (1<<4) ; Output/Pull Configuration bit 1
.equ PORT_OPC2_bm = (1<<5) ; Output/Pull Configuration bit 2
.equ PORT_ISC_gm = 0x07 ; Input/Sense Configuration
.equ PORT_ISC0_bm = (1<<0) ; Input/Sense Configuration bit 0
.equ PORT_ISC1_bm = (1<<1) ; Input/Sense Configuration bit 1
.equ PORT_ISC2_bm = (1<<2) ; Input/Sense Configuration bit 2

; PORT_PIN1CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined

; PORT_PIN2CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined

; PORT_PIN3CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined

; PORT_PIN4CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined

; PORT_PIN5CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined

; PORT_PIN6CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined

; PORT_PIN7CTRL masks
; Masks for SRLEN aready defined
; Masks for INVEN aready defined
; Masks for OPC aready defined
; Masks for ISC aready defined
 
; Virtual Port 0 Mapping
.equ PORTCFG_VP0MAP_PORTA_gc = (0x00<<0) ; Mapped To PORTA
.equ PORTCFG_VP0MAP_PORTB_gc = (0x01<<0) ; Mapped To PORTB
.equ PORTCFG_VP0MAP_PORTC_gc = (0x02<<0) ; Mapped To PORTC
.equ PORTCFG_VP0MAP_PORTD_gc = (0x03<<0) ; Mapped To PORTD
.equ PORTCFG_VP0MAP_PORTE_gc = (0x04<<0) ; Mapped To PORTE
.equ PORTCFG_VP0MAP_PORTF_gc = (0x05<<0) ; Mapped To PORTF
.equ PORTCFG_VP0MAP_PORTG_gc = (0x06<<0) ; Mapped To PORTG
.equ PORTCFG_VP0MAP_PORTH_gc = (0x07<<0) ; Mapped To PORTH
.equ PORTCFG_VP0MAP_PORTJ_gc = (0x08<<0) ; Mapped To PORTJ
.equ PORTCFG_VP0MAP_PORTK_gc = (0x09<<0) ; Mapped To PORTK
.equ PORTCFG_VP0MAP_PORTL_gc = (0x0A<<0) ; Mapped To PORTL
.equ PORTCFG_VP0MAP_PORTM_gc = (0x0B<<0) ; Mapped To PORTM
.equ PORTCFG_VP0MAP_PORTN_gc = (0x0C<<0) ; Mapped To PORTN
.equ PORTCFG_VP0MAP_PORTP_gc = (0x0D<<0) ; Mapped To PORTP
.equ PORTCFG_VP0MAP_PORTQ_gc = (0x0E<<0) ; Mapped To PORTQ
.equ PORTCFG_VP0MAP_PORTR_gc = (0x0F<<0) ; Mapped To PORTR

; Virtual Port 1 Mapping
.equ PORTCFG_VP1MAP_PORTA_gc = (0x00<<4) ; Mapped To PORTA
.equ PORTCFG_VP1MAP_PORTB_gc = (0x01<<4) ; Mapped To PORTB
.equ PORTCFG_VP1MAP_PORTC_gc = (0x02<<4) ; Mapped To PORTC
.equ PORTCFG_VP1MAP_PORTD_gc = (0x03<<4) ; Mapped To PORTD
.equ PORTCFG_VP1MAP_PORTE_gc = (0x04<<4) ; Mapped To PORTE
.equ PORTCFG_VP1MAP_PORTF_gc = (0x05<<4) ; Mapped To PORTF
.equ PORTCFG_VP1MAP_PORTG_gc = (0x06<<4) ; Mapped To PORTG
.equ PORTCFG_VP1MAP_PORTH_gc = (0x07<<4) ; Mapped To PORTH
.equ PORTCFG_VP1MAP_PORTJ_gc = (0x08<<4) ; Mapped To PORTJ
.equ PORTCFG_VP1MAP_PORTK_gc = (0x09<<4) ; Mapped To PORTK
.equ PORTCFG_VP1MAP_PORTL_gc = (0x0A<<4) ; Mapped To PORTL
.equ PORTCFG_VP1MAP_PORTM_gc = (0x0B<<4) ; Mapped To PORTM
.equ PORTCFG_VP1MAP_PORTN_gc = (0x0C<<4) ; Mapped To PORTN
.equ PORTCFG_VP1MAP_PORTP_gc = (0x0D<<4) ; Mapped To PORTP
.equ PORTCFG_VP1MAP_PORTQ_gc = (0x0E<<4) ; Mapped To PORTQ
.equ PORTCFG_VP1MAP_PORTR_gc = (0x0F<<4) ; Mapped To PORTR

; Virtual Port 2 Mapping
.equ PORTCFG_VP2MAP_PORTA_gc = (0x00<<0) ; Mapped To PORTA
.equ PORTCFG_VP2MAP_PORTB_gc = (0x01<<0) ; Mapped To PORTB
.equ PORTCFG_VP2MAP_PORTC_gc = (0x02<<0) ; Mapped To PORTC
.equ PORTCFG_VP2MAP_PORTD_gc = (0x03<<0) ; Mapped To PORTD
.equ PORTCFG_VP2MAP_PORTE_gc = (0x04<<0) ; Mapped To PORTE
.equ PORTCFG_VP2MAP_PORTF_gc = (0x05<<0) ; Mapped To PORTF
.equ PORTCFG_VP2MAP_PORTG_gc = (0x06<<0) ; Mapped To PORTG
.equ PORTCFG_VP2MAP_PORTH_gc = (0x07<<0) ; Mapped To PORTH
.equ PORTCFG_VP2MAP_PORTJ_gc = (0x08<<0) ; Mapped To PORTJ
.equ PORTCFG_VP2MAP_PORTK_gc = (0x09<<0) ; Mapped To PORTK
.equ PORTCFG_VP2MAP_PORTL_gc = (0x0A<<0) ; Mapped To PORTL
.equ PORTCFG_VP2MAP_PORTM_gc = (0x0B<<0) ; Mapped To PORTM
.equ PORTCFG_VP2MAP_PORTN_gc = (0x0C<<0) ; Mapped To PORTN
.equ PORTCFG_VP2MAP_PORTP_gc = (0x0D<<0) ; Mapped To PORTP
.equ PORTCFG_VP2MAP_PORTQ_gc = (0x0E<<0) ; Mapped To PORTQ
.equ PORTCFG_VP2MAP_PORTR_gc = (0x0F<<0) ; Mapped To PORTR

; Virtual Port 3 Mapping
.equ PORTCFG_VP3MAP_PORTA_gc = (0x00<<4) ; Mapped To PORTA
.equ PORTCFG_VP3MAP_PORTB_gc = (0x01<<4) ; Mapped To PORTB
.equ PORTCFG_VP3MAP_PORTC_gc = (0x02<<4) ; Mapped To PORTC
.equ PORTCFG_VP3MAP_PORTD_gc = (0x03<<4) ; Mapped To PORTD
.equ PORTCFG_VP3MAP_PORTE_gc = (0x04<<4) ; Mapped To PORTE
.equ PORTCFG_VP3MAP_PORTF_gc = (0x05<<4) ; Mapped To PORTF
.equ PORTCFG_VP3MAP_PORTG_gc = (0x06<<4) ; Mapped To PORTG
.equ PORTCFG_VP3MAP_PORTH_gc = (0x07<<4) ; Mapped To PORTH
.equ PORTCFG_VP3MAP_PORTJ_gc = (0x08<<4) ; Mapped To PORTJ
.equ PORTCFG_VP3MAP_PORTK_gc = (0x09<<4) ; Mapped To PORTK
.equ PORTCFG_VP3MAP_PORTL_gc = (0x0A<<4) ; Mapped To PORTL
.equ PORTCFG_VP3MAP_PORTM_gc = (0x0B<<4) ; Mapped To PORTM
.equ PORTCFG_VP3MAP_PORTN_gc = (0x0C<<4) ; Mapped To PORTN
.equ PORTCFG_VP3MAP_PORTP_gc = (0x0D<<4) ; Mapped To PORTP
.equ PORTCFG_VP3MAP_PORTQ_gc = (0x0E<<4) ; Mapped To PORTQ
.equ PORTCFG_VP3MAP_PORTR_gc = (0x0F<<4) ; Mapped To PORTR

; Clock Output Port
.equ PORTCFG_CLKOUT_OFF_gc = (0x00<<0) ; Clock Output Disabled
.equ PORTCFG_CLKOUT_PC7_gc = (0x01<<0) ; Clock Output on Port C pin 7
.equ PORTCFG_CLKOUT_PD7_gc = (0x02<<0) ; Clock Output on Port D pin 7
.equ PORTCFG_CLKOUT_PE7_gc = (0x03<<0) ; Clock Output on Port E pin 7

; Event Output Port
.equ PORTCFG_EVOUT_OFF_gc = (0x00<<4) ; Event Output Disabled
.equ PORTCFG_EVOUT_PC7_gc = (0x01<<4) ; Event Channel 7 Output on Port C pin 7
.equ PORTCFG_EVOUT_PD7_gc = (0x02<<4) ; Event Channel 7 Output on Port D pin 7
.equ PORTCFG_EVOUT_PE7_gc = (0x03<<4) ; Event Channel 7 Output on Port E pin 7

; Port Interrupt 0 Level
.equ PORT_INT0LVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ PORT_INT0LVL_LO_gc = (0x01<<0) ; Low Level
.equ PORT_INT0LVL_MED_gc = (0x02<<0) ; Medium Level
.equ PORT_INT0LVL_HI_gc = (0x03<<0) ; High Level

; Port Interrupt 1 Level
.equ PORT_INT1LVL_OFF_gc = (0x00<<2) ; Interrupt Disabled
.equ PORT_INT1LVL_LO_gc = (0x01<<2) ; Low Level
.equ PORT_INT1LVL_MED_gc = (0x02<<2) ; Medium Level
.equ PORT_INT1LVL_HI_gc = (0x03<<2) ; High Level

; Output/Pull Configuration
.equ PORT_OPC_TOTEM_gc = (0x00<<3) ; Totempole
.equ PORT_OPC_BUSKEEPER_gc = (0x01<<3) ; Totempole w/ Bus keeper on Input and Output
.equ PORT_OPC_PULLDOWN_gc = (0x02<<3) ; Totempole w/ Pull-down on Input
.equ PORT_OPC_PULLUP_gc = (0x03<<3) ; Totempole w/ Pull-up on Input
.equ PORT_OPC_WIREDOR_gc = (0x04<<3) ; Wired OR
.equ PORT_OPC_WIREDAND_gc = (0x05<<3) ; Wired AND
.equ PORT_OPC_WIREDORPULL_gc = (0x06<<3) ; Wired OR w/ Pull-down
.equ PORT_OPC_WIREDANDPULL_gc = (0x07<<3) ; Wired AND w/ Pull-up

; Input/Sense Configuration
.equ PORT_ISC_BOTHEDGES_gc = (0x00<<0) ; Sense Both Edges
.equ PORT_ISC_RISING_gc = (0x01<<0) ; Sense Rising Edge
.equ PORT_ISC_FALLING_gc = (0x02<<0) ; Sense Falling Edge
.equ PORT_ISC_LEVEL_gc = (0x03<<0) ; Sense Level (Transparent For Events)
.equ PORT_ISC_INPUT_DISABLE_gc = (0x07<<0) ; Disable Digital Input Buffer


;***************************************************************************
;** TC - 16-bit Timer/Counter With PWM
;***************************************************************************/

; TC0_CTRLA masks
.equ TC0_CLKSEL_gm = 0x0F ; Clock Selection
.equ TC0_CLKSEL0_bm = (1<<0) ; Clock Selection bit 0
.equ TC0_CLKSEL1_bm = (1<<1) ; Clock Selection bit 1
.equ TC0_CLKSEL2_bm = (1<<2) ; Clock Selection bit 2
.equ TC0_CLKSEL3_bm = (1<<3) ; Clock Selection bit 3

; TC0_CTRLB masks
.equ TC0_CCDEN_bm = 0x80 ; Compare or Capture D Enable
.equ TC0_CCCEN_bm = 0x40 ; Compare or Capture C Enable
.equ TC0_CCBEN_bm = 0x20 ; Compare or Capture B Enable
.equ TC0_CCAEN_bm = 0x10 ; Compare or Capture A Enable
.equ TC0_WGMODE_gm = 0x07 ; Waveform generation mode
.equ TC0_WGMODE0_bm = (1<<0) ; Waveform generation mode bit 0
.equ TC0_WGMODE1_bm = (1<<1) ; Waveform generation mode bit 1
.equ TC0_WGMODE2_bm = (1<<2) ; Waveform generation mode bit 2

; TC0_CTRLC masks
.equ TC0_CMPD_bm = 0x08 ; Compare D Output Value
.equ TC0_CMPC_bm = 0x04 ; Compare C Output Value
.equ TC0_CMPB_bm = 0x02 ; Compare B Output Value
.equ TC0_CMPA_bm = 0x01 ; Compare A Output Value

; TC0_CTRLD masks
.equ TC0_EVACT_gm = 0xE0 ; Event Action
.equ TC0_EVACT0_bm = (1<<5) ; Event Action bit 0
.equ TC0_EVACT1_bm = (1<<6) ; Event Action bit 1
.equ TC0_EVACT2_bm = (1<<7) ; Event Action bit 2
.equ TC0_EVDLY_bm = 0x10 ; Event Delay
.equ TC0_EVSEL_gm = 0x0F ; Event Source Select
.equ TC0_EVSEL0_bm = (1<<0) ; Event Source Select bit 0
.equ TC0_EVSEL1_bm = (1<<1) ; Event Source Select bit 1
.equ TC0_EVSEL2_bm = (1<<2) ; Event Source Select bit 2
.equ TC0_EVSEL3_bm = (1<<3) ; Event Source Select bit 3

; TC0_CTRLE masks
.equ TC0_DTHM_bm = 0x02 ; Dead Time Hold Mode
.equ TC0_BYTEM_bm = 0x01 ; Byte Mode

; TC0_INTCTRLA masks
.equ TC0_ERRINTLVL_gm = 0x0C ; Error Interrupt Level
.equ TC0_ERRINTLVL0_bm = (1<<2) ; Error Interrupt Level bit 0
.equ TC0_ERRINTLVL1_bm = (1<<3) ; Error Interrupt Level bit 1
.equ TC0_OVFINTLVL_gm = 0x03 ; Overflow interrupt level
.equ TC0_OVFINTLVL0_bm = (1<<0) ; Overflow interrupt level bit 0
.equ TC0_OVFINTLVL1_bm = (1<<1) ; Overflow interrupt level bit 1

; TC0_INTCTRLB masks
.equ TC0_CCDINTLVL_gm = 0xC0 ; Compare or Capture D Interrupt Level
.equ TC0_CCDINTLVL0_bm = (1<<6) ; Compare or Capture D Interrupt Level bit 0
.equ TC0_CCDINTLVL1_bm = (1<<7) ; Compare or Capture D Interrupt Level bit 1
.equ TC0_CCCINTLVL_gm = 0x30 ; Compare or Capture C Interrupt Level
.equ TC0_CCCINTLVL0_bm = (1<<4) ; Compare or Capture C Interrupt Level bit 0
.equ TC0_CCCINTLVL1_bm = (1<<5) ; Compare or Capture C Interrupt Level bit 1
.equ TC0_CCBINTLVL_gm = 0x0C ; Compare or Capture B Interrupt Level
.equ TC0_CCBINTLVL0_bm = (1<<2) ; Compare or Capture B Interrupt Level bit 0
.equ TC0_CCBINTLVL1_bm = (1<<3) ; Compare or Capture B Interrupt Level bit 1
.equ TC0_CCAINTLVL_gm = 0x03 ; Compare or Capture A Interrupt Level
.equ TC0_CCAINTLVL0_bm = (1<<0) ; Compare or Capture A Interrupt Level bit 0
.equ TC0_CCAINTLVL1_bm = (1<<1) ; Compare or Capture A Interrupt Level bit 1

; TC0_CTRLFCLR masks
.equ TC0_CMD_gm = 0x0C ; Command
.equ TC0_CMD0_bm = (1<<2) ; Command bit 0
.equ TC0_CMD1_bm = (1<<3) ; Command bit 1
.equ TC0_LUPD_bm = 0x02 ; Lock Update
.equ TC0_DIR_bm = 0x01 ; Direction

; TC0_CTRLFSET masks
; Masks for CMD aready defined
; Masks for LUPD aready defined
; Masks for DIR aready defined

; TC0_CTRLGCLR masks
.equ TC0_CCDBV_bm = 0x10 ; Compare or Capture D Buffer Valid
.equ TC0_CCCBV_bm = 0x08 ; Compare or Capture C Buffer Valid
.equ TC0_CCBBV_bm = 0x04 ; Compare or Capture B Buffer Valid
.equ TC0_CCABV_bm = 0x02 ; Compare or Capture A Buffer Valid
.equ TC0_PERBV_bm = 0x01 ; Period Buffer Valid

; TC0_CTRLGSET masks
; Masks for CCDBV aready defined
; Masks for CCCBV aready defined
; Masks for CCBBV aready defined
; Masks for CCABV aready defined
; Masks for PERBV aready defined

; TC0_INTFLAGS masks
.equ TC0_CCDIF_bm = 0x80 ; Compare or Capture D Interrupt Flag
.equ TC0_CCCIF_bm = 0x40 ; Compare or Capture C Interrupt Flag
.equ TC0_CCBIF_bm = 0x20 ; Compare or Capture B Interrupt Flag
.equ TC0_CCAIF_bm = 0x10 ; Compare or Capture A Interrupt Flag
.equ TC0_ERRIF_bm = 0x02 ; Error Interrupt Flag
.equ TC0_OVFIF_bm = 0x01 ; Overflow Interrupt Flag

; TC1_CTRLA masks
.equ TC1_CLKSEL_gm = 0x0F ; Clock Selection
.equ TC1_CLKSEL0_bm = (1<<0) ; Clock Selection bit 0
.equ TC1_CLKSEL1_bm = (1<<1) ; Clock Selection bit 1
.equ TC1_CLKSEL2_bm = (1<<2) ; Clock Selection bit 2
.equ TC1_CLKSEL3_bm = (1<<3) ; Clock Selection bit 3

; TC1_CTRLB masks
.equ TC1_CCBEN_bm = 0x20 ; Compare or Capture B Enable
.equ TC1_CCAEN_bm = 0x10 ; Compare or Capture A Enable
.equ TC1_WGMODE_gm = 0x07 ; Waveform generation mode
.equ TC1_WGMODE0_bm = (1<<0) ; Waveform generation mode bit 0
.equ TC1_WGMODE1_bm = (1<<1) ; Waveform generation mode bit 1
.equ TC1_WGMODE2_bm = (1<<2) ; Waveform generation mode bit 2

; TC1_CTRLC masks
.equ TC1_CMPB_bm = 0x02 ; Compare B Output Value
.equ TC1_CMPA_bm = 0x01 ; Compare A Output Value

; TC1_CTRLD masks
.equ TC1_EVACT_gm = 0xE0 ; Event Action
.equ TC1_EVACT0_bm = (1<<5) ; Event Action bit 0
.equ TC1_EVACT1_bm = (1<<6) ; Event Action bit 1
.equ TC1_EVACT2_bm = (1<<7) ; Event Action bit 2
.equ TC1_EVDLY_bm = 0x10 ; Event Delay
.equ TC1_EVSEL_gm = 0x0F ; Event Source Select
.equ TC1_EVSEL0_bm = (1<<0) ; Event Source Select bit 0
.equ TC1_EVSEL1_bm = (1<<1) ; Event Source Select bit 1
.equ TC1_EVSEL2_bm = (1<<2) ; Event Source Select bit 2
.equ TC1_EVSEL3_bm = (1<<3) ; Event Source Select bit 3

; TC1_CTRLE masks
.equ TC1_DTHM_bm = 0x02 ; Dead Time Hold Mode
.equ TC1_BYTEM_bm = 0x01 ; Byte Mode

; TC1_INTCTRLA masks
.equ TC1_ERRINTLVL_gm = 0x0C ; Error Interrupt Level
.equ TC1_ERRINTLVL0_bm = (1<<2) ; Error Interrupt Level bit 0
.equ TC1_ERRINTLVL1_bm = (1<<3) ; Error Interrupt Level bit 1
.equ TC1_OVFINTLVL_gm = 0x03 ; Overflow interrupt level
.equ TC1_OVFINTLVL0_bm = (1<<0) ; Overflow interrupt level bit 0
.equ TC1_OVFINTLVL1_bm = (1<<1) ; Overflow interrupt level bit 1

; TC1_INTCTRLB masks
.equ TC1_CCBINTLVL_gm = 0x0C ; Compare or Capture B Interrupt Level
.equ TC1_CCBINTLVL0_bm = (1<<2) ; Compare or Capture B Interrupt Level bit 0
.equ TC1_CCBINTLVL1_bm = (1<<3) ; Compare or Capture B Interrupt Level bit 1
.equ TC1_CCAINTLVL_gm = 0x03 ; Compare or Capture A Interrupt Level
.equ TC1_CCAINTLVL0_bm = (1<<0) ; Compare or Capture A Interrupt Level bit 0
.equ TC1_CCAINTLVL1_bm = (1<<1) ; Compare or Capture A Interrupt Level bit 1

; TC1_CTRLFCLR masks
.equ TC1_CMD_gm = 0x0C ; Command
.equ TC1_CMD0_bm = (1<<2) ; Command bit 0
.equ TC1_CMD1_bm = (1<<3) ; Command bit 1
.equ TC1_LUPD_bm = 0x02 ; Lock Update
.equ TC1_DIR_bm = 0x01 ; Direction

; TC1_CTRLFSET masks
; Masks for CMD aready defined
; Masks for LUPD aready defined
; Masks for DIR aready defined

; TC1_CTRLGCLR masks
.equ TC1_CCBBV_bm = 0x04 ; Compare or Capture B Buffer Valid
.equ TC1_CCABV_bm = 0x02 ; Compare or Capture A Buffer Valid
.equ TC1_PERBV_bm = 0x01 ; Period Buffer Valid

; TC1_CTRLGSET masks
; Masks for CCBBV aready defined
; Masks for CCABV aready defined
; Masks for PERBV aready defined

; TC1_INTFLAGS masks
.equ TC1_CCBIF_bm = 0x20 ; Compare or Capture B Interrupt Flag
.equ TC1_CCAIF_bm = 0x10 ; Compare or Capture A Interrupt Flag
.equ TC1_ERRIF_bm = 0x02 ; Error Interrupt Flag
.equ TC1_OVFIF_bm = 0x01 ; Overflow Interrupt Flag

; AWEX_CTRL masks
.equ AWEX_PGM_bm = 0x20 ; Pattern Generation Mode
.equ AWEX_CWCM_bm = 0x10 ; Common Waveform Channel Mode
.equ AWEX_DTICCDEN_bm = 0x08 ; Dead Time Insertion Compare Channel D Enable
.equ AWEX_DTICCCEN_bm = 0x04 ; Dead Time Insertion Compare Channel C Enable
.equ AWEX_DTICCBEN_bm = 0x02 ; Dead Time Insertion Compare Channel B Enable
.equ AWEX_DTICCAEN_bm = 0x01 ; Dead Time Insertion Compare Channel A Enable

; AWEX_FDCTRL masks
.equ AWEX_FDDBD_bm = 0x10 ; Fault Detect on Disable Break Disable
.equ AWEX_FDMODE_bm = 0x04 ; Fault Detect Mode
.equ AWEX_FDACT_gm = 0x03 ; Fault Detect Action
.equ AWEX_FDACT0_bm = (1<<0) ; Fault Detect Action bit 0
.equ AWEX_FDACT1_bm = (1<<1) ; Fault Detect Action bit 1

; AWEX_STATUS masks
.equ AWEX_FDF_bm = 0x04 ; Fault Detect Flag
.equ AWEX_DTHSBUFV_bm = 0x02 ; Dead Time High Side Buffer Valid
.equ AWEX_DTLSBUFV_bm = 0x01 ; Dead Time Low Side Buffer Valid

; HIRES_CTRL masks
.equ HIRES_HREN_gm = 0x03 ; High Resolution Enable
.equ HIRES_HREN0_bm = (1<<0) ; High Resolution Enable bit 0
.equ HIRES_HREN1_bm = (1<<1) ; High Resolution Enable bit 1
 
; Clock Selection
.equ TC_CLKSEL_OFF_gc = (0x00<<0) ; Timer Off
.equ TC_CLKSEL_DIV1_gc = (0x01<<0) ; System Clock
.equ TC_CLKSEL_DIV2_gc = (0x02<<0) ; System Clock / 2
.equ TC_CLKSEL_DIV4_gc = (0x03<<0) ; System Clock / 4
.equ TC_CLKSEL_DIV8_gc = (0x04<<0) ; System Clock / 8
.equ TC_CLKSEL_DIV64_gc = (0x05<<0) ; System Clock / 64
.equ TC_CLKSEL_DIV256_gc = (0x06<<0) ; System Clock / 256
.equ TC_CLKSEL_DIV1024_gc = (0x07<<0) ; System Clock / 1024
.equ TC_CLKSEL_EVCH0_gc = (0x08<<0) ; Event Channel 0
.equ TC_CLKSEL_EVCH1_gc = (0x09<<0) ; Event Channel 1
.equ TC_CLKSEL_EVCH2_gc = (0x0A<<0) ; Event Channel 2
.equ TC_CLKSEL_EVCH3_gc = (0x0B<<0) ; Event Channel 3
.equ TC_CLKSEL_EVCH4_gc = (0x0C<<0) ; Event Channel 4
.equ TC_CLKSEL_EVCH5_gc = (0x0D<<0) ; Event Channel 5
.equ TC_CLKSEL_EVCH6_gc = (0x0E<<0) ; Event Channel 6
.equ TC_CLKSEL_EVCH7_gc = (0x0F<<0) ; Event Channel 7

; Waveform Generation Mode
.equ TC_WGMODE_NORMAL_gc = (0x00<<0) ; Normal Mode
.equ TC_WGMODE_FRQ_gc = (0x01<<0) ; Frequency Generation Mode
.equ TC_WGMODE_SS_gc = (0x03<<0) ; Single Slope
.equ TC_WGMODE_DS_T_gc = (0x05<<0) ; Dual Slope, Update on TOP
.equ TC_WGMODE_DS_TB_gc = (0x06<<0) ; Dual Slope, Update on TOP and BOTTOM
.equ TC_WGMODE_DS_B_gc = (0x07<<0) ; Dual Slope, Update on BOTTOM

; Event Action
.equ TC_EVACT_OFF_gc = (0x00<<5) ; No Event Action
.equ TC_EVACT_CAPT_gc = (0x01<<5) ; Input Capture
.equ TC_EVACT_UPDOWN_gc = (0x02<<5) ; Externally Controlled Up/Down Count
.equ TC_EVACT_QDEC_gc = (0x03<<5) ; Quadrature Decode
.equ TC_EVACT_RESTART_gc = (0x04<<5) ; Restart
.equ TC_EVACT_FRW_gc = (0x05<<5) ; Frequency Capture
.equ TC_EVACT_PW_gc = (0x06<<5) ; Pulse-width Capture

; Event Selection
.equ TC_EVSEL_OFF_gc = (0x00<<0) ; No Event Source
.equ TC_EVSEL_CH0_gc = (0x08<<0) ; Event Channel 0
.equ TC_EVSEL_CH1_gc = (0x09<<0) ; Event Channel 1
.equ TC_EVSEL_CH2_gc = (0x0A<<0) ; Event Channel 2
.equ TC_EVSEL_CH3_gc = (0x0B<<0) ; Event Channel 3
.equ TC_EVSEL_CH4_gc = (0x0C<<0) ; Event Channel 4
.equ TC_EVSEL_CH5_gc = (0x0D<<0) ; Event Channel 5
.equ TC_EVSEL_CH6_gc = (0x0E<<0) ; Event Channel 6
.equ TC_EVSEL_CH7_gc = (0x0F<<0) ; Event Channel 7

; Error Interrupt Level
.equ TC_ERRINTLVL_OFF_gc = (0x00<<2) ; Interrupt Disabled
.equ TC_ERRINTLVL_LO_gc = (0x01<<2) ; Low Level
.equ TC_ERRINTLVL_MED_gc = (0x02<<2) ; Medium Level
.equ TC_ERRINTLVL_HI_gc = (0x03<<2) ; High Level

; Overflow Interrupt Level
.equ TC_OVFINTLVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ TC_OVFINTLVL_LO_gc = (0x01<<0) ; Low Level
.equ TC_OVFINTLVL_MED_gc = (0x02<<0) ; Medium Level
.equ TC_OVFINTLVL_HI_gc = (0x03<<0) ; High Level

; Compare or Capture D Interrupt Level
.equ TC_CCDINTLVL_OFF_gc = (0x00<<6) ; Interrupt Disabled
.equ TC_CCDINTLVL_LO_gc = (0x01<<6) ; Low Level
.equ TC_CCDINTLVL_MED_gc = (0x02<<6) ; Medium Level
.equ TC_CCDINTLVL_HI_gc = (0x03<<6) ; High Level

; Compare or Capture C Interrupt Level
.equ TC_CCCINTLVL_OFF_gc = (0x00<<4) ; Interrupt Disabled
.equ TC_CCCINTLVL_LO_gc = (0x01<<4) ; Low Level
.equ TC_CCCINTLVL_MED_gc = (0x02<<4) ; Medium Level
.equ TC_CCCINTLVL_HI_gc = (0x03<<4) ; High Level

; Compare or Capture B Interrupt Level
.equ TC_CCBINTLVL_OFF_gc = (0x00<<2) ; Interrupt Disabled
.equ TC_CCBINTLVL_LO_gc = (0x01<<2) ; Low Level
.equ TC_CCBINTLVL_MED_gc = (0x02<<2) ; Medium Level
.equ TC_CCBINTLVL_HI_gc = (0x03<<2) ; High Level

; Compare or Capture A Interrupt Level
.equ TC_CCAINTLVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ TC_CCAINTLVL_LO_gc = (0x01<<0) ; Low Level
.equ TC_CCAINTLVL_MED_gc = (0x02<<0) ; Medium Level
.equ TC_CCAINTLVL_HI_gc = (0x03<<0) ; High Level

; Timer/Counter Command
.equ TC_CMD_NONE_gc = (0x00<<2) ; No Command
.equ TC_CMD_UPDATE_gc = (0x01<<2) ; Force Update
.equ TC_CMD_RESTART_gc = (0x02<<2) ; Force Restart
.equ TC_CMD_RESET_gc = (0x03<<2) ; Force Hard Reset

; Fault Detect Action
.equ AWEX_FDACT_NONE_gc = (0x00<<0) ; No Fault Protection
.equ AWEX_FDACT_CLEAROE_gc = (0x01<<0) ; Clear Output Enable Bits
.equ AWEX_FDACT_CLEARDIR_gc = (0x03<<0) ; Clear I/O Port Direction Bits

; High Resolution Enable
.equ HIRES_HREN_NONE_gc = (0x00<<0) ; No Fault Protection
.equ HIRES_HREN_TC0_gc = (0x01<<0) ; Enable High Resolution on Timer/Counter 0
.equ HIRES_HREN_TC1_gc = (0x02<<0) ; Enable High Resolution on Timer/Counter 1
.equ HIRES_HREN_BOTH_gc = (0x03<<0) ; Enable High Resolution both Timer/Counters


;***************************************************************************
;** USART - Universal Asynchronous Receiver-Transmitter
;***************************************************************************/

; USART_STATUS masks
.equ USART_RXCIF_bm = 0x80 ; Receive Interrupt Flag
.equ USART_TXCIF_bm = 0x40 ; Transmit Interrupt Flag
.equ USART_DREIF_bm = 0x20 ; Data Register Empty Flag
.equ USART_FERR_bm = 0x10 ; Frame Error
.equ USART_BUFOVF_bm = 0x08 ; Buffer Overflow
.equ USART_PERR_bm = 0x04 ; Parity Error
.equ USART_RXB8_bm = 0x01 ; Receive Bit 8

; USART_CTRLA masks
.equ USART_RXCINTLVL_gm = 0x30 ; Receive Interrupt Level
.equ USART_RXCINTLVL0_bm = (1<<4) ; Receive Interrupt Level bit 0
.equ USART_RXCINTLVL1_bm = (1<<5) ; Receive Interrupt Level bit 1
.equ USART_TXCINTLVL_gm = 0x0C ; Transmit Interrupt Level
.equ USART_TXCINTLVL0_bm = (1<<2) ; Transmit Interrupt Level bit 0
.equ USART_TXCINTLVL1_bm = (1<<3) ; Transmit Interrupt Level bit 1
.equ USART_DREINTLVL_gm = 0x03 ; Data Register Empty Interrupt Level
.equ USART_DREINTLVL0_bm = (1<<0) ; Data Register Empty Interrupt Level bit 0
.equ USART_DREINTLVL1_bm = (1<<1) ; Data Register Empty Interrupt Level bit 1

; USART_CTRLB masks
.equ USART_RXEN_bm = 0x10 ; Receiver Enable
.equ USART_TXEN_bm = 0x08 ; Transmitter Enable
.equ USART_CLK2X_bm = 0x04 ; Double transmission speed
.equ USART_MPCM_bm = 0x02 ; Multi-processor Communication Mode
.equ USART_TXB8_bm = 0x01 ; Transmit bit 8

; USART_CTRLC masks
.equ USART_CMODE_gm = 0xC0 ; Communication Mode
.equ USART_CMODE0_bm = (1<<6) ; Communication Mode bit 0
.equ USART_CMODE1_bm = (1<<7) ; Communication Mode bit 1
.equ USART_PMODE_gm = 0x30 ; Parity Mode
.equ USART_PMODE0_bm = (1<<4) ; Parity Mode bit 0
.equ USART_PMODE1_bm = (1<<5) ; Parity Mode bit 1
.equ USART_SBMODE_bm = 0x08 ; Stop Bit Mode
.equ USART_CHSIZE_gm = 0x07 ; Character Size
.equ USART_CHSIZE0_bm = (1<<0) ; Character Size bit 0
.equ USART_CHSIZE1_bm = (1<<1) ; Character Size bit 1
.equ USART_CHSIZE2_bm = (1<<2) ; Character Size bit 2

; USART_BAUDCTRLA masks
.equ USART_BSEL_gm = 0xFF ; Baud Rate Selection Bits [7:0]
.equ USART_BSEL0_bm = (1<<0) ; Baud Rate Selection Bits [7:0] bit 0
.equ USART_BSEL1_bm = (1<<1) ; Baud Rate Selection Bits [7:0] bit 1
.equ USART_BSEL2_bm = (1<<2) ; Baud Rate Selection Bits [7:0] bit 2
.equ USART_BSEL3_bm = (1<<3) ; Baud Rate Selection Bits [7:0] bit 3
.equ USART_BSEL4_bm = (1<<4) ; Baud Rate Selection Bits [7:0] bit 4
.equ USART_BSEL5_bm = (1<<5) ; Baud Rate Selection Bits [7:0] bit 5
.equ USART_BSEL6_bm = (1<<6) ; Baud Rate Selection Bits [7:0] bit 6
.equ USART_BSEL7_bm = (1<<7) ; Baud Rate Selection Bits [7:0] bit 7

; USART_BAUDCTRLB masks
.equ USART_BSCALE_gm = 0xF0 ; Baud Rate Scale
.equ USART_BSCALE0_bm = (1<<4) ; Baud Rate Scale bit 0
.equ USART_BSCALE1_bm = (1<<5) ; Baud Rate Scale bit 1
.equ USART_BSCALE2_bm = (1<<6) ; Baud Rate Scale bit 2
.equ USART_BSCALE3_bm = (1<<7) ; Baud Rate Scale bit 3
; Masks for BSEL aready defined
 
; Receive Complete Interrupt level
.equ USART_RXCINTLVL_OFF_gc = (0x00<<4) ; Interrupt Disabled
.equ USART_RXCINTLVL_LO_gc = (0x01<<4) ; Low Level
.equ USART_RXCINTLVL_MED_gc = (0x02<<4) ; Medium Level
.equ USART_RXCINTLVL_HI_gc = (0x03<<4) ; High Level

; Transmit Complete Interrupt level
.equ USART_TXCINTLVL_OFF_gc = (0x00<<2) ; Interrupt Disabled
.equ USART_TXCINTLVL_LO_gc = (0x01<<2) ; Low Level
.equ USART_TXCINTLVL_MED_gc = (0x02<<2) ; Medium Level
.equ USART_TXCINTLVL_HI_gc = (0x03<<2) ; High Level

; Data Register Empty Interrupt level
.equ USART_DREINTLVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ USART_DREINTLVL_LO_gc = (0x01<<0) ; Low Level
.equ USART_DREINTLVL_MED_gc = (0x02<<0) ; Medium Level
.equ USART_DREINTLVL_HI_gc = (0x03<<0) ; High Level

; Character Size
.equ USART_CHSIZE_5BIT_gc = (0x00<<0) ; Character size: 5 bit
.equ USART_CHSIZE_6BIT_gc = (0x01<<0) ; Character size: 6 bit
.equ USART_CHSIZE_7BIT_gc = (0x02<<0) ; Character size: 7 bit
.equ USART_CHSIZE_8BIT_gc = (0x03<<0) ; Character size: 8 bit
.equ USART_CHSIZE_9BIT_gc = (0x07<<0) ; Character size: 9 bit

; Communication Mode
.equ USART_CMODE_ASYNCHRONOUS_gc = (0x00<<6) ; Asynchronous Mode
.equ USART_CMODE_SYNCHRONOUS_gc = (0x01<<6) ; Synchronous Mode
.equ USART_CMODE_IRDA_gc = (0x02<<6) ; IrDA Mode
.equ USART_CMODE_MSPI_gc = (0x03<<6) ; Master SPI Mode

; Parity Mode
.equ USART_PMODE_DISABLED_gc = (0x00<<4) ; No Parity
.equ USART_PMODE_EVEN_gc = (0x02<<4) ; Even Parity
.equ USART_PMODE_ODD_gc = (0x03<<4) ; Odd Parity


;***************************************************************************
;** SPI - Serial Peripheral Interface
;***************************************************************************/

; SPI_CTRL masks
.equ SPI_CLK2X_bm = 0x80 ; Enable Double Speed
.equ SPI_ENABLE_bm = 0x40 ; Enable Module
.equ SPI_DORD_bm = 0x20 ; Data Order Setting
.equ SPI_MASTER_bm = 0x10 ; Master Operation Enable
.equ SPI_MODE_gm = 0x0C ; SPI Mode
.equ SPI_MODE0_bm = (1<<2) ; SPI Mode bit 0
.equ SPI_MODE1_bm = (1<<3) ; SPI Mode bit 1
.equ SPI_PRESCALER_gm = 0x03 ; Prescaler
.equ SPI_PRESCALER0_bm = (1<<0) ; Prescaler bit 0
.equ SPI_PRESCALER1_bm = (1<<1) ; Prescaler bit 1

; SPI_INTCTRL masks
.equ SPI_INTLVL_gm = 0x03 ; Interrupt level
.equ SPI_INTLVL0_bm = (1<<0) ; Interrupt level bit 0
.equ SPI_INTLVL1_bm = (1<<1) ; Interrupt level bit 1

; SPI_STATUS masks
.equ SPI_IF_bm = 0x80 ; Interrupt Flag
.equ SPI_WRCOL_bm = 0x40 ; Write Collision
 
; SPI Mode
.equ SPI_MODE_0_gc = (0x00<<2) ; SPI Mode 0
.equ SPI_MODE_1_gc = (0x01<<2) ; SPI Mode 1
.equ SPI_MODE_2_gc = (0x02<<2) ; SPI Mode 2
.equ SPI_MODE_3_gc = (0x03<<2) ; SPI Mode 3

; Prescaler setting
.equ SPI_PRESCALER_DIV4_gc = (0x00<<0) ; System Clock / 4
.equ SPI_PRESCALER_DIV16_gc = (0x01<<0) ; System Clock / 16
.equ SPI_PRESCALER_DIV64_gc = (0x02<<0) ; System Clock / 64
.equ SPI_PRESCALER_DIV128_gc = (0x03<<0) ; System Clock / 128

; Interrupt level
.equ SPI_INTLVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ SPI_INTLVL_LO_gc = (0x01<<0) ; Low Level
.equ SPI_INTLVL_MED_gc = (0x02<<0) ; Medium Level
.equ SPI_INTLVL_HI_gc = (0x03<<0) ; High Level


;***************************************************************************
;** IRCOM - IR Communication Module
;***************************************************************************/

; IRCOM_CTRL masks
.equ IRCOM_EVSEL_gm = 0x0F ; Event Channel Select
.equ IRCOM_EVSEL0_bm = (1<<0) ; Event Channel Select bit 0
.equ IRCOM_EVSEL1_bm = (1<<1) ; Event Channel Select bit 1
.equ IRCOM_EVSEL2_bm = (1<<2) ; Event Channel Select bit 2
.equ IRCOM_EVSEL3_bm = (1<<3) ; Event Channel Select bit 3
 
; Event channel selection
.equ IRDA_EVSEL_OFF_gc = (0x00<<0) ; No Event Source
.equ IRDA_EVSEL_0_gc = (0x08<<0) ; Event Channel 0
.equ IRDA_EVSEL_1_gc = (0x09<<0) ; Event Channel 1
.equ IRDA_EVSEL_2_gc = (0x0A<<0) ; Event Channel 2
.equ IRDA_EVSEL_3_gc = (0x0B<<0) ; Event Channel 3
.equ IRDA_EVSEL_4_gc = (0x0C<<0) ; Event Channel 4
.equ IRDA_EVSEL_5_gc = (0x0D<<0) ; Event Channel 5
.equ IRDA_EVSEL_6_gc = (0x0E<<0) ; Event Channel 6
.equ IRDA_EVSEL_7_gc = (0x0F<<0) ; Event Channel 7


;***************************************************************************
;** AES - AES Module
;***************************************************************************/

; AES_CTRL masks
.equ AES_START_bm = 0x80 ; Start/Run
.equ AES_AUTO_bm = 0x40 ; Auto Start Trigger
.equ AES_RESET_bm = 0x20 ; AES Software Reset
.equ AES_DECRYPT_bm = 0x10 ; Decryption / Direction
.equ AES_XOR_bm = 0x04 ; State XOR Load Enable

; AES_STATUS masks
.equ AES_ERROR_bm = 0x80 ; AES Error
.equ AES_SRIF_bm = 0x01 ; State Ready Interrupt Flag

; AES_INTCTRL masks
.equ AES_INTLVL_gm = 0x03 ; Interrupt level
.equ AES_INTLVL0_bm = (1<<0) ; Interrupt level bit 0
.equ AES_INTLVL1_bm = (1<<1) ; Interrupt level bit 1
 
; Interrupt level
.equ AES_INTLVL_OFF_gc = (0x00<<0) ; Interrupt Disabled
.equ AES_INTLVL_LO_gc = (0x01<<0) ; Low Level
.equ AES_INTLVL_MED_gc = (0x02<<0) ; Medium Level
.equ AES_INTLVL_HI_gc = (0x03<<0) ; High Level




; ***** CPU REGISTER DEFINITIONS *****************************************
.def	XH	= r27
.def	XL	= r26
.def	YH	= r29
.def	YL	= r28
.def	ZH	= r31
.def	ZL	= r30


; ***** DATA MEMORY DECLARATIONS *****************************************


#define PROGMEM_START 0x0000
#define PROGMEM_SIZE 0x22000
#define PROGMEM_END (0x0000 + 0x22000 - 1)

#define APP_SECTION_START 0x0000
#define APP_SECTION_SIZE 0x20000
#define APP_SECTION_END (0x0000 + 0x20000 - 1)

#define APPTABLE_SECTION_START 0x1E000
#define APPTABLE_SECTION_SIZE 0x2000
#define APPTABLE_SECTION_END (0x1E000 + 0x2000 - 1)

#define BOOT_SECTION_START 0x20000
#define BOOT_SECTION_SIZE 0x2000
#define BOOT_SECTION_END (0x20000 + 0x2000 - 1)

#define DATAMEM_START 0x0000
#define DATAMEM_SIZE 0x1000000
#define DATAMEM_END (0x0000 + 0x1000000 - 1)

#define IO_START 0x0000
#define IO_SIZE 0x1000
#define IO_END (0x0000 + 0x1000 - 1)

#define MAPPED_EEPROM_START 0x1000
#define MAPPED_EEPROM_SIZE 0x0800
#define MAPPED_EEPROM_END (0x1000 + 0x0800 - 1)

#define INTERNAL_SRAM_START 0x2000
#define INTERNAL_SRAM_SIZE 0x2000
#define INTERNAL_SRAM_END (0x2000 + 0x2000 - 1)

#define EXTERNAL_SRAM_START 0x4000
#define EXTERNAL_SRAM_SIZE 0xFFC000
#define EXTERNAL_SRAM_END (0x4000 + 0xFFC000 - 1)

#define EEPROM_START 0x0000
#define EEPROM_SIZE 0x0800
#define EEPROM_END (0x0000 + 0x0800 - 1)

#define FUSE_START 0x0000
#define FUSE_SIZE 0x0006
#define FUSE_END (0x0000 + 0x0006 - 1)

#define LOCKBIT_START 0x0000
#define LOCKBIT_SIZE 0x0001
#define LOCKBIT_END (0x0000 + 0x0001 - 1)

#define SIGNATURES_START 0x0000
#define SIGNATURES_SIZE 0x0003
#define SIGNATURES_END (0x0000 + 0x0003 - 1)

#define USER_SIGNATURES_START 0x0000
#define USER_SIGNATURES_SIZE 0x0200
#define USER_SIGNATURES_END (0x0000 + 0x0200 - 1)

#define PROD_SIGNATURES_START 0x0000
#define PROD_SIGNATURES_SIZE 0x0034
#define PROD_SIGNATURES_END (0x0000 + 0x0034 - 1)


; Legacy definitions
.equ	FLASHEND    = (PROGMEM_END / 2)  ; Note: Word address
.equ	IOEND       = IO_END
.equ	SRAM_START	= INTERNAL_SRAM_START
.equ	SRAM_SIZE	= INTERNAL_SRAM_SIZE
.equ	RAMEND	    = INTERNAL_SRAM_END
.equ	XRAMEND	    = EXTERNAL_SRAM_END
.equ	E2END	    = EEPROM_END
.equ	EEPROMEND	= EEPROM_END

; Definitions used by the assembler
#pragma AVRPART MEMORY PROG_FLASH 0x22000
#pragma AVRPART MEMORY EEPROM 0x0800
#pragma AVRPART MEMORY INT_SRAM SIZE 0x2000
#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x2000



; ***** INTERRUPT VECTORS ************************************************

; OSC interrupt vectors
.equ OSC_XOSCF_vect = 2 ; External Oscillator Failure Interrupt (NMI)

; PORTC interrupt vectors
.equ PORTC_INT0_vect = 4 ; External Interrupt 0
.equ PORTC_INT1_vect = 6 ; External Interrupt 1

; PORTR interrupt vectors
.equ PORTR_INT0_vect = 8 ; External Interrupt 0
.equ PORTR_INT1_vect = 10 ; External Interrupt 1

; DMA interrupt vectors
.equ DMA_CH0_vect = 12 ; Channel 0 Interrupt
.equ DMA_CH1_vect = 14 ; Channel 1 Interrupt
.equ DMA_CH2_vect = 16 ; Channel 2 Interrupt
.equ DMA_CH3_vect = 18 ; Channel 3 Interrupt

; RTC interrupt vectors
.equ RTC_OVF_vect = 20 ; Overflow Interrupt
.equ RTC_COMP_vect = 22 ; Compare Interrupt

; TWIC interrupt vectors
.equ TWIC_TWIS_vect = 24 ; TWI Slave Interrupt
.equ TWIC_TWIM_vect = 26 ; TWI Master Interrupt

; TCC0 interrupt vectors
.equ TCC0_OVF_vect = 28 ; Overflow Interrupt
.equ TCC0_ERR_vect = 30 ; Error Interrupt
.equ TCC0_CCA_vect = 32 ; Compare or Capture A Interrupt
.equ TCC0_CCB_vect = 34 ; Compare or Capture B Interrupt
.equ TCC0_CCC_vect = 36 ; Compare or Capture C Interrupt
.equ TCC0_CCD_vect = 38 ; Compare or Capture D Interrupt

; TCC1 interrupt vectors
.equ TCC1_OVF_vect = 40 ; Overflow Interrupt
.equ TCC1_ERR_vect = 42 ; Error Interrupt
.equ TCC1_CCA_vect = 44 ; Compare or Capture A Interrupt
.equ TCC1_CCB_vect = 46 ; Compare or Capture B Interrupt

; SPIC interrupt vectors
.equ SPIC_INT_vect = 48 ; SPI Interrupt

; USARTC0 interrupt vectors
.equ USARTC0_RXC_vect = 50 ; Reception Complete Interrupt
.equ USARTC0_DRE_vect = 52 ; Data Register Empty Interrupt
.equ USARTC0_TXC_vect = 54 ; Transmission Complete Interrupt

; USARTC1 interrupt vectors
.equ USARTC1_RXC_vect = 56 ; Reception Complete Interrupt
.equ USARTC1_DRE_vect = 58 ; Data Register Empty Interrupt
.equ USARTC1_TXC_vect = 60 ; Transmission Complete Interrupt

; AES interrupt vectors
.equ AES_INT_vect = 62 ; AES Interrupt

; NVM interrupt vectors
.equ NVM_EE_vect = 64 ; EE Interrupt
.equ NVM_SPM_vect = 66 ; SPM Interrupt

; PORTB interrupt vectors
.equ PORTB_INT0_vect = 68 ; External Interrupt 0
.equ PORTB_INT1_vect = 70 ; External Interrupt 1

; ACB interrupt vectors
.equ ACB_ACW_vect = 76 ; ACW Window Mode Interrupt
.equ ACB_AC1_vect = 74 ; AC1 Interrupt
.equ ACB_AC0_vect = 72 ; AC0 Interrupt

; ADCB interrupt vectors
.equ ADCB_CH0_vect = 78 ; Interrupt 0
.equ ADCB_CH1_vect = 80 ; Interrupt 1
.equ ADCB_CH2_vect = 82 ; Interrupt 2
.equ ADCB_CH3_vect = 84 ; Interrupt 3

; PORTE interrupt vectors
.equ PORTE_INT0_vect = 86 ; External Interrupt 0
.equ PORTE_INT1_vect = 88 ; External Interrupt 1

; TWIE interrupt vectors
.equ TWIE_TWIS_vect = 90 ; TWI Slave Interrupt
.equ TWIE_TWIM_vect = 92 ; TWI Master Interrupt

; TCE0 interrupt vectors
.equ TCE0_OVF_vect = 94 ; Overflow Interrupt
.equ TCE0_ERR_vect = 96 ; Error Interrupt
.equ TCE0_CCA_vect = 98 ; Compare or Capture A Interrupt
.equ TCE0_CCB_vect = 100 ; Compare or Capture B Interrupt
.equ TCE0_CCC_vect = 102 ; Compare or Capture C Interrupt
.equ TCE0_CCD_vect = 104 ; Compare or Capture D Interrupt

; TCE1 interrupt vectors
.equ TCE1_OVF_vect = 106 ; Overflow Interrupt
.equ TCE1_ERR_vect = 108 ; Error Interrupt
.equ TCE1_CCA_vect = 110 ; Compare or Capture A Interrupt
.equ TCE1_CCB_vect = 112 ; Compare or Capture B Interrupt

; SPIE interrupt vectors
.equ SPIE_INT_vect = 114 ; SPI Interrupt

; USARTE0 interrupt vectors
.equ USARTE0_RXC_vect = 116 ; Reception Complete Interrupt
.equ USARTE0_DRE_vect = 118 ; Data Register Empty Interrupt
.equ USARTE0_TXC_vect = 120 ; Transmission Complete Interrupt

; USARTE1 interrupt vectors
.equ USARTE1_RXC_vect = 122 ; Reception Complete Interrupt
.equ USARTE1_DRE_vect = 124 ; Data Register Empty Interrupt
.equ USARTE1_TXC_vect = 126 ; Transmission Complete Interrupt

; PORTD interrupt vectors
.equ PORTD_INT0_vect = 128 ; External Interrupt 0
.equ PORTD_INT1_vect = 130 ; External Interrupt 1

; PORTA interrupt vectors
.equ PORTA_INT0_vect = 132 ; External Interrupt 0
.equ PORTA_INT1_vect = 134 ; External Interrupt 1

; ACA interrupt vectors
.equ ACA_ACW_vect = 140 ; ACW Window Mode Interrupt
.equ ACA_AC1_vect = 138 ; AC1 Interrupt
.equ ACA_AC0_vect = 136 ; AC0 Interrupt

; ADCA interrupt vectors
.equ ADCA_CH0_vect = 142 ; Interrupt 0
.equ ADCA_CH1_vect = 144 ; Interrupt 1
.equ ADCA_CH2_vect = 146 ; Interrupt 2
.equ ADCA_CH3_vect = 148 ; Interrupt 3

; TWID interrupt vectors
.equ TWID_TWIS_vect = 150 ; TWI Slave Interrupt
.equ TWID_TWIM_vect = 152 ; TWI Master Interrupt

; TCD0 interrupt vectors
.equ TCD0_OVF_vect = 154 ; Overflow Interrupt
.equ TCD0_ERR_vect = 156 ; Error Interrupt
.equ TCD0_CCA_vect = 158 ; Compare or Capture A Interrupt
.equ TCD0_CCB_vect = 160 ; Compare or Capture B Interrupt
.equ TCD0_CCC_vect = 162 ; Compare or Capture C Interrupt
.equ TCD0_CCD_vect = 164 ; Compare or Capture D Interrupt

; TCD1 interrupt vectors
.equ TCD1_OVF_vect = 166 ; Overflow Interrupt
.equ TCD1_ERR_vect = 168 ; Error Interrupt
.equ TCD1_CCA_vect = 170 ; Compare or Capture A Interrupt
.equ TCD1_CCB_vect = 172 ; Compare or Capture B Interrupt

; SPID interrupt vectors
.equ SPID_INT_vect = 174 ; SPI Interrupt

; USARTD0 interrupt vectors
.equ USARTD0_RXC_vect = 176 ; Reception Complete Interrupt
.equ USARTD0_DRE_vect = 178 ; Data Register Empty Interrupt
.equ USARTD0_TXC_vect = 180 ; Transmission Complete Interrupt

; USARTD1 interrupt vectors
.equ USARTD1_RXC_vect = 182 ; Reception Complete Interrupt
.equ USARTD1_DRE_vect = 184 ; Data Register Empty Interrupt
.equ USARTD1_TXC_vect = 186 ; Transmission Complete Interrupt

; PORTQ interrupt vectors
.equ PORTQ_INT0_vect = 188 ; External Interrupt 0
.equ PORTQ_INT1_vect = 190 ; External Interrupt 1

; PORTH interrupt vectors
.equ PORTH_INT0_vect = 192 ; External Interrupt 0
.equ PORTH_INT1_vect = 194 ; External Interrupt 1

; PORTJ interrupt vectors
.equ PORTJ_INT0_vect = 196 ; External Interrupt 0
.equ PORTJ_INT1_vect = 198 ; External Interrupt 1

; PORTK interrupt vectors
.equ PORTK_INT0_vect = 200 ; External Interrupt 0
.equ PORTK_INT1_vect = 202 ; External Interrupt 1

; PORTF interrupt vectors
.equ PORTF_INT0_vect = 208 ; External Interrupt 0
.equ PORTF_INT1_vect = 210 ; External Interrupt 1

; TWIF interrupt vectors
.equ TWIF_TWIS_vect = 212 ; TWI Slave Interrupt
.equ TWIF_TWIM_vect = 214 ; TWI Master Interrupt

; TCF0 interrupt vectors
.equ TCF0_OVF_vect = 216 ; Overflow Interrupt
.equ TCF0_ERR_vect = 218 ; Error Interrupt
.equ TCF0_CCA_vect = 220 ; Compare or Capture A Interrupt
.equ TCF0_CCB_vect = 222 ; Compare or Capture B Interrupt
.equ TCF0_CCC_vect = 224 ; Compare or Capture C Interrupt
.equ TCF0_CCD_vect = 226 ; Compare or Capture D Interrupt

; TCF1 interrupt vectors
.equ TCF1_OVF_vect = 228 ; Overflow Interrupt
.equ TCF1_ERR_vect = 230 ; Error Interrupt
.equ TCF1_CCA_vect = 232 ; Compare or Capture A Interrupt
.equ TCF1_CCB_vect = 234 ; Compare or Capture B Interrupt

; SPIF interrupt vectors
.equ SPIF_INT_vect = 236 ; SPI Interrupt

; USARTF0 interrupt vectors
.equ USARTF0_RXC_vect = 238 ; Reception Complete Interrupt
.equ USARTF0_DRE_vect = 240 ; Data Register Empty Interrupt
.equ USARTF0_TXC_vect = 242 ; Transmission Complete Interrupt

; USARTF1 interrupt vectors
.equ USARTF1_RXC_vect = 244 ; Reception Complete Interrupt
.equ USARTF1_DRE_vect = 246 ; Data Register Empty Interrupt
.equ USARTF1_TXC_vect = 248 ; Transmission Complete Interrupt


.equ INT_VECTORS_SIZE = 250 ; size in words


#endif  /* _ATxmega128A1DEF_INC_ */

; ***** END OF FILE ******************************************************



