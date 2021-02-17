#include "st7735.h"

void st7735_command(uint8_t command) 
{
  cbi(ST_PORT, DC_pin);  
  cbi(ST_PORT, CS_pin);  
  SPI_byte(command);
  sbi(ST_PORT, CS_pin);    
}

void st7735_data(uint8_t data) 
{
  sbi(ST_PORT, DC_pin);  
  cbi(ST_PORT, CS_pin);  
  SPI_byte(data);
  sbi(ST_PORT, CS_pin);    
}

void st7735_reset_hw(void)
{
  #ifdef RESET_HW
    cbi(ST_PORT, RESET_HW_pin); 
    _delay_us(RESET_HW_DELAY);
    sbi(ST_PORT, RESET_HW_pin);     
    _delay_ms(120);    
  #endif  
}

void st7735_reset_sw(void)
{
  st7735_command(ST7735_RESET_SW);
  _delay_ms(120);  
}

void st7735_init(void)
{
  SPI_init();
  #ifndef RESET_HW
    st7735_reset_sw();     
  #else
    st7735_reset_hw();      
  #endif   
  st7735_command(ST7735_SLPOUT);        
  _delay_ms(120);
  st7735_command(ST7735_FRMCTR1); 
    st7735_data(0x01); st7735_data(0x2C); st7735_data(0x2D); 
  st7735_command(ST7735_FRMCTR2); 
    st7735_data(0x01); st7735_data(0x2C); st7735_data(0x2D);     
  st7735_command(ST7735_FRMCTR3); 
    st7735_data(0x01); st7735_data(0x2C); st7735_data(0x2D);     
    st7735_data(0x01); st7735_data(0x2C); st7735_data(0x2D);     
  st7735_command(ST7735_INVCTR);
    st7735_data(0x07);        
  st7735_command(ST7735_PWCTR1); 
    st7735_data(0xA2); st7735_data(0x02); st7735_data(0x84);    
  st7735_command(ST7735_PWCTR2); 
    st7735_data(0xC5);
  st7735_command(ST7735_PWCTR3); 
    st7735_data(0x0A); st7735_data(0x00);     
  st7735_command(ST7735_PWCTR4); 
    st7735_data(0x8A); st7735_data(0x2A);     
  st7735_command(ST7735_PWCTR5); 
    st7735_data(0x8A); st7735_data(0xEE);
  st7735_command(ST7735_VMCTR1); 
    st7735_data(0x0E);
  st7735_command(ST7735_INVOFF);  
  st7735_command(ST7735_MADCTL);
    st7735_data(0xC0); 
  st7735_command(ST7735_COLMOD);
    st7735_data(0x05); 
  st7735_command(ST7735_GMCTRP1); 
    st7735_data(0x02); st7735_data(0x1C); st7735_data(0x07); st7735_data(0x12);
    st7735_data(0x37); st7735_data(0x32); st7735_data(0x29); st7735_data(0x2D);
    st7735_data(0x29); st7735_data(0x25); st7735_data(0x2B); st7735_data(0x39);
    st7735_data(0x00); st7735_data(0x01); st7735_data(0x03); st7735_data(0x10);
  st7735_command(ST7735_GMCTRN1); 
    st7735_data(0x03); st7735_data(0x1D); st7735_data(0x07); st7735_data(0x06);
    st7735_data(0x2E); st7735_data(0x2C); st7735_data(0x29); st7735_data(0x2D);
    st7735_data(0x2E); st7735_data(0x2E); st7735_data(0x37); st7735_data(0x3F);
    st7735_data(0x00); st7735_data(0x00); st7735_data(0x02); st7735_data(0x10);
  st7735_command(ST7735_CASET); 
    st7735_data(0x00); st7735_data(0x00); 
    st7735_data(0x00); st7735_data(0x7F);
  st7735_command(ST7735_RASET); 
    st7735_data(0x00); st7735_data(0x00); 
    st7735_data(0x00); st7735_data(0x9F);
  st7735_command(ST7735_NORON); 
  st7735_command(ST7735_DISPON); 
  _delay_ms(120);  
}

void st7735_fill_screen(uint16_t color)
{
  st7735_command(ST7735_CASET); 
    st7735_data(0x00); st7735_data(0x00); 
    st7735_data(0x00); st7735_data(_width - 1);
  st7735_command(ST7735_RASET); 
    st7735_data(0x00); st7735_data(0x00); 
    st7735_data(0x00); st7735_data(_height - 1);
  st7735_command(ST7735_RAMWR);
  for(uint16_t counter = 0; counter < (_width*_height); counter++)
    {
      st7735_data(color >> 8);
      st7735_data(color);   
    } 
}

void st7735_set_rotation(uint8_t rotation)
{
  st7735_command(ST7735_MADCTL);
  switch (rotation) 
    {
      case 0:
        st7735_data(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
        _width  = ST7735_TFTWIDTH;
        _height = ST7735_TFTHEIGHT;
      break;
      
      case 1:
        st7735_data(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
        _width = ST7735_TFTHEIGHT;
        _height = ST7735_TFTWIDTH;
      break;
    }  
}
void draw_pixel(int16_t x, int16_t y, uint16_t color)
{
  st7735_command(ST7735_CASET); 
    st7735_data(x >> 8); st7735_data(x); 
    st7735_data((x + 1) >> 8); st7735_data(x + 1);
  
  st7735_command(ST7735_RASET); 
    st7735_data(y >> 8); st7735_data(y); 
    st7735_data((y + 1) >> 8); st7735_data(y + 1);
      
  st7735_command(ST7735_RAMWR);
    st7735_data(color >> 8);
    st7735_data(color);     
}
