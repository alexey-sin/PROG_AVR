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
#define ST_BTN      		1		//есть событие по кнопкам
#define ST_BTN_WAIT 		2		//антидребезг
#define ST_MENU     		3		//0 - работа, вывод на дисплей оперативных параметров 1 - режим меню
#define ST_WORK_PAUSE_HIGH	4		//Запрос на паузу верхняя дверца
#define ST_WORK_PAUSE_LOW	5		//Запрос на паузу нижняя дверца
#define ST_WORK_PAUSE_STOP	6		//Запрос на выход из паузы

volatile uint8_t BUTTON = 0;
#define MMM      0		//кнопка М
#define PLS      1		//кнопка +
#define MIN      2		//кнопка -

#define MUL_BTN_WAIT	10	//множитель на 16.4мкс антидребезг кнопок
volatile uint8_t CountMUL_BTN_WAIT = 0;

volatile uint8_t MENU = 0;	//текущая позиция меню (индекс. от 0 до 11 включительно)
#define MUL_MENU_WAIT	10	//множитель на ~1сек, ожидание выхода из меню
volatile uint8_t CountMUL_MENU_WAIT = 0;


volatile uint8_t DS_TempData[9];
volatile double dbDS_OUT;				//дачик "Улица"
volatile double dbDS_ROOM;				//дачик "Комната"
volatile double dbDS_SERVE;				//дачик "Подача"
volatile double dbDS_RETURN;			//дачик "Обратка"
volatile uint8_t STATUS_DS_OUT;
volatile uint8_t STATUS_DS_ROOM;
volatile uint8_t STATUS_DS_SERVE;
volatile uint8_t STATUS_DS_RETURN;
/**  Датчики Т1 - Т4
	STATUS_DS регистр состояния датчика DS18B20
	0 - неопределенное состояние (начальное) или ошибка
	1 - нормально, есть измеренное значение температуры в регистре DS_Temp (double)
*/
volatile uint8_t STATUS_TC = 0;
volatile uint16_t VALUE_TC = 0;
/**  Датчик Т2
	STATUS_TC1 регистр состояния датчика TC1 (термопара К-типа)
	0 - неопределенное состояние (начальное)
	1 - нормально, есть измеренное значение температуры => значение в градусах в регистре VALUE_TC1
	2 - обрыв датчика
	3 - превышение максимального значения (больше 999 Гр. по цельсию)
*/

volatile uint8_t BOILER_STATUS = 10;	//Регистр состояния котла
/**  
	0 - Неопределенное состояние
	1 - ОЖИДАНИЕ
		VALUE_TC <= SETVAL_TEMP_BOILER_OFF температура дымохода меньше температуры уставки отключения управления котла
			В этом режиме если установлен автоматический подогрев ТЭНами возможно включение ТЭНов когда
			DS_Temp4(обратка) <= EE_SETVAL_TEMP_TEN_ON и выключение ТЭНов когда DS_Temp4(обратка) >= SETVAL_TEMP_TEN_OFF
	2 - РАБОТА АВТОМАТ
	3 - ОСТАНОВЛЕН
	4 - ОШИБКА
		Состояние любого из датчиков TC, DS2, DS3, DS4 имеет ошибку или неисправен
	5 - ПАУЗА
	6 - РУЧНОЙ
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

volatile uint8_t FLAP_STATE = 0;		//текущее положение фаз (0 - 3)
volatile int16_t FLAP_STEPS = 0;		//текущее положение в шагах (0 - 250)
#define FLAP_STEP_MAX	250

#define time_flap	5
#define FLAP_INIT		3	//закрыть до нуля заслонку независимо от FLAP_STEPS
#define FLAP_START		2	//закрыть до нуля заслонку
#define FLAP_FORVARD	1	//открывать заслонку
#define FLAP_REVERCE	0	//закрывать заслонку

volatile uint16_t Count_PAUSE_SEC = 0;
volatile uint16_t EXTREMUM = 0;
#define DELTA_EXTREMUM	2
volatile int8_t ALARM_TRIGGER = 0;		//1 - взведенное положение разрешающее сигнализацию о прогорании дров
#define ALARM_TRIGGER_OFF		0
#define ALARM_TRIGGER_ON		1
//===============================================================================
// Сохраняемые уставки и переменные
volatile uint8_t SETVAL_AUTO_CONTROL;					// Автоматическая регулировка / Ручная	Индекс в МЕНЮ - 0
uint8_t EEMEM EE_SETVAL_AUTO_CONTROL = 1;				// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_AUTO_CONTROL_MAN	0					// Ручная регулировка
#define DEF_SETVAL_AUTO_CONTROL_AUTO	1				// Автоматическая регулировка
//Индекс в МЕНЮ - 1: Включение/Выключение ТЭНов в ручном режиме
volatile uint8_t SETVAL_FLAP_MANUAL_STEP;				// Количество шагов двигателя заслонки в ручном режиме	Индекс в МЕНЮ - 2
uint8_t EEMEM EE_SETVAL_FLAP_MANUAL_STEP = 5;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_FLAP_MANUAL_STEP_MIN	1				//от
#define DEF_SETVAL_FLAP_MANUAL_STEP_MAX	25				//до
#define DEF_SETVAL_FLAP_MANUAL_STEP_STEP	1			//шаг
volatile uint8_t SETVAL_SMOKE_DEAD_BAND;				// Зона нечувствительности датчика дымохода в каждую сторону (целые градусы)	Индекс в МЕНЮ - 3
uint8_t EEMEM EE_SETVAL_SMOKE_DEAD_BAND = 5;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_SMOKE_DEAD_BAND_MIN	1				//от
#define DEF_SETVAL_SMOKE_DEAD_BAND_MAX	20				//до
#define DEF_SETVAL_SMOKE_DEAD_BAND_STEP	1				//шаг
volatile uint8_t SETVAL_FLAP_AUTO_STEP;					// Количество шагов двигателя заслонки в автоматическом режиме	Индекс в МЕНЮ - 4
uint8_t EEMEM EE_SETVAL_FLAP_AUTO_STEP = 5;				// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_FLAP_AUTO_STEP_MIN	1				//от
#define DEF_SETVAL_FLAP_AUTO_STEP_MAX	25				//до
#define DEF_SETVAL_FLAP_AUTO_STEP_STEP	1				//шаг
volatile uint8_t SETVAL_FLAP_START_STEPS;				// Стартовое положение заслонки в %	Индекс в МЕНЮ - 5
uint8_t EEMEM EE_SETVAL_FLAP_START_STEPS = 30;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_FLAP_START_STEPS_MIN	5				//от
#define DEF_SETVAL_FLAP_START_STEPS_MAX	50				//до
#define DEF_SETVAL_FLAP_START_STEPS_STEP	1			//шаг
														
volatile uint8_t SETVAL_TEMP_SMOKE;						// температура газов дымохода (целые градусы) 	Индекс в МЕНЮ - 6
uint8_t EEMEM EE_SETVAL_TEMP_SMOKE = 200;				// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_SMOKE_MIN	100					//от
#define DEF_SETVAL_TEMP_SMOKE_MAX	255					//до
#define DEF_SETVAL_TEMP_SMOKE_STEP	5					//шаг
volatile uint8_t SETVAL_MAX_TEMP_SERVE;					// максимальная температура подачи (целые градусы)	Индекс в МЕНЮ - 7
uint8_t EEMEM EE_SETVAL_MAX_TEMP_SERVE = 80;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_MAX_TEMP_SERVE_MIN	40				//от
#define DEF_SETVAL_MAX_TEMP_SERVE_MAX	95				//до
#define DEF_SETVAL_MAX_TEMP_SERVE_STEP	1				//шаг
volatile uint8_t SETVAL_HYSTER_TEMP_SERVE;				// Гистерезис температуры подачи (целые градусы)	Индекс в МЕНЮ - 8
uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_SERVE = 5;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_HYSTER_TEMP_SERVE_MIN	1			//от
#define DEF_SETVAL_HYSTER_TEMP_SERVE_MAX	20			//до
#define DEF_SETVAL_HYSTER_TEMP_SERVE_STEP	1			//шаг
volatile uint8_t SETVAL_MAX_TEMP_ROOM;					// максимальная температура комнаты (целые градусы)	Индекс в МЕНЮ - 9
uint8_t EEMEM EE_SETVAL_MAX_TEMP_ROOM = 18;				// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_MAX_TEMP_ROOM_MIN	15				//от
#define DEF_SETVAL_MAX_TEMP_ROOM_MAX	28				//до
#define DEF_SETVAL_MAX_TEMP_ROOM_STEP	1				//шаг
volatile uint8_t SETVAL_HYSTER_TEMP_ROOM;				// Гистерезис температуры комнаты (целые градусы)	Индекс в МЕНЮ - 10
uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_ROOM = 2;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_HYSTER_TEMP_ROOM_MIN	1				//от
#define DEF_SETVAL_HYSTER_TEMP_ROOM_MAX	5				//до
#define DEF_SETVAL_HYSTER_TEMP_ROOM_STEP	1			//шаг
volatile uint8_t SETVAL_TEMP_BOILER_OFF;				// Температура дымохода отключения управления котла (целые градусы)	Индекс в МЕНЮ - 11
uint8_t EEMEM EE_SETVAL_TEMP_BOILER_OFF = 60;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_BOILER_OFF_MIN	20				//от
#define DEF_SETVAL_TEMP_BOILER_OFF_MAX	100				//до
#define DEF_SETVAL_TEMP_BOILER_OFF_STEP	5				//шаг
volatile uint8_t SETVAL_HYSTER_TEMP_BOILER_OFF;			// Гистерезис температуры дымохода отключения управления котла (целые градусы)	Индекс в МЕНЮ - 12
uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_BOILER_OFF = 10;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MIN	2		//от
#define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MAX	50		//до
#define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_STEP	1		//шаг
														
volatile uint8_t SETVAL_LIGHT_DISPLAY;					// Подсветка индикатора	Индекс в МЕНЮ - 13
uint8_t EEMEM EE_SETVAL_LIGHT_DISPLAY = 1;				// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_LIGHT_AUTO	0						// Включение по датчику присутствия / с кнопок
#define DEF_SETVAL_LIGHT_ALL_ON	1						// Всегда включен
volatile uint8_t SETVAL_TIME_LIGHT;						// Время работы подсветки индикатора а автоматич. режиме (целые минуты)	Индекс в МЕНЮ - 14
uint8_t EEMEM EE_SETVAL_TIME_LIGHT = 5;					// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TIME_LIGHT_MIN	1					//от
#define DEF_SETVAL_TIME_LIGHT_MAX	10					//до
#define DEF_SETVAL_TIME_LIGHT_STEP	1					//шаг
volatile uint8_t SETVAL_ALARM_FUEL_ON;					// Сигнализация прогорания топлива	Индекс в МЕНЮ - 15
uint8_t EEMEM EE_SETVAL_ALARM_FUEL_ON = 1;				// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_ALARM_FUEL_OFF	0					// Выключена
#define DEF_SETVAL_ALARM_FUEL_ON	1					// Включена
volatile uint8_t SETVAL_TEMP_ALARM_FUEL;				// Температура сигнализации прогорания топлива (целые градусы)	Индекс в МЕНЮ - 16
uint8_t EEMEM EE_SETVAL_TEMP_ALARM_FUEL = 150;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_ALARM_FUEL_MIN	80				//от
#define DEF_SETVAL_TEMP_ALARM_FUEL_MAX	150				//до
#define DEF_SETVAL_TEMP_ALARM_FUEL_STEP	5				//шаг

volatile uint8_t SETVAL_TEN_MODE;						// Режим работы ТЕНов	Индекс в МЕНЮ - 17
uint8_t EEMEM EE_SETVAL_TEN_MODE = 1;					// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEN_MODE_OFF	0						// ТЕНы выключены
#define DEF_SETVAL_TEN_MODE_NOTFROZE	1				// Режим подогрева против замерзания теплоносителя
#define DEF_SETVAL_TEN_MODE_HEAT_ROOM	2				// Режим поддержания температуры в помещении
volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_ON;			// температура включения ТЕНов аварийный подогрев (целые градусы)	Индекс в МЕНЮ - 18
uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_ON = 5;		// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MIN	2			//от
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MAX	25			//до
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_STEP	1		//шаг
volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_OFF;			// температура выключения ТЕНов аварийный подогрев (целые градусы)	Индекс в МЕНЮ - 19
uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_OFF = 5;		// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MIN	1		//от
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MAX	30		//до
#define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_STEP	1		//шаг
volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_ON;			// температура включения ТЕНов по нагреву помещения (целые градусы)	Индекс в МЕНЮ - 20
uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_ON = 10;		// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MIN	5		//от
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MAX	25		//до
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_STEP	1		//шаг
volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_OFF;			// температура выключения ТЕНов по нагреву помещения (целые градусы)	Индекс в МЕНЮ - 21
uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_OFF = 15;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MIN	6		//от
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MAX	30		//до
#define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_STEP	1		//шаг
volatile uint8_t SETVAL_TEN_POWER;						// Мощность ТЕНов (кВт)	Индекс в МЕНЮ - 22
uint8_t EEMEM EE_SETVAL_TEN_POWER = 1;					// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEN_POWER_MIN	1					//от
#define DEF_SETVAL_TEN_POWER_MAX	6					//до
#define DEF_SETVAL_TEN_POWER_STEP	1					//шаг

volatile uint8_t SETVAL_TIME_DELAY_PAUSE;				// Задержка в регулировании в режиме рауза (целые минуты)	Индекс в МЕНЮ - 23
uint8_t EEMEM EE_SETVAL_TIME_DELAY_PAUSE = 3;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TIME_DELAY_PAUSE_MIN	1				//от
#define DEF_SETVAL_TIME_DELAY_PAUSE_MAX	10				//до
#define DEF_SETVAL_TIME_DELAY_PAUSE_STEP	1			//шаг
volatile uint8_t SETVAL_PUMP_PERFORMANCE;				// Производительность насоса циркуляции (л/мин)	Индекс в МЕНЮ - 24
uint8_t EEMEM EE_SETVAL_PUMP_PERFORMANCE = 20;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_PUMP_PERFORMANCE_MIN	1				//от
#define DEF_SETVAL_PUMP_PERFORMANCE_MAX	100				//до
#define DEF_SETVAL_PUMP_PERFORMANCE_STEP	1			//шаг
//Индекс в МЕНЮ - 25: Сброс счетчика тепловой энергии
//Индекс в МЕНЮ - 26: Сброс счетчика электрической энергии
//===============================================================================
// const char* LabelHeader PROGMEM = "Boiler2 v2.0 18.09.2020";
// char* LabelT_out PROGMEM = "Улица: ";
// char* LabelT_room PROGMEM = "Комната: ";
// char* LabelT_serve PROGMEM = "Подача: ";
// char* LabelT_return PROGMEM = "Обратка: ";
// char* LabelT_smoke PROGMEM = "Дымоход: ";
// char* Label_Flap PROGMEM = "Заслонка тяги: ";
// char* Label_Sunro PROGMEM = "Шибер: ";
// char* Label_Pump PROGMEM = "Насос: ";
char* LabelUNC PROGMEM = "UNC";
// // char* LabelBRK PROGMEM = "BREAK";
char* LabelBRK PROGMEM = "-----";
char* LabelOVR PROGMEM = "OVER";
char* LabelERR PROGMEM = "ERROR";
// char* LabelSetting PROGMEM = "НАСТРОЙКА";
//===============================================================================
#define DS18B20_SEARCH_ROM 		0xF0	//Поиск адресов всех устройств по спецалгоритму
#define DS18B20_READ_ROM 		0x33	//Считываение адреса единственного устройства
#define DS18B20_MATCH_ROM 		0x55	//Активация конкретного устройства по его адресу
#define DS18B20_SKIP_ROM 		0xCC	//Обращение к единственному на шине устройству без указания его адреса
#define DS18B20_ALARM_SEARCH 	0xEC	//Поиск устройств, у которых сработал ALARM (алгоритм поиска как у CMD_SERCH_ROM)
#define DS18B20_CONVERT_T 		0x44	//Старт преобразования температуры
#define DS18B20_W_SCRATCHPAD 	0x4E	//Запись во внутренний буфер (регистры)
#define DS18B20_R_SCRATCHPAD 	0xBE	//Чтение внутреннего буфера (регистров)
#define DS18B20_C_SCRATCHPAD 	0x48	//Сохранение регистров в EEPROM 
#define DS18B20_RECALL_EE 		0xB8	//Заносит в буфер из EEPROM значение порога ALARM
#define DS18B20_READ_POWER 		0xB4	//Определение, есть ли в шине устройства с паразитным питанием
#define DS18B20_RES_9BIT 		0x1F	//Разрешение датчика (9 бит)
#define DS18B20_RES_10BIT 		0x3F	//Разрешение датчика (10 бит)
#define DS18B20_RES_11BIT 		0x5F	//Разрешение датчика (11 бит)
#define DS18B20_RES_12BIT 		0x7F	//Разрешение датчика (12 бит)
#define CRC8INIT				0x00
#define CRC8POLY				0x18	//0X18 = X^8+X^5+X^4+X^0
//===============================================================================
// BOARD
//===============================================================================
// Кнопки
#define PORT_PLS PORTB
#define DDR_PLS DDRB
#define PIN_PLS PINB
#define B_PLS 0

#define PORT_MIN PORTA
#define DDR_MIN DDRA
#define PIN_MIN PINA
#define B_MIN 0

// м/с управления шаговым двигателем заслонки(FLAP) TLE4729G
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

// Датчики DS18B20
//Out	Уличный
#define PORT_DS_OUT PORTD
#define DDR_DS_OUT DDRD
#define PIN_DS_OUT PIND
#define B_DS_OUT 1

//Room	Комната
#define PORT_DS_ROOM PORTD
#define DDR_DS_ROOM DDRD
#define PIN_DS_ROOM PIND
#define B_DS_ROOM 2

//Serve (температура подачи)
#define PORT_DS_SRV PORTD
#define DDR_DS_SRV DDRD
#define PIN_DS_SRV PIND
#define B_DS_SRV 3

//Return (температура обратки)
#define PORT_DS_RTN PORTD
#define DDR_DS_RTN DDRD
#define PIN_DS_RTN PIND
#define B_DS_RTN 4

// Датчик дымовых газов - термопара мс MAX6675
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

//Beep Зуммер активный
#define PORT_BEEP PORTB
#define DDR_BEEP DDRB
#define PIN_BEEP PINB
#define B_BEEP 1

//Preheating реле включения ТЭНов подогрева
#define PORT_PHEAT PORTB
#define DDR_PHEAT DDRB
#define PIN_PHEAT PINB
#define B_PHEAT 3
//===============================================================================
void init(void);
void CheckButton(void);
void ExecuteButton(void);
void FlapTravel(uint8_t direct, uint16_t steps);
void OutFullInfoToLCD(void);	//вывод оперативной инфомации на дисплей (рисование полного экрана)
void OutVarInfoToLCD(void);	//вывод оперативной инфомации на дисплей (только переменные)
void OutTemperatureDS(uint8_t dat, uint16_t x, uint16_t y);	//вывод на дисплей значений температуры датчика DS18B20
void ReadMAX6675(void);	//данные сохраняются в VALUE_TC и STATUS_TC
void OutTemperatureTC(uint16_t x, uint16_t y);	//вывод на дисплей значений температуры датчика TC
void OutPercent(uint16_t value, uint16_t max_value, uint16_t x, uint16_t y);		//вывод на дисплей значений в процентах: value - входное значение; max_value - значение при 100%
void LoadValuesFromEEPROM(void);
void UpdateValuesFromEEPROM(void);
void OutMenuToLCD(void);	//вывод меню и уставки на дисплей
void OutMenuToLCDValue(void);	//вывод уставок меню
uint16_t OutNumber(uint8_t val, uint16_t x, uint16_t y);
uint16_t OutBigNumber(uint16_t val, uint16_t x, uint16_t y);
void Work(void);	//анализ и работа

//===============================================================================
uint8_t DS18B20_init(uint8_t dat);	//Out: 0 - OK; 1 - нет датчика
uint8_t DS18B20_send(uint8_t dat, uint8_t sbyte);	//если делать ВЫВОД 0хFF, то на выходе будет ПРИНЯТЫЙ байт
void DS18B20_convertTemp(uint8_t dat);	//отправка команды преобразования температуры.
void DS18B20_getTemp(uint8_t dat);	//запрос, получение и вычисление температуры
uint8_t DS18B20_read(uint8_t dat);	//считывает 9 байтов в буфер, подсчитывает CRC. Out: 0 - OK, 1 - error CRC
uint8_t DS_calcCRC8(uint8_t data, uint8_t crc);	//Out: CRC
// void DS18B20_SetResolution(uint8_t numBit);	//установка разрешения датчика, по умолчанию используется разрешение 12 бит
//===============================================================================
// char GetHex(uint8_t b);
// void Work_T12_M12(void);	//анализ и работа контура с датчиками Т1 и Т2 и вентиляторами М1 и М2
// void Work_T3_M3(void);	//анализ и работа контура с датчиком Т3 и вентилятором М3
// uint8_t OutBigNumber(uint16_t val);	//вывод переменных значений уставок на дисплей
// uint8_t CalculatePercent(uint8_t diskr);

//===============================================================================
/**
	АВТОМАТ УПРАВЛЕНИЯ ТВЕРДОТОПЛИВНЫМ КОТЛОМ ТЕПЛОДАР ОК-20
	// Датчики:
		// Т1 - DS18B20 до +125гр.С => контроль температуры в помещении.
		// Т2 - термопара К-типа (хромель-алюмель) до 999гр.С => контроль поверхности плиты печи.
		// Т3 - термопара К-типа (хромель-алюмель) до 999гр.С => контроль камеры дожига пиролизных газов.
	// Вентиляторы: (питание всех 12 вольт, мощность 6-12Вт)
		// М1 => подача воздуха в подколосниковое пространство.
		// М2 => подача воздуха в основную топку.
		// М3 => подача воздуха в камеру дожига пиролизных газов.

	// Индикация и управление:
		// Основной ЖК дисплей 84х48пикс.
			// В нормальном режиме показывает температуру и состояние всех датчиков
			// и состояние вентиляторов в % выдаваемой мощности воздушного потока
			// В режиме настройки позволяет просмотреть и корректировать уставки работы автомата
		// Светодиод:
			// 2 режима: кратковременные вспышки (1 раз в секунду) - автомат выключен, контроль датчиков
					  // кратковременные выключения (1 раз в секунду) - автомат включен.

		// Три кнопки: "М", "+", "-" 
			// "М" - вход  меню настройки и перелистывание параметров по кольцу.
			// "+" и "-" - в режиме настройки коррекция соответствующего параметра уставок
				// в нормальном режиме вкл и выкл подсветки для дежурного режима
			// Выход из режима меню - 10 секунд без нажатий на кнопки.
	
	// АЛГОРИТМ РАБОТЫ:
	// Работа автомата разбита условно на 2 независимых контура.
		// 1 контур: датчики Т1, Т2 и вентиляторы М1, М2
		// 2 контур: датчик Т3 и вентилятор М3

		// 1 контур включается в работу когда разность Т2 - Т1 больше заданной уставки
		// и выключается соответственно когда разность Т2 - Т1 меньше уставки.

		// Процедура запуска вентияторов М1 и М2:
			// разгон М1 от старта (уставка) до 100% в течении времени уставки в мин.
			// ожидание 30 секунд
			// разгон М2 от старта (уставка) до 100% в течении 30 секунд

		// Процедура остановки вентиляторов М1 и М2:
			// выключение вентилятора М2
			// ожидание 15 секунд
			// плавная остановка вентилятора М1 в течении времени уставки в мин.
			
		// Условия запуска М1-М2:
			// температура Т1 меньше или равно (уставка - дельта)
			// и
			// температура Т2 меньше или равно (уставка - дельта)
			
		// Условия остановки М1-М2:
			// температура Т1 либо Т2 достигла или выше своих уставок

		// 2 контур включен всегда, вентилятор М3 включается в работу с мощностью своей уставки
		// при достижении Т3 температуры "старт пиролиз" и пропорционально увеличивает мощность
		// с ростом температуры Т3. На 100% мощности вентилятор выходит при достижении Т3 
		// температуры "полный пиролиз" и продолжает работать на этой мощности в случае 
		// дальнейшего роста температуры Т3. При снижении темтературы Т3 в диапазоне
		// "полный пиролиз" - "старт пиролиз" М3 пропорционально уменьшает мощность до полной остановки.

*/



// volatile uint8_t SETVAL_AUTO_CONTROL;					// Автоматическая регулировка / Ручная	Индекс в МЕНЮ - 0
// uint8_t EEMEM EE_SETVAL_AUTO_CONTROL = 1;				// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_AUTO_CONTROL_MAN	0					// Ручная регулировка
// #define DEF_SETVAL_AUTO_CONTROL_AUTO	1				// Автоматическая регулировка

// //Индекс в МЕНЮ - 1: Включение/Выключение ТЭНов в ручном режиме

// volatile uint8_t SETVAL_FLAP_MANUAL_STEP;				// Количество шагов двигателя заслонки в ручном режиме	Индекс в МЕНЮ - 2
// uint8_t EEMEM EE_SETVAL_FLAP_MANUAL_STEP = 5;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_FLAP_MANUAL_STEP_MIN	1				//от
// #define DEF_SETVAL_FLAP_MANUAL_STEP_MAX	25				//до
// #define DEF_SETVAL_FLAP_MANUAL_STEP_STEP	1			//шаг

// volatile uint8_t SETVAL_SMOKE_DEAD_BAND;				// Зона нечувствительности датчика дымохода в каждую сторону (целые градусы)	Индекс в МЕНЮ - 3
// uint8_t EEMEM EE_SETVAL_SMOKE_DEAD_BAND = 5;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_SMOKE_DEAD_BAND_MIN	1				//от
// #define DEF_SETVAL_SMOKE_DEAD_BAND_MAX	20				//до
// #define DEF_SETVAL_SMOKE_DEAD_BAND_STEP	1				//шаг

// volatile uint8_t SETVAL_FLAP_AUTO_STEP;					// Количество шагов двигателя заслонки в автоматическом режиме	Индекс в МЕНЮ - 4
// uint8_t EEMEM EE_SETVAL_FLAP_AUTO_STEP = 5;				// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_FLAP_AUTO_STEP_MIN	1				//от
// #define DEF_SETVAL_FLAP_AUTO_STEP_MAX	25				//до
// #define DEF_SETVAL_FLAP_AUTO_STEP_STEP	1				//шаг

// volatile uint8_t SETVAL_FLAP_START_STEPS;				// Стартовое положение заслонки в %	Индекс в МЕНЮ - 5
// uint8_t EEMEM EE_SETVAL_FLAP_START_STEPS = 30;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_FLAP_START_STEPS_MIN	5				//от
// #define DEF_SETVAL_FLAP_START_STEPS_MAX	50				//до
// #define DEF_SETVAL_FLAP_START_STEPS_STEP	1			//шаг
														
// volatile uint8_t SETVAL_TEMP_SMOKE;						// температура газов дымохода (целые градусы) 	Индекс в МЕНЮ - 6
// uint8_t EEMEM EE_SETVAL_TEMP_SMOKE = 200;				// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_SMOKE_MIN	100					//от
// #define DEF_SETVAL_TEMP_SMOKE_MAX	255					//до
// #define DEF_SETVAL_TEMP_SMOKE_STEP	5					//шаг

// volatile uint8_t SETVAL_MAX_TEMP_SERVE;					// максимальная температура подачи (целые градусы)	Индекс в МЕНЮ - 7
// uint8_t EEMEM EE_SETVAL_MAX_TEMP_SERVE = 80;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_MAX_TEMP_SERVE_MIN	40				//от
// #define DEF_SETVAL_MAX_TEMP_SERVE_MAX	95				//до
// #define DEF_SETVAL_MAX_TEMP_SERVE_STEP	1				//шаг

// volatile uint8_t SETVAL_HYSTER_TEMP_SERVE;				// Гистерезис температуры подачи (целые градусы)	Индекс в МЕНЮ - 8
// uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_SERVE = 5;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_HYSTER_TEMP_SERVE_MIN	1			//от
// #define DEF_SETVAL_HYSTER_TEMP_SERVE_MAX	20			//до
// #define DEF_SETVAL_HYSTER_TEMP_SERVE_STEP	1			//шаг

// volatile uint8_t SETVAL_MAX_TEMP_ROOM;					// максимальная температура комнаты (целые градусы)	Индекс в МЕНЮ - 9
// uint8_t EEMEM EE_SETVAL_MAX_TEMP_ROOM = 18;				// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_MAX_TEMP_ROOM_MIN	15				//от
// #define DEF_SETVAL_MAX_TEMP_ROOM_MAX	28				//до
// #define DEF_SETVAL_MAX_TEMP_ROOM_STEP	1				//шаг

// volatile uint8_t SETVAL_HYSTER_TEMP_ROOM;				// Гистерезис температуры комнаты (целые градусы)	Индекс в МЕНЮ - 10
// uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_ROOM = 2;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_HYSTER_TEMP_ROOM_MIN	1				//от
// #define DEF_SETVAL_HYSTER_TEMP_ROOM_MAX	5				//до
// #define DEF_SETVAL_HYSTER_TEMP_ROOM_STEP	1			//шаг

// volatile uint8_t SETVAL_TEMP_BOILER_OFF;				// Температура дымохода отключения управления котла (целые градусы)	Индекс в МЕНЮ - 11
// uint8_t EEMEM EE_SETVAL_TEMP_BOILER_OFF = 60;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_BOILER_OFF_MIN	20				//от
// #define DEF_SETVAL_TEMP_BOILER_OFF_MAX	70				//до
// #define DEF_SETVAL_TEMP_BOILER_OFF_STEP	5				//шаг

// volatile uint8_t SETVAL_HYSTER_TEMP_BOILER_OFF;			// Гистерезис температуры дымохода отключения управления котла (целые градусы)	Индекс в МЕНЮ - 12
// uint8_t EEMEM EE_SETVAL_HYSTER_TEMP_BOILER_OFF = 10;	// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MIN	2		//от
// #define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MAX	50		//до
// #define DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_STEP	1		//шаг
														
// volatile uint8_t SETVAL_LIGHT_DISPLAY;					// Подсветка индикатора	Индекс в МЕНЮ - 13
// uint8_t EEMEM EE_SETVAL_LIGHT_DISPLAY = 1;				// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_LIGHT_AUTO	0						// Включение по датчику присутствия / с кнопок
// #define DEF_SETVAL_LIGHT_ALL_ON	1						// Всегда включен

// volatile uint8_t SETVAL_TIME_LIGHT;						// Время работы подсветки индикатора а автоматич. режиме (целые минуты)	Индекс в МЕНЮ - 14
// uint8_t EEMEM EE_SETVAL_TIME_LIGHT = 5;					// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TIME_LIGHT_MIN	1					//от
// #define DEF_SETVAL_TIME_LIGHT_MAX	10					//до
// #define DEF_SETVAL_TIME_LIGHT_STEP	1					//шаг

// volatile uint8_t SETVAL_ALARM_FUEL_ON;					// Сигнализация прогорания топлива	Индекс в МЕНЮ - 15
// uint8_t EEMEM EE_SETVAL_ALARM_FUEL_ON = 1;				// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_ALARM_FUEL_OFF	0					// Выключена
// #define DEF_SETVAL_ALARM_FUEL_ON	1					// Включена

// volatile uint8_t SETVAL_TEMP_ALARM_FUEL;				// Температура сигнализации прогорания топлива (целые градусы)	Индекс в МЕНЮ - 16
// uint8_t EEMEM EE_SETVAL_TEMP_ALARM_FUEL = 150;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_ALARM_FUEL_MIN	80				//от
// #define DEF_SETVAL_TEMP_ALARM_FUEL_MAX	150				//до
// #define DEF_SETVAL_TEMP_ALARM_FUEL_STEP	5				//шаг

// volatile uint8_t SETVAL_TEN_MODE;						// Режим работы ТЕНов	Индекс в МЕНЮ - 17
// uint8_t EEMEM EE_SETVAL_TEN_MODE = 1;					// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEN_MODE_OFF	0						// ТЕНы выключены
// #define DEF_SETVAL_TEN_MODE_NOTFROZE	1				// Режим подогрева против замерзания теплоносителя
// #define DEF_SETVAL_TEN_MODE_HEAT_ROOM	2				// Режим поддержания температуры в помещении

// volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_ON;			// температура включения ТЕНов аварийный подогрев (целые градусы)	Индекс в МЕНЮ - 18
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_ON = 5;		// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MIN	2			//от
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MAX	25			//до
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_STEP	1		//шаг

// volatile uint8_t SETVAL_TEMP_TEN_NOTFROZE_OFF;			// температура выключения ТЕНов аварийный подогрев (целые градусы)	Индекс в МЕНЮ - 19
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_NOTFROZE_OFF = 5;		// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MIN	1		//от
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MAX	30		//до
// #define DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_STEP	1		//шаг

// volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_ON;			// температура включения ТЕНов по нагреву помещения (целые градусы)	Индекс в МЕНЮ - 20
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_ON = 10;		// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MIN	5		//от
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MAX	25		//до
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_STEP	1		//шаг

// volatile uint8_t SETVAL_TEMP_TEN_HEAT_ROOM_OFF;			// температура выключения ТЕНов по нагреву помещения (целые градусы)	Индекс в МЕНЮ - 21
// uint8_t EEMEM EE_SETVAL_TEMP_TEN_HEAT_ROOM_OFF = 15;	// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MIN	6		//от
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MAX	30		//до
// #define DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_STEP	1		//шаг

// volatile uint8_t SETVAL_TEN_POWER;						// Мощность ТЕНов (кВт)	Индекс в МЕНЮ - 22
// uint8_t EEMEM EE_SETVAL_TEN_POWER = 1;					// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TEN_POWER_MIN	1					//от
// #define DEF_SETVAL_TEN_POWER_MAX	6					//до
// #define DEF_SETVAL_TEN_POWER_STEP	1					//шаг

// volatile uint8_t SETVAL_TIME_DELAY_PAUSE;				// Задержка в регулировании в режиме рауза (целые минуты)	Индекс в МЕНЮ - 23
// uint8_t EEMEM EE_SETVAL_TIME_DELAY_PAUSE = 3;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_TIME_DELAY_PAUSE_MIN	1				//от
// #define DEF_SETVAL_TIME_DELAY_PAUSE_MAX	10				//до
// #define DEF_SETVAL_TIME_DELAY_PAUSE_STEP	1			//шаг

// volatile uint8_t SETVAL_PUMP_PERFORMANCE;				// Производительность насоса циркуляции (л/мин)	Индекс в МЕНЮ - 24
// uint8_t EEMEM EE_SETVAL_PUMP_PERFORMANCE = 20;			// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_PUMP_PERFORMANCE_MIN	1				//от
// #define DEF_SETVAL_PUMP_PERFORMANCE_MAX	100				//до
// #define DEF_SETVAL_PUMP_PERFORMANCE_STEP	1			//шаг

// //Индекс в МЕНЮ - 25: Сброс счетчика тепловой энергии

// //Индекс в МЕНЮ - 26: Сброс счетчика электрической энергии





















