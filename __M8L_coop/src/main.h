/**
	Курятник.
	


*/
#include <avr/io.h>
#include <avr/iom8.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <avr/pgmspace.h>
// #include <avr/eeprom.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>

// конфигурация фьюзов: http://www.engbedded.com/fusecalc/
//Fclk = 32kHz

#define		SetBit(reg, bit)			reg |= (1<<bit)           
#define		ClrBit(reg, bit)			reg &= (~(1<<bit))
#define		InvBit(reg, bit)			reg ^= (1<<bit)
#define		BitIsSet(reg, bit)			((reg & (1<<bit)) != 0)
#define		BitIsClr(reg, bit)		((reg & (1<<bit)) == 0)

//===============================================================================
// BOARD
//===============================================================================
/**
пины индикатора PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,  PC0,PC1,  PD3,PD4,PD5,PD6,PD7
пины кнопок "+" -> PC4, "-" -> PC5
выход на реле PD2
вход АЦП ADC7 (22вывод)
       A
     ---
  F//  //B
    ---G
 E//  //C
   ---
    D
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

// #define PinD_LCD_3C 3
// #define PinD_LCD_3E 4
// #define PinD_LCD_3G 5
// #define PinD_LCD_3A 6
// #define PinD_LCD_3F 7
//===============================================================================
// volatile uint8_t CntLCDdigit = 0;	//индекс обновляемого разряда LCD
// volatile uint8_t LCDvolume = 0;		//число выводимое на LCD

// volatile uint8_t ADCvolume = 0;		//вычисленное напряжение питания (*10)
// volatile uint16_t ADCbuffer = 0;	//буфер для складывания дискрет АЦП
// volatile uint8_t ADCcntBuff = 0;	//текущий счетчик измерений
// #define ADC_MAX_CNT_BUFF	64		//количество измерений в буфере
// #define ADC_MAX_VOLTAGE		17.603	//макимально допустимое напряжение(по резист делителю когда Vadc = Vопорн = 1.1В
// #define ADC_COEFFICIENT		(((ADC_MAX_VOLTAGE / 1023) / ADC_MAX_CNT_BUFF) * 10)

// volatile uint8_t STATUS = 0;
/**
	0 - LCD погашен, анализ АЦП, включение ДХО по уставкам, монитор кнопок
	1 - LCD выводит текущее значение АЦП, анализ АЦП, включение ДХО по уставкам, монитор кнопок
	2 - режим меню коррекции уставок, LCD в мигающем режиме выводит текущую уставку
	
	переход в меню коррекции уставок, листание уставок - одновременное нажатие "+" и "-"
	выход из меню коррекции уставок и уход в дежурный режим по времении соотв. define-ов
*/
// volatile uint8_t FLAGS1 = 0;
// #define FL1_BLC250			0		//тик ~0,25сек
// #define FL1_BLC2			1		//тик ~2мс
// #define FL1_BTN				2		//есть событие по кнопкам
// #define FL1_BTN_WAIT		3		//антидребезг
// #define FL1_BTN_MMM      	4		//кнопка M
// #define FL1_BTN_PLS      	5		//кнопка +
// #define FL1_BTN_MIN      	6		//кнопка -
// #define FL1_LCD_FAIR      	7		//режим мигания LCD. 1 - горит, 0 - погашен

// volatile uint8_t FLAGS2 = 0;
// #define FL2_ADC				0		//АЦП ~125кГц
// #define FL2_ADC_OK			1		//АЦП вычисленные значения похожи на достоверные
// #define FL2_RELE_DELAY		2		//ожидание паузы выключения реле
//===============================================================================
// volatile uint8_t CountMUL_BTN_WAIT = 0;
// #define MUL_BTN_WAIT	100	//множитель на 2мс антидребезг кнопок

// volatile uint8_t CountMUL_LCD = 0;	//счетчик ухода в спящий режим
// #define MUL_LCD_WAIT_SLEEP	40	//множитель на 0,25сек - выход из индикации напряжения
// #define MUL_MENU_EXIT	40	//множитель на 0,25сек - выход из настроек

// volatile uint8_t MENU = 0;	//текущая позиция меню (индекс. от 0 до 2 включительно)
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
// void Tick2(void);
// void Tick250(void);
// void CalculateVoltage(void);
// void CheckButton(void);
// void ExecuteButton(void);
// void ClearLCD(void);
// void OutLCD(void);	//вывод на дисплей числа
// void LoadValuesFromEEPROM(void);
// void UpdateValuesFromEEPROM(void);
//===============================================================================
//===============================================================================
//===============================================================================
















