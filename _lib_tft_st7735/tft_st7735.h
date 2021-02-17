#ifndef __ST7735_H__
#define __ST7735_H__

#include <avr/io.h>
#include "../../_libs_macros/bits_macros.h"
#include <inttypes.h>
#include <stdint.h>
#include <tft_st7735_config.h>

/**
ниже описанные define должны лежать в файле tft_st7735_config.h в src проекта
#define ST7735_PORT PORTD
#define ST7735_DDR DDRD
#define ST7735_RS 2
#define ST7735_CS 4
#define ST7735_CLK 0
#define ST7735_SDA 1
#define ST7735_RST 3
*/

	#define DELAY   0x80

	#define NOP     0x00
	#define SWRESET 0x01
	#define RDDID   0x04
	#define RDDST   0x09

	#define SLPIN   0x10
	#define SLPOUT  0x11
	#define PTLON   0x12
	#define NORON   0x13

	#define INVOFF  0x20
	#define INVON   0x21
	#define DISPOFF 0x28
	#define DISPON  0x29
	#define RAMRD   0x2E
	#define CASET   0x2A
	#define RASET   0x2B
	#define RAMWR   0x2C

	#define PTLAR   0x30
	#define MADCTL  0x36
	#define COLMOD  0x3A

	#define FRMCTR1 0xB1
	#define FRMCTR2 0xB2
	#define FRMCTR3 0xB3
	#define INVCTR  0xB4
	#define DISSET5 0xB6

	#define PWCTR1  0xC0
	#define PWCTR2  0xC1
	#define PWCTR3  0xC2
	#define PWCTR4  0xC3
	#define PWCTR5  0xC4
	#define VMCTR1  0xC5

	#define RDID1   0xDA
	#define RDID2   0xDB
	#define RDID3   0xDC
	#define RDID4   0xDD

	#define GMCTRP1 0xE0
	#define GMCTRN1 0xE1

	#define PWCTR6  0xFC

	// Colors
	#define BLACK   0x0000
	#define WHITE   0xFFFF
	#define RED     0xF000
	#define AQUA	0x667B
	#define CRIMSON	0xDB17

	#define ST7735_SUCCESS 0
	#define ST7735_ERROR   1

	// MV = 0 in MADCTL

	#define MAX_X   160  // max columns
	#define MAX_Y   128  // max rows
	#define SIZE_X  MAX_X - 1  // columns max counter
	#define SIZE_Y  MAX_Y - 1  // rows max counter
	#define CACHE_SIZE_MEM (MAX_X * MAX_Y)  // whole pixels
	#define CHARS_COLS_LEN 5  // number of columns for chars
	#define CHARS_ROWS_LEN 8  // number of rows for chars


	/** @const Command list ST7735B */
	extern const uint8_t INIT_ST7735B[];

	/** @const Characters */
	extern const uint8_t CHARACTERS[][CHARS_COLS_LEN];

	/** @enum Font sizes */
	typedef enum {
		// 1x high & 1x wide size - 16 строк (без интервалов) в строке 21 символ (поля с краев по 1 пикселю)
		X1 = 0x11,
		// 2x high & 1x wide size - 8 строк (без интервалов) в строке 21 символ (поля с краев по 1 пикселю)
		X2 = 0x21,
		// 2x high & 2x wider size - 8 строк (без интервалов) в строке 13 символов (поля с краев по 2 пикселя)
		X3 = 0x22
	} ESizes;

	void St7735Init(void);
	void St7735Commands(const uint8_t *commands);
	void CommandSend(uint8_t);
	void SPI_byte(uint8_t data);
	void Data8BitsSend(uint8_t);
	void Data16BitsSend(uint16_t);
	uint8_t SetWindow(uint8_t, uint8_t, uint8_t, uint8_t);
	char CheckPosition(uint8_t, uint8_t, ESizes);
	void SetPosition(uint8_t, uint8_t);
	uint8_t SetPartialArea(uint8_t, uint8_t);
	void SendColor565(uint16_t, uint16_t);
	void DrawPixel(uint8_t, uint8_t, uint16_t);
	char DrawChar(char, uint16_t, uint16_t, ESizes);
	void DrawString(volatile const char*, uint16_t, uint16_t, ESizes);
	char DrawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
	void DrawLineHorizontal(uint8_t, uint8_t, uint8_t, uint16_t);
	void DrawLineVertical(uint8_t, uint8_t, uint8_t, uint16_t);
	void ClearRectangle(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
	void ClearScreen(uint16_t);
	void UpdateScreen(void);

#endif
