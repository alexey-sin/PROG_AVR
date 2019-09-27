#include <avr/io.h>
#include <avr/iotn2313.h>
// #include <avr/interrupt.h>
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
	конфигурация фьюзов: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313&LOW=66&HIGH=9F&EXTENDED=FF&LOCKBIT=FF

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
 CKDIV8 - V
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
#define PortBVK		PORTD
#define PortPinBVK	PIND
#define PortDDrBVK	DDRD
#define PinBVK_L	0
#define PinBVK_LJ	1
#define PinBVK_R	2
#define PinBVK_RJ	3
#define PinBVK_T	4
#define PinBVK_TJ	5

#define PortKL		PORTB
#define PortPinKL	PINB
#define PortDDrKL	DDRB
#define PinKL_L		0
#define PinKL_R		1

//===============================================================================
volatile uint8_t STATUS = 0;	// 1 - гильотина в режиме работа (есть движение в любую сторону)
volatile uint8_t LEFT = 0;		// 1 - идет движение штока поршня влево
volatile uint8_t RIGTH = 0;		// 1 - идет движение штока поршня вправо
volatile uint8_t BVK_T = 0;		// 1 - сигнальное состояние транспортера
volatile uint8_t BVK_L = 0;		// 1 - сигнальное состояние левого положения
volatile uint8_t BVK_R = 0;		// 1 - сигнальное состояние правого положения
//===============================================================================
void init(void);
void CheckBVK(void);
void ExecuteBVK(void);
//===============================================================================
/**
	Расчет состояния БВК ориентировано на тип P-N-P (сигнальное состояние - 1 => растущий фронт активный)
	для них джамперы должны быть установлены.
	В случае если необходимо установить БВК типа N-P-N (сигнальное состояние - 0 => падающий фронт активный)
	то джамперы необходимо снять.
	
	Условия включения клапанов:
	- сигнальное состояние БВК транспортера
	- нет состояния статуса "в работе", "идет движение влево", "идет движение вправо"
	- есть сигнальное состояние одного из положений штока поршня либо влево либо вправо,
		сигнальное состояние обоих положений или отсутствие одного из них - недопустимо.
	Включаем клапан:
		"влево" - если есть сигнальное состояние правого положения
		"вправо" - если есть сигнальное состояние левого положения
		
	Условие выключения клапанов:
	- есть состояние статуса "в работе"
		клапан "влево":
			- есть статус "идет движение влево"
			- есть сигнальное состояние правого положения штока поршня
		клапан "вправо":
			- есть статус "идет движение вправо"
			- есть сигнальное состояние левого положения штока поршня
	Перед выключением даем задержку в 500мс.
	После выключения даем задержку в 1сек.
			
*/
























