#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "tft_st7735.h"


/** @array Charset */
const uint8_t CHARACTERS[][5] PROGMEM = {
	{0x00, 0x00, 0x00, 0x00, 0x00}, // 20 space
	{0x00, 0x00, 0x5f, 0x00, 0x00}, // 21 !
	{0x00, 0x07, 0x00, 0x07, 0x00}, // 22 "
	{0x14, 0x7f, 0x14, 0x7f, 0x14}, // 23 #
	{0x24, 0x2a, 0x7f, 0x2a, 0x12}, // 24 $
	{0x23, 0x13, 0x08, 0x64, 0x62}, // 25 %
	{0x36, 0x49, 0x55, 0x22, 0x50}, // 26 &
	{0x00, 0x05, 0x03, 0x00, 0x00}, // 27 '
	{0x00, 0x1c, 0x22, 0x41, 0x00}, // 28 (
	{0x00, 0x41, 0x22, 0x1c, 0x00}, // 29 )
	{0x14, 0x08, 0x3e, 0x08, 0x14}, // 2a *
	{0x08, 0x08, 0x3e, 0x08, 0x08}, // 2b +
	{0x00, 0x50, 0x30, 0x00, 0x00}, // 2c ,
	{0x08, 0x08, 0x08, 0x08, 0x08}, // 2d -
	{0x00, 0x60, 0x60, 0x00, 0x00}, // 2e .
	{0x20, 0x10, 0x08, 0x04, 0x02}, // 2f /
	{0x3e, 0x51, 0x49, 0x45, 0x3e}, // 30 0
	{0x00, 0x42, 0x7f, 0x40, 0x00}, // 31 1
	{0x42, 0x61, 0x51, 0x49, 0x46}, // 32 2
	{0x21, 0x41, 0x45, 0x4b, 0x31}, // 33 3
	{0x18, 0x14, 0x12, 0x7f, 0x10}, // 34 4
	{0x27, 0x45, 0x45, 0x45, 0x39}, // 35 5
	{0x3c, 0x4a, 0x49, 0x49, 0x30}, // 36 6
	{0x01, 0x71, 0x09, 0x05, 0x03}, // 37 7
	{0x36, 0x49, 0x49, 0x49, 0x36}, // 38 8
	{0x06, 0x49, 0x49, 0x29, 0x1e}, // 39 9
	{0x00, 0x36, 0x36, 0x00, 0x00}, // 3a :
	{0x00, 0x56, 0x36, 0x00, 0x00}, // 3b ;
	{0x08, 0x14, 0x22, 0x41, 0x00}, // 3c <
	{0x14, 0x14, 0x14, 0x14, 0x14}, // 3d =
	{0x00, 0x41, 0x22, 0x14, 0x08}, // 3e >
	{0x02, 0x01, 0x51, 0x09, 0x06}, // 3f ?
	{0x32, 0x49, 0x79, 0x41, 0x3e}, // 40 @
	{0x7e, 0x11, 0x11, 0x11, 0x7e}, // 41 A
	{0x7f, 0x49, 0x49, 0x49, 0x36}, // 42 B
	{0x3e, 0x41, 0x41, 0x41, 0x22}, // 43 C
	{0x7f, 0x41, 0x41, 0x22, 0x1c}, // 44 D
	{0x7f, 0x49, 0x49, 0x49, 0x41}, // 45 E
	{0x7f, 0x09, 0x09, 0x09, 0x01}, // 46 F
	{0x3e, 0x41, 0x49, 0x49, 0x7a}, // 47 G
	{0x7f, 0x08, 0x08, 0x08, 0x7f}, // 48 H
	{0x00, 0x41, 0x7f, 0x41, 0x00}, // 49 I
	{0x20, 0x40, 0x41, 0x3f, 0x01}, // 4a J
	{0x7f, 0x08, 0x14, 0x22, 0x41}, // 4b K
	{0x7f, 0x40, 0x40, 0x40, 0x40}, // 4c L
	{0x7f, 0x02, 0x0c, 0x02, 0x7f}, // 4d M
	{0x7f, 0x04, 0x08, 0x10, 0x7f}, // 4e N
	{0x3e, 0x41, 0x41, 0x41, 0x3e}, // 4f O
	{0x7f, 0x09, 0x09, 0x09, 0x06}, // 50 P
	{0x3e, 0x41, 0x51, 0x21, 0x5e}, // 51 Q
	{0x7f, 0x09, 0x19, 0x29, 0x46}, // 52 R
	{0x46, 0x49, 0x49, 0x49, 0x31}, // 53 S
	{0x01, 0x01, 0x7f, 0x01, 0x01}, // 54 T
	{0x3f, 0x40, 0x40, 0x40, 0x3f}, // 55 U
	{0x1f, 0x20, 0x40, 0x20, 0x1f}, // 56 V
	{0x3f, 0x40, 0x38, 0x40, 0x3f}, // 57 W
	{0x63, 0x14, 0x08, 0x14, 0x63}, // 58 X
	{0x07, 0x08, 0x70, 0x08, 0x07}, // 59 Y
	{0x61, 0x51, 0x49, 0x45, 0x43}, // 5a Z
	{0x00, 0x7f, 0x41, 0x41, 0x00}, // 5b [
	{0x02, 0x04, 0x08, 0x10, 0x20}, // 5c backslash
	{0x00, 0x41, 0x41, 0x7f, 0x00}, // 5d ]
	{0x04, 0x02, 0x01, 0x02, 0x04}, // 5e ^
	{0x40, 0x40, 0x40, 0x40, 0x40}, // 5f _
	{0x00, 0x01, 0x02, 0x04, 0x00}, // 60 `
	{0x20, 0x54, 0x54, 0x54, 0x78}, // 61 a
	{0x7f, 0x48, 0x44, 0x44, 0x38}, // 62 b
	{0x38, 0x44, 0x44, 0x44, 0x20}, // 63 c
	{0x38, 0x44, 0x44, 0x48, 0x7f}, // 64 d
	{0x38, 0x54, 0x54, 0x54, 0x18}, // 65 e
	{0x08, 0x7e, 0x09, 0x01, 0x02}, // 66 f
	{0x0c, 0x52, 0x52, 0x52, 0x3e}, // 67 g
	{0x7f, 0x08, 0x04, 0x04, 0x78}, // 68 h
	{0x00, 0x44, 0x7d, 0x40, 0x00}, // 69 i
	{0x20, 0x40, 0x44, 0x3d, 0x00}, // 6a j
	{0x7f, 0x10, 0x28, 0x44, 0x00}, // 6b k
	{0x00, 0x41, 0x7f, 0x40, 0x00}, // 6c l
	{0x7c, 0x04, 0x18, 0x04, 0x78}, // 6d m
	{0x7c, 0x08, 0x04, 0x04, 0x78}, // 6e n
	{0x38, 0x44, 0x44, 0x44, 0x38}, // 6f o
	{0x7c, 0x14, 0x14, 0x14, 0x08}, // 70 p
	{0x08, 0x14, 0x14, 0x14, 0x7c}, // 71 q
	{0x7c, 0x08, 0x04, 0x04, 0x08}, // 72 r
	{0x48, 0x54, 0x54, 0x54, 0x20}, // 73 s
	{0x04, 0x3f, 0x44, 0x40, 0x20}, // 74 t
	{0x3c, 0x40, 0x40, 0x20, 0x7c}, // 75 u
	{0x1c, 0x20, 0x40, 0x20, 0x1c}, // 76 v
	{0x3c, 0x40, 0x30, 0x40, 0x3c}, // 77 w
	{0x44, 0x28, 0x10, 0x28, 0x44}, // 78 x
	{0x0c, 0x50, 0x50, 0x50, 0x3c}, // 79 y
	{0x44, 0x64, 0x54, 0x4c, 0x44}, // 7a z
	{0x00, 0x08, 0x36, 0x41, 0x00}, // 7b {
	{0x00, 0x00, 0x7f, 0x00, 0x00}, // 7c |
	{0x00, 0x41, 0x36, 0x08, 0x00}, // 7d }
	{0x10, 0x08, 0x08, 0x10, 0x08}, // 7e ~
	{0x06, 0x09, 0x09, 0x06, 0x00}, // 7f

	{0x7e, 0x11, 0x11, 0x11, 0x7e}, // c0 À
	{0x7f, 0x49, 0x49, 0x49, 0x31}, // c1 Á
	{0x7f, 0x49, 0x49, 0x49, 0x36}, // c2 Â
	{0x7f, 0x01, 0x01, 0x01, 0x01}, // c3 Ã
	{0xc0, 0x7e, 0x41, 0x7f, 0xc0}, // c4 Ä
	{0x7f, 0x49, 0x49, 0x49, 0x41}, // c5 Å
	{0x77, 0x08, 0x7f, 0x08, 0x77}, // c6 Æ
	{0x22, 0x49, 0x49, 0x49, 0x36}, // c7 Ç
	{0x7f, 0x20, 0x10, 0x08, 0x7f}, // c8 È
	{0x7e, 0x21, 0x11, 0x09, 0x7e}, // c9 É
	{0x7f, 0x08, 0x14, 0x22, 0x41}, // ca Ê
	{0x40, 0x7e, 0x01, 0x01, 0x7f}, // cb Ë
	{0x7f, 0x02, 0x04, 0x02, 0x7f}, // cc Ì
	{0x7f, 0x08, 0x08, 0x08, 0x7f}, // cd Í
	{0x3e, 0x41, 0x41, 0x41, 0x3e}, // ce Î
	{0x7f, 0x01, 0x01, 0x01, 0x7f}, // cf Ï
	{0x7f, 0x09, 0x09, 0x09, 0x06}, // d0 Ð
	{0x3e, 0x41, 0x41, 0x41, 0x22}, // d1 Ñ
	{0x01, 0x01, 0x7f, 0x01, 0x01}, // d2 Ò
	{0x27, 0x48, 0x48, 0x48, 0x3f}, // d3 Ó
	{0x0e, 0x11, 0x7f, 0x11, 0x0e}, // d4 Ô
	{0x63, 0x14, 0x08, 0x14, 0x63}, // d5 Õ
	{0x7f, 0x40, 0x40, 0x7f, 0xc0}, // d6 Ö
	{0x07, 0x08, 0x08, 0x08, 0x7f}, // d7 ×
	{0x7f, 0x40, 0x7f, 0x40, 0x7f}, // d8 Ø
	{0x7f, 0x40, 0x7f, 0x40, 0xff}, // d9 Ù
	{0x01, 0x7f, 0x48, 0x48, 0x30}, // da Ú
	{0x7f, 0x48, 0x48, 0x30, 0x7f}, // db Û
	{0x7f, 0x48, 0x48, 0x48, 0x30}, // dc Ü
	{0x22, 0x41, 0x49, 0x49, 0x3e}, // dd Ý
	{0x7f, 0x08, 0x3e, 0x41, 0x3e}, // de Þ
	{0x66, 0x19, 0x09, 0x09, 0x7f}, // df ß
	{0x20, 0x54, 0x54, 0x54, 0x78}, // e0 à
	{0x3c, 0x4a, 0x4a, 0x4a, 0x31}, // e1 á
	{0x7c, 0x54, 0x54, 0x54, 0x28}, // e2 â
	{0x7c, 0x04, 0x04, 0x0c, 0x00}, // e3 ã
	{0xc0, 0x78, 0x44, 0x7c, 0xc0}, // e4 ä
	{0x38, 0x54, 0x54, 0x54, 0x08}, // e5 å
	{0x6c, 0x10, 0x7c, 0x10, 0x6c}, // e6 æ
	{0x28, 0x44, 0x54, 0x54, 0x28}, // e7 ç
	{0x7c, 0x20, 0x10, 0x08, 0x7c}, // e8 è
	{0x7c, 0x20, 0x12, 0x0a, 0x7c}, // e9 é
	{0x7c, 0x10, 0x28, 0x44, 0x00}, // ea ê
	{0x40, 0x38, 0x04, 0x04, 0x7c}, // eb ë
	{0x7c, 0x08, 0x10, 0x08, 0x7c}, // ec ì
	{0x7c, 0x10, 0x10, 0x10, 0x7c}, // ed í
	{0x38, 0x44, 0x44, 0x44, 0x38}, // ee î
	{0x7c, 0x04, 0x04, 0x04, 0x7c}, // ef ï
	{0xfc, 0x44, 0x44, 0x44, 0x38}, // f0 ð
	{0x38, 0x44, 0x44, 0x44, 0x28}, // f1 ñ
	{0x04, 0x04, 0x7c, 0x04, 0x04}, // f2 ò
	{0x9c, 0xa0, 0x60, 0x3c, 0x00}, // f3 ó
	{0x18, 0x24, 0x7c, 0x24, 0x18}, // f4 ô
	{0x6c, 0x10, 0x10, 0x6c, 0x00}, // f5 õ
	{0x7c, 0x40, 0x40, 0x7c, 0xc0}, // f6 ö
	{0x0c, 0x10, 0x10, 0x10, 0x7c}, // f7 ÷
	{0x7c, 0x40, 0x7c, 0x40, 0x7c}, // f8 ø
	{0x7c, 0x40, 0x7c, 0x40, 0xfc}, // f9 ù
	{0x04, 0x7c, 0x50, 0x50, 0x20}, // fa ú
	{0x7c, 0x50, 0x50, 0x20, 0x7c}, // fb û
	{0x7c, 0x50, 0x50, 0x50, 0x20}, // fc ü
	{0x28, 0x44, 0x54, 0x54, 0x38}, // fd ý
	{0x7c, 0x10, 0x38, 0x44, 0x38}, // fe þ
	{0x48, 0x34, 0x14, 0x14, 0x7c}  // ff ÿ
};

/** @var array Chache memory char index row */
int indexRow = 0;
/** @var array Chache memory char index column */
int indexCol = 0;

void St7735Init(void){
	ST7735_DDR |= (1 << ST7735_CLK) | (1 << ST7735_SDA) | (1 << ST7735_RS) | (1 << ST7735_RST) | (1 << ST7735_CS);
	ST7735_PORT &= ~((1 << ST7735_CLK) | (1 << ST7735_SDA) | (1 << ST7735_RS) | (1 << ST7735_RST) | (1 << ST7735_CS));
	
	//RESET
	ST7735_PORT |= (1 << ST7735_RST);
	_delay_ms(200);
	ST7735_PORT &= ~(1 << ST7735_RST);
	_delay_ms(200);
	ST7735_PORT |=  (1 << ST7735_RST);

	// Software reset - no arguments,  delay
	CommandSend(SWRESET);
	_delay_ms(150);
	// Out of sleep mode, no arguments, delay
	CommandSend(SLPOUT);
	_delay_ms(200);
	// Set color mode, 1 argument delay
	CommandSend(COLMOD);
	Data8BitsSend(0x05);
	_delay_ms(10);
 
	// D7  D6  D5  D4  D3  D2  D1  D0
	// MY  MX  MV  ML RGB  MH   -   -
	// ------------------------------
	// ------------------------------
	// MV  MX  MY -> {MV (row / column exchange) MX (column address order), MY (row address order)}
	// ------------------------------
	//  0   0   0 -> begin left-up corner, end right-down corner 
	//               left-right (normal view) 
	//  0   0   1 -> begin left-down corner, end right-up corner 
	//               left-right (Y-mirror)
	//  0   1   0 -> begin right-up corner, end left-down corner 
	//               right-left (X-mirror)
	//  0   1   1 -> begin right-down corner, end left-up corner
	//               right-left (X-mirror, Y-mirror)
	//  1   0   0 -> begin left-up corner, end right-down corner
	//               up-down (X-Y exchange)  
	//  1   0   1 -> begin left-down corner, end right-up corner
	//               down-up (X-Y exchange, Y-mirror)
	//  1   1   0 -> begin right-up corner, end left-down corner 
	//               up-down (X-Y exchange, X-mirror)  
	//  1   1   1 -> begin right-down corner, end left-up corner
	//               down-up (X-Y exchange, X-mirror, Y-mirror)
	// ------------------------------
	//  ML: vertical refresh order 
	//      0 -> refresh top to bottom 
	//      1 -> refresh bottom to top
	// ------------------------------
	// RGB: filter panel
	//      0 -> RGB 
	//      1 -> BGR        
	// ------------------------------ 
	//  MH: horizontal refresh order 
	//      0 -> refresh left to right 
	//      1 -> refresh right to left
	// 0xA0 = 1010 0000
	CommandSend(MADCTL);
	Data8BitsSend(0xA0);

	// Main screen turn on
	CommandSend(DISPON);
	_delay_ms(200);
}
void CommandSend(uint8_t data){
	// chip enable - active low
	ST7735_PORT &= ~(1 << ST7735_CS);
	// command (active low)
	ST7735_PORT &= ~(1 << ST7735_RS);
	// transmitting data
	SPI_byte(data);
	// chip disable - idle high
	ST7735_PORT |= (1 << ST7735_CS);
}
void SPI_byte(uint8_t data){
	for(uint8_t i = 8; i; i--){
		if(data & 0x80) ST7735_PORT |= (1 << ST7735_SDA); else ST7735_PORT &= ~(1 << ST7735_SDA);
		data <<= 1;
		ST7735_PORT |= (1 << ST7735_CLK);
		ST7735_PORT &= ~(1 << ST7735_CLK);
	}
}
void Data8BitsSend(uint8_t data){
	// chip enable - active low
	ST7735_PORT &= ~(1 << ST7735_CS);
	// data (active high)
	ST7735_PORT |= (1 << ST7735_RS);
	// transmitting data
	SPI_byte(data);
	// chip disable - idle high
	ST7735_PORT |= (1 << ST7735_CS);
}
void Data16BitsSend(uint16_t data){
	ST7735_PORT &= ~(1 << ST7735_CS);	// chip enable - active low
	ST7735_PORT |= (1 << ST7735_RS);	// data (active high)
	SPI_byte((uint8_t)(data >> 8));	// transmitting data high byte
	SPI_byte((uint8_t)data);	// transmitting data low byte
	ST7735_PORT |= (1 << ST7735_CS);	// chip disable - idle high
}
uint8_t SetPartialArea(uint8_t sRow, uint8_t eRow){
	// check if coordinates is out of range
	if((sRow > SIZE_Y) || (eRow > SIZE_Y)) return 0;	// out of range

	// column address set
	CommandSend(PTLAR);
	// start start Row
	Data8BitsSend(0x00);
	// start start Row
	Data8BitsSend(sRow);
	// row end Row
	Data8BitsSend(0x00);
	// end end Row
	Data8BitsSend(eRow);
	// column address set
	CommandSend(PTLON);
	// success
	return 1;
}
uint8_t SetWindow(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1){
	// check if coordinates is out of range
	if((x0 > x1) || (x1 > SIZE_X) || (y0 > y1) || (y1 > SIZE_Y)) return ST7735_ERROR;	// out of range

	// column address set
	CommandSend(CASET);
	// send start x position
	Data16BitsSend(x0);
	// send end x position
	Data16BitsSend(x1);

	// row address set
	CommandSend(RASET);
	// send start y position
	Data16BitsSend(y0);
	// send end y position
	Data16BitsSend(y1);

	// success
	return ST7735_SUCCESS;
}
void SendColor565(uint16_t color, uint16_t count){
	// access to RAM
	CommandSend(RAMWR);
	// counter
	ST7735_PORT &= ~(1 << ST7735_CS);	// chip enable - active low
	ST7735_PORT |= (1 << ST7735_RS);	// data (active high)

	while(count--){
		SPI_byte((uint8_t)(color >> 8));	// transmitting data high byte
		SPI_byte((uint8_t)color);	// transmitting data low byte
	}
	ST7735_PORT |= (1 << ST7735_CS);	// chip disable - idle high
}
void DrawPixel(uint8_t x, uint8_t y, uint16_t color){
	// set window
	SetWindow(x, x, y, y);
	// draw pixel by 565 mode
	SendColor565(color, 1);
}
char DrawChar(char character, uint16_t color, uint16_t bgcolor, ESizes size){
	uint8_t letter, idxCol, idxRow;

	character -= 0x20;
	if(character > 0x5F) character -= 0x40;	//russian simbol
	idxCol = CHARS_COLS_LEN;	// last column of character array - 5 columns 
	idxRow = CHARS_ROWS_LEN;	// last row of character array - 8 rows / bits

	// --------------------------------------
	// SIZE X1 - normal font 1x high, 1x wide
	// --------------------------------------
	if(size == X1){  
		while(idxCol--){	// loop through 5 bits
			letter = pgm_read_byte(&CHARACTERS[(uint8_t)character][idxCol]);	// read from ROM memory 
			while(idxRow--){	// loop through 8 bits
				if(letter & (1 << idxRow)){	// check if bit set
					DrawPixel(indexCol + idxCol, indexRow + idxRow, color);	// draw pixel
				}else{	// draw backgraund pixel
					DrawPixel(indexCol + idxCol, indexRow + idxRow, bgcolor);
				}
			}
			idxRow = CHARS_ROWS_LEN;	// fill index row again
		}
	// --------------------------------------
	// SIZE X2 - font 2x higher, normal wide
	// --------------------------------------
	}else if(size == X2){
		while(idxCol--){	// loop through 5 bytes
			letter = pgm_read_byte(&CHARACTERS[(uint8_t)character][idxCol]);	// read from ROM memory 
			while(idxRow--){	// loop through 8 bits
				if(letter & (1 << idxRow)){	// check if bit set
					// draw first left up pixel; 
					// (idxRow << 1) - 2x multiplied 
					DrawPixel(indexCol + idxCol, indexRow + (idxRow << 1), color);
					// draw second left down pixel
					DrawPixel(indexCol + idxCol, indexRow + (idxRow << 1) + 1, color);
				}else{	// draw backgraund pixel
					DrawPixel(indexCol + idxCol, indexRow + (idxRow << 1), bgcolor);
					DrawPixel(indexCol + idxCol, indexRow + (idxRow << 1) + 1, bgcolor);
				}
			}
			idxRow = CHARS_ROWS_LEN;	// fill index row again
		}
	// --------------------------------------
	// SIZE X3 - font 2x higher, 2x wider
	// --------------------------------------
	}else if(size == X3){
		while(idxCol--){	// loop through 5 bytes
			letter = pgm_read_byte(&CHARACTERS[(uint8_t)character][idxCol]);	// read from ROM memory 
			while(idxRow--){	// loop through 8 bits
				if(letter & (1 << idxRow)){	// check if bit set
					// draw first left up pixel; 
					// (idxRow << 1) - 2x multiplied 
					DrawPixel(indexCol + (idxCol << 1), indexRow + (idxRow << 1), color);
					// draw second left down pixel
					DrawPixel(indexCol + (idxCol << 1), indexRow + (idxRow << 1) + 1, color);
					// draw third right up pixel
					DrawPixel(indexCol + (idxCol << 1) + 1, indexRow + (idxRow << 1), color);
					// draw fourth right down pixel
					DrawPixel(indexCol + (idxCol << 1) + 1, indexRow + (idxRow << 1) + 1, color);
				}else{	// draw backgraund pixel
					DrawPixel(indexCol + (idxCol << 1), indexRow + (idxRow << 1), bgcolor);
					// draw second left down pixel
					DrawPixel(indexCol + (idxCol << 1), indexRow + (idxRow << 1) + 1, bgcolor);
					// draw third right up pixel
					DrawPixel(indexCol + (idxCol << 1) + 1, indexRow + (idxRow << 1), bgcolor);
					// draw fourth right down pixel
					DrawPixel(indexCol + (idxCol << 1) + 1, indexRow + (idxRow << 1) + 1, bgcolor);
				}
			}
			idxRow = CHARS_ROWS_LEN;	// fill index row again
		}
	}
	indexCol += (CHARS_COLS_LEN + 1) * (size & 0x0F);
	SetPosition(indexCol, indexRow);
	// return exit
	return ST7735_SUCCESS;
}
void SetPosition(uint8_t x, uint8_t y){
	indexRow = y;
	indexCol = x;
}
char CheckPosition(uint8_t x, uint8_t y, ESizes size){
	// check if coordinates is out of range
	if((x > MAX_X) && (y > MAX_Y)) return ST7735_ERROR;	// out of range

	// check if coordinates is out of range
	if((x > MAX_X) && (y <= MAX_Y)){
		// set position y
		indexRow = indexRow + (CHARS_ROWS_LEN + 1) * (size >> 4) + 2;
		// set position x
		indexCol = 2;
	}

	// success
	return ST7735_SUCCESS;
}
void DrawString(volatile const char *str, uint16_t color, uint16_t bgcolor, ESizes size){
	uint8_t i = 0;

	while(str[i] != '\0') DrawChar(str[i++], color, bgcolor, size);
}
char DrawLine(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color){
	int16_t D;	// determinant
	int16_t delta_x, delta_y;	// deltas
	int16_t trace_x = 1, trace_y = 1;	// steps

	delta_x = x2 - x1;	// delta x
	delta_y = y2 - y1;	// delta y

	if(delta_x < 0){	// check if x2 > x1
		delta_x = -delta_x;	// negate delta x
		trace_x = -trace_x;	// negate step x
	}

	if(delta_y < 0){	// check if y2 > y1
		delta_y = -delta_y;	// negate detla y
		trace_y = -trace_y;	// negate step y
	}

	if(delta_y < delta_x){	// Bresenham condition for m < 1 (dy < dx)
		D = (delta_y << 1) - delta_x;	// calculate determinant
		DrawPixel(x1, y1, color);	// draw first pixel
	
		while(x1 != x2){	// check if x1 equal x2
			x1 += trace_x;	// update x1
			if(D >= 0){	// check if determinant is positive
				y1 += trace_y;	// update y1
				D -= 2*delta_x;	// update determinant
			}
			D += 2*delta_y;	// update deteminant
			DrawPixel(x1, y1, color);	// draw next pixel
		}
	}else{	// for m > 1 (dy > dx)    
		D = delta_y - (delta_x << 1);	// calculate determinant
		DrawPixel(x1, y1, color);	// draw first pixel
		while(y1 != y2){	// check if y2 equal y1
			y1 += trace_y;	// update y1
			if(D <= 0){	// check if determinant is positive
				x1 += trace_x;	// update y1
				D += 2*delta_y;	// update determinant
			}
			D -= 2*delta_x;	// update deteminant
			DrawPixel(x1, y1, color);	// draw next pixel
		}
	}
	// success return
	return 1;
}
void DrawLineHorizontal(uint8_t xs, uint8_t xe, uint8_t y, uint16_t color){
	uint8_t temp;
	
	if(xs > xe){	// check if start is > as end  
		temp = xs;	// temporary safe
		xe = xs;	// start change for end
		xs = temp;	// end change for start
	}
	SetWindow(xs, xe, y, y);	// set window
	SendColor565(color, xe - xs);	// draw pixel by 565 mode
}
void DrawLineVertical(uint8_t x, uint8_t ys, uint8_t ye, uint16_t color){
	uint8_t temp;
	
	if(ys > ye){	// check if start is > as end
		temp = ys;	// temporary safe
		ye = ys;	// start change for end
		ys = temp;	// end change for start
	}
	SetWindow(x, x, ys, ye);	// set window
	SendColor565(color, ye - ys);	// draw pixel by 565 mode
}
void ClearRectangle(uint8_t xs, uint8_t xe, uint8_t ys, uint8_t ye, uint16_t color){
	uint8_t temp;
	
	if(xs > xe){	// check if start is > as end  
		temp = xe;	// temporary safe
		xe = xs;	// start change for end
		xs = temp;	// end change for start
	}
	if(ys > ye){	// check if start is > as end
		temp = ye;	// temporary safe
		ye = ys;	// start change for end
		ys = temp;	// end change for start
	}
	SetWindow(xs, xe, ys, ye);	// set window
	SendColor565(color, (xe-xs+1)*(ye-ys+1));	// send color
}
void ClearScreen(uint16_t color){
	SetWindow(0, SIZE_X, 0, SIZE_Y);	// set whole window
	SendColor565(color, CACHE_SIZE_MEM);	// draw individual pixels
}
void UpdateScreen(void){
	// display on
	CommandSend(DISPON);
}





