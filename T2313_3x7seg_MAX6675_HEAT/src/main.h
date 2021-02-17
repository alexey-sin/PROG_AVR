#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
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
// PORTB	порт на сегменты индикатора KEM-3361
#define PinB_SEGM_E		7
#define PinB_SEGM_D		6
#define PinB_SEGM_T		5
#define PinB_SEGM_C		4
#define PinB_SEGM_G		3
#define PinB_SEGM_B		2
#define PinB_SEGM_F		1
#define PinB_SEGM_A		0

// PORTD
#define PinD_RAZR_1		6	//1 разряд индикатора KEM-3361
#define PinD_RAZR_3		5	//3 разряд индикатора KEM-3361
#define PinD_RAZR_2		4	//2 разряд индикатора KEM-3361

#define PinD_KnPLS		3		//вывод кнопки плюс
#define PinD_KnMIN		2		//вывод кнопки минус

#define PinA_HEAT		0		//вывод управления нагревателем (1 - включен; 0 - выключен)

#define PinA_SCK		1		//вывод SCK MAX6675
#define PinD_SO			0		//вывод SO MAX6675 (вход)
#define PinD_CS			1		//вывод CS MAX6675
//===============================================================================
volatile uint8_t FLAGS1 = 0;
#define FL1_BLC1			0		//тик ~1.024mc
#define FL1_MODE_EDIT		1		//режим коррекции уставки
#define FL1_MODE_PID		2		//режим коррекции коэффициента П регулятора
#define FL1_BLC8			3		//тик ~8mc
//===============================================================================
volatile uint8_t BUTTON = 0;			//режим индикации: 0 - текущая температура; 1 - показ уставки; 2 - коррекция уставки
#define BTN_PLS				0		//нажатие "+" 
#define BTN_MIN				1		//нажатие "-"
#define BTN_SHORT_MAKED		2		//короткое нажатие кнопок отработано
#define BTN_LONG			3		//длинное нажатие

volatile uint16_t EDIT_OUT_CNT = 0;// счетчик тиков выхода из режима коррекции уставки
#define EDIT_OUT			3000	// значение счетчика для выхода из режима коррекции уставки

volatile uint8_t EDIT_CNT = 0;
#define EDIT_CHANGE			150	// значение счетчика очередного изменения значения уставки
#define CHANGE_PLS			0	// значение уставки увеличить
#define CHANGE_MIN			1	// значение уставки уменьшить

volatile uint8_t SEGM_RAZR[3];		// цифры разрядов для вывода на дисплей(старший индекс - старший разряд)
volatile uint8_t IND_RAZR = 0;		// индекс разряда выводимого на дисплей

volatile uint8_t SHAKE_CNT = 0;		// счетчик антидребезга
#define BUTTON_PUSH_SHORT_CNT		30
#define BUTTON_PUSH_LONG_CNT		250
/**
	счетчик антидребезга инкременируется каждый такт ~1mc
	антидребезг - нажатие на кнопку менее чем на BUTTON_SHORT_PUSH_CNT (значение SHAKE_CNT) - игнорируем.
	нажатие более BUTTON_PUSH_LONG_CNT воспринимаем как длинное нажатие
	
	В обычном режиме (флаг FL1_MODE_EDIT сброшен) дисплей показывает текущую температуру
	Короткое нажатие любой из кнопок переводит в режим показа уставки (флаг FL1_MODE_EDIT установлен)
	Нажатие кнопок приводит к коррекции уставки:
		- короткое - изменение на 1 градус
		- длинное - изменение на 1 градус в 0,15сек
	Из любого режима есть свои таймауты бездействия для возврата в обычный режим
*/
volatile uint8_t PID_CNT = 0;		// счетчик для тактов на П - регулятор
volatile uint8_t PID_VAL = 0;		// вычисленное значение для счетчика П - регулятора
/**
	Если PID_VAL = 0 нагреватель выключен.
	Включение нагревателя при PID_CNT = 0 (если PID_VAL != 0)
	и выключается при PID_CNT = PID_VAL.
	Счетчик PID_CNT всегда увеличивается по кругу.
	
	Диапазон регулировки PID_VAL от 1 до 255.
	При PID_VAL = 1 нагреватель включен в течении 8мс за период в 2,048сек т.е. 0,39%
	При PID_VAL = 255 нагреватель включен в течении 2048мс за период в 2,048сек т.е. 100%

*/
//-------------------------------------------------------------------------------
// Сохраняемые уставки и переменные
volatile uint16_t SETVAL_TEMP;		// температура уставки
uint16_t EEMEM EE_SETVAL_TEMP = 250;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_TEMP_MIN	 20		//от
#define DEF_SETVAL_TEMP_MAX	 400	//до
#define DEF_SETVAL_TEMP_STEP	1	//шаг
volatile uint8_t SETVAL_PID;		// значение коэффициента П регулятора
uint16_t EEMEM EE_SETVAL_PID = 10;	// начальное значение сохраненное в EEPROM
#define DEF_SETVAL_PID_MIN	 1		//от
#define DEF_SETVAL_PID_MAX	 20		//до
#define DEF_SETVAL_PID_STEP	1		//шаг
//-------------------------------------------------------------------------------
volatile uint16_t CNT_TEMP_START = 0;
#define TEMP_START	1000	//шаг
volatile uint16_t VALUE_TC = 0;		//значение температуры измеренное с датчика
volatile uint8_t STATUS_TC = 0;
/**  Датчик Т2
	STATUS_TC1 регистр состояния датчика TC1 (термопара К-типа)
	0 - неопределенное состояние (начальное)
	1 - нормально, есть измеренное значение температуры => значение в градусах в регистре VALUE_TC1
	2 - обрыв датчика
	3 - превышение максимального значения (больше 999 Гр. по цельсию)
*/
const uint8_t DigSeg[12] PROGMEM = {0xD7,0x14,0xCD,0x5D,0x1E,0x5B,0xDB,0x15,0xDF,0x5F,0x08,0x8F};
//===============================================================================
void init(void);
void NumberToButeBuffer(uint16_t number);
void Display(void);
void CheckButtons(void);
void ProcessingButtons(void);
void ChangeVALUE(uint8_t ditect);
void LoadValuesFromEEPROM(void);
void UpdateValuesFromEEPROM(void);
void ReadMAX6675(void);
void CalculatePID(void);
void ProcessingPID(void);
//===============================================================================
























