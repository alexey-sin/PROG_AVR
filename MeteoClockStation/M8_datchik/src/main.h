/**
	Метео датчик.
	Собирает данные:
		с GPS модуля, с выделением метки времени, даты и достоверности данных из сообщения $GPRMC
		с датчика освещенности
		с датчика BME280 с выделением данных о температуре, влажности и атмосферном давлении воздуха
	Собранные данные передает через модуль NRF24L01+ в эфир в посылке 16 байт.
	01 02 03 04 ... 15 16
	
	индекс
	00 - $
	01 - часы	(время UTC)
	02 - минуты
	03 - секунды
	04 - число
	05 - месяц
	06 - год (два последних разряда)
	07 - 77 => данные достоверны, 00 => не достоверны 
	
	08 - Старший разряд АЦП освещенности
	09 - Старший разряд АЦП освещенности
	
	10 - знак температуры
	11 - температура
	12 - влажность в %
	13 - старший разряд атмосферного давления
	14 - младший разряд атмосферного давления
	
	15 - Контрольная сумма посылки (8 битное исключающее ИЛИ (^) всех байтов 00-30 включительно)
	
	Вариант 1: со старым модулем GPS из BC-337 с чипом SiRFSTAR III - скорость обмена 4800бод
	Вариант 2: с новым модулем GPS GY-NEO6MV2 с чипом NEO-6M-0-001 - скорость обмена 9600бод
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

// конфигурация фьюзов: Внешний кварц 8MHz, медленный старт
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
#define SIZE_RX_ARRAY	16		//максимум 32!
unsigned char tx_nrf[SIZE_RX_ARRAY] = {0x00};
unsigned char* nrf_address = (unsigned char*)"METEO";

#define SIZE_RX_GPS_BUFFER	90
volatile uint8_t rx_gps[SIZE_RX_GPS_BUFFER];
volatile uint8_t cntRxGPS = 0;	//индекс принимаемого байта по RS232
volatile uint8_t statusRxGPS = 0;	//статус буфера rx_gps
/** statusRxGPS
	0 - свободен для приема
	1 - в процессе приема
	2 - принят конец строки / готов к анализу
	
*/

// const char* nrf_address PROGMEM = {0x4D,0x45,0x54,0x45,0x4F};	//METEO
// unsigned char nrf_address[5] = {0x4D,0x45,0x54,0x45,0x4F};	//METEO
// volatile uint8_t LCDvolume = 0;		//число выводимое на LCD

volatile uint16_t ADCvolume = 0;	//вычисленное значение АЦП
volatile uint32_t ADCbuffer = 0;	//буфер для складывания дискрет АЦП
volatile uint8_t ADCcntBuff = 0;	//текущий счетчик измерений
#define ADC_MAX_CNT_BUFF	64		//количество измерений в буфере

// volatile uint8_t STATUS = 0;
/**
	0 - LCD погашен, анализ АЦП, включение ДХО по уставкам, монитор кнопок
	1 - LCD выводит текущее значение АЦП, анализ АЦП, включение ДХО по уставкам, монитор кнопок
	2 - режим меню коррекции уставок, LCD в мигающем режиме выводит текущую уставку
	
	переход в меню коррекции уставок, листание уставок - одновременное нажатие "+" и "-"
	выход из меню коррекции уставок и уход в дежурный режим по времении соотв. define-ов
*/
volatile uint8_t FLAGS1 = 0;
// #define FL1_BLC2				0		//тик ~2мс - конец приема посылки по RS232 от GPS модуля
#define FL1_BLC1000				1		//тик ~1сек
// #define FL1_ADC_OK				2		//АЦП вычисленные значения похожи на достоверные
// #define FL1_BTN_WAIT		3		//антидребезг
// #define FL1_BTN_MMM      	4		//кнопка M
// #define FL1_BTN_PLS      	5		//кнопка +
// #define FL1_BTN_MIN      	6		//кнопка -
// #define FL1_LCD_FAIR      	7		//режим мигания LCD. 1 - горит, 0 - погашен

// volatile uint8_t FLAGS2 = 0;
// // #define FL2_ADC				0		//АЦП ~125кГц
// // #define FL2_ADC_OK			1		//АЦП вычисленные значения похожи на достоверные
// #define FL2_RELE_DELAY		2		//ожидание паузы выключения реле
//===============================================================================
// volatile uint8_t CountMUL_BTN_WAIT = 0;
// #define MUL_BTN_WAIT	100	//множитель на 2мс антидребезг кнопок

// volatile uint8_t CountMUL_LCD = 0;	//счетчик ухода в спящий режим
// #define MUL_LCD_WAIT_SLEEP	40	//множитель на 0,25сек - выход из индикации напряжения
// #define MUL_MENU_EXIT	40	//множитель на 0,25сек - выход из настроек

// volatile uint8_t MENU = 0;	//текущая позиция меню (индекс. от 0 до 2 включительно)
/**
	0 - коррекция напряжения включения ходовых огней
	1 - коррекция напряжения выключения ходовых огней
	2 - коррекция времени задержки выключения ходовых огней
*/
// volatile uint8_t RELEcntDelay = 0;	//счетчик тиков 0,25сек до выключения реле
//===============================================================================
// Сохраняемые уставки и переменные
// volatile uint8_t SETVAL_HIGH;			// напряжение включения ходовых огней		индекс в меню уставок 0
// uint8_t EEMEM EE_SETVAL_HIGH = 140;		// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_HIGH_MIN		120		//от
// #define DEF_SETVAL_HIGH_MAX		160		//до
// #define DEF_SETVAL_HIGH_STEP	1		//шаг
// volatile uint8_t SETVAL_LOW;			// напряжение выключения ходовых огней		индекс в меню уставок 1
// uint8_t EEMEM EE_SETVAL_LOW = 125;		// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_LOW_MIN		100		//от
// #define DEF_SETVAL_LOW_MAX		140		//до
// #define DEF_SETVAL_LOW_STEP		1		//шаг
// volatile uint8_t SETVAL_DELAY;			// время задержки выключения ходовых огней(в сек)		индекс в меню уставок 2
// uint8_t EEMEM EE_SETVAL_DELAY = 5;		// начальное значение сохраненное в EEPROM
// #define DEF_SETVAL_DELAY_MIN	0		//от
// #define DEF_SETVAL_DELAY_MAX	19		//до
// #define DEF_SETVAL_DELAY_STEP	1		//шаг

// #define DEF_SETVAL_MIN_INT		5		//минимальный интервал между SETVAL_HIGH и SETVAL_LOW

//===============================================================================
void init(void);
void AnalizRxGPS(void);
void CalculateLight(void);
void SendPacket(void);
// void CheckButton(void);
// void ExecuteButton(void);
// void ClearLCD(void);
// void OutLCD(void);	//вывод на дисплей числа
// void LoadValuesFromEEPROM(void);
// void UpdateValuesFromEEPROM(void);
//===============================================================================
//===============================================================================
//===============================================================================
















