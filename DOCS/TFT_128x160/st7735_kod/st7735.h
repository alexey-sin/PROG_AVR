/* File st7735.h */
#ifndef ST7735_H
#define ST7735_H

#define BLACK                   0x0000
#define BLUE                    0x001F
#define RED                     0xF800
#define GREEN                   0x07E0
#define CYAN                    0x07FF
#define MAGENTA                 0xF81F
#define YELLOW                  0xFFE0  
#define WHITE                   0xFFFF

#define ST7735_TFTWIDTH         128
#define ST7735_TFTHEIGHT        160
  
#define ST7735_NOP              0x00
#define ST7735_RESET_SW         0x01
#define ST7735_SLPIN            0x10
#define ST7735_SLPOUT           0x11
#define ST7735_PTLON            0x12
#define ST7735_NORON            0x13
#define ST7735_INVOFF           0x20
#define ST7735_INVON            0x21
#define ST7735_DISPOFF          0x28
#define ST7735_DISPON           0x29
#define ST7735_CASET            0x2A
#define ST7735_RASET            0x2B
#define ST7735_RAMWR            0x2C
#define ST7735_PTLAR            0x30
#define ST7735_COLMOD           0x3A
#define ST7735_MADCTL           0x36
  #define MADCTL_MY             0x80
  #define MADCTL_MX             0x40
  #define MADCTL_MV             0x20
  #define MADCTL_ML             0x10
  #define MADCTL_RGB            0x00
  #define MADCTL_BGR            0x08
  #define MADCTL_MH             0x04
#define ST7735_FRMCTR1          0xB1
#define ST7735_FRMCTR2          0xB2
#define ST7735_FRMCTR3          0xB3
#define ST7735_INVCTR           0xB4
#define ST7735_PWCTR1           0xC0
#define ST7735_PWCTR2           0xC1
#define ST7735_PWCTR3           0xC2
#define ST7735_PWCTR4           0xC3
#define ST7735_PWCTR5           0xC4
#define ST7735_VMCTR1           0xC5
#define ST7735_GMCTRP1          0xE0
#define ST7735_GMCTRN1          0xE1

void st7735_command(uint8_t command); 
void st7735_data(uint8_t data); 
void st7735_reset_hw(void);
void st7735_reset_sw(void);
void st7735_init(void);
void st7735_fill_screen(uint16_t color);
void st7735_set_rotation(uint8_t rotation);
void draw_pixel(int16_t x, int16_t y, uint16_t color);

uint8_t _width = ST7735_TFTWIDTH;
uint8_t _height = ST7735_TFTHEIGHT; 

#endif /* ST7735_H */
