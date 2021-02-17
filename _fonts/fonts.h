#ifndef _FONTS
#define _FONTS

#include <avr/pgmspace.h>
#include <stdint.h>

typedef struct{
	char* data;
	uint16_t* offset;
	uint8_t* index;
	uint8_t* width;
	uint8_t height;
} FONT;
FONT font_Tahoma20;
FONT font_Tahoma10;
FONT font_Tahoma36num;

#endif
