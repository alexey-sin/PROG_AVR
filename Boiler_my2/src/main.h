#include <avr/io.h>
#include <avr/iom32.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <string.h>
#include <stdio.h>
// #include <stdlib.h>
#include <stdint.h>

#include "../../_libs_macros/bits_macros.h"
#include "../../_lib_tft_ili9486/tft_ili9486.h"
#include "../../_fonts/fonts.h"

// F = 16 MHz

volatile uint8_t STATUS = 0;
#define ST_BLC      		0
#define ST_BTN      		1		//���� ������� �� �������
#define ST_BTN_WAIT 		2		//�����������
#define ST_MENU     		3		//0 - ������, ����� �� ������� ����������� ���������� 1 - ����� ����
#define ST_WORK_PAUSE_HIGH	4		//������ �� ����� ������� ������
#define ST_WORK_PAUSE_LOW	5		//������ �� ����� ������ ������
#define ST_WORK_PAUSE_STOP	6		//������ �� ����� �� �����

volatile uint8_t BUTTON = 0;
#define MMM      0		//������ �
#define PLS      1		//������ +
#define MIN      2		//������ -

#define MUL_BTN_WAIT	10	//��������� �� 16.4��� ����������� ������
volatile uint8_t CountMUL_BTN_WAIT = 0;

volatile uint8_t MENU = 0;	//������� ������� ���� (������. �� 0 �� 11 ������������)
#define MUL_MENU_WAIT	10	//��������� �� ~1���, �������� ������ �� ����
volatile uint8_t CountMUL_MENU_WAIT = 0;


volatile uint8_t DS_TempData[9];
volatile double dbDS_OUT;				//����� "�����"
volatile double dbDS_ROOM;				//����� "�������"
volatile double dbDS_SERVE;				//����� "������"
volatile double dbDS_RETURN;			//����� "�������"
volatile uint8_t STATUS_DS_OUT;
volatile uint8_t STATUS_DS_ROOM;
volatile uint8_t STATUS_DS_SERVE;
volatile uint8_t STATUS_DS_RETURN;
/**  ������� �1 - �4
	STATUS_DS ������� ��������� ������� DS18B20
	0 - �������������� ��������� (���������) ��� ������
	1 - ���������, ���� ���������� �������� ����������� � �������� DS_Temp (double)
*/
volatile uint8_t STATUS_TC = 0;
volatile uint16_t VALUE_TC = 0;
/**  ������ �2
	STATUS_TC1 ������� ��������� ������� TC1 (��������� �-����)
	0 - �������������� ��������� (���������)
	1 - ���������, ���� ���������� �������� ����������� => �������� � �������� � �������� VALUE_TC1
	2 - ����� �������
	3 - ���������� ������������� �������� (������ 999 ��. �� �������)
*/

volatile uint8_t BOILER_STATUS = 10;	//������� ��������� �����
/**  
	0 - �������������� ���������
	1 - ��������
		VALUE_TC <= SETVAL_TEMP_BOILER_OFF ����������� �������� ������ ����������� ������� ���������� ���������� �����
			� ���� ������ ���� ���������� �������������� �������� ������ �������� ��������� ����� �����
			DS_Temp4(�������) <= EE_SETVAL_TEMP_TEN_ON � ���������� ����� ����� DS_Temp4(�������) >= SETVAL_TEMP_TEN_OFF
	2 - ������ �������
	3 - ����������
	4 - ������
		��������� ������ �� �������� TC, DS2, DS3, DS4 ����� ������ ��� ����������
	5 - �����
	6 - ������
*/
#define BOILER_STATUS_UNKNOWN	0
#define BOILER_STATUS_IDLE		1
#define BOILER_STATUS_AUTO		2
#define BOILER_STATUS_STOP		3
#define BOILER_STATUS_ERROR		4
#define BOILER_STATUS_PAUSE		5
#define BOILER_STATUS_MANUAL	6

// volatile uint8_t *PORT_Reg;
volatile uint8_t *DDR_Reg;
volatile uint8_t *PIN_Reg;

volatile uint8_t FLAP_STATE = 0;		//������� ��������� ��� (0 - 3)
volatile int16_t FLAP_STEPS = 0;		//������� ��������� � ����� (0 - 250)
#define FLAP_STEP_MAX	250

#define time_flap	5
#define FLAP_INIT		3	//������� �� ���� �������� ���������� �� FLAP_STEPS
#define FLAP_START		2	//������� �� ���� ��������
#define FLAP_FORVARD	1	//��������� ��������
#define FLAP_REVERCE	0	//��������� ��������

volatile uint16_t Count_PAUSE_SEC = 0;
volatile uint16_t EXTREMUM = 0;
#define DELTA_EXTREMUM	2
volatile int8_t ALARM_TRIGGER = 0;		//1 - ���������� ��������� ����������� ������������ � ���������� ����
#define ALARM_TRIGGER_OFF		0
#define ALARM_TRIGGER_ON		1
//===============================================================================
// ����������� ������� � ����������
volatile uint8_t SETVAL_AUTO_CONTROL;					// �������������� ����������� / ������	������ � ���� - 0
uint8_t EEMEM EE_SETVAL_AUTO_CONTROL = 1;				// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_AUTO_CONTROL_MAN	0					// ������ �����������
#define DEF_SETVAL_AUTO_CONTROL_AUTO	1				// �������������� �����������
//������ � ���� - 1: ���������/���������� ����� � ������ ������
volatile uint8_t SETVAL_FLAP_MANUAL_STEP;				// ���������� ����� ��������� �������� � ������ ������	������ � ���� - 2
uint8_t EEMEM EE_SETVAL_FLAP_MANUAL_STEP = 5;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_FLAP_MANUAL_STEP_MIN	1				//��
#define DEF_SETVAL_FLAP_MANUAL_STEP_MAX	25				//��
#define DEF_SETVAL_FLAP_MANUAL_STEP_STEP	1			//���
volatile uint8_t SETVAL_SMOKE_DEAD_BAND;				// ���� ������������������ ������� �������� � ������ ������� (����� �������)	������ � ���� - 3
uint8_t EEMEM EE_SETVAL_SMOKE_DEAD_BAND = 5;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_SMOKE_DEAD_BAND_MIN	1				//��
#define DEF_SETVAL_SMOKE_DEAD_BAND_MAX	20				//��
#define DEF_SETVAL_SMOKE_DEAD_BAND_STEP	1				//���
volatile uint8_t SETVAL_FLAP_AUTO_STEP;					// ���������� ����� ��������� �������� � �������������� ������	������ � ���� - 4
uint8_t EEMEM EE_SETVAL_FLAP_AUTO_STEP = 5;				// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_FLAP_AUTO_STEP_MIN	1				//��
#define DEF_SETVAL_FLAP_AUTO_STEP_MAX	25				//��
#define DEF_SETVAL_FLAP_AUTO_STEP_STEP	1				//���
volatile uint8_t SETVAL_FLAP_START_STEPS;				// ��������� ��������� �������� � %	������ � ���� - 5
uint8_t EEMEM EE_SETVAL_FLAP_START_STEPS = 30;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_FLAP_START_STEPS_MIN	5				//��
#define DEF_SETVAL_FLAP_START_STEPS_MAX	50				//��
#define DEF_SETVAL_FLAP_START_STEPS_STEP	1			//���
														
volatile uint8_t SETVAL_TEMP_SMOKE;						// ����������� ����� �������� (����� �������) 	������ � ���� - 6
uint8_t EEMEM EE_SETVAL_TEMP_SMOKE = 200;				// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_SMOKE_MIN	100					//��
#define DEF_SETVAL_TEMP_SMOKE_MAX	255					//��
#define DEF_SETVAL_TEMP_SMOKE_STEP	5					//���
volatile uint8_t SETVAL_MAX_TEMP_SERVE;					// ������������ ����������� ������ (����� �������)	������ � ���� - 7
uint8_t EEMEM EE_SETVAL_MAX_TEMP_SERVE = 80;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_MAX_TEMP_SERVE_MIN	40				//��
#define DEF_SETVAL_MAX_TEMP_SERVE_MAX	95				//��
#define DEF_SETVAL_MAX_TEMP_SERVE_STEP	1				//���
volatile uint8_t SETVAL_HYSTER_TEMP_SERVE;				// ���������� ����������� ������ (����� �������)	������ � ���� - 8
uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_SERVE = 5;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_HYSTER_TEMP_SERVE_MIN	1			//��
#define DEF_SETVAL_HYSTER_TEMP_SERVE_MAX	20			//��
#define DEF_SETVAL_HYSTER_TEMP_SERVE_STEP	1			//���
volatile uint8_t SETVAL_MAX_TEMP_ROOM;					// ������������ ����������� ������� (����� �������)	������ � ���� - 9
uint8_t EEMEM EE_SETVAL_MAX_TEMP_ROOM = 18;				// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_MAX_TEMP_ROOM_MIN	15				//��
#define DEF_SETVAL_MAX_TEMP_ROOM_MAX	28				//��
#define DEF_SETVAL_MAX_TEMP_ROOM_STEP	1				//���
volatile uint8_t SETVAL_HYSTER_TEMP_ROOM;				// ���������� ����������� ������� (����� �������)	������ � ���� - 10
uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_ROOM = 2;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_HYSTER_TEMP_ROOM_MIN	1				//��
#define DEF_SETVAL_HYSTER_TEMP_ROOM_MAX	5				//��
#define DEF_SETVAL_HYSTER_TEMP_ROOM_STEP	1			//���
volatile uint8_t SETVAL_TEMP_BOILER_OFF;				// ����������� �������� ���������� ���������� ����� (����� �������)	������ � ���� - 11
uint8_t EEMEM EE_SETVAL_TEMP_BOILER_OFF = 60;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_BOILER_OFF_MIN	20				//��
#define DEF_SETVAL_TEMP_BOILER_OFF_MAX	100				//��
#define DEF_SETVAL_TEMP_BOILER_OFF_STEP	5				//���
volatile uint8_t SETVAL_HYSTER_TEMP_BOILER_OFF;			// ���������� ����������� �������� ���������� ���������� ����� (����� �������)	������ � ���� - 12
uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_BOILER_OFF = 10;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MIN	2		//��
#define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MAX	50		//��
#define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_STEP	1		//���
														
volatile uint8_t SETVAL_LIGHT_DISPLAY;					// ��������� ����������	������ � ���� - 13
uint8_t EEMEM EE_SETVAL_LIGHT_DISPLAY = 1;				// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_LIGHT_AUTO	0						// ��������� �� ������� ����������� / � ������
#define DEF_SETVAL_LIGHT_ALL_ON	1						// ������ �������
volatile uint8_t SETVAL_TIME_LIGHT;						// ����� ������ ��������� ���������� � ���������. ������ (����� ������)	������ � ���� - 14
uint8_t EEMEM EE_SETVAL_TIME_LIGHT = 5;					// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TIME_LIGHT_MIN	1					//��
#define DEF_SETVAL_TIME_LIGHT_MAX	10					//��
#define DEF_SETVAL_TIME_LIGHT_STEP	1					//���
volatile uint8_t SETVAL_ALARM_FUEL_ON;					// ������������ ���������� �������	������ � ���� - 15
uint8_t EEMEM EE_SETVAL_ALARM_FUEL_ON = 1;				// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_ALARM_FUEL_OFF	0					// ���������
#define DEF_SETVAL_ALARM_FUEL_ON	1					// ��������
volatile uint8_t SETVAL_TEMP_ALARM_FUEL;				// ����������� ������������ ���������� ������� (����� �������)	������ � ���� - 16
uint8_t EEMEM EE_SETVAL_TEMP_ALARM_FUEL = 150;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_ALARM_FUEL_MIN	80				//��
#define DEF_SETVAL_TEMP_ALARM_FUEL_MAX	150				//��
#define DEF_SETVAL_TEMP_ALARM_FUEL_STEP	5				//���

volatile uint8_t SETVAL_TEN_MODE;						// ����� ������ �����	������ � ���� - 17
uint8_t EEMEM EE_SETVAL_TEN_MODE = 1;					// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEN_MODE_OFF	0						// ���� ���������
#define DEF_SETVAL_TEN_MODE_NOTFROZE	1				// ����� ��������� ������ ���������� �������������
#define DEF_SETVAL_TEN_MODE_HEAT_ROOM	2				// ����� ����������� ����������� � ���������
volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_ON;			// ����������� ��������� ����� ��������� �������� (����� �������)	������ � ���� - 18
uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_ON = 5;		// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MIN	2			//��
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MAX	25			//��
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_STEP	1		//���
volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_OFF;			// ����������� ���������� ����� ��������� �������� (����� �������)	������ � ���� - 19
uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_OFF = 5;		// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MIN	1		//��
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MAX	30		//��
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_STEP	1		//���
volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_ON;			// ����������� ��������� ����� �� ������� ��������� (����� �������)	������ � ���� - 20
uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_ON = 10;		// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MIN	5		//��
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MAX	25		//��
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_STEP	1		//���
volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_OFF;			// ����������� ���������� ����� �� ������� ��������� (����� �������)	������ � ���� - 21
uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_OFF = 15;	// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MIN	6		//��
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MAX	30		//��
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_STEP	1		//���
volatile uint8_t SETVAL_TEN_POWER;						// �������� ����� (���)	������ � ���� - 22
uint8_t EEMEM EE_SETVAL_TEN_POWER = 1;					// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TEN_POWER_MIN	1					//��
#define DEF_SETVAL_TEN_POWER_MAX	6					//��
#define DEF_SETVAL_TEN_POWER_STEP	1					//���

volatile uint8_t SETVAL_TIME_DELAY_PAUSE;				// �������� � ������������� � ������ ����� (����� ������)	������ � ���� - 23
uint8_t EEMEM EE_SETVAL_TIME_DELAY_PAUSE = 3;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_TIME_DELAY_PAUSE_MIN	1				//��
#define DEF_SETVAL_TIME_DELAY_PAUSE_MAX	10				//��
#define DEF_SETVAL_TIME_DELAY_PAUSE_STEP	1			//���
volatile uint8_t SETVAL_PUMP_PERFORMANCE;				// ������������������ ������ ���������� (�/���)	������ � ���� - 24
uint8_t EEMEM EE_SETVAL_PUMP_PERFORMANCE = 20;			// ��������� �������� ����������� � EEPROM
#define DEF_SETVAL_PUMP_PERFORMANCE_MIN	1				//��
#define DEF_SETVAL_PUMP_PERFORMANCE_MAX	100				//��
#define DEF_SETVAL_PUMP_PERFORMANCE_STEP	1			//���
//������ � ���� - 25: ����� �������� �������� �������
//������ � ���� - 26: ����� �������� ������������� �������
//===============================================================================
// const char* LabelHeader PROGMEM = "Boiler2 v2.0 18.09.2020";
// char* LabelT_out PROGMEM = "�����: ";
// char* LabelT_room PROGMEM = "�������: ";
// char* LabelT_serve PROGMEM = "������: ";
// char* LabelT_return PROGMEM = "�������: ";
// char* LabelT_smoke PROGMEM = "�������: ";
// char* Label_Flap PROGMEM = "�������� ����: ";
// char* Label_Sunro PROGMEM = "�����: ";
// char* Label_Pump PROGMEM = "�����: ";
char* LabelUNC PROGMEM = "UNC";
// // char* LabelBRK PROGMEM = "BREAK";
char* LabelBRK PROGMEM = "-----";
char* LabelOVR PROGMEM = "OVER";
char* LabelERR PROGMEM = "ERROR";
// char* LabelSetting PROGMEM = "���������";
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
// ������
#define PORT_PLS PORTB
#define DDR_PLS DDRB
#define PIN_PLS PINB
#define B_PLS 0

#define PORT_MIN PORTA
#define DDR_MIN DDRA
#define PIN_MIN PINA
#define B_MIN 0

// �/� ���������� ������� ���������� ��������(FLAP) TLE4729G
#define PORT_FLAP_FH PORTA
#define DDR_FLAP_FH DDRA
#define PIN_FLAP_FH PINA
#define B_FLAP_FH 2

#define PORT_FLAP_FVR PORTB
#define DDR_FLAP_FVR DDRB
#define PIN_FLAP_FVR PINB
#define B_FLAP_FVR 2

#define PORT_FLAP_REV PORTA
#define DDR_FLAP_REV DDRA
#define PIN_FLAP_REV PINA
#define B_FLAP_REV 1

// ������� DS18B20
//Out	�������
#define PORT_DS_OUT PORTD
#define DDR_DS_OUT DDRD
#define PIN_DS_OUT PIND
#define B_DS_OUT 1

//Room	�������
#define PORT_DS_ROOM PORTD
#define DDR_DS_ROOM DDRD
#define PIN_DS_ROOM PIND
#define B_DS_ROOM 2

//Serve (����������� ������)
#define PORT_DS_SRV PORTD
#define DDR_DS_SRV DDRD
#define PIN_DS_SRV PIND
#define B_DS_SRV 3

//Return (����������� �������)
#define PORT_DS_RTN PORTD
#define DDR_DS_RTN DDRD
#define PIN_DS_RTN PIND
#define B_DS_RTN 4

// ������ ������� ����� - ��������� �� MAX6675
#define PORT_TC_SO PORTD
#define DDR_TC_SO DDRD
#define PIN_TC_SO PIND
#define B_TC_SO 5

#define PORT_TC_CS PORTD
#define DDR_TC_CS DDRD
#define PIN_TC_CS PIND
#define B_TC_CS 6

#define PORT_TC_SCK PORTD
#define DDR_TC_SCK DDRD
#define PIN_TC_SCK PIND
#define B_TC_SCK 7

//Beep ������ ��������
#define PORT_BEEP PORTB
#define DDR_BEEP DDRB
#define PIN_BEEP PINB
#define B_BEEP 1

//Preheating ���� ��������� ����� ���������
#define PORT_PHEAT PORTB
#define DDR_PHEAT DDRB
#define PIN_PHEAT PINB
#define B_PHEAT 3
//===============================================================================
void init(void);
void CheckButton(void);
void ExecuteButton(void);
void FlapTravel(uint8_t direct, uint16_t steps);
void OutFullInfoToLCD(void);	//����� ����������� ��������� �� ������� (��������� ������� ������)
void OutVarInfoToLCD(void);	//����� ����������� ��������� �� ������� (������ ����������)
void OutTemperatureDS(uint8_t dat, uint16_t x, uint16_t y);	//����� �� ������� �������� ����������� ������� DS18B20
void ReadMAX6675(void);	//������ ����������� � VALUE_TC � STATUS_TC
void OutTemperatureTC(uint16_t x, uint16_t y);	//����� �� ������� �������� ����������� ������� TC
void OutPercent(uint16_t value, uint16_t max_value, uint16_t x, uint16_t y);		//����� �� ������� �������� � ���������: value - ������� ��������; max_value - �������� ��� 100%
void LoadValuesFromEEPROM(void);
void UpdateValuesFromEEPROM(void);
void OutMenuToLCD(void);	//����� ���� � ������� �� �������
void OutMenuToLCDValue(void);	//����� ������� ����
uint16_t OutNumber(uint8_t val, uint16_t x, uint16_t y);
uint16_t OutBigNumber(uint16_t val, uint16_t x, uint16_t y);
void Work(void);	//������ � ������

//===============================================================================
uint8_t DS18B20_init(uint8_t dat);	//Out: 0 - OK; 1 - ��� �������
uint8_t DS18B20_send(uint8_t dat, uint8_t sbyte);	//���� ������ ����� 0�FF, �� �� ������ ����� �������� ����
void DS18B20_convertTemp(uint8_t dat);	//�������� ������� �������������� �����������.
void DS18B20_getTemp(uint8_t dat);	//������, ��������� � ���������� �����������
uint8_t DS18B20_read(uint8_t dat);	//��������� 9 ������ � �����, ������������ CRC. Out: 0 - OK, 1 - error CRC
uint8_t DS_calcCRC8(uint8_t data, uint8_t crc);	//Out: CRC
// void DS18B20_SetResolution(uint8_t numBit);	//��������� ���������� �������, �� ��������� ������������ ���������� 12 ���
//===============================================================================
// char GetHex(uint8_t b);
// void Work_T12_M12(void);	//������ � ������ ������� � ��������� �1 � �2 � ������������� �1 � �2
// void Work_T3_M3(void);	//������ � ������ ������� � �������� �3 � ������������ �3
// uint8_t OutBigNumber(uint16_t val);	//����� ���������� �������� ������� �� �������
// uint8_t CalculatePercent(uint8_t diskr);

//===============================================================================
/**
	������� ���������� ��������������� ������ �������� ��-20
	// �������:
		// �1 - DS18B20 �� +125��.� => �������� ����������� � ���������.
		// �2 - ��������� �-���� (�������-�������) �� 999��.� => �������� ����������� ����� ����.
		// �3 - ��������� �-���� (�������-�������) �� 999��.� => �������� ������ ������ ���������� �����.
	// �����������: (������� ���� 12 �����, �������� 6-12��)
		// �1 => ������ ������� � ��������������� ������������.
		// �2 => ������ ������� � �������� �����.
		// �3 => ������ ������� � ������ ������ ���������� �����.

	// ��������� � ����������:
		// �������� �� ������� 84�48����.
			// � ���������� ������ ���������� ����������� � ��������� ���� ��������
			// � ��������� ������������ � % ���������� �������� ���������� ������
			// � ������ ��������� ��������� ����������� � �������������� ������� ������ ��������
		// ���������:
			// 2 ������: ��������������� ������� (1 ��� � �������) - ������� ��������, �������� ��������
					  // ��������������� ���������� (1 ��� � �������) - ������� �������.

		// ��� ������: "�", "+", "-" 
			// "�" - ����  ���� ��������� � �������������� ���������� �� ������.
			// "+" � "-" - � ������ ��������� ��������� ���������������� ��������� �������
				// � ���������� ������ ��� � ���� ��������� ��� ��������� ������
			// ����� �� ������ ���� - 10 ������ ��� ������� �� ������.
	
	// �������� ������:
	// ������ �������� ������� ������� �� 2 ����������� �������.
		// 1 ������: ������� �1, �2 � ����������� �1, �2
		// 2 ������: ������ �3 � ���������� �3

		// 1 ������ ���������� � ������ ����� �������� �2 - �1 ������ �������� �������
		// � ����������� �������������� ����� �������� �2 - �1 ������ �������.

		// ��������� ������� ����������� �1 � �2:
			// ������ �1 �� ������ (�������) �� 100% � ������� ������� ������� � ���.
			// �������� 30 ������
			// ������ �2 �� ������ (�������) �� 100% � ������� 30 ������

		// ��������� ��������� ������������ �1 � �2:
			// ���������� ����������� �2
			// �������� 15 ������
			// ������� ��������� ����������� �1 � ������� ������� ������� � ���.
			
		// ������� ������� �1-�2:
			// ����������� �1 ������ ��� ����� (������� - ������)
			// �
			// ����������� �2 ������ ��� ����� (������� - ������)
			
		// ������� ��������� �1-�2:
			// ����������� �1 ���� �2 �������� ��� ���� ����� �������

		// 2 ������ ������� ������, ���������� �3 ���������� � ������ � ��������� ����� �������
		// ��� ���������� �3 ����������� "����� �������" � ��������������� ����������� ��������
		// � ������ ����������� �3. �� 100% �������� ���������� ������� ��� ���������� �3 
		// ����������� "������ �������" � ���������� �������� �� ���� �������� � ������ 
		// ����������� ����� ����������� �3. ��� �������� ����������� �3 � ���������
		// "������ �������" - "����� �������" �3 ��������������� ��������� �������� �� ������ ���������.

*/



// volatile uint8_t SETVAL_AUTO_CONTROL;					// �������������� ����������� / ������	������ � ���� - 0
// uint8_t EEMEM EE_SETVAL_AUTO_CONTROL = 1;				// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_AUTO_CONTROL_MAN	0					// ������ �����������
// #define DEF_SETVAL_AUTO_CONTROL_AUTO	1				// �������������� �����������

// //������ � ���� - 1: ���������/���������� ����� � ������ ������

// volatile uint8_t SETVAL_FLAP_MANUAL_STEP;				// ���������� ����� ��������� �������� � ������ ������	������ � ���� - 2
// uint8_t EEMEM EE_SETVAL_FLAP_MANUAL_STEP = 5;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_FLAP_MANUAL_STEP_MIN	1				//��
// #define DEF_SETVAL_FLAP_MANUAL_STEP_MAX	25				//��
// #define DEF_SETVAL_FLAP_MANUAL_STEP_STEP	1			//���

// volatile uint8_t SETVAL_SMOKE_DEAD_BAND;				// ���� ������������������ ������� �������� � ������ ������� (����� �������)	������ � ���� - 3
// uint8_t EEMEM EE_SETVAL_SMOKE_DEAD_BAND = 5;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_SMOKE_DEAD_BAND_MIN	1				//��
// #define DEF_SETVAL_SMOKE_DEAD_BAND_MAX	20				//��
// #define DEF_SETVAL_SMOKE_DEAD_BAND_STEP	1				//���

// volatile uint8_t SETVAL_FLAP_AUTO_STEP;					// ���������� ����� ��������� �������� � �������������� ������	������ � ���� - 4
// uint8_t EEMEM EE_SETVAL_FLAP_AUTO_STEP = 5;				// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_FLAP_AUTO_STEP_MIN	1				//��
// #define DEF_SETVAL_FLAP_AUTO_STEP_MAX	25				//��
// #define DEF_SETVAL_FLAP_AUTO_STEP_STEP	1				//���

// volatile uint8_t SETVAL_FLAP_START_STEPS;				// ��������� ��������� �������� � %	������ � ���� - 5
// uint8_t EEMEM EE_SETVAL_FLAP_START_STEPS = 30;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_FLAP_START_STEPS_MIN	5				//��
// #define DEF_SETVAL_FLAP_START_STEPS_MAX	50				//��
// #define DEF_SETVAL_FLAP_START_STEPS_STEP	1			//���
														
// volatile uint8_t SETVAL_TEMP_SMOKE;						// ����������� ����� �������� (����� �������) 	������ � ���� - 6
// uint8_t EEMEM EE_SETVAL_TEMP_SMOKE = 200;				// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_SMOKE_MIN	100					//��
// #define DEF_SETVAL_TEMP_SMOKE_MAX	255					//��
// #define DEF_SETVAL_TEMP_SMOKE_STEP	5					//���

// volatile uint8_t SETVAL_MAX_TEMP_SERVE;					// ������������ ����������� ������ (����� �������)	������ � ���� - 7
// uint8_t EEMEM EE_SETVAL_MAX_TEMP_SERVE = 80;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_MAX_TEMP_SERVE_MIN	40				//��
// #define DEF_SETVAL_MAX_TEMP_SERVE_MAX	95				//��
// #define DEF_SETVAL_MAX_TEMP_SERVE_STEP	1				//���

// volatile uint8_t SETVAL_HYSTER_TEMP_SERVE;				// ���������� ����������� ������ (����� �������)	������ � ���� - 8
// uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_SERVE = 5;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_HYSTER_TEMP_SERVE_MIN	1			//��
// #define DEF_SETVAL_HYSTER_TEMP_SERVE_MAX	20			//��
// #define DEF_SETVAL_HYSTER_TEMP_SERVE_STEP	1			//���

// volatile uint8_t SETVAL_MAX_TEMP_ROOM;					// ������������ ����������� ������� (����� �������)	������ � ���� - 9
// uint8_t EEMEM EE_SETVAL_MAX_TEMP_ROOM = 18;				// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_MAX_TEMP_ROOM_MIN	15				//��
// #define DEF_SETVAL_MAX_TEMP_ROOM_MAX	28				//��
// #define DEF_SETVAL_MAX_TEMP_ROOM_STEP	1				//���

// volatile uint8_t SETVAL_HYSTER_TEMP_ROOM;				// ���������� ����������� ������� (����� �������)	������ � ���� - 10
// uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_ROOM = 2;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_HYSTER_TEMP_ROOM_MIN	1				//��
// #define DEF_SETVAL_HYSTER_TEMP_ROOM_MAX	5				//��
// #define DEF_SETVAL_HYSTER_TEMP_ROOM_STEP	1			//���

// volatile uint8_t SETVAL_TEMP_BOILER_OFF;				// ����������� �������� ���������� ���������� ����� (����� �������)	������ � ���� - 11
// uint8_t EEMEM EE_SETVAL_TEMP_BOILER_OFF = 60;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_BOILER_OFF_MIN	20				//��
// #define DEF_SETVAL_TEMP_BOILER_OFF_MAX	70				//��
// #define DEF_SETVAL_TEMP_BOILER_OFF_STEP	5				//���

// volatile uint8_t SETVAL_HYSTER_TEMP_BOILER_OFF;			// ���������� ����������� �������� ���������� ���������� ����� (����� �������)	������ � ���� - 12
// uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_BOILER_OFF = 10;	// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MIN	2		//��
// #define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MAX	50		//��
// #define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_STEP	1		//���
														
// volatile uint8_t SETVAL_LIGHT_DISPLAY;					// ��������� ����������	������ � ���� - 13
// uint8_t EEMEM EE_SETVAL_LIGHT_DISPLAY = 1;				// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_LIGHT_AUTO	0						// ��������� �� ������� ����������� / � ������
// #define DEF_SETVAL_LIGHT_ALL_ON	1						// ������ �������

// volatile uint8_t SETVAL_TIME_LIGHT;						// ����� ������ ��������� ���������� � ���������. ������ (����� ������)	������ � ���� - 14
// uint8_t EEMEM EE_SETVAL_TIME_LIGHT = 5;					// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TIME_LIGHT_MIN	1					//��
// #define DEF_SETVAL_TIME_LIGHT_MAX	10					//��
// #define DEF_SETVAL_TIME_LIGHT_STEP	1					//���

// volatile uint8_t SETVAL_ALARM_FUEL_ON;					// ������������ ���������� �������	������ � ���� - 15
// uint8_t EEMEM EE_SETVAL_ALARM_FUEL_ON = 1;				// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_ALARM_FUEL_OFF	0					// ���������
// #define DEF_SETVAL_ALARM_FUEL_ON	1					// ��������

// volatile uint8_t SETVAL_TEMP_ALARM_FUEL;				// ����������� ������������ ���������� ������� (����� �������)	������ � ���� - 16
// uint8_t EEMEM EE_SETVAL_TEMP_ALARM_FUEL = 150;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_ALARM_FUEL_MIN	80				//��
// #define DEF_SETVAL_TEMP_ALARM_FUEL_MAX	150				//��
// #define DEF_SETVAL_TEMP_ALARM_FUEL_STEP	5				//���

// volatile uint8_t SETVAL_TEN_MODE;						// ����� ������ �����	������ � ���� - 17
// uint8_t EEMEM EE_SETVAL_TEN_MODE = 1;					// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEN_MODE_OFF	0						// ���� ���������
// #define DEF_SETVAL_TEN_MODE_NOTFROZE	1				// ����� ��������� ������ ���������� �������������
// #define DEF_SETVAL_TEN_MODE_HEAT_ROOM	2				// ����� ����������� ����������� � ���������

// volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_ON;			// ����������� ��������� ����� ��������� �������� (����� �������)	������ � ���� - 18
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_ON = 5;		// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MIN	2			//��
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MAX	25			//��
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_STEP	1		//���

// volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_OFF;			// ����������� ���������� ����� ��������� �������� (����� �������)	������ � ���� - 19
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_OFF = 5;		// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MIN	1		//��
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MAX	30		//��
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_STEP	1		//���

// volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_ON;			// ����������� ��������� ����� �� ������� ��������� (����� �������)	������ � ���� - 20
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_ON = 10;		// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MIN	5		//��
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MAX	25		//��
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_STEP	1		//���

// volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_OFF;			// ����������� ���������� ����� �� ������� ��������� (����� �������)	������ � ���� - 21
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_OFF = 15;	// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MIN	6		//��
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MAX	30		//��
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_STEP	1		//���

// volatile uint8_t SETVAL_TEN_POWER;						// �������� ����� (���)	������ � ���� - 22
// uint8_t EEMEM EE_SETVAL_TEN_POWER = 1;					// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TEN_POWER_MIN	1					//��
// #define DEF_SETVAL_TEN_POWER_MAX	6					//��
// #define DEF_SETVAL_TEN_POWER_STEP	1					//���

// volatile uint8_t SETVAL_TIME_DELAY_PAUSE;				// �������� � ������������� � ������ ����� (����� ������)	������ � ���� - 23
// uint8_t EEMEM EE_SETVAL_TIME_DELAY_PAUSE = 3;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_TIME_DELAY_PAUSE_MIN	1				//��
// #define DEF_SETVAL_TIME_DELAY_PAUSE_MAX	10				//��
// #define DEF_SETVAL_TIME_DELAY_PAUSE_STEP	1			//���

// volatile uint8_t SETVAL_PUMP_PERFORMANCE;				// ������������������ ������ ���������� (�/���)	������ � ���� - 24
// uint8_t EEMEM EE_SETVAL_PUMP_PERFORMANCE = 20;			// ��������� �������� ����������� � EEPROM
// #define DEF_SETVAL_PUMP_PERFORMANCE_MIN	1				//��
// #define DEF_SETVAL_PUMP_PERFORMANCE_MAX	100				//��
// #define DEF_SETVAL_PUMP_PERFORMANCE_STEP	1			//���

// //������ � ���� - 25: ����� �������� �������� �������

// //������ � ���� - 26: ����� �������� ������������� �������





















