#include <avr/io.h>
#include <util/delay.h>
#include "spi.c"
#include "st7735.c"


int main(void)
	{
	
	  st7735_init();
	  st7735_set_rotation(1);
  	  st7735_fill_screen(BLUE);
	  draw_pixel(10, 40, YELLOW);

		while(1)
		{
		}
		return 0;
	}
