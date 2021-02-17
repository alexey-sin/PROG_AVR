#include <util/delay.h>
#include "bits_macros.h"

const unsigned char signal_color[][3] PROGMEM = {
   {0xE0, 0xFF, 0xFF},
   {0xE0, 0xEC, 0xFF},
   {0xE0, 0xEC, 0x1C},
};

const unsigned char battery_color[][4] PROGMEM = {
   {0x00, 0xFF, 0xFF, 0xFF},
   {0xE0, 0xE0, 0xFF, 0xFF},
   {0xEC, 0xEC, 0xEC, 0xFF},
   {0x15, 0x15, 0x15, 0x15}
};


void graph_init(void);
void signal(unsigned char data); /* в поле "данные" допустимо значение 0 - 2 */
void battery_model(unsigned char data); /* В поле "уст-во" допустимые значения 0-1, в поле "данные" 0-3*/
void battery_control(unsigned char data);
void graph_warning(void);
void button_show(unsigned char button, unsigned char water);

void graph_init(void)
{
   LCD_FillScreen(WHITE);

   LCD_FillRect(5, 57, 3, 5, DARK_BLUE);
   LCD_FillRect(5, 66, 3, 10, DARK_BLUE);
   LCD_DrawLine(10, 63, 10, 66, DARK_BLUE);
   LCD_DrawLine(13, 60, 13, 69, DARK_BLUE);
   LCD_DrawLine(16, 57, 16, 72, DARK_BLUE);
   LCD_Putchar_Shadow('Т', 25, 68, DARK_BLUE, 2, 1, 0);
}

void signal(unsigned char data)
{
      LCD_FillRect(5, 5, 15, 15, pgm_read_byte(&(signal_color[data][2])));
      LCD_FillRect(5, 21, 10, 15, pgm_read_byte(&(signal_color[data][1])));
      LCD_FillRect(5, 37, 5, 15, pgm_read_byte(&(signal_color[data][0])));
}

void battery_model(unsigned char data)
{
      LCD_DrawRect(35, 5, 45, 25, 2, pgm_read_byte(&(battery_color[data][0])));
      LCD_FillRect(32, 12, 4, 9, pgm_read_byte(&(battery_color[data][0])));
      LCD_FillRect(39, 9, 11, 16, pgm_read_byte(&(battery_color[data][3])));
      LCD_FillRect(52, 9, 11, 16, pgm_read_byte(&(battery_color[data][2])));
      LCD_FillRect(65, 9, 11, 16, pgm_read_byte(&(battery_color[data][1])));
      LCD_Putchar_Shadow('М', 85, 10, DARK_BLUE, 2, 2, 0);
}

void battery_control(unsigned char data)
{
      LCD_DrawRect(35, 35, 45, 25, 2, pgm_read_byte(&(battery_color[data][0])));
      LCD_FillRect(32, 42, 4, 9, pgm_read_byte(&(battery_color[data][0])));
      LCD_FillRect(39, 39, 11, 16, pgm_read_byte(&(battery_color[data][3])));
      LCD_FillRect(52, 39, 11, 16, pgm_read_byte(&(battery_color[data][2])));
      LCD_FillRect(65, 39, 11, 16, pgm_read_byte(&(battery_color[data][1])));
      LCD_Putchar_Shadow('У', 85, 40, DARK_BLUE, 2, 2, 0);
}

void graph_warning(void)
{
   LCD_FillScreen(DARK_BLUE);
   LCD_Puts_Shadow("Течь!", 20, 15, WHITE, 2, 2, 0);
}

void button_show(unsigned char button, unsigned char water)
{
   if (BitIsClear(button, 0))
   {
      if (water == 0) LCD_FillRect(76, 65, 4, 10, WHITE); else LCD_FillRect(76, 65, 4, 10, DARK_BLUE);
   } else LCD_FillRect(76, 65, 4, 10, DARK_GREEN);
   if (BitIsClear(button, 1))
   {
      if (water == 0) LCD_FillRect(82, 65, 4, 10, WHITE); else LCD_FillRect(82, 65, 4, 10, DARK_BLUE);
   } else LCD_FillRect(82, 65, 4, 10, DARK_GREEN);
   if (BitIsClear(button, 2))
   {
      if (water == 0) LCD_FillRect(88, 65, 4, 10, WHITE); else LCD_FillRect(88, 65, 4, 10, DARK_BLUE);
   } else LCD_FillRect(88, 65, 4, 10, DARK_GREEN);
   if (BitIsClear(button, 3))
   {
      if (water == 0) LCD_FillRect(94, 65, 4, 10, WHITE); else LCD_FillRect(94, 65, 4, 10, DARK_BLUE);
   } else LCD_FillRect(94, 65, 4, 10, DARK_GREEN);
}
