/**
	����� ������.
	�������� ������:
		� GPS ������, � ���������� ����� �������, ���� � ������������� ������ �� ��������� $GPRMC
		� ������� ������������
		� ������� BME280 � ���������� ������ � �����������, ��������� � ����������� �������� �������
	��������� ������ �������� ����� ������ NRF24L01+ � ���� � ������� 16 ����.
	01 02 03 04 ... 15 16
	
	������
	00 - $
	01 - ����	(����� UTC)
	02 - ������
	03 - �������
	04 - �����
	05 - �����
	06 - ��� (��� ��������� �������)
	07 - 77 => ������ ����������, 00 => �� ���������� 
	
	08 - ������� ������ ��� ������������
	09 - ������� ������ ��� ������������
	
	10 - ���� �����������
	11 - �����������
	12 - ��������� � %
	13 - ������� ������ ������������ ��������
	14 - ������� ������ ������������ ��������
	
	15 - ����������� ����� ������� (8 ������ ����������� ��� (^) ���� ������ 00-30 ������������)
	
	������� 1: �� ������ ������� GPS �� BC-337 � ����� SiRFSTAR III - �������� ������ 4800���
	������� 2: � ����� ������� GPS GY-NEO6MV2 � ����� NEO-6M-0-001 - �������� ������ 9600���
*/
#include <avr/io.h>
#include "../../../_libs_macros/bits_macros.h"
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <avr/pgmspace.h>
// #include <avr/eeprom.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
#include "../../../_lib_nrf24l01/nrf24.h"

// ������������ ������: ������� ����� 8MHz, ��������� �����
//Fclk = 8MHz


//===============================================================================
// BOARD
//===============================================================================
/**

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

#define PinD_BTN 3
#define PinD_LED 4
// #define PinD_LCD_3G 5
// #define PinD_LCD_3A 6
// #define PinD_LCD_3F 7
//===============================================================================
#define SIZE_RX_ARRAY	16		//�������� 32!
unsigned char tx_nrf[SIZE_RX_ARRAY] = {0x00};
unsigned char* nrf_address = (unsigned char*)"METEO";

#define SIZE_RX_GPS_BUFFER	90
volatile uint8_t rx_gps[SIZE_RX_GPS_BUFFER];
volatile uint8_t cntRxGPS = 0;	//������ ������������ ����� �� RS232
volatile uint8_t statusRxGPS = 0;	//������ ������ rx_gps
/** statusRxGPS
	0 - �������� ��� ������
	1 - � �������� ������
	2 - ������ ����� ������ / ����� � �������
	
*/

// const char* nrf_address PROGMEM = {0x4D,0x45,0x54,0x45,0x4F};	//METEO
// unsigned char nrf_address[5] = {0x4D,0x45,0x54,0x45,0x4F};	//METEO
// volatile uint8_t LCDvolume = 0;		//����� ��������� �� LCD

volatile uint16_t ADCvolume = 0;	//����������� �������� ���
volatile uint32_t ADCbuffer = 0;	//����� ��� ����������� ������� ���
volatile uint8_t ADCcntBuff = 0;	//������� ������� ���������
#define ADC_MAX_CNT_BUFF	64		//���������� ��������� � ������

// volatile uint8_t STATUS = 0;
/**
	0 - LCD �������, ������ ���, ��������� ��� �� ��������, ������� ������
	1 - LCD ������� ������� �������� ���, ������ ���, ��������� ��� �� ��������, ������� ������
	2 - ����� ���� ��������� �������, LCD � �������� ������ ������� ������� �������
	
	������� � ���� ��������� �������, �������� ������� - ������������� ������� "+" � "-"
	����� �� ���� ��������� ������� � ���� � �������� ����� �� �������� �����. define-��
*/
volatile uint8_t FLAGS1 = 0;
// #define FL1_BLC2				0		//��� ~2�� - ����� ������ ������� �� RS232 �� GPS ������
#define FL1_BLC1000				1		//��� ~1���
// #define FL1_ADC_OK				2		//��� ����������� �������� ������ �� �����������
// #define FL1_BTN_WAIT		3		//�����������
// #define FL1_BTN_MMM      	4		//������ M
// #define FL1_BTN_PLS      	5		//������ +
// #define FL1_BTN_MIN      	6		//������ -
// #define FL1_LCD_FAIR      	7		//����� ������� LCD. 1 - �����, 0 - �������

// volatile uint8_t FLAGS2 = 0;
// // #define FL2_ADC				0		//��� ~125���
// // #define FL2_ADC_OK			1		//��� ����������� �������� ������ �� �����������
// #define FL2_RELE_DELAY		2		//�������� ����� ���������� ����
//===============================================================================
// volatile uint8_t CountMUL_BTN_WAIT = 0;
// #define MUL_BTN_WAIT	100	//��������� �� 2�� ����������� ������

// volatile uint8_t CountMUL_LCD = 0;	//������� ����� � ������ �����
// #define MUL_LCD_WAIT_SLEEP	40	//��������� �� 0,25��� - ����� �� ��������� ����������
// #define MUL_MENU_EXIT	40	//��������� �� 0,25��� - ����� �� ��������

// volatile uint8_t MENU = 0;	//������� ������� ���� (������. �� 0 �� 2 ������������)
/**
	0 - ��������� ���������� ��������� ������� �����
	1 - ��������� ���������� ���������� ������� �����
	2 - ��������� ������� �������� ���������� ������� �����
*/
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
void AnalizRxGPS(void);
void CalculateLight(void);
void SendPacket(void);
// void CheckButton(void);
// void ExecuteButton(void);
// void ClearLCD(void);
// void OutLCD(void);	//����� �� ������� �����
// void LoadValuesFromEEPROM(void);
// void UpdateValuesFromEEPROM(void);
//===============================================================================
//===============================================================================
//===============================================================================
















