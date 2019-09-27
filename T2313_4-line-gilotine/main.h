#include <avr/io.h>
#include <avr/iotn2313.h>
// #include <avr/interrupt.h>
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
	������������ ������: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313&LOW=66&HIGH=9F&EXTENDED=FF&LOCKBIT=FF

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
 CKDIV8 - V
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
#define PortBVK		PORTD
#define PortPinBVK	PIND
#define PortDDrBVK	DDRD
#define PinBVK_L	0
#define PinBVK_LJ	1
#define PinBVK_R	2
#define PinBVK_RJ	3
#define PinBVK_T	4
#define PinBVK_TJ	5

#define PortKL		PORTB
#define PortPinKL	PINB
#define PortDDrKL	DDRB
#define PinKL_L		0
#define PinKL_R		1

//===============================================================================
volatile uint8_t STATUS = 0;	// 1 - ��������� � ������ ������ (���� �������� � ����� �������)
volatile uint8_t LEFT = 0;		// 1 - ���� �������� ����� ������ �����
volatile uint8_t RIGTH = 0;		// 1 - ���� �������� ����� ������ ������
volatile uint8_t BVK_T = 0;		// 1 - ���������� ��������� ������������
volatile uint8_t BVK_L = 0;		// 1 - ���������� ��������� ������ ���������
volatile uint8_t BVK_R = 0;		// 1 - ���������� ��������� ������� ���������
//===============================================================================
void init(void);
void CheckBVK(void);
void ExecuteBVK(void);
//===============================================================================
/**
	������ ��������� ��� ������������� �� ��� P-N-P (���������� ��������� - 1 => �������� ����� ��������)
	��� ��� �������� ������ ���� �����������.
	� ������ ���� ���������� ���������� ��� ���� N-P-N (���������� ��������� - 0 => �������� ����� ��������)
	�� �������� ���������� �����.
	
	������� ��������� ��������:
	- ���������� ��������� ��� ������������
	- ��� ��������� ������� "� ������", "���� �������� �����", "���� �������� ������"
	- ���� ���������� ��������� ������ �� ��������� ����� ������ ���� ����� ���� ������,
		���������� ��������� ����� ��������� ��� ���������� ������ �� ��� - �����������.
	�������� ������:
		"�����" - ���� ���� ���������� ��������� ������� ���������
		"������" - ���� ���� ���������� ��������� ������ ���������
		
	������� ���������� ��������:
	- ���� ��������� ������� "� ������"
		������ "�����":
			- ���� ������ "���� �������� �����"
			- ���� ���������� ��������� ������� ��������� ����� ������
		������ "������":
			- ���� ������ "���� �������� ������"
			- ���� ���������� ��������� ������ ��������� ����� ������
	����� ����������� ���� �������� � 500��.
	����� ���������� ���� �������� � 1���.
			
*/
























