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

 для F=1MГц (внутренний RC генератор) старт 14СК + 64мс
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

#define PortIN			PORTD	//порт входящих посылок
#define PinIN_C			2	//CLK
#define PinIN_D			3	//DATA
//===============================================================================
volatile uint8_t FLAGS1 = 0;
#define FL1_0_OVER			0		//переполнение счетчика 0
#define FL1_REC_BITS		1		//переполнение счетчика 0
#define FL1_BLC1			2		//тик ~1.024mc вывод очередного разряда индикации
#define FL1_BLC250			3		//флаг погашенного состояния в режиме мигания 250/250
//===============================================================================
// volatile uint8_t STATUS = 0;	// 1 - гильотина в режиме работа (есть движение в любую сторону)
volatile uint8_t CNT_BITS = 0;		// счетчик принимаемых бит по SPI
volatile uint32_t BLINK250 = 0;		// счетчик мигающего режима 250/250
volatile uint8_t SEGM_RAZR[8];		// сегменты разрядов подготовленные для вывода на дисплей(старший индекс - старший разряд)
volatile uint8_t IND_RAZR = 0;		// индекс разряда выводимого на дисплей
volatile uint32_t BUFFER = 0;		// Приемный буфер SPI
volatile uint8_t RAZR_REGIM[8] = {0};		// режим работы каждого разряда: 0 - всегда горит; 1- мигание 250/250мс;
//===============================================================================
void init(void);
void AnalizBUFFER(void);
uint8_t DigToSeg(uint8_t di);
void Display(void);
//===============================================================================
/**
Контроллер осуществляет динамическую индикацию 8 семисегментных индикаторов
Формат входных посылок из 3 байтов следующих старшим байтом вперед и старшим битом вперед:
	1 байт - формат данных (командный байт)
		младшие 3 бита(0,1,2) определяют разряд индикатора
		бит №3 - режим мигания с частотой 0,25/0,25сек
		старшие 4 бита - формат данных:
			0 - каждый бит данных означает свечение соответствующего сегмента
			1 - данные - число в BCD формате (расширенное) 0 1 2 3 4 5 6 7 8 9 A b c d E F
			2 - 
	2 байт - данные
	3 байт - контрольная сумма первых двух байтов.
	
	
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
























