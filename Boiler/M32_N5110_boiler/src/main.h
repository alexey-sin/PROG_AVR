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

// конфигурация фьюзов: https://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega32&LOW=4F&HIGH=99&LOCKBIT=FF

#define		SetBit(reg, bit)			reg |= (1<<bit)           
#define		ClrBit(reg, bit)			reg &= (~(1<<bit))
#define		InvBit(reg, bit)			reg ^= (1<<bit)
#define		BitIsSet(reg, bit)			((reg & (1<<bit)) != 0)
#define		BitIsClr(reg, bit)		((reg & (1<<bit)) == 0)

volatile uint8_t STATUS = 0;
#define ST_BLC      0
#define ST_BTN      1		//есть событие по кнопкам
#define ST_BTN_WAIT 2		//антидребезг
#define ST_MENU     3		//0 - работа, вывод на дисплей оперативных параметров 1 - режим меню
#define ST_WORK_T12_M12  4	//1 - контур М1,М2,Т1 и Т2 находится в работе
#define ST_WORK_T3_M3    5	//1 - контур М3 и Т3 находится в работе
#define ST_M12_START  6		//1 - М1 и М2 в состоянии разгона
#define ST_M12_STOP	  7		//1 - М1 и М2 в состоянии торможения

volatile uint8_t BUTTON = 0;
#define MMM      0		//кнопка М
#define PLS      1		//кнопка +
#define MIN      2		//кнопка -

#define MUL_BTN_WAIT	6	//множитель на 32мкс антидребезг кнопок
volatile uint8_t CountMUL_BTN_WAIT = 0;

volatile uint8_t MENU = 0;	//текущая позиция меню (индекс. от 0 до 11 включительно)
#define MUL_MENU_WAIT	10	//множитель на ~1сек, ожидание выхода из меню
volatile uint8_t CountMUL_MENU_WAIT = 0;


volatile uint8_t DS_TempData[9];
volatile double DS_Temp;
volatile uint8_t STATUS_DS = 0;
/**  Датчик Т1
	STATUS_DS регистр состояния датчика DS18B20
	0 - неопределенное состояние (начальное) или ошибка
	1 - нормально, есть измеренное значение температуры в регистре DS_Temp (double)
*/
volatile uint8_t STATUS_TC1 = 0;
volatile uint16_t VALUE_TC1 = 0;
/**  Датчик Т2
	STATUS_TC1 регистр состояния датчика TC1 (термопара К-типа)
	0 - неопределенное состояние (начальное)
	1 - нормально, есть измеренное значение температуры => значение в градусах в регистре VALUE_TC1
	2 - обрыв датчика
	3 - превышение максимального значения (больше 999 Гр. по цельсию)
*/

volatile uint8_t STATUS_TC2 = 0;
volatile uint16_t VALUE_TC2 = 0;
/** Датчик Т3
	STATUS_TC2 регистр состояния датчика TC2 (термопара К-типа)
	0 - неопределенное состояние (начальное)
	1 - нормально, есть измеренное значение температуры => значение в градусах в регистре VALUE_TC2
	2 - обрыв датчика
	3 - превышение максимального значения (больше 999 Гр. по цельсию)
*/
#define PWM1	1
#define PWM2	2
#define PWM3	3

// #define M1_START_PERIOD	30		//время разгона М1 в секундах
#define M2_START_PERIOD	30		//время разгона М2 в секундах
#define M12_BETWEEN_START_PERIOD	30		//время между разгонами М1 и М2 в секундах
#define M12_BETWEEN_STOP_PERIOD	15		//время между торможениями М1 и М2 в секундах

volatile uint8_t ui8_PWM1_LEVEL = 0;
volatile uint8_t ui8_PWM2_LEVEL = 0;
volatile uint8_t ui8_PWM3_LEVEL = 0;
volatile double dbl_PWM1_LEVEL = 0.0;
volatile double dbl_PWM2_LEVEL = 0.0;

volatile uint8_t ui8_M12_STATUS = 0;	//состояние вентиляторов М1 и М2
/**
	Для состояния разгона
	0 - М1 и М2 остановлены
	1 - М1 в середине разгона/торможения, М2 остановлен
	2 - М1 в 100% работает, М2 остановлен
	3 - М1 в 100% работает, М2 в середине разгона/торможения
	4 - М1 в 100% работает, М2 в 100% работает
	
	Для состояния торможения
	0 - М1 в 100% работает, М2 в 100% работает
	1 - М1 в 100% работает, М2 остановлен
	2 - М1 в 100% середине торможения, М2 остановлен
	3 - М1 и М2 остановлены
*/
volatile uint16_t ui16_M12_CNT = 0;		//счетчик циклов 1сек в паузах между М1 и М2
//===============================================================================
// Вычисляемые константы
volatile uint8_t ui8_PWM1_Start_Discret = 0;	//значение PWM1 для старта в дискретах 
volatile uint8_t ui8_PWM2_Start_Discret = 0;	//значение PWM2 для старта в дискретах 
volatile uint8_t ui8_PWM3_Start_Discret = 0;	//значение PWM3 для старта в дискретах 
volatile double dbl_PWM1_Step_Start_Discret = 0.0;	//сколько дискрет добавить к стартовому значению на 1 шаг разгона(секунду)
volatile double dbl_PWM1_Step_Stop_Discret = 0.0;	//сколько дискрет отнять от текущего значения на 1 шаг торможения(секунду)
volatile double dbl_PWM2_Step_Discret = 0.0;	//сколько дискрет добавить к стартовому значению на 1 шаг разгона(секунду)
volatile double dbl_PWM3_Step_Discret = 0.0;	//сколько дискрет добавить к стартовому значению на 1 градус
//===============================================================================
volatile uint8_t LED_LIGTH;				//подсетка дисплея. 1 - горит
uint8_t EEMEM EE_LED_LIGTH = 1;			// начальное значение сохраненное в EEPROM
//===============================================================================
// Сохраняемые уставки и переменные
volatile uint8_t SETVAL_TEMP_ROOM;				// температура в помещении (целые градусы)	Индекс в МЕНЮ - 0
uint8_t EEMEM EE_SETVAL_TEMP_ROOM = 25;			// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_ROOM_MIN	10			//от
#define DEF_SETVAL_TEMP_ROOM_MAX	30			//до
#define DEF_SETVAL_TEMP_ROOM_STEP	1			//шаг
volatile uint8_t SETVAL_DELTA_TEMP_ROOM;		// дельта (вниз) температуры в помещении (целые градусы) 	Индекс в МЕНЮ - 1
uint8_t EEMEM EE_SETVAL_DELTA_TEMP_ROOM = 2;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_DELTA_TEMP_ROOM_MIN	1		//от
#define DEF_SETVAL_DELTA_TEMP_ROOM_MAX	5		//до
#define DEF_SETVAL_DELTA_TEMP_ROOM_STEP	1		//шаг
volatile uint8_t SETVAL_TEMP_TOP_OVEN;			// температура поверхности печи (целые градусы)	Индекс в МЕНЮ - 2
uint8_t EEMEM EE_SETVAL_TEMP_TOP_OVEN = 100;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_TOP_OVEN_MIN	55		//от
#define DEF_SETVAL_TEMP_TOP_OVEN_MAX	140		//до
#define DEF_SETVAL_TEMP_TOP_OVEN_STEP	1		//шаг
volatile uint8_t SETVAL_DELTA_TEMP_TOP_OVEN;	// дельта температуры поверхности печи (целые градусы)	Индекс в МЕНЮ - 3
uint8_t EEMEM EE_SETVAL_DELTA_TEMP_TOP_OVEN = 10;// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_DELTA_TEMP_TOP_OVEN_MIN	1	//от
#define DEF_SETVAL_DELTA_TEMP_TOP_OVEN_MAX	20	//до
#define DEF_SETVAL_DELTA_TEMP_TOP_OVEN_STEP	1	//шаг
volatile uint16_t SETVAL_TEMP_PYROLYS_START;	//температура начала пиролиза - старта автоматики	Индекс в МЕНЮ - 4
uint16_t EEMEM EE_SETVAL_TEMP_PYROLYS_START = 400;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_PYROLYS_START_MIN	200	//от
#define DEF_SETVAL_TEMP_PYROLYS_START_MAX	600	//до
#define DEF_SETVAL_TEMP_PYROLYS_START_STEP	10	//шаг
volatile uint16_t SETVAL_TEMP_PYROLYS_FULL;		//температура пиролиза - полная мощность вентилятора автоматики	Индекс в МЕНЮ - 5
uint16_t EEMEM EE_SETVAL_TEMP_PYROLYS_FULL = 600;	// начальное значение сохраненное в EEPROM
//минимальное значение - SETVAL_TEMP_PYROLYS_START + 100
#define DEF_SETVAL_TEMP_PYROLYS_FULL_MAX	990	//до
#define DEF_SETVAL_TEMP_PYROLYS_FULL_STEP	10	//шаг
volatile uint8_t SETVAL_DELTA_TEMP_SHUTDOWN;	//уставка разности Т пов.печи и Т в помещении (Т2-Т1) для выключения автоматики	Индекс в МЕНЮ - 6
uint8_t EEMEM EE_SETVAL_DELTA_TEMP_SHUTDOWN = 10;// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_DELTA_TEMP_SHUTDOWN_MIN	5	//от
#define DEF_SETVAL_DELTA_TEMP_SHUTDOWN_MAX	20	//до
#define DEF_SETVAL_DELTA_TEMP_SHUTDOWN_STEP	1	//шаг
volatile uint8_t SETVAL_PWM_FAN1_START;			//стартовое значение PWM вентилятора 1 в %	Индекс в МЕНЮ - 7
uint8_t EEMEM EE_SETVAL_PWM_FAN1_START = 2;		// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_PWM_FAN1_START_MIN	0		//от
#define DEF_SETVAL_PWM_FAN1_START_MAX	20		//до
#define DEF_SETVAL_PWM_FAN1_START_STEP	1		//шаг
volatile uint8_t SETVAL_PWM_FAN2_START;			//стартовое значение PWM вентилятора 2 в %	Индекс в МЕНЮ - 8
uint8_t EEMEM EE_SETVAL_PWM_FAN2_START = 2;		// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_PWM_FAN2_START_MIN	0		//от
#define DEF_SETVAL_PWM_FAN2_START_MAX	20		//до
#define DEF_SETVAL_PWM_FAN2_START_STEP	1		//шаг
volatile uint8_t SETVAL_PWM_FAN3_START;			//стартовое значение PWM вентилятора 3 в %	Индекс в МЕНЮ - 9
uint8_t EEMEM EE_SETVAL_PWM_FAN3_START = 2;		// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_PWM_FAN3_START_MIN	0		//от
#define DEF_SETVAL_PWM_FAN3_START_MAX	20		//до
#define DEF_SETVAL_PWM_FAN3_START_STEP	1		//шаг
volatile uint8_t SETVAL_M1_START_PERIOD;		//время разгона вентилятора 1 в мин	Индекс в МЕНЮ - 10
uint8_t EEMEM EE_SETVAL_M1_START_PERIOD = 10;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_M1_START_PERIOD_MIN	0		//от
#define DEF_SETVAL_M1_START_PERIOD_MAX	60		//до
#define DEF_SETVAL_M1_START_PERIOD_STEP	1		//шаг
volatile uint8_t SETVAL_M1_STOP_PERIOD;			//время торможения вентилятора 1 в мин	Индекс в МЕНЮ - 11
uint8_t EEMEM EE_SETVAL_M1_STOP_PERIOD = 10;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_M1_STOP_PERIOD_MIN	0		//от
#define DEF_SETVAL_M1_STOP_PERIOD_MAX	60		//до
#define DEF_SETVAL_M1_STOP_PERIOD_STEP	1		//шаг
//===============================================================================
const char LabelT1[] PROGMEM = {4,'T','1',':',' '};
const char LabelT2[] PROGMEM = {4,'T','2',':',' '};
const char LabelT3[] PROGMEM = {4,'T','3',':',' '};
const char LabelM1[] PROGMEM = {4,'М','1',':',' '};
const char LabelM2[] PROGMEM = {4,'М','2',':',' '};
const char LabelM3[] PROGMEM = {4,'М','3',':',' '};
const char LabelUNC[] PROGMEM = {3,'U','N','C'};
const char LabelBRK[] PROGMEM = {5,'B','R','E','A','K'};
const char LabelOVR[] PROGMEM = {4,'O','V','E','R'};
const char LabelERR[] PROGMEM = {5,'E','R','R','O','R'};
const char LabelCC[] PROGMEM = {2,0xb7,'C'};
const char LabelSetting[] PROGMEM = {9,'Н','а','с','т','р','о','й','к','а'};
//===============================================================================
const char Label_TEMP[] PROGMEM = {11,'Т','е','м','п','е','р','а','т','у','р','а'};
const char Label_ROOM[] PROGMEM = {9,'п','о','м','е','щ','е','н','и','я'};
const char Label_DELTA_TEMP[] PROGMEM = {14,'Д','е','л','ь','т','а',' ','т','е','м','п','е','р','.'};
const char Label_TOP_OVEN[] PROGMEM = {13,'п','о','в','е','р','х','н','.',' ','п','е','ч','и'};
const char Label_START_PYROLYS[] PROGMEM = {14,'с','т','а','р','т','а',' ','п','и','р','о','л','и','з'};
const char Label_FULL_PYROLYS[] PROGMEM = {14,'п','о','л','н','ы','й',' ','п','и','р','о','л','и','з'};
const char Label_RAZN_TEMP[] PROGMEM = {14,'Р','а','з','н','о','с','т','ь',' ','т','е','м','п','.'};
const char Label_SHUTDOWN[] PROGMEM = {14,'в','ы','к','л','ю','ч','е','н','и','я',' ','а','в','т'};
const char Label_PWM_START[] PROGMEM = {12,'М','о','щ','н','.',' ','с','т','а','р','т','а'};
const char Label_FAN[] PROGMEM = {12,'в','е','н','т','и','л','я','т','о','р','а',' '};
const char Label_TIME_START[] PROGMEM = {13,'В','р','е','м','я',' ','р','а','з','г','о','н','а'};
const char Label_TIME_STOP[] PROGMEM = {14,'В','р','е','м','я',' ','о','с','т','а','н','о','в','.'};
const char Label_MIN[] PROGMEM = {3,'м','и','н'};
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
void Work_T12_M12(void);	//анализ и работа контура с датчиками Т1 и Т2 и вентиляторами М1 и М2
void Work_T3_M3(void);	//анализ и работа контура с датчиком Т3 и вентилятором М3
void CalculateCoefficients(void);	//вычисление промежуточных констант для анализа
void ExecuteButton(void);
void init(void);
void LoadValuesFromEEPROM(void);
void UpdateValuesFromEEPROM(void);
void LCD_init(void);
void LCD_sendByte(uint8_t byte, uint8_t cmd);	//byte - посылаемый байт; cmd = 1 - данные, 0 - команда
void LCD_gotoXY(uint8_t row, uint8_t col);	// row = 0-5(строки); col = 0-83(столбцы)
void LCD_clear(uint8_t bit);	//заполнение экрана bit=0 0x00 или bit>0 0xFF
void LCD_OutChar6x8(char ch);
void LCD_OutString6x8(char* str);
uint8_t LCD_OutChar12x16(char ch, uint8_t row, uint8_t col);
uint8_t LCD_OutString12x16(char* str, uint8_t row, uint8_t col);
uint8_t LCD_OutNumber12x16(uint8_t val, uint8_t row, uint8_t col);
uint8_t LCD_OutBigNumber12x16(uint16_t val, uint8_t row, uint8_t col);
void ReadMAX6675(uint8_t dat);	//dat - номер датчика 1,2; данные сохраняются в свои переменные VALUE_TC1(2) и STATUS_TC1(2)
void OutMenuToLCD(void);	//вывод меню и уставки на дисплей
void OutInfoToLCD(void);	//вывод оперативной инфомации на дисплей
uint8_t CalculatePercent(uint8_t diskr);
void OutTemperatureTC(uint8_t status, uint16_t value);	//вывод на дисплей значений температуры датчиков TC
void OutTemperatureDS(void);	//вывод на дисплей значений температуры датчика DS18B20
void OutUI8_6x8(uint8_t value);		//вывод на дисплей значений мощности вентиляторов
void CheckButton(void);
//===============================================================================
uint8_t DS18B20_init(void);	//Out: 0 - OK; 1 - нет датчика
uint8_t DS18B20_send(uint8_t sbyte);	//если делать ВЫВОД 0хFF, то на выходе будет ПРИНЯТЫЙ байт
void DS18B20_convertTemp(void);	//отправка команды преобразования температуры.
void DS18B20_getTemp(void);	//запрос, получение и вычисление температуры
uint8_t DS18B20_read(void);	//считывает 9 байтов в буфер, подсчитывает CRC. Out: 0 - OK, 1 - error CRC
uint8_t DS_calcCRC8(uint8_t data, uint8_t crc);	//Out: CRC
void DS18B20_SetResolution(uint8_t numBit);	//установка разрешения датчика, по умолчанию используется разрешение 12 бит
//===============================================================================
void PWM_send(uint8_t numPWM, uint8_t level);
void PWM_sendByte(uint8_t data);
//===============================================================================
// char GetHex(uint8_t b);

//===============================================================================
/**
	АВТОМАТ УПРАВЛЕНИЯ ВЕНТИЛЯТОРАМИ ПИРОЛИЗНОЙ ПЕЧИ
	Датчики:
		Т1 - DS18B20 до +125гр.С => контроль температуры в помещении.
		Т2 - термопара К-типа (хромель-алюмель) до 999гр.С => контроль поверхности плиты печи.
		Т3 - термопара К-типа (хромель-алюмель) до 999гр.С => контроль камеры дожига пиролизных газов.
	Вентиляторы: (питание всех 12 вольт, мощность 6-12Вт)
		М1 => подача воздуха в подколосниковое пространство.
		М2 => подача воздуха в основную топку.
		М3 => подача воздуха в камеру дожига пиролизных газов.

	Индикация и управление:
		Основной ЖК дисплей 84х48пикс.
			В нормальном режиме показывает температуру и состояние всех датчиков
			и состояние вентиляторов в % выдаваемой мощности воздушного потока
			В режиме настройки позволяет просмотреть и корректировать уставки работы автомата
		Светодиод:
			2 режима: кратковременные вспышки (1 раз в секунду) - автомат выключен, контроль датчиков
					  кратковременные выключения (1 раз в секунду) - автомат включен.

		Три кнопки: "М", "+", "-" 
			"М" - вход  меню настройки и перелистывание параметров по кольцу.
			"+" и "-" - в режиме настройки коррекция соответствующего параметра уставок
				в нормальном режиме вкл и выкл подсветки для дежурного режима
			Выход из режима меню - 10 секунд без нажатий на кнопки.
	
	АЛГОРИТМ РАБОТЫ:
	Работа автомата разбита условно на 2 независимых контура.
		1 контур: датчики Т1, Т2 и вентиляторы М1, М2
		2 контур: датчик Т3 и вентилятор М3

		1 контур включается в работу когда разность Т2 - Т1 больше заданной уставки
		и выключается соответственно когда разность Т2 - Т1 меньше уставки.

		Процедура запуска вентияторов М1 и М2:
			разгон М1 от старта (уставка) до 100% в течении времени уставки в мин.
			ожидание 30 секунд
			разгон М2 от старта (уставка) до 100% в течении 30 секунд

		Процедура остановки вентиляторов М1 и М2:
			выключение вентилятора М2
			ожидание 15 секунд
			плавная остановка вентилятора М1 в течении времени уставки в мин.
			
		Условия запуска М1-М2:
			температура Т1 меньше или равно (уставка - дельта)
			и
			температура Т2 меньше или равно (уставка - дельта)
			
		Условия остановки М1-М2:
			температура Т1 либо Т2 достигла или выше своих уставок

		2 контур включен всегда, вентилятор М3 включается в работу с мощностью своей уставки
		при достижении Т3 температуры "старт пиролиз" и пропорционально увеличивает мощность
		с ростом температуры Т3. На 100% мощности вентилятор выходит при достижении Т3 
		температуры "полный пиролиз" и продолжает работать на этой мощности в случае 
		дальнейшего роста температуры Т3. При снижении темтературы Т3 в диапазоне
		"полный пиролиз" - "старт пиролиз" М3 пропорционально уменьшает мощность до полной остановки.

*/
























