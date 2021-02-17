#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>

// #include "Font 6x8.c"
// #include "Font 12x16.c"

/**
	������������ ������: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313&LOW=E4&HIGH=9F&EXTENDED=FF&LOCKBIT=FF

 ��� F=8M�� (���������� RC ���������) ����� 14�� + 64��
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
// PORTB	���� �� �������� ���������� KEM-3361
#define PinB_SEGM_E		7
#define PinB_SEGM_D		6
#define PinB_SEGM_T		5
#define PinB_SEGM_C		4
#define PinB_SEGM_G		3
#define PinB_SEGM_B		2
#define PinB_SEGM_F		1
#define PinB_SEGM_A		0

// PORTD
#define PinD_RAZR_1		6	//1 ������ ���������� KEM-3361
#define PinD_RAZR_3		5	//3 ������ ���������� KEM-3361
#define PinD_RAZR_2		4	//2 ������ ���������� KEM-3361

#define PinD_KnPLS		3		//����� ������ ����
#define PinD_KnMIN		2		//����� ������ �����

#define PinA_HEAT		0		//����� ���������� ������������ (1 - �������; 0 - ��������)

#define PinA_SCK		1		//����� SCK MAX6675
#define PinD_SO			0		//����� SO MAX6675 (����)
#define PinD_CS			1		//����� CS MAX6675
//===============================================================================
volatile uint8_t FLAGS1 = 0;
#define FL1_BLC1			0		//��� ~1.024mc
#define FL1_MODE_EDIT		1		//����� ��������� �������
#define FL1_MODE_PID		2		//����� ��������� ������������ � ����������
#define FL1_BLC8			3		//��� ~8mc
//===============================================================================
volatile uint8_t BUTTON = 0;			//����� ���������: 0 - ������� �����������; 1 - ����� �������; 2 - ��������� �������
#define BTN_PLS				0		//������� "+" 
#define BTN_MIN				1		//������� "-"
#define BTN_SHORT_MAKED		2		//�������� ������� ������ ����������
#define BTN_LONG			3		//������� �������

volatile uint16_t EDIT_OUT_CNT = 0;// ������� ����� ������ �� ������ ��������� �������
#define EDIT_OUT			3000	// �������� �������� ��� ������ �� ������ ��������� �������

volatile uint8_t EDIT_CNT = 0;
#define EDIT_CHANGE			150	// �������� �������� ���������� ��������� �������� �������
#define CHANGE_PLS			0	// �������� ������� ���������
#define CHANGE_MIN			1	// �������� ������� ���������

volatile uint8_t SEGM_RAZR[3];		// ����� �������� ��� ������ �� �������(������� ������ - ������� ������)
volatile uint8_t IND_RAZR = 0;		// ������ ������� ���������� �� �������

volatile uint8_t SHAKE_CNT = 0;		// ������� ������������
#define BUTTON_PUSH_SHORT_CNT		30
#define BUTTON_PUSH_LONG_CNT		250
/**
	������� ������������ ��������������� ������ ���� ~1mc
	����������� - ������� �� ������ ����� ��� �� BUTTON_SHORT_PUSH_CNT (�������� SHAKE_CNT) - ����������.
	������� ����� BUTTON_PUSH_LONG_CNT ������������ ��� ������� �������
	
	� ������� ������ (���� FL1_MODE_EDIT �������) ������� ���������� ������� �����������
	�������� ������� ����� �� ������ ��������� � ����� ������ ������� (���� FL1_MODE_EDIT ����������)
	������� ������ �������� � ��������� �������:
		- �������� - ��������� �� 1 ������
		- ������� - ��������� �� 1 ������ � 0,15���
	�� ������ ������ ���� ���� �������� ����������� ��� �������� � ������� �����
*/
volatile uint8_t PID_CNT = 0;		// ������� ��� ������ �� � - ���������
volatile uint8_t PID_VAL = 0;		// ����������� �������� ��� �������� � - ����������
/**
	���� PID_VAL = 0 ����������� ��������.
	��������� ����������� ��� PID_CNT = 0 (���� PID_VAL != 0)
	� ����������� ��� PID_CNT = PID_VAL.
	������� PID_CNT ������ ������������� �� �����.
	
	�������� ����������� PID_VAL �� 1 �� 255.
	��� PID_VAL = 1 ����������� ������� � ������� 8�� �� ������ � 2,048��� �.�. 0,39%
	��� PID_VAL = 255 ����������� ������� � ������� 2048�� �� ������ � 2,048��� �.�. 100%

*/
//-------------------------------------------------------------------------------
// ����������� ������� � ����������
volatile uint16_t SETVAL_TEMP;		// ����������� �������
uint16_t EEMEM EE_SETVAL_TEMP = 250;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_MIN	 20		//��
#define DEF_SETVAL_TEMP_MAX	 400	//��
#define DEF_SETVAL_TEMP_STEP	1	//���
volatile uint8_t SETVAL_PID;		// �������� ������������ � ����������
uint16_t EEMEM EE_SETVAL_PID = 10;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_PID_MIN	 1		//��
#define DEF_SETVAL_PID_MAX	 20		//��
#define DEF_SETVAL_PID_STEP	1		//���
//-------------------------------------------------------------------------------
volatile uint16_t CNT_TEMP_START = 0;
#define TEMP_START	1000	//���
volatile uint16_t VALUE_TC = 0;		//�������� ����������� ���������� � �������
volatile uint8_t STATUS_TC = 0;
/**  ������ �2
	STATUS_TC1 ������� ��������� ������� TC1 (��������� �-����)
	0 - �������������� ��������� (���������)
	1 - ���������, ���� ���������� �������� ����������� => �������� � �������� � �������� VALUE_TC1
	2 - ����� �������
	3 - ���������� ������������� �������� (������ 999 ��. �� �������)
*/
const uint8_t DigSeg[12] PROGMEM = {0xD7,0x14,0xCD,0x5D,0x1E,0x5B,0xDB,0x15,0xDF,0x5F,0x08,0x8F};
//===============================================================================
void init(void);
void NumberToButeBuffer(uint16_t number);
void Display(void);
void CheckButtons(void);
void ProcessingButtons(void);
void ChangeVALUE(uint8_t ditect);
void LoadValuesFromEEPROM(void);
void UpdateValuesFromEEPROM(void);
void ReadMAX6675(void);
void CalculatePID(void);
void ProcessingPID(void);
//===============================================================================
























