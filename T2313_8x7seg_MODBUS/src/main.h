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
	конфигурация фьюзов: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313&LOW=E4&HIGH=9F&EXTENDED=FF&LOCKBIT=FF

 для F=8MГц (внутренний RC генератор) старт 14СК + 64мс
 Фьюзы: для USBASP и PonyProg 	1 => установленный бит => галки нет		( )
								0 => сброшенный бит => отмечено галкой 	(V)
 все биты LOCK установлены
---- Fuse High Byte ----
 DWEN - 				(если V и LOCK не установлены влючается режим отладки debugWIRE через вывод RESET
 EESAVE -
 SPIEN - V (недоступен)
 WDTON -
 BODLEVEL2 -
 BODLEVEL1 -
 BODLEVEL0 -
 RSTDISBL -   (недоступен)
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
#define PortRAZR		PORTB	//порт на ULN2803 управления разрядами
#define PinRAZR_1		7	//старший разряд
#define PinRAZR_2		6
#define PinRAZR_3		5
#define PinRAZR_4		4
#define PinRAZR_5		3
#define PinRAZR_6		2
#define PinRAZR_7		1
#define PinRAZR_8		0	//младший разряд

#define PortSEGM		PORTD	//порт на 74HC595 сдвиговый регистр на сегменты
#define PinSEGM_C		6	//CLK
#define PinSEGM_D		5	//DATA
#define PinSEGM_E		4	//CE

#define PortKnMNU		PORTD	//порт кнопки меню
#define PortKnPLS		PORTD	//порт кнопки плюс
#define PortKnMIN		PORTA	//порт кнопки минус
#define PINKnMNU		PIND	//порт кнопки меню
#define PINKnPLS		PIND	//порт кнопки плюс
#define PINKnMIN		PINA	//порт кнопки минус
#define PinKnMNU		3		//вывод кнопки меню
#define PinKnPLS		2		//вывод кнопки плюс
#define PinKnMIN		0		//вывод кнопки минус

#define PortRS232		PORTD	//порт Tx и Rx RS232
#define PortTxRx		PORTA	//порт управления прием/передача RS485
#define PinTx			1		//вывод Tx
#define PinRx			0		//вывод Rx
#define PinTxRx			1		//вывод управления прием/передача RS485 (0-прием; 1-передача)
//===============================================================================
volatile uint8_t FLAGS1 = 0;
#define FL1_0_OVER			0		//переполнение счетчика 0
#define FL1_REC_BITS		1		//переполнение счетчика 0
#define FL1_BLC1			2		//тик ~1.024mc вывод очередного разряда индикации
#define FL1_BLC250			3		//флаг погашенного состояния в режиме мигания 250/250
#define FL1_TX				4		//передача байта по RS485 завершена
//===============================================================================
volatile uint8_t BUTTON = 0;		// состояние кнопок: 0бит - "меню"; 1бит - "плюс"; 2бит - "минус"
volatile uint8_t CNT_BITS = 0;		// счетчик принимаемых бит по SPI
volatile uint32_t BLINK250 = 0;		// счетчик мигающего режима 250/250
volatile uint8_t SEGM_RAZR[8];		// сегменты разрядов подготовленные для вывода на дисплей(старший индекс - старший разряд)
volatile uint8_t IND_RAZR = 0;		// индекс разряда выводимого на дисплей
volatile uint32_t BUFFER = 0;		// Приемный буфер SPI
volatile uint8_t RAZR_REGIM[8] = {0};		// режим работы каждого разряда: 0 - всегда горит; 1- мигание 250/250мс;

volatile uint8_t MY_ADDR = 201;		//адрес на который отзывается котроллер по шине RS485
#define RX_BUFFER_SIZE		30		//размер приемного буфера в байтах
volatile uint8_t RX_BUFFER[RX_BUFFER_SIZE] = {0};	//приемный буфер RS232/485
volatile uint8_t RX_cnt = 0;		//счетчик принимаемых байт по RS232/485
#define TX_BUFFER_SIZE		10		//размер передаваемого буфера в байтах
volatile uint8_t TX_BUFFER[TX_BUFFER_SIZE] = {0};	//передаваемый буфер RS232/485
volatile uint8_t TX_cnt = 0;		//счетчик передаваемых байт по RS232/485
volatile uint8_t TX_size = 0;		//кол-во передаваемых байт по RS232/485
//===============================================================================
void init(void);
void AnalizBUFFER(void);
uint16_t CalkCRC16(uint8_t* buffer, uint8_t cnt);
uint8_t DigToSeg(uint8_t di);
void Display(void);
void StartTX_RS232(uint8_t tx);
//===============================================================================
/**
Контроллер осуществляет динамическую индикацию 8 семисегментных индикаторов
Связь с ведущим устройством по протоколу MODBUS RTU.
Параметры порта: 9600,8,N,1
На входе преобразователь RS485 - RS232.
Формат входящей посылки (in HEX) пример:
	C9	адрес МК(мой) 8 бит
	10	команда - запись группы регистров 8 бит
	00	адрес регистров приемника ст.разряд (у нас будет всегда 00)
	00	адрес регистров приемника мл.разряд (у нас будет всегда 00)
	00	число регистров ст.разряд (у нас будет всегда 00)
	08	число регистров мл.разряд (у нас будет всегда 08)
	10	число байт данных (у нас будет всегда 10 -> 16байт, т.к. регистру у DVP 16 битные)
	00		байт режима работы 7 (старшего) разряда
	07		байт данных 7 (старшего) разряда
	00		байт режима работы 6 разряда
	06		байт данных 6 разряда
	02		байт режима работы 5 разряда
	05		байт данных 5 разряда
	00		байт режима работы 4 разряда
	04		байт данных 4 разряда
	00		байт режима работы 3 разряда
	03		байт данных 3 разряда
	00		байт режима работы 2 разряда
	02		байт данных 2 разряда
	00		байт режима работы 1 разряда
	01		байт данных 1 разряда
	00		байт режима работы 0 (младшего) разряда
	00		байт данных 0 (младшего) разряда
	AD	CRC ст.байт
	31	CRC мл.байт

	байт режима работы
		0 бит - формат данных:
			0 - данные - число в BCD формате (расширенное) 0 1 2 3 4 5 6 7 8 9 A b c d E F
			1 - каждый бит данных означает свечение соответствующего сегмента
		1 бит - 0-обычный режим; 1-режим мигания с частотой 0,25/0,25сек
	
Ответ:
	C9	адрес МК(мой) 8 бит
	10	команда - запись группы регистров 8 бит
	00	код исключительного ответа
	XX	CRC ст.байт
	XX	CRC мл.байт


ЗАпрос клавиатуры - команда 02 см. стр. 396





Биты - Сегменты
	0 - G
	1 - dt
	2 - F
	3 - E
	4 - D
	5 - C
	6 - B
	7 - A
*/
























