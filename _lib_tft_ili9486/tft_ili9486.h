#ifndef TFT_ILI9486_H
#define TFT_ILI9486_H

#include <avr/io.h>
#include "../../_libs_macros/bits_macros.h"
#include "../../_fonts/fonts.h"
#include <inttypes.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>
#include <string.h>

// #include "HW_ATmega32U4.h"
// #include "memorysaver.h"
// #include "HW_AVR_defines.h"

#include <tft_ili9486_config.h>

/**
ниже описанные define должны лежать в файле tft_st9486_config.h в src проекта

#define DDR_RST		DDRA
#define PORT_RST	PORTA
#define B_RST		7

#define DDR_CS		DDRA
#define PORT_CS		PORTA
#define B_CS		6

#define DDR_RS		DDRA
#define PORT_RS		PORTA
#define B_RS		5

#define DDR_WR		DDRA
#define PORT_WR		PORTA
#define B_WR		4

#define DDR_RD		DDRA
#define PORT_RD		PORTA
#define B_RD		3

#define DDR_DATA		DDRC
#define PORT_DATA		PORTC

//X_MAX и Y_MAX определяют ориентацию экрана: портретная или альбомная
//X это размер по горизонтали слева на право
//Y это размер по вертикали с верха в низ
#define X_MAX	320			//по горизонтали
#define Y_MAX	480			//по вертикали
*/

//*********************************
// COLORS
//*********************************
// VGA color palette
#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER		0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON		0x8000
#define VGA_YELLOW		0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA		0xF81F
#define VGA_PURPLE		0x8010
#define VGA_TRANSPARENT	0xFFFFFFFF


FONT currFont;

void InitLCD(void);

void LCD_Write_COM(uint8_t cmd);
void LCD_Write_DATA8(uint8_t VL);
void LCD_Write_DATA16(uint8_t VH,uint8_t VL);
uint8_t LCD_Read_DATA8(void);
void SetColorRGB(uint8_t r, uint8_t g, uint8_t b);
void SetColor(uint16_t color);
uint16_t GetColor(void);
void SetBackColorRGB(uint8_t r, uint8_t g, uint8_t b);
void SetBackColor(uint16_t color);
uint16_t GetBackColor(void);
void ClrScr(void);
void SetXY(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void FillScr(uint8_t colorH, uint8_t colorL, uint32_t cntPix);
void SetFont(FONT font);
uint16_t DrawChar(uint8_t c, uint16_t x, uint16_t y);
uint16_t DrawString(char* str, uint16_t x, uint16_t y);
void DrawRectFill(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t color);

		// void	clrScr(void);
		// void	drawPixel(uint16_t x, uint16_t y);
		// 
		// // void	fillScr(uint8_t r, uint8_t g, uint8_t b);
// void fillScr(uint16_t color);
		// void	drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		// void	drawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		// void	fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		// void	fillRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
		// void	drawCircle(uint16_t x, uint16_t y, uint16_t radius);
		// void	fillCircle(uint16_t x, uint16_t y, uint16_t radius);
		// void	print(char *st, uint16_t x, uint16_t y, uint16_t deg);
		// // void	print(String st, uint16_t x, uint16_t y, uint16_t deg);
		// // void	printNumI(uint16_t num, uint16_t x, uint16_t y, uint16_t length, char filler);
		// // void	printNumF(double num, uint8_t dec, uint16_t x, uint16_t y, char divider, uint16_t length, char filler);
		// uint8_t* getFont(void);
		// uint8_t	getFontXsize(void);
		// uint8_t	getFontYsize(void);
		// void	drawBitmap(uint16_t x, uint16_t y, uint16_t sx, uint16_t sy, uint8_t* data, uint16_t scale);
		// void	drawBitmap(uint16_t x, uint16_t y, uint16_t sx, uint16_t sy, uint* data, uint16_t deg, uint16_t rox, uint16_t roy);
		// void	lcdOff(void);
		// void	lcdOn(void);
		// void	setContrast(char c);
		// uint16_t		getDisplayXSize(void);
		// uint16_t		getDisplayYSize(void);
		// void	setBrightness(uint8_t br);
		// void	setDisplayPage(uint8_t page);
		// void	setWritePage(uint8_t page);


		// void LCD_Write_DATA(char VL);
		// void LCD_Write_COM_DATA(char com1,uint16_t dat1);
		// void LCD_Write_Bus_8(char VL);
		// void LCD_Write_DATA_8(char VL);		
		// void _hw_special_init(void);
		// void setPixel(uint16_t color);
		// void drawHLine(uint16_t x, uint16_t y, uint16_t l);
		// void drawVLine(uint16_t x, uint16_t y, uint16_t l);
		// void printChar(uint8_t c, uint16_t x, uint16_t y);
// void clrXY(void);
		// void rotateChar(uint8_t c, uint16_t x, uint16_t y, uint16_t pos, uint16_t deg);
		// // void _set_direction_registers(uint8_t mode);
// void _fast_fill_16(uint16_t ch, uint16_t cl, uint32_t pix);
		// void _fast_fill_8(uint16_t ch, uint16_t pix);
		// void _convert_float(char *buf, double num, uint16_t width, uint8_t prec);


#endif

