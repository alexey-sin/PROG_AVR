#include "tft_ili9486.h"

uint8_t fch, fcl;	//цвет шрифтов
uint8_t bch, bcl;	//цвет фона
// uint8_t orient;
// uint16_t disp_x_size = 319, disp_y_size = 479;
// struct current_font	cfont;
// uint8_t transparent = FALSE;

void InitLCD(void){
	DDR_RST |= (1 << B_RST);
	DDR_CS |= (1 << B_CS);
	DDR_RS |= (1 << B_RS);
	DDR_WR |= (1 << B_WR);
	DDR_RD |= (1 << B_RD);

	PORT_RST |= (1 << B_RST);
	PORT_CS |= (1 << B_CS);
	PORT_RS |= (1 << B_RS);
	PORT_WR |= (1 << B_WR);
	PORT_RD |= (1 << B_RD);

	DDR_DATA = 0xFF;
	
	// ClrBit(PORT_RST, B_RST);	// HW RESET
	// _delay_ms(1);
	// SetBit(PORT_RST, B_RST);
	// _delay_ms(200);

	ClrBit(PORT_CS, B_CS);

	LCD_Write_COM(0x01);		// SW RESET
	_delay_ms(200);

	// COLMOD: Pixel Format Set
	LCD_Write_COM(0x3A);
	LCD_Write_DATA8(0x55);		//один пиксель будет кодироваться 16 битами
	
	//Normal Display Mode ON
	LCD_Write_COM(0x13);

	//Display Inversion OFF
	LCD_Write_COM(0x20);

	//Tearing Effect Line OFF
	LCD_Write_COM(0x34);

	// Memory Access Control
	LCD_Write_COM(0x36); 		//выбираем ориентацию дисплея
	/**
	0b10001000 Перевернутая Портретная
	0b01001000 Нормальная Портретная
	0b00001000 Нормальная Портретная справа-налево
	0b00101000 Нормальная Альбомная
	0b10101000 Нормальная Альбомная справа-налево
	0b01101000 Перевернутая Альбомная справа-налево
	0b11101000 Перевернутая Альбомная
	*/
	if(X_MAX > Y_MAX) LCD_Write_DATA8(0b00101000);	//Альбомная
	else LCD_Write_DATA8(0b01001000);	//Портретная

	// Sleep Out
	LCD_Write_COM(0x11);

	_delay_ms(200);

	//Display On
	LCD_Write_COM(0x29);	
	SetBit(PORT_CS, B_CS); 
	
	SetColor(VGA_WHITE);
	SetBackColor(VGA_BLACK);
	ClrScr();
}
void LCD_Write_COM(uint8_t cmd){   
	ClrBit(PORT_RS, B_RS);
	
	ClrBit(PORT_WR, B_WR);
	PORT_DATA = cmd;
	SetBit(PORT_WR, B_WR);

}
void LCD_Write_DATA8(uint8_t VL){
	SetBit(PORT_RS, B_RS);
	
	ClrBit(PORT_WR, B_WR);
	PORT_DATA = VL;
	SetBit(PORT_WR, B_WR);
}
void LCD_Write_DATA16(uint8_t VH,uint8_t VL){
	SetBit(PORT_RS, B_RS);
	
	ClrBit(PORT_WR, B_WR);
	PORT_DATA = VH;
	SetBit(PORT_WR, B_WR);
	
	ClrBit(PORT_WR, B_WR);
	PORT_DATA = VL;
	SetBit(PORT_WR, B_WR);
}
uint8_t LCD_Read_DATA8(void){
	
	// DDR_DATA = 0x00;
	SetBit(PORT_RS, B_RS);
	SetBit(PORT_WR, B_WR);
	
	ClrBit(PORT_RD, B_RD);
	SetBit(PORT_RD, B_RD);
	
	// DDR_DATA = 0xFF;
	
	return PIN_DATA;
}
void SetColorRGB(uint8_t r, uint8_t g, uint8_t b){
	fch=((r & 248) | g >> 5);
	fcl=((g & 28) << 3 | b >> 3);
}
void SetColor(uint16_t color){
	fch = (color >> 8);
	fcl = (color & 0xFF);
}
uint16_t GetColor(){
	return (fch<<8) | fcl;
}
void SetBackColorRGB(uint8_t r, uint8_t g, uint8_t b){
	bch=((r & 248) | g >> 5);
	bcl=((g & 28) << 3 | b >> 3);
}
void SetBackColor(uint16_t color){
	bch = (color >> 8);
	bcl = (color & 0xFF);
}
uint16_t GetBackColor(){
	return (bch << 8) | bcl;
}
void ClrScr(void){
	ClrBit(PORT_CS, B_CS);
	SetXY(0, (X_MAX - 1), 0, (Y_MAX - 1));
	
	FillScr(bch, bcl, ((uint32_t)X_MAX * (uint32_t)Y_MAX));
	SetBit(PORT_CS, B_CS);
}
void SetXY(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){
	LCD_Write_COM(0x2A);		//Column
	LCD_Write_DATA16((x1 >> 8), x1);
	LCD_Write_DATA16((x2 >> 8), x2);
	LCD_Write_COM(0x2B);		//Row
	LCD_Write_DATA16((y1 >> 8), y1);
	LCD_Write_DATA16((y2 >> 8), y2);
	LCD_Write_COM(0x2C);
}
void FillScr(uint8_t colorH, uint8_t colorL, uint32_t cntPix){
	SetBit(PORT_RS, B_RS);
	for(uint32_t i = 0; i < cntPix; i++){
		ClrBit(PORT_WR, B_WR);
		PORT_DATA = colorH;
		SetBit(PORT_WR, B_WR);
		ClrBit(PORT_WR, B_WR);
		PORT_DATA = colorL;
		SetBit(PORT_WR, B_WR);
	}
}
void SetFont(FONT font){
	currFont = font;
}
uint16_t DrawChar(uint8_t c, uint16_t x, uint16_t y){	//возвращает измененное значение х для следующего символа
	uint8_t index = pgm_read_byte(&currFont.index[c]);
	uint16_t offset = pgm_read_word(&currFont.offset[index]);
	char* pData = &currFont.data[offset];
	uint8_t iw, width = pgm_read_byte(&currFont.width[index]);
	uint8_t height = currFont.height;
	uint8_t ib, byte;
	
	SetXY(x, (x + width - 1), y, (y + height - 1));
	ib = 8;
	byte = pgm_read_byte(pData++);
	while(height--){	//проход по строкам
		iw = width;
		while(iw--){	//проход по столбцам
			if(ib == 0){
				ib = 8;
				byte = pgm_read_byte(pData++);
			}
			if(byte & 0x80) LCD_Write_DATA16(fch, fcl);	//бит закрашен
			else LCD_Write_DATA16(bch, bcl);
			byte <<= 1;
			ib--;
		}
	}
	return (x + width);
}
uint16_t DrawString(char* str, uint16_t x, uint16_t y){
	uint8_t i = 0;
	
	ClrBit(PORT_CS, B_CS);
	while(str[i] != 0) x = DrawChar(str[i++], x, y);
	SetBit(PORT_CS, B_CS);
	
	return x;
}
void DrawRectFill(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint16_t color){	//start X, stop X, start Y, stop Y, color
	uint32_t cntPix = (uint32_t)((x2 + 1) - x1) * (uint32_t)((y2 + 1) - y1);
	
	ClrBit(PORT_CS, B_CS);
	SetXY(x1, x2, y1, y2);
	FillScr((color >> 8), (color & 0xFF), cntPix);
	SetBit(PORT_CS, B_CS);
}
// void printChar(uint8_t c, uint16_t x, uint16_t y){
	// uint8_t i,ch;
	// uint16_t j;
	// uint16_t temp; 

	// ClrBit(PORT_CS, B_CS);
  
	// if (!_transparent)
	// {
		// if (orient==PORTRAIT)
		// {
			// setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			// for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			// {
				// ch=pgm_read_byte(&cfont.font[temp]);
				// for(i=0;i<8;i++)
				// {   
					// if((ch&(1<<(7-i)))!=0)   
					// {
						// setPixel((fch<<8)|fcl);
					// } 
					// else
					// {
						// setPixel((bch<<8)|bcl);
					// }   
				// }
				// temp++;
			// }
		// }
		// else
		// {
			// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			// for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			// {
				// setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				// for (uint16_t zz=(cfont.x_size/8)-1; zz>=0; zz--)
				// {
					// ch=pgm_read_byte(&cfont.font[temp+zz]);
					// for(i=0;i<8;i++)
					// {   
						// if((ch&(1<<i))!=0)   
						// {
							// setPixel((fch<<8)|fcl);
						// } 
						// else
						// {
							// setPixel((bch<<8)|bcl);
						// }   
					// }
				// }
				// temp+=(cfont.x_size/8);
			// }
		// }
	// }
	// else
	// {
		// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		// for(j=0;j<cfont.y_size;j++) 
		// {
			// for (uint16_t zz=0; zz<(cfont.x_size/8); zz++)
			// {
				// ch=pgm_read_byte(&cfont.font[temp+zz]); 
				// for(i=0;i<8;i++)
				// {   
				
					// if((ch&(1<<(7-i)))!=0)   
					// {
						// setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
						// setPixel((fch<<8)|fcl);
					// } 
				// }
			// }
			// temp+=(cfont.x_size/8);
		// }
	// }

	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }


// void clrXY(){
	// if (orient==PORTRAIT)
		// setXY(0,0,disp_x_size,disp_y_size);
	// else
		// setXY(0,0,disp_y_size,disp_x_size);
// }

// void drawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	// if (x1>x2)
	// {
		// swap(uint16_t, x1, x2);
	// }
	// if (y1>y2)
	// {
		// swap(uint16_t, y1, y2);
	// }

	// drawHLine(x1, y1, x2-x1);
	// drawHLine(x1, y2, x2-x1);
	// drawVLine(x1, y1, y2-y1);
	// drawVLine(x2, y1, y2-y1);
// }

// void drawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	// if (x1>x2)
	// {
		// swap(uint16_t, x1, x2);
	// }
	// if (y1>y2)
	// {
		// swap(uint16_t, y1, y2);
	// }
	// if ((x2-x1)>4 && (y2-y1)>4)
	// {
		// drawPixel(x1+1,y1+1);
		// drawPixel(x2-1,y1+1);
		// drawPixel(x1+1,y2-1);
		// drawPixel(x2-1,y2-1);
		// drawHLine(x1+2, y1, x2-x1-4);
		// drawHLine(x1+2, y2, x2-x1-4);
		// drawVLine(x1, y1+2, y2-y1-4);
		// drawVLine(x2, y1+2, y2-y1-4);
	// }
// }

// void fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	// // if(x1>x2) swap(uint16_t, x1, x2);
	// // if(y1>y2)swap(uint16_t, y1, y2);

	// ClrBit(PORT_CS, B_CS);
	// setXY(x1, y1, x2, y2);
	// SetBit(PORT_RS, B_RS);
	// _fast_fill_16(fch,fcl,(((x2-x1)+1)*((y2-y1)+1)));
	// SetBit(PORT_CS, B_CS);
// }

// void fillRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	// if(x1>x2) swap(uint16_t, x1, x2);
	// if(y1>y2)swap(uint16_t, y1, y2);

	// if((x2 - x1) > 4 && (y2 - y1) > 4){
		// for (uint16_t i=0; i<((y2-y1)/2)+1; i++)
		// {
			// switch(i)
			// {
			// case 0:
				// drawHLine(x1+2, y1+i, x2-x1-4);
				// drawHLine(x1+2, y2-i, x2-x1-4);
				// break;
			// case 1:
				// drawHLine(x1+1, y1+i, x2-x1-2);
				// drawHLine(x1+1, y2-i, x2-x1-2);
				// break;
			// default:
				// drawHLine(x1, y1+i, x2-x1);
				// drawHLine(x1, y2-i, x2-x1);
			// }
		// }
	// }
// }

// void drawCircle(uint16_t x, uint16_t y, uint16_t radius){
	// uint16_t f = 1 - radius;
	// uint16_t ddF_x = 1;
	// uint16_t ddF_y = -2 * radius;
	// uint16_t x1 = 0;
	// uint16_t y1 = radius;
 
	// ClrBit(PORT_CS, B_CS);
	// setXY(x, y + radius, x, y + radius);
	// LCD_Write_DATA(fch,fcl);
	// setXY(x, y - radius, x, y - radius);
	// LCD_Write_DATA(fch,fcl);
	// setXY(x + radius, y, x + radius, y);
	// LCD_Write_DATA(fch,fcl);
	// setXY(x - radius, y, x - radius, y);
	// LCD_Write_DATA(fch,fcl);
 
	// while(x1 < y1)
	// {
		// if(f >= 0) 
		// {
			// y1--;
			// ddF_y += 2;
			// f += ddF_y;
		// }
		// x1++;
		// ddF_x += 2;
		// f += ddF_x;    
		// setXY(x + x1, y + y1, x + x1, y + y1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x - x1, y + y1, x - x1, y + y1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x + x1, y - y1, x + x1, y - y1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x - x1, y - y1, x - x1, y - y1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x + y1, y + x1, x + y1, y + x1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x - y1, y + x1, x - y1, y + x1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x + y1, y - x1, x + y1, y - x1);
		// LCD_Write_DATA(fch,fcl);
		// setXY(x - y1, y - x1, x - y1, y - x1);
		// LCD_Write_DATA(fch,fcl);
	// }
	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }

// void fillCircle(uint16_t x, uint16_t y, uint16_t radius){
	// for(uint16_t y1=-radius; y1<=0; y1++) 
		// for(uint16_t x1=-radius; x1<=0; x1++)
			// if(x1*x1+y1*y1 <= radius*radius) 
			// {
				// drawHLine(x+x1, y+y1, 2*(-x1));
				// drawHLine(x+x1, y-y1, 2*(-x1));
				// break;
			// }
// }

// void clrScr(void){
	// ClrBit(PORT_CS, B_CS);
	// clrXY();
	// SetBit(PORT_RS, B_RS);
	// _fast_fill_16(0, 0, ((disp_x_size + 1) * (disp_y_size + 1)));
	// SetBit(PORT_CS, B_CS);
// }

// // void fillScr(uint8_t r, uint8_t g, uint8_t b){
	// // uint16_t color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	// // fillScr(color);
// // }

// void fillScr(uint16_t color){
	// uint8_t ch, cl;
	
	// ch = (color >> 8);
	// cl = (color & 0xFF);

	// ClrBit(PORT_CS, B_CS);
	// // clrXY();
	// SetBit(PORT_RS, B_RS);
	// // _fast_fill_16(ch,cl,(uint32_t)(X_MAX * Y_MAX));
	// SetBit(PORT_CS, B_CS);
// }


// void setPixel(uint16_t color){
	// LCD_Write_DATA((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
// }

// void drawPixel(uint16_t x, uint16_t y){
	// ClrBit(PORT_CS, B_CS);
	// setXY(x, y, x, y);
	// setPixel((fch << 8)|fcl);
	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }

// void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	// if (y1==y2)
		// drawHLine(x1, y1, x2-x1);
	// else if (x1==x2)
		// drawVLine(x1, y1, y2-y1);
	// else
	// {
		// uint16_t	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		// uint8_t			xstep =  x2 > x1 ? 1 : -1;
		// uint16_t	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		// uint8_t			ystep =  y2 > y1 ? 1 : -1;
		// uint16_t				col = x1, row = y1;

		// ClrBit(PORT_CS, B_CS);
		// if (dx < dy)
		// {
			// uint16_t t = - (dy >> 1);
			// while (true)
			// {
				// setXY (col, row, col, row);
				// LCD_Write_DATA (fch, fcl);
				// if (row == y2)
					// return;
				// row += ystep;
				// t += dx;
				// if (t >= 0)
				// {
					// col += xstep;
					// t   -= dy;
				// }
			// } 
		// }
		// else
		// {
			// uint16_t t = - (dx >> 1);
			// while (true)
			// {
				// setXY (col, row, col, row);
				// LCD_Write_DATA (fch, fcl);
				// if (col == x2)
					// return;
				// col += xstep;
				// t += dy;
				// if (t >= 0)
				// {
					// row += ystep;
					// t   -= dx;
				// }
			// } 
		// }
		// SetBit(PORT_CS, B_CS);
	// }
	// clrXY();
// }

// void drawHLine(uint16_t x, uint16_t y, uint16_t l){
	// if (l<0)
	// {
		// l = -l;
		// x -= l;
	// }
	// ClrBit(PORT_CS, B_CS);
	// setXY(x, y, x+l, y);
	// SetBit(PORT_RS, B_RS);
	// _fast_fill_16(fch,fcl,l);
	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }

// void drawVLine(uint16_t x, uint16_t y, uint16_t l){
	// if (l<0)
	// {
		// l = -l;
		// y -= l;
	// }
	// ClrBit(PORT_CS, B_CS);
	// setXY(x, y, x, y+l);
	// SetBit(PORT_RS, B_RS);
	// _fast_fill_16(fch,fcl,l);
	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }

// void printChar(uint8_t c, uint16_t x, uint16_t y){
	// uint8_t i,ch;
	// uint16_t j;
	// uint16_t temp; 

	// ClrBit(PORT_CS, B_CS);
  
	// if (!_transparent)
	// {
		// if (orient==PORTRAIT)
		// {
			// setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			// for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			// {
				// ch=pgm_read_byte(&cfont.font[temp]);
				// for(i=0;i<8;i++)
				// {   
					// if((ch&(1<<(7-i)))!=0)   
					// {
						// setPixel((fch<<8)|fcl);
					// } 
					// else
					// {
						// setPixel((bch<<8)|bcl);
					// }   
				// }
				// temp++;
			// }
		// }
		// else
		// {
			// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			// for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			// {
				// setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				// for (uint16_t zz=(cfont.x_size/8)-1; zz>=0; zz--)
				// {
					// ch=pgm_read_byte(&cfont.font[temp+zz]);
					// for(i=0;i<8;i++)
					// {   
						// if((ch&(1<<i))!=0)   
						// {
							// setPixel((fch<<8)|fcl);
						// } 
						// else
						// {
							// setPixel((bch<<8)|bcl);
						// }   
					// }
				// }
				// temp+=(cfont.x_size/8);
			// }
		// }
	// }
	// else
	// {
		// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
		// for(j=0;j<cfont.y_size;j++) 
		// {
			// for (uint16_t zz=0; zz<(cfont.x_size/8); zz++)
			// {
				// ch=pgm_read_byte(&cfont.font[temp+zz]); 
				// for(i=0;i<8;i++)
				// {   
				
					// if((ch&(1<<(7-i)))!=0)   
					// {
						// setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);
						// setPixel((fch<<8)|fcl);
					// } 
				// }
			// }
			// temp+=(cfont.x_size/8);
		// }
	// }

	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }

// void rotateChar(uint8_t c, uint16_t x, uint16_t y, uint16_t pos, uint16_t deg){
	// uint8_t i,j,ch;
	// uint16_t temp; 
	// uint16_t newx,newy;
	// double radian;
	// radian=deg*0.0175;  

	// ClrBit(PORT_CS, B_CS);

	// temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	// for(j=0;j<cfont.y_size;j++) 
	// {
		// for (uint16_t zz=0; zz<(cfont.x_size/8); zz++)
		// {
			// ch=pgm_read_byte(&cfont.font[temp+zz]); 
			// for(i=0;i<8;i++)
			// {   
				// newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				// newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				// setXY(newx,newy,newx+1,newy+1);
				
				// if((ch&(1<<(7-i)))!=0)   
				// {
					// setPixel((fch<<8)|fcl);
				// } 
				// else  
				// {
					// if (!_transparent)
						// setPixel((bch<<8)|bcl);
				// }   
			// }
		// }
		// temp+=(cfont.x_size/8);
	// }
	// SetBit(PORT_CS, B_CS);
	// clrXY();
// }

// void print(char *st, uint16_t x, uint16_t y, uint16_t deg){
	// uint16_t stl, i;

	// stl = strlen(st);

	// if (orient==PORTRAIT)
	// {
	// if (x==RIGHT)
		// x=(disp_x_size+1)-(stl*cfont.x_size);
	// if (x==CENTER)
		// x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	// }
	// else
	// {
	// if (x==RIGHT)
		// x=(disp_y_size+1)-(stl*cfont.x_size);
	// if (x==CENTER)
		// x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	// }

	// for (i=0; i<stl; i++)
		// if (deg==0)
			// printChar(*st++, x + (i*(cfont.x_size)), y);
		// else
			// rotateChar(*st++, x, y, i, deg);
// }

// void print(String st, uint16_t x, uint16_t y, uint16_t deg){
	// char buf[st.length()+1];

	// st.toCharArray(buf, st.length()+1);
	// print(buf, x, y, deg);
// }

// void printNumI(uint16_t num, uint16_t x, uint16_t y, uint16_t length, char filler){
	// char buf[25];
	// char st[27];
	// char neg=false;
	// uint16_t c=0, f=0;
  
	// if (num==0)
	// {
		// if (length!=0)
		// {
			// for (c=0; c<(length-1); c++)
				// st[c]=filler;
			// st[c]=48;
			// st[c+1]=0;
		// }
		// else
		// {
			// st[0]=48;
			// st[1]=0;
		// }
	// }
	// else
	// {
		// if (num<0)
		// {
			// neg=true;
			// num=-num;
		// }
	  
		// while (num>0)
		// {
			// buf[c]=48+(num % 10);
			// c++;
			// num=(num-(num % 10))/10;
		// }
		// buf[c]=0;
	  
		// if (neg)
		// {
			// st[0]=45;
		// }
	  
		// if (length>(c+neg))
		// {
			// for (uint16_t i=0; i<(length-c-neg); i++)
			// {
				// st[i+neg]=filler;
				// f++;
			// }
		// }

		// for (uint16_t i=0; i<c; i++)
		// {
			// st[i+neg+f]=buf[c-i-1];
		// }
		// st[c+neg+f]=0;

	// }

	// print(st,x,y);
// }

// void printNumF(double num, uint8_t dec, uint16_t x, uint16_t y, char divider, uint16_t length, char filler){
	// char st[27];
	// boolean neg=false;

	// if (dec<1)
		// dec=1;
	// else if (dec>5)
		// dec=5;

	// if (num<0)
		// neg = true;

	// _convert_float(st, num, length, dec);

	// if (divider != '.')
	// {
		// for (uint16_t i=0; i<sizeof(st); i++)
			// if (st[i]=='.')
				// st[i]=divider;
	// }

	// if (filler != ' ')
	// {
		// if (neg)
		// {
			// st[0]='-';
			// for (uint16_t i=1; i<sizeof(st); i++)
				// if ((st[i]==' ') || (st[i]=='-'))
					// st[i]=filler;
		// }
		// else
		// {
			// for (uint16_t i=0; i<sizeof(st); i++)
				// if (st[i]==' ')
					// st[i]=filler;
		// }
	// }

	// print(st,x,y);
// }

// void setFont(uint8_t* font){
	// cfont.font=font;
	// cfont.x_size=fontbyte(0);
	// cfont.y_size=fontbyte(1);
	// cfont.offset=fontbyte(2);
	// cfont.numchars=fontbyte(3);
// }

// uint8_t* getFont(){
	// return cfont.font;
// }

// uint8_t getFontXsize(){
	// return cfont.x_size;
// }

// uint8_t getFontYsize(){
	// return cfont.y_size;
// }

// void drawBitmap(uint16_t x, uint16_t y, uint16_t sx, uint16_t sy, uint8_t* data, uint16_t scale){
	// uint16_t col;
	// uint16_t tx, ty, tc, tsx, tsy;

	// if (scale==1)
	// {
		// if (orient==PORTRAIT)
		// {
			// ClrBit(PORT_CS, B_CS);
			// setXY(x, y, x+sx-1, y+sy-1);
			// for (tc=0; tc<(sx*sy); tc++)
			// {
				// col=pgm_read_word(&data[tc]);
				// LCD_Write_DATA(col>>8,col & 0xff);
			// }
			// SetBit(PORT_CS, B_CS);
		// }
		// else
		// {
			// ClrBit(PORT_CS, B_CS);
			// for (ty=0; ty<sy; ty++)
			// {
				// setXY(x, y+ty, x+sx-1, y+ty);
				// for (tx=sx-1; tx>=0; tx--)
				// {
					// col=pgm_read_word(&data[(ty*sx)+tx]);
					// LCD_Write_DATA(col>>8,col & 0xff);
				// }
			// }
			// SetBit(PORT_CS, B_CS);
		// }
	// }
	// else
	// {
		// if (orient==PORTRAIT)
		// {
			// ClrBit(PORT_CS, B_CS);
			// for (ty=0; ty<sy; ty++)
			// {
				// setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				// for (tsy=0; tsy<scale; tsy++)
					// for (tx=0; tx<sx; tx++)
					// {
						// col=pgm_read_word(&data[(ty*sx)+tx]);
						// for (tsx=0; tsx<scale; tsx++)
							// LCD_Write_DATA(col>>8,col & 0xff);
					// }
			// }
			// SetBit(PORT_CS, B_CS);
		// }
		// else
		// {
			// ClrBit(PORT_CS, B_CS);
			// for (ty=0; ty<sy; ty++)
			// {
				// for (tsy=0; tsy<scale; tsy++)
				// {
					// setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					// for (tx=sx-1; tx>=0; tx--)
					// {
						// col=pgm_read_word(&data[(ty*sx)+tx]);
						// for (tsx=0; tsx<scale; tsx++)
							// LCD_Write_DATA(col>>8,col & 0xff);
					// }
				// }
			// }
			// SetBit(PORT_CS, B_CS);
		// }
	// }
	// clrXY();
// }

// void drawBitmap(uint16_t x, uint16_t y, uint16_t sx, uint16_t sy, uint* data, uint16_t deg, uint16_t rox, uint16_t roy){
	// unsigned uint16_t col;
	// uint16_t tx, ty, newx, newy;
	// double radian;
	// radian=deg*0.0175;  

	// if (deg==0)
		// drawBitmap(x, y, sx, sy, data);
	// else
	// {
		// ClrBit(PORT_CS, B_CS);
		// for (ty=0; ty<sy; ty++)
			// for (tx=0; tx<sx; tx++)
			// {
				// col=pgm_read_word(&data[(ty*sx)+tx]);

				// newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				// newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				// setXY(newx, newy, newx, newy);
				// LCD_Write_DATA(col>>8,col & 0xff);
			// }
		// SetBit(PORT_CS, B_CS);
	// }
	// clrXY();
// }

// void lcdOff(){
	// ClrBit(PORT_CS, B_CS);
	// SetBit(PORT_CS, B_CS);
// }

// void lcdOn(){
	// ClrBit(PORT_CS, B_CS);
	// SetBit(PORT_CS, B_CS);
// }

// void setContrast(char c){
	// ClrBit(PORT_CS, B_CS);
	// SetBit(PORT_CS, B_CS);
// }

// uint16_t getDisplayXSize(){
	// if (orient==PORTRAIT)
		// return disp_x_size+1;
	// else
		// return disp_y_size+1;
// }

// uint16_t getDisplayYSize(){
	// if (orient==PORTRAIT)
		// return disp_y_size+1;
	// else
		// return disp_x_size+1;
// }

// void setBrightness(uint8_t br){
	// ClrBit(PORT_CS, B_CS);
	// SetBit(PORT_CS, B_CS);
// }

// void setDisplayPage(uint8_t page){
	// ClrBit(PORT_CS, B_CS);
	// SetBit(PORT_CS, B_CS);
// }

// void setWritePage(uint8_t page){
	// ClrBit(PORT_CS, B_CS);
	// SetBit(PORT_CS, B_CS);
// }


// void LCD_Write_Bus_8(char VL){
    // cport(PORTC, 0xBF);
    // cport(PORTD, 0x60);
    // cport(PORTE, 0xBF);
    // PORTC += ((VL & 0x20)<<1);
    // PORTD += ((VL & 0x40)<<1) + (VL & 0x10) + ((VL & 0x08)>>3) + ((VL & 0x04)>>1) + ((VL & 0x03)<<2);
    // PORTE += ((VL & 0x80)>>1);
    // pulse_low(PORT_WR, B_WR);		
// }

// void _set_direction_registers(uint8_t mode){
	// switch (mode)
	// {
	// case 8:
	// case LATCHED_16:
		// DDRC |= 0x40;
		// DDRD |= 0x9F;
		// DDRE |= 0x40;
		// break;
	// case 16:
		// DDRB |= 0xF0;
		// DDRC |= 0xC0;
		// DDRD |= 0xDF;
		// DDRE |= 0x40;
		// DDRF |= 0xC0;
		// break;
	// }
// }

// void _fast_fill_16(uint16_t ch, uint16_t cl, uint32_t pix){
	// uint32_t blocks;

	// // cport(PORTB, 0x0F);
	// // cport(PORTC, 0x3F);
	// // cport(PORTD, 0x20);
	// // cport(PORTE, 0xBF);
	// // cport(PORTF, 0x3F);

	// // PORTB |= ((cl & 0x0F)<<4);
	// // PORTC |= ((cl & 0x20)<<2) + ((ch & 0x20)<<1);
	// // PORTD |= ((ch & 0x40)<<1) + (ch & 0x10) + ((ch & 0x08)>>3) + ((ch & 0x04)>>1) + ((ch & 0x03)<<2) + ((cl & 0x10)<<2);
	// // PORTE |= ((ch & 0x80)>>1);
	// // PORTF |= ((cl & 0x80)>>1) + ((cl & 0x40)<<1);

	// PORT_DATA = ch;
	// blocks = pix/16;
	// for (uint32_t i=0; i<blocks; i++)
	// {
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
		// pulse_low(PORT_WR, B_WR);
	// }
	// if ((pix % 16) != 0)
		// for (uint16_t i=0; i<(pix % 16)+1; i++)
		// {
			// pulse_low(PORT_WR, B_WR);
		// }
// }

// void _fast_fill_8(uint16_t ch, uint16_t pix){
	// // uint16_t blocks;

	// // cport(PORTC, 0xBF);
	// // cport(PORTD, 0x60);
	// // cport(PORTE, 0xBF);

	// // PORTC |= ((ch & 0x20)<<1);
	// // PORTD |= ((ch & 0x40)<<1) + (ch & 0x10) + ((ch & 0x08)>>3) + ((ch & 0x04)>>1) + ((ch & 0x03)<<2);
	// // PORTE |= ((ch & 0x80)>>1);

	// // blocks = pix/16;
	// // for (uint16_t i=0; i<blocks; i++)
	// // {
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
	// // }
	// // if ((pix % 16) != 0)
		// // for (uint16_t i=0; i<(pix % 16)+1; i++)
		// // {
			// // pulse_low(PORT_WR, B_WR);pulse_low(PORT_WR, B_WR);
		// // }
// }

