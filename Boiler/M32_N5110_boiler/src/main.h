#include <avr/io.h>
#include <avr/iom32.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "Font 6x8.c"
#include "Font 12x16.c"

// ������������ ������: https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega32&LOW=4F&HIGH=99&LOCKBIT=FF

#define		SetBit(reg, bit)			reg |= (1<<bit)           
#define		ClrBit(reg, bit)			reg &= (~(1<<bit))
#define		InvBit(reg, bit)			reg ^= (1<<bit)
#define		BitIsSet(reg, bit)			((reg & (1<<bit)) != 0)
#define		BitIsClr(reg, bit)		((reg & (1<<bit)) == 0)

volatile uint8_t STATUS = 0;
#define ST_BLC      0
#define ST_BTN      1		//���� ������� �� �������
#define ST_BTN_WAIT 2		//�����������
#define ST_MENU     3		//0 - ������, ����� �� ������� ����������� ���������� 1 - ����� ����
#define ST_WORK_T12_M12  4	//1 - ������ �1,�2,�1 � �2 ��������� � ������
#define ST_WORK_T3_M3    5	//1 - ������ �3 � �3 ��������� � ������
#define ST_M12_START  6		//1 - �1 � �2 � ��������� �������
#define ST_M12_STOP	  7		//1 - �1 � �2 � ��������� ����������

volatile uint8_t BUTTON = 0;
#define MMM      0		//������ �
#define PLS      1		//������ +
#define MIN      2		//������ -

#define MUL_BTN_WAIT	6	//��������� �� 32��� ����������� ������
volatile uint8_t CountMUL_BTN_WAIT = 0;

volatile uint8_t MENU = 0;	//������� ������� ���� (������. �� 0 �� 11 ������������)
#define MUL_MENU_WAIT	10	//��������� �� ~1���, �������� ������ �� ����
volatile uint8_t CountMUL_MENU_WAIT = 0;


volatile uint8_t DS_TempData[9];
volatile double DS_Temp;
volatile uint8_t STATUS_DS = 0;
/**  ������ �1
	STATUS_DS ������� ��������� ������� DS18B20
	0 - �������������� ��������� (���������) ��� ������
	1 - ���������, ���� ���������� �������� ����������� � �������� DS_Temp (double)
*/
volatile uint8_t STATUS_TC1 = 0;
volatile uint16_t VALUE_TC1 = 0;
/**  ������ �2
	STATUS_TC1 ������� ��������� ������� TC1 (��������� �-����)
	0 - �������������� ��������� (���������)
	1 - ���������, ���� ���������� �������� ����������� => �������� � �������� � �������� VALUE_TC1
	2 - ����� �������
	3 - ���������� ������������� �������� (������ 999 ��. �� �������)
*/

volatile uint8_t STATUS_TC2 = 0;
volatile uint16_t VALUE_TC2 = 0;
/** ������ �3
	STATUS_TC2 ������� ��������� ������� TC2 (��������� �-����)
	0 - �������������� ��������� (���������)
	1 - ���������, ���� ���������� �������� ����������� => �������� � �������� � �������� VALUE_TC2
	2 - ����� �������
	3 - ���������� ������������� �������� (������ 999 ��. �� �������)
*/
#define PWM1	1
#define PWM2	2
#define PWM3	3

// #define M1_START_PERIOD	30		//����� ������� �1 � ��������
#define M2_START_PERIOD	30		//����� ������� �2 � ��������
#define M12_BETWEEN_START_PERIOD	30		//����� ����� ��������� �1 � �2 � ��������
#define M12_BETWEEN_STOP_PERIOD	15		//����� ����� ������������ �1 � �2 � ��������

volatile uint8_t ui8_PWM1_LEVEL = 0;
volatile uint8_t ui8_PWM2_LEVEL = 0;
volatile uint8_t ui8_PWM3_LEVEL = 0;
volatile double dbl_PWM1_LEVEL = 0.0;
volatile double dbl_PWM2_LEVEL = 0.0;

volatile uint8_t ui8_M12_STATUS = 0;	//��������� ������������ �1 � �2
/**
	��� ��������� �������
	0 - �1 � �2 �����������
	1 - �1 � �������� �������/����������, �2 ����������
	2 - �1 � 100% ��������, �2 ����������
	3 - �1 � 100% ��������, �2 � �������� �������/����������
	4 - �1 � 100% ��������, �2 � 100% ��������
	
	��� ��������� ����������
	0 - �1 � 100% ��������, �2 � 100% ��������
	1 - �1 � 100% ��������, �2 ����������
	2 - �1 � 100% �������� ����������, �2 ����������
	3 - �1 � �2 �����������
*/
volatile uint16_t ui16_M12_CNT = 0;		//������� ������ 1��� � ������ ����� �1 � �2
//===============================================================================
// ����������� ���������
volatile uint8_t ui8_PWM1_Start_Discret = 0;	//�������� PWM1 ��� ������ � ��������� 
volatile uint8_t ui8_PWM2_Start_Discret = 0;	//�������� PWM2 ��� ������ � ��������� 
volatile uint8_t ui8_PWM3_Start_Discret = 0;	//�������� PWM3 ��� ������ � ��������� 
volatile double dbl_PWM1_Step_Start_Discret = 0.0;	//������� ������� �������� � ���������� �������� �� 1 ��� �������(�������)
volatile double dbl_PWM1_Step_Stop_Discret = 0.0;	//������� ������� ������ �� �������� �������� �� 1 ��� ����������(�������)
volatile double dbl_PWM2_Step_Discret = 0.0;	//������� ������� �������� � ���������� �������� �� 1 ��� �������(�������)
volatile double dbl_PWM3_Step_Discret = 0.0;	//������� ������� �������� � ���������� �������� �� 1 ������
//===============================================================================
volatile uint8_t LED_LIGTH;				//�������� �������. 1 - �����
uint8_t EEMEM EE_LED_LIGTH = 1;			// ��������� �������� ����������� � EEPROM
//===============================================================================
// ����������� ������� � ����������
volatile uint8_t SETVAL_TEMP_ROOM;				// ����������� � ��������� (����� �������)	������ � ���� - 0
uint8_t EEMEM EE_SETVAL_TEMP_ROOM = 25;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_ROOM_MIN	10			//��
#define DEF_SETVAL_TEMP_ROOM_MAX	30			//��
#define DEF_SETVAL_TEMP_ROOM_STEP	1			//���
volatile uint8_t SETVAL_DELTA_TEMP_ROOM;		// ������ (����) ����������� � ��������� (����� �������) 	������ � ���� - 1
uint8_t EEMEM EE_SETVAL_DELTA_TEMP_ROOM = 2;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_DELTA_TEMP_ROOM_MIN	1		//��
#define DEF_SETVAL_DELTA_TEMP_ROOM_MAX	5		//��
#define DEF_SETVAL_DELTA_TEMP_ROOM_STEP	1		//���
volatile uint8_t SETVAL_TEMP_TOP_OVEN;			// ����������� ����������� ���� (����� �������)	������ � ���� - 2
uint8_t EEMEM EE_SETVAL_TEMP_TOP_OVEN = 100;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_TOP_OVEN_MIN	55		//��
#define DEF_SETVAL_TEMP_TOP_OVEN_MAX	140		//��
#define DEF_SETVAL_TEMP_TOP_OVEN_STEP	1		//���
volatile uint8_t SETVAL_DELTA_TEMP_TOP_OVEN;	// ������ ����������� ����������� ���� (����� �������)	������ � ���� - 3
uint8_t EEMEM EE_SETVAL_DELTA_TEMP_TOP_OVEN = 10;// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_DELTA_TEMP_TOP_OVEN_MIN	1	//��
#define DEF_SETVAL_DELTA_TEMP_TOP_OVEN_MAX	20	//��
#define DEF_SETVAL_DELTA_TEMP_TOP_OVEN_STEP	1	//���
volatile uint16_t SETVAL_TEMP_PYROLYS_START;	//����������� ������ �������� - ������ ����������	������ � ���� - 4
uint16_t EEMEM EE_SETVAL_TEMP_PYROLYS_START = 400;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_PYROLYS_START_MIN	200	//��
#define DEF_SETVAL_TEMP_PYROLYS_START_MAX	600	//��
#define DEF_SETVAL_TEMP_PYROLYS_START_STEP	10	//���
volatile uint16_t SETVAL_TEMP_PYROLYS_FULL;		//����������� �������� - ������ �������� ����������� ����������	������ � ���� - 5
uint16_t EEMEM EE_SETVAL_TEMP_PYROLYS_FULL = 600;	// ��������� �������� ����������� � EEPROM
//����������� �������� - SETVAL_TEMP_PYROLYS_START + 100
#define DEF_SETVAL_TEMP_PYROLYS_FULL_MAX	990	//��
#define DEF_SETVAL_TEMP_PYROLYS_FULL_STEP	10	//���
volatile uint8_t SETVAL_DELTA_TEMP_SHUTDOWN;	//������� �������� � ���.���� � � � ��������� (�2-�1) ��� ���������� ����������	������ � ���� - 6
uint8_t EEMEM EE_SETVAL_DELTA_TEMP_SHUTDOWN = 10;// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_DELTA_TEMP_SHUTDOWN_MIN	5	//��
#define DEF_SETVAL_DELTA_TEMP_SHUTDOWN_MAX	20	//��
#define DEF_SETVAL_DELTA_TEMP_SHUTDOWN_STEP	1	//���
volatile uint8_t SETVAL_PWM_FAN1_START;			//��������� �������� PWM ����������� 1 � %	������ � ���� - 7
uint8_t EEMEM EE_SETVAL_PWM_FAN1_START = 2;		// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_PWM_FAN1_START_MIN	0		//��
#define DEF_SETVAL_PWM_FAN1_START_MAX	20		//��
#define DEF_SETVAL_PWM_FAN1_START_STEP	1		//���
volatile uint8_t SETVAL_PWM_FAN2_START;			//��������� �������� PWM ����������� 2 � %	������ � ���� - 8
uint8_t EEMEM EE_SETVAL_PWM_FAN2_START = 2;		// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_PWM_FAN2_START_MIN	0		//��
#define DEF_SETVAL_PWM_FAN2_START_MAX	20		//��
#define DEF_SETVAL_PWM_FAN2_START_STEP	1		//���
volatile uint8_t SETVAL_PWM_FAN3_START;			//��������� �������� PWM ����������� 3 � %	������ � ���� - 9
uint8_t EEMEM EE_SETVAL_PWM_FAN3_START = 2;		// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_PWM_FAN3_START_MIN	0		//��
#define DEF_SETVAL_PWM_FAN3_START_MAX	20		//��
#define DEF_SETVAL_PWM_FAN3_START_STEP	1		//���
volatile uint8_t SETVAL_M1_START_PERIOD;		//����� ������� ����������� 1 � ���	������ � ���� - 10
uint8_t EEMEM EE_SETVAL_M1_START_PERIOD = 10;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_M1_START_PERIOD_MIN	0		//��
#define DEF_SETVAL_M1_START_PERIOD_MAX	60		//��
#define DEF_SETVAL_M1_START_PERIOD_STEP	1		//���
volatile uint8_t SETVAL_M1_STOP_PERIOD;			//����� ���������� ����������� 1 � ���	������ � ���� - 11
uint8_t EEMEM EE_SETVAL_M1_STOP_PERIOD = 10;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_M1_STOP_PERIOD_MIN	0		//��
#define DEF_SETVAL_M1_STOP_PERIOD_MAX	60		//��
#define DEF_SETVAL_M1_STOP_PERIOD_STEP	1		//���
//===============================================================================
const char LabelT1[] PROGMEM = {4,'T','1',':',' '};
const char LabelT2[] PROGMEM = {4,'T','2',':',' '};
const char LabelT3[] PROGMEM = {4,'T','3',':',' '};
const char LabelM1[] PROGMEM = {4,'�','1',':',' '};
const char LabelM2[] PROGMEM = {4,'�','2',':',' '};
const char LabelM3[] PROGMEM = {4,'�','3',':',' '};
const char LabelUNC[] PROGMEM = {3,'U','N','C'};
const char LabelBRK[] PROGMEM = {5,'B','R','E','A','K'};
const char LabelOVR[] PROGMEM = {4,'O','V','E','R'};
const char LabelERR[] PROGMEM = {5,'E','R','R','O','R'};
const char LabelCC[] PROGMEM = {2,0xb7,'C'};
const char LabelSetting[] PROGMEM = {9,'�','�','�','�','�','�','�','�','�'};
//===============================================================================
const char Label_TEMP[] PROGMEM = {11,'�','�','�','�','�','�','�','�','�','�','�'};
const char Label_ROOM[] PROGMEM = {9,'�','�','�','�','�','�','�','�','�'};
const char Label_DELTA_TEMP[] PROGMEM = {14,'�','�','�','�','�','�',' ','�','�','�','�','�','�','.'};
const char Label_TOP_OVEN[] PROGMEM = {13,'�','�','�','�','�','�','�','.',' ','�','�','�','�'};
const char Label_START_PYROLYS[] PROGMEM = {14,'�','�','�','�','�','�',' ','�','�','�','�','�','�','�'};
const char Label_FULL_PYROLYS[] PROGMEM = {14,'�','�','�','�','�','�',' ','�','�','�','�','�','�','�'};
const char Label_RAZN_TEMP[] PROGMEM = {14,'�','�','�','�','�','�','�','�',' ','�','�','�','�','.'};
const char Label_SHUTDOWN[] PROGMEM = {14,'�','�','�','�','�','�','�','�','�','�',' ','�','�','�'};
const char Label_PWM_START[] PROGMEM = {12,'�','�','�','�','.',' ','�','�','�','�','�','�'};
const char Label_FAN[] PROGMEM = {12,'�','�','�','�','�','�','�','�','�','�','�',' '};
const char Label_TIME_START[] PROGMEM = {13,'�','�','�','�','�',' ','�','�','�','�','�','�','�'};
const char Label_TIME_STOP[] PROGMEM = {14,'�','�','�','�','�',' ','�','�','�','�','�','�','�','.'};
const char Label_MIN[] PROGMEM = {3,'�','�','�'};
//===============================================================================
#define DS18B20_SEARCH_ROM 		0xF0	//����� ������� ���� ��������� �� �������������
#define DS18B20_READ_ROM 		0x33	//����������� ������ ������������� ����������
#define DS18B20_MATCH_ROM 		0x55	//��������� ����������� ���������� �� ��� ������
#define DS18B20_SKIP_ROM 		0xCC	//��������� � ������������� �� ���� ���������� ��� �������� ��� ������
#define DS18B20_ALARM_SEARCH 	0xEC	//����� ���������, � ������� �������� ALARM (�������� ������ ��� � CMD_SERCH_ROM)
#define DS18B20_CONVERT_T 		0x44	//����� �������������� �����������
#define DS18B20_W_SCRATCHPAD 	0x4E	//������ �� ���������� ����� (��������)
#define DS18B20_R_SCRATCHPAD 	0xBE	//������ ����������� ������ (���������)
#define DS18B20_C_SCRATCHPAD 	0x48	//���������� ��������� � EEPROM 
#define DS18B20_RECALL_EE 		0xB8	//������� � ����� �� EEPROM �������� ������ ALARM
#define DS18B20_READ_POWER 		0xB4	//�����������, ���� �� � ���� ���������� � ���������� ��������
#define DS18B20_RES_9BIT 		0x1F	//���������� ������� (9 ���)
#define DS18B20_RES_10BIT 		0x3F	//���������� ������� (10 ���)
#define DS18B20_RES_11BIT 		0x5F	//���������� ������� (11 ���)
#define DS18B20_RES_12BIT 		0x7F	//���������� ������� (12 ���)
#define CRC8INIT				0x00
#define CRC8POLY				0x18	//0X18 = X^8+X^5+X^4+X^0
//===============================================================================
// BOARD
//===============================================================================
#define PortLED PORTB
#define PortPinLED PINB
#define PinLED 3

#define PortLCD PORTD
#define PinLCD_RST 4
#define PinLCD_CE 3
#define PinLCD_DC 2
#define PinLCD_DIN 1
#define PinLCD_CLK 0
#define PinLCD_BL 6

#define PortBTN PORTC
#define PortPinBTN PINC
#define PinBTN_MMM 0
#define PinBTN_PLS 1
#define PinBTN_MIN 2

#define PortTC PORTC
#define PortPinTC PINC
#define PinTC_SCL 7
#define PinTC_SO 4
#define PinTC_CE1 6
#define PinTC_CE2 5

#define PortDS PORTA
#define PortPinDS PINA
#define PortDdrDS DDRA
#define PinDS_DQ 6

#define PortPWM PORTA
#define PinPWM_SCK 2
#define PinPWM_MOSI 3
//===============================================================================
void Work_T12_M12(void);	//������ � ������ ������� � ��������� �1 � �2 � ������������� �1 � �2
void Work_T3_M3(void);	//������ � ������ ������� � �������� �3 � ������������ �3
void CalculateCoefficients(void);	//���������� ������������� �������� ��� �������
void ExecuteButton(void);
void init(void);
void LoadValuesFromEEPROM(void);
void UpdateValuesFromEEPROM(void);
void LCD_init(void);
void LCD_sendByte(uint8_t byte, uint8_t cmd);	//byte - ���������� ����; cmd = 1 - ������, 0 - �������
void LCD_gotoXY(uint8_t row, uint8_t col);	// row = 0-5(������); col = 0-83(�������)
void LCD_clear(uint8_t bit);	//���������� ������ bit=0 0x00 ��� bit>0 0xFF
void LCD_OutChar6x8(char ch);
void LCD_OutString6x8(char* str);
uint8_t LCD_OutChar12x16(char ch, uint8_t row, uint8_t col);
uint8_t LCD_OutString12x16(char* str, uint8_t row, uint8_t col);
uint8_t LCD_OutNumber12x16(uint8_t val, uint8_t row, uint8_t col);
uint8_t LCD_OutBigNumber12x16(uint16_t val, uint8_t row, uint8_t col);
void ReadMAX6675(uint8_t dat);	//dat - ����� ������� 1,2; ������ ����������� � ���� ���������� VALUE_TC1(2) � STATUS_TC1(2)
void OutMenuToLCD(void);	//����� ���� � ������� �� �������
void OutInfoToLCD(void);	//����� ����������� ��������� �� �������
uint8_t CalculatePercent(uint8_t diskr);
void OutTemperatureTC(uint8_t status, uint16_t value);	//����� �� ������� �������� ����������� �������� TC
void OutTemperatureDS(void);	//����� �� ������� �������� ����������� ������� DS18B20
void OutUI8_6x8(uint8_t value);		//����� �� ������� �������� �������� ������������
void CheckButton(void);
//===============================================================================
uint8_t DS18B20_init(void);	//Out: 0 - OK; 1 - ��� �������
uint8_t DS18B20_send(uint8_t sbyte);	//���� ������ ����� 0�FF, �� �� ������ ����� �������� ����
void DS18B20_convertTemp(void);	//�������� ������� �������������� �����������.
void DS18B20_getTemp(void);	//������, ��������� � ���������� �����������
uint8_t DS18B20_read(void);	//��������� 9 ������ � �����, ������������ CRC. Out: 0 - OK, 1 - error CRC
uint8_t DS_calcCRC8(uint8_t data, uint8_t crc);	//Out: CRC
void DS18B20_SetResolution(uint8_t numBit);	//��������� ���������� �������, �� ��������� ������������ ���������� 12 ���
//===============================================================================
void PWM_send(uint8_t numPWM, uint8_t level);
void PWM_sendByte(uint8_t data);
//===============================================================================
// char GetHex(uint8_t b);

//===============================================================================
/**
	������� ���������� ������������� ���������� ����
	�������:
		�1 - DS18B20 �� +125��.� => �������� ����������� � ���������.
		�2 - ��������� �-���� (�������-�������) �� 999��.� => �������� ����������� ����� ����.
		�3 - ��������� �-���� (�������-�������) �� 999��.� => �������� ������ ������ ���������� �����.
	�����������: (������� ���� 12 �����, �������� 6-12��)
		�1 => ������ ������� � ��������������� ������������.
		�2 => ������ ������� � �������� �����.
		�3 => ������ ������� � ������ ������ ���������� �����.

	��������� � ����������:
		�������� �� ������� 84�48����.
			� ���������� ������ ���������� ����������� � ��������� ���� ��������
			� ��������� ������������ � % ���������� �������� ���������� ������
			� ������ ��������� ��������� ����������� � �������������� ������� ������ ��������
		���������:
			2 ������: ��������������� ������� (1 ��� � �������) - ������� ��������, �������� ��������
					  ��������������� ���������� (1 ��� � �������) - ������� �������.

		��� ������: "�", "+", "-" 
			"�" - ����  ���� ��������� � �������������� ���������� �� ������.
			"+" � "-" - � ������ ��������� ��������� ���������������� ��������� �������
				� ���������� ������ ��� � ���� ��������� ��� ��������� ������
			����� �� ������ ���� - 10 ������ ��� ������� �� ������.
	
	�������� ������:
	������ �������� ������� ������� �� 2 ����������� �������.
		1 ������: ������� �1, �2 � ����������� �1, �2
		2 ������: ������ �3 � ���������� �3

		1 ������ ���������� � ������ ����� �������� �2 - �1 ������ �������� �������
		� ����������� �������������� ����� �������� �2 - �1 ������ �������.

		��������� ������� ����������� �1 � �2:
			������ �1 �� ������ (�������) �� 100% � ������� ������� ������� � ���.
			�������� 30 ������
			������ �2 �� ������ (�������) �� 100% � ������� 30 ������

		��������� ��������� ������������ �1 � �2:
			���������� ����������� �2
			�������� 15 ������
			������� ��������� ����������� �1 � ������� ������� ������� � ���.
			
		������� ������� �1-�2:
			����������� �1 ������ ��� ����� (������� - ������)
			�
			����������� �2 ������ ��� ����� (������� - ������)
			
		������� ��������� �1-�2:
			����������� �1 ���� �2 �������� ��� ���� ����� �������

		2 ������ ������� ������, ���������� �3 ���������� � ������ � ��������� ����� �������
		��� ���������� �3 ����������� "����� �������" � ��������������� ����������� ��������
		� ������ ����������� �3. �� 100% �������� ���������� ������� ��� ���������� �3 
		����������� "������ �������" � ���������� �������� �� ���� �������� � ������ 
		����������� ����� ����������� �3. ��� �������� ����������� �3 � ���������
		"������ �������" - "����� �������" �3 ��������������� ��������� �������� �� ������ ���������.

*/
























