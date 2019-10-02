/**
	��������.
	


*/
#include <avr/io.h>
#include <avr/iom8.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <avr/pgmspace.h>
// #include <avr/eeprom.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>

// ������������ ������: http://www.engbedded.com/fusecalc/
//Fclk = 32kHz

#define		SetBit(reg, bit)			reg |= (1<<bit)           
#define		ClrBit(reg, bit)			reg &= (~(1<<bit))
#define		InvBit(reg, bit)			reg ^= (1<<bit)
#define		BitIsSet(reg, bit)			((reg & (1<<bit)) != 0)
#define		BitIsClr(reg, bit)		((reg & (1<<bit)) == 0)

//===============================================================================
// BOARD
//===============================================================================
/**
���� ���������� PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,  PC0,PC1,  PD3,PD4,PD5,PD6,PD7
���� ������ "+" -> PC4, "-" -> PC5
����� �� ���� PD2
���� ��� ADC7 (22�����)
       A
     ---
  F//  //B
    ---G
 E//  //C
   ---
    D
*/
// #define PinD_RELE 2
// #define PinC_KN_MMM 3
// #define PinC_KN_PLS 4
// #define PinC_KN_MIN 5

// #define PinC_LCD_2E 0
// #define PinC_LCD_2C 1

// #define PinB_LCD_3B 0
// #define PinB_LCD_2B 1
// #define PinB_LCD_2F 2
// #define PinB_LCD_2A 3
// #define PinB_LCD_2G 4
// #define PinB_LCD_2D 5
// #define PinB_LCD_3D 6
// #define PinB_LCD_1EF 7

// #define PinD_LCD_3C 3
// #define PinD_LCD_3E 4
// #define PinD_LCD_3G 5
// #define PinD_LCD_3A 6
// #define PinD_LCD_3F 7
//===============================================================================
// volatile uint8_t CntLCDdigit = 0;	//������ ������������ ������� LCD
// volatile uint8_t LCDvolume = 0;		//����� ��������� �� LCD

// volatile uint8_t ADCvolume = 0;		//����������� ���������� ������� (*10)
// volatile uint16_t ADCbuffer = 0;	//����� ��� ����������� ������� ���
// volatile uint8_t ADCcntBuff = 0;	//������� ������� ���������
// #define ADC_MAX_CNT_BUFF	64		//���������� ��������� � ������
// #define ADC_MAX_VOLTAGE		17.603	//���������� ���������� ����������(�� ������ �������� ����� Vadc = V����� = 1.1�
// #define ADC_COEFFICIENT		(((ADC_MAX_VOLTAGE / 1023) / ADC_MAX_CNT_BUFF) * 10)

// volatile uint8_t STATUS = 0;
/**
	0 - LCD �������, ������ ���, ��������� ��� �� ��������, ������� ������
	1 - LCD ������� ������� �������� ���, ������ ���, ��������� ��� �� ��������, ������� ������
	2 - ����� ���� ��������� �������, LCD � �������� ������ ������� ������� �������
	
	������� � ���� ��������� �������, �������� ������� - ������������� ������� "+" � "-"
	����� �� ���� ��������� ������� � ���� � �������� ����� �� �������� �����. define-��
*/
// volatile uint8_t FLAGS1 = 0;
// #define FL1_BLC250			0		//��� ~0,25���
// #define FL1_BLC2			1		//��� ~2��
// #define FL1_BTN				2		//���� ������� �� �������
// #define FL1_BTN_WAIT		3		//�����������
// #define FL1_BTN_MMM      	4		//������ M
// #define FL1_BTN_PLS      	5		//������ +
// #define FL1_BTN_MIN      	6		//������ -
// #define FL1_LCD_FAIR      	7		//����� ������� LCD. 1 - �����, 0 - �������

// volatile uint8_t FLAGS2 = 0;
// #define FL2_ADC				0		//��� ~125���
// #define FL2_ADC_OK			1		//��� ����������� �������� ������ �� �����������
// #define FL2_RELE_DELAY		2		//�������� ����� ���������� ����
//===============================================================================
// volatile uint8_t CountMUL_BTN_WAIT = 0;
// #define MUL_BTN_WAIT	100	//��������� �� 2�� ����������� ������

// volatile uint8_t CountMUL_LCD = 0;	//������� ����� � ������ �����
// #define MUL_LCD_WAIT_SLEEP	40	//��������� �� 0,25��� - ����� �� ��������� ����������
// #define MUL_MENU_EXIT	40	//��������� �� 0,25��� - ����� �� ��������

// volatile uint8_t MENU = 0;	//������� ������� ���� (������. �� 0 �� 2 ������������)
// volatile uint8_t RELEcntDelay = 0;	//������� ����� 0,25��� �� ���������� ����
//===============================================================================
// ����������� ������� � ����������
// volatile uint8_t SETVAL_HIGH;			// ���������� ��������� ������� �����		������ � ���� ������� 0
// uint8_t EEMEM EE_SETVAL_HIGH = 140;		// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_HIGH_MIN		120		//��
// #define DEF_SETVAL_HIGH_MAX		160		//��
// #define DEF_SETVAL_HIGH_STEP	1		//���
// volatile uint8_t SETVAL_LOW;			// ���������� ���������� ������� �����		������ � ���� ������� 1
// uint8_t EEMEM EE_SETVAL_LOW = 125;		// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_LOW_MIN		100		//��
// #define DEF_SETVAL_LOW_MAX		140		//��
// #define DEF_SETVAL_LOW_STEP		1		//���
// volatile uint8_t SETVAL_DELAY;			// ����� �������� ���������� ������� �����(� ���)		������ � ���� ������� 2
// uint8_t EEMEM EE_SETVAL_DELAY = 5;		// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_DELAY_MIN	0		//��
// #define DEF_SETVAL_DELAY_MAX	19		//��
// #define DEF_SETVAL_DELAY_STEP	1		//���

// #define DEF_SETVAL_MIN_INT		5		//����������� �������� ����� SETVAL_HIGH � SETVAL_LOW

//===============================================================================
void init(void);
// void Tick2(void);
// void Tick250(void);
// void CalculateVoltage(void);
// void CheckButton(void);
// void ExecuteButton(void);
// void ClearLCD(void);
// void OutLCD(void);	//����� �� ������� �����
// void LoadValuesFromEEPROM(void);
// void UpdateValuesFromEEPROM(void);
//===============================================================================
//===============================================================================
//===============================================================================
















