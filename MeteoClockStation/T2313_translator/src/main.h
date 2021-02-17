/**




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

/** конфигурация фьюзов: https://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny2313a&LOW=E4&HIGH=9F&EXTENDED=F9&LOCKBIT=FF
	Fclk = 8MHz
	для F=8MГц (внешний кварц 8МГц) старт 14СК + 64мс
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
	SUT_CKSEL: внешний кварц 8МГц старт 14СК + 64мс
*/

//===============================================================================
#define SIZE_RX_ARRAY	16		//максимум 32!
unsigned char rx_array[SIZE_RX_ARRAY];
unsigned char* nrf_address = (unsigned char*)"METEO";
// #define nrf_address "METEO"
// unsigned char tx_array[8];
// unsigned char rx_address[5] = {0x4D,0x45,0x54,0x45,0x4F};	//METEO
// unsigned char tx_address[5] = {0x52,0x53,0x56,0x30,0x31};	//RSV01
// volatile uint8_t CntLCDdigit = 0;	//индекс обновляемого разряда LCD
// volatile uint8_t LCDvolume = 0;		//число выводимое на LCD

// volatile uint8_t ADCvolume = 0;		//вычисленное напряжение питания (*10)
// volatile uint16_t ADCbuffer = 0;	//буфер для складывания дискрет АЦП
// volatile uint8_t ADCcntBuff = 0;	//текущий счетчик измерений
// #define ADC_MAX_CNT_BUFF	64		//количество измерений в буфере
// #define ADC_MAX_VOLTAGE		17.603	//макимально допустимое напряжение(по резист делителю когда Vadc = Vопорн = 1.1В
// #define ADC_COEFFICIENT		(((ADC_MAX_VOLTAGE / 1023) / ADC_MAX_CNT_BUFF) * 10)
volatile uint8_t FLAGS = 0;
#define FL_INT_IRQ				0		//прерывание по IRQ
#define FL_TRANFER				1		//идет передача по SPI в NRF24

//===============================================================================
void init(void);
// void StartTX_RS232(uint8_t tx);
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
















