#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <avr/pgmspace.h>
// #include <avr/eeprom.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>

// #include "Font 6x8.c"
// #include "Font 12x16.c"

/**
	������������ ������: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313&LOW=E4&HIGH=9F&EXTENDED=FF&LOCKBIT=FF

 ��� F=1M�� (���������� RC ���������) ����� 14�� + 64��
 �����: ��� USBASP � PonyProg 	1 => ������������� ��� => ����� ���		( )
								0 => ���������� ��� => �������� ������ 	(V)
 ��� ���� LOCK �����������
---- Fuse High Byte ----
 DWEN - 				(���� V � LOCK �� ����������� ��������� ����� ������� debugWIRE ����� ����� RESET
 EESAVE -
 SPIEN - V (����������)
 WDTON -
 BODLEVEL2 -
 BODLEVEL1 -
 BODLEVEL0 -
 RSTDISBL -   (����������)
---- Fuse Low Byte ----
 CKDIV8 - 
 CKOUT -
 SUT1 -
 SUT0 - V
 CKSEL3 - V
 CKSEL2 -
 CKSEL1 - V
 CKSEL0 - V
*/

#define		SetBit(reg, bit)			reg |= (1<<bit)           
#define		ClrBit(reg, bit)			reg &= (~(1<<bit))
#define		InvBit(reg, bit)			reg ^= (1<<bit)
#define		BitIsSet(reg, bit)			((reg & (1<<bit)) != 0)
#define		BitIsClr(reg, bit)		((reg & (1<<bit)) == 0)

//===============================================================================
// BOARD
//===============================================================================
#define PortRAZR		PORTB	//���� �� ULN2803 ���������� ���������
#define PinRAZR_1		7	//������� ������
#define PinRAZR_2		6
#define PinRAZR_3		5
#define PinRAZR_4		4
#define PinRAZR_5		3
#define PinRAZR_6		2
#define PinRAZR_7		1
#define PinRAZR_8		0	//������� ������

#define PortSEGM		PORTD	//���� �� 74HC595 ��������� ������� �� ��������
#define PinSEGM_C		6	//CLK
#define PinSEGM_D		5	//DATA
#define PinSEGM_E		4	//CE

#define PortIN			PORTD	//���� �������� �������
#define PinIN_C			2	//CLK
#define PinIN_D			3	//DATA
//===============================================================================
volatile uint8_t FLAGS1 = 0;
#define FL1_0_OVER			0		//������������ �������� 0
#define FL1_REC_BITS		1		//������������ �������� 0
#define FL1_BLC1			2		//��� ~1.024mc ����� ���������� ������� ���������
#define FL1_BLC250			3		//���� ����������� ��������� � ������ ������� 250/250
//===============================================================================
// volatile uint8_t STATUS = 0;	// 1 - ��������� � ������ ������ (���� �������� � ����� �������)
volatile uint8_t CNT_BITS = 0;		// ������� ����������� ��� �� SPI
volatile uint32_t BLINK250 = 0;		// ������� ��������� ������ 250/250
volatile uint8_t SEGM_RAZR[8];		// �������� �������� �������������� ��� ������ �� �������(������� ������ - ������� ������)
volatile uint8_t IND_RAZR = 0;		// ������ ������� ���������� �� �������
volatile uint32_t BUFFER = 0;		// �������� ����� SPI
volatile uint8_t RAZR_REGIM[8] = {0};		// ����� ������ ������� �������: 0 - ������ �����; 1- ������� 250/250��;
//===============================================================================
void init(void);
void AnalizBUFFER(void);
uint8_t DigToSeg(uint8_t di);
void Display(void);
//===============================================================================
/**
���������� ������������ ������������ ��������� 8 �������������� �����������
������ ������� ������� �� 3 ������ ��������� ������� ������ ������ � ������� ����� ������:
	1 ���� - ������ ������ (��������� ����)
		������� 3 ����(0,1,2) ���������� ������ ����������
		��� �3 - ����� ������� � �������� 0,25/0,25���
		������� 4 ���� - ������ ������:
			0 - ������ ��� ������ �������� �������� ���������������� ��������
			1 - ������ - ����� � BCD ������� (�����������) 0 1 2 3 4 5 6 7 8 9 A b c d E F
			2 - 
	2 ���� - ������
	3 ���� - ����������� ����� ������ ���� ������.
	
	
���� - ��������
	0 - G
	1 - dt
	2 - F
	3 - E
	4 - D
	5 - C
	6 - B
	7 - A
*/
























