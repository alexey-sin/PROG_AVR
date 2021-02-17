//---���������� ������� Siemens �60,C60,�55,MC60,S55---
//           ������� �� LPH9116 (LPH8731)
//                  101�80 ��������
//��������� ����� ���� � � � - ����� ������� ���� �������
//          ==== ����� ����� ====
//                  v 1.1
//-----------------------------------------------------


#include "Symbols.h"  //���������� ���� � ��������
#include <util/delay.h>

#define _GEOMETRICAL  //������������� ������� ������ �������������� �����


//===============================================================
//                    ����������� ������
//===============================================================

#define GREEN       0x1C
#define DARK_GREEN  0x15
#define RED         0xE0
#define BLUE        0x1F
#define DARK_BLUE   0x03
#define YELLOW      0xFC
#define ORANGE      0xEC
#define VIOLET      0xE3
#define WHITE       0xFF
#define BLACK       0x00
#define GREY        0x6D

//*************************************************************
//���������� ������� �����
#define LCD_CS_0  PORTD &= ~(1 << 0) //����� ����
#define LCD_CS_1  PORTD |= (1 << 0)
#define LCD_RESET_0  PORTD &= ~(1 << 1) //�����
#define LCD_RESET_1	 PORTD |= (1 << 1)
#define LCD_RS_0  PORTD &= ~(1 << 4) //CD - ��� ������������ ������
#define LCD_RS_1  PORTD |= (1 << 4)
#define LCD_CLK_0  PORTD &= ~(1 << 5) //�������������
#define LCD_CLK_1  PORTD |= (1 << 5)
#define LCD_DATA_0  PORTD &= ~(1 << 6) //������
#define LCD_DATA_1 	PORTD |= (1 << 6)

//*************************************************************
//�������/������
#define CMD 	0
#define DAT 	1
char RS_old;

//*************************************************************
unsigned char com_row=0, com_column=0; //���������� ��� �������� ��������� "�������" (������ ������ � �������), ���������� ��� ��������������� ����������� ������ ��� ������ ��������


//*************************************************************
//���������
void reset();
void LCD_init();
void Send_to_lcd (unsigned char RS, unsigned char data);
void Send_Symbol_Shadow (unsigned char symbol, char x, char y, int t_color, char zoom_width, char zoom_height, int rot);
void LCD_Putchar_Shadow (char symbol, char x, char y, int t_color, char zoom_width, char zoom_height, int rot);
void LCD_Puts_Shadow (char *str, int x, int y, int t_color, char zoom_width, char zoom_height, int rot);
void LCD_FillScreen (unsigned int color);

#ifdef _GEOMETRICAL
void LCD_DrawLine (char x1, char y1, char x2, char y2, int color);
void LCD_DrawRect (char x1, char y1, char width, char height, char size, int color);
void LCD_FillRect (char x1, char y1, char width, char height, int color);
#endif

//===============================================================
//                           �����
//===============================================================
void reset ()
{
 LCD_CS_0;
 LCD_DATA_0;
 LCD_RESET_0;
 _delay_ms(100);
 LCD_RESET_1;
 _delay_ms(500);
 LCD_CS_1;
 _delay_us(5);
 LCD_CS_0;
}

//===============================================================
//                          �������������
//===============================================================
void LCD_init ()
{
 reset();
 Send_to_lcd(CMD,0x01); //reset sw
 _delay_ms(50);
 Send_to_lcd(CMD,0xc6); //initial escape
 _delay_ms(40);
 Send_to_lcd(CMD,0xb9); //Refresh set
 Send_to_lcd(DAT,0x00);
 Send_to_lcd(CMD,0xb6); //Display control
 Send_to_lcd(DAT,0x80); //
 Send_to_lcd(DAT,0x04); //
 Send_to_lcd(DAT,0x8a); //
 Send_to_lcd(DAT,0x54); //
 Send_to_lcd(DAT,0x45); //
 Send_to_lcd(DAT,0x52); //
 Send_to_lcd(DAT,0x43); //
 Send_to_lcd(CMD,0xb3); //Gray scale position set 0
 Send_to_lcd(DAT,0x02); //
 Send_to_lcd(DAT,0x0a); //
 Send_to_lcd(DAT,0x15); //
 Send_to_lcd(DAT,0x1f); //
 Send_to_lcd(DAT,0x28); //
 Send_to_lcd(DAT,0x30); //
 Send_to_lcd(DAT,0x37); //
 Send_to_lcd(DAT,0x3f); //
 Send_to_lcd(DAT,0x47); //
 Send_to_lcd(DAT,0x4c); //
 Send_to_lcd(DAT,0x54); //
 Send_to_lcd(DAT,0x65); //
 Send_to_lcd(DAT,0x75); //
 Send_to_lcd(DAT,0x80); //
 Send_to_lcd(DAT,0x85); //
 Send_to_lcd(CMD,0xb5); //Gamma curve
 Send_to_lcd(DAT,0x01); //
 Send_to_lcd(CMD,0xb7); //Temperature gradient
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(CMD,0xbd); //Common driver output select
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(CMD,0x36); //Memory access control - ����� ������� � ���������� ������ (��������� �������� ������ �������� � �������, ����������� ������ � ������ �������)
 Send_to_lcd(DAT,0x48); //6-� ���, ������������� � "1" ������ ��������� ������� - �����-�������, � 3-� ��� � "1" - ������� RGB (� � �������� - ������!!!)
 Send_to_lcd(CMD,0x2d); //Colour set
 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x03); //
 Send_to_lcd(DAT,0x05); //
 Send_to_lcd(DAT,0x07); //
 Send_to_lcd(DAT,0x09); //
 Send_to_lcd(DAT,0x0b); //
 Send_to_lcd(DAT,0x0d); //
 Send_to_lcd(DAT,0x0f); //

 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x03); //
 Send_to_lcd(DAT,0x05); //
 Send_to_lcd(DAT,0x07); //
 Send_to_lcd(DAT,0x09); //
 Send_to_lcd(DAT,0x0b); //
 Send_to_lcd(DAT,0x0d); //
 Send_to_lcd(DAT,0x0f); //

 Send_to_lcd(DAT,0x00); //
 Send_to_lcd(DAT,0x05); //
 Send_to_lcd(DAT,0x0b); //
 Send_to_lcd(DAT,0x0f); //
 Send_to_lcd(CMD,0xba); //Voltage control
 Send_to_lcd(DAT,0x2f); //
 Send_to_lcd(DAT,0x03); //
 Send_to_lcd(CMD,0x25); //��������� ���������
 Send_to_lcd(DAT,0x61); //
 Send_to_lcd(CMD,0xbe); //Power control
 Send_to_lcd(DAT,0x58); //

 Send_to_lcd(CMD,0x3a); //���������� � ������ �������
 Send_to_lcd(DAT,0x02); //8-�� ������ ����
 Send_to_lcd(CMD,0x03); //Booster voltage ON
 _delay_ms(40);
 Send_to_lcd(CMD,0x11); //����� �� ������� ������

 _delay_ms(20); 		//����� ���������� ���� 20 �����������
 Send_to_lcd(CMD,0x29); //��������� �������
}

//===============================================================
//������� ������ �������/������ � LCD (RS==0 - �������, RS==1 - ������)
//===============================================================
void Send_to_lcd (unsigned char RS, unsigned char data)
{
 //unsigned char count;
 LCD_CLK_0;
 LCD_DATA_0;
 if ((RS_old != RS) || (!RS_old && !RS)) //��������� ������ �������� RS (���� ��������� ���� ������� �� ������� CS)
 {
  LCD_CS_1;	// ��������� CS
  if (RS == 0){
	  LCD_RS_0;
  }else{
	  LCD_RS_1;
  }
  LCD_CS_0;	// ����� CS
 }
 //******************************************************************************
 //����� ���� ������������ ����� ���������� ��� ��� ������ ����� � �������
 //******************************************************************************
 unsigned char count;
 for (count = 0; count < 8; count++) //���� �������� ������
 {
  if(data&0x80)		LCD_DATA_1;
  else			LCD_DATA_0;
  LCD_CLK_1;
  data <<= 1;
  LCD_CLK_0;
 }
}

//===============================================================
//              ������� ������������� ������� ������
//===============================================================
void SetArea(char x1, char x2, char y1, char y2)
{
 Send_to_lcd( CMD, 0x2A );  //������ ������� �� X
 Send_to_lcd( DAT, x1 );    //���������
 Send_to_lcd( DAT, x2 );    //��������

 Send_to_lcd( CMD, 0x2B );  //������ ������� �� Y
 Send_to_lcd( DAT, y1+1 );  //��������� (� ����� ����������� Y ������������� �� 1, � �� �� 0)
 Send_to_lcd( DAT, y2+1 );  //��������

 Send_to_lcd( CMD, 0x2C );  //���������� ������� �� ������ ������ � ������ � �������� �������� ������
}

//===============================================================
//   			 ������ �����
//===============================================================
void Put_Pixel (char x, char y, unsigned int color)
{
 SetArea( x, x, y, y );
 Send_to_lcd( DAT, color ); //������ - ����� ���� �������
}

//===============================================================
//     ������� ���������� ������� �� ������� ��� ����� ����
//===============================================================
void Send_Symbol_Shadow (unsigned char symbol, char x, char y, int t_color, char zoom_width, char zoom_height, int rot)
{
 volatile unsigned char temp_symbol = 0, a = 0, b = 0, zw = 0, zh = 0, mask = 0;
 char m = 0, n = 0;
 m=x;
 n=y;
 if (symbol>127) symbol-=64;    //������� ������������� ����� ������� ASCII
 for ( a = 0; a < 5; a++) //��������� 5 ����, ������������ ������
 {
  temp_symbol = pgm_read_byte(&(font_5x8[symbol-32][a]));
  zw = 0;
  while(zw != zoom_width) //����� ����� ����������� zw ���
  {
   switch(rot)
   {
    case 0: case 180: n=y; break;
    case 90: case 270: m=x; break;
   }
   mask=0x01;
   for ( b = 0; b < 8; b++ ) //���� ����������� 8 ��� �����
   {
    zh = 0; //� zoom_height ��� ���������� ������ �������
    while(zh != zoom_height) //����� ������� ����������� z ���
    {
     switch(rot)
     {
      case 0: case 180:
      if (temp_symbol&mask)
      {
       Put_Pixel (m+zw, n+zh, t_color);
      }
      break;
      case 90: case 270:
      if (temp_symbol&mask)
      {
       Put_Pixel (m+zh, n+zw, t_color);
      }
      break; //�������� ����� ���������� ������� �� ��� y ��� ������ ���������� �����
     }
     zh++;
    }
    mask<<=1; //������ ���������� mask �� 1 ��� �����;
    switch(rot)
    {
     case 0: case 180: n=n+zoom_height; break;
     case 90: case 270: m=m+zoom_height; break;
    }
   }
   zw++;
  }
  switch(rot)
  {
   case 0: case 180: m=m+zoom_width; break;
   case 90: case 270: n=n+zoom_width; break;
  }
 }
}

//===============================================================
// ������� ������ ������ ������� ASCII-���� ��� ����� ����
//===============================================================
void LCD_Putchar_Shadow (char symbol, char x, char y, int t_color, char zoom_width, char zoom_height, int rot)
{
 unsigned char m;
 if(zoom_width == 0)   zoom_width = 1;
 if(zoom_height == 0)  zoom_height = 1;
 switch (rot)
 {
  case 0:  //��������� ����� ���� � � � - ����� ������� ���� �������
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  break;
  //================================
  case 90:
  m=y+3; y=x; x=m;
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x08); //��������� ����� ���� � � � - ������ ������� ���� �������
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  break;
  //================================
  case 180:
  x+=3; y+=2;
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x88); //��������� ����� ���� � � � - ������ ������ ���� �������
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  break;
  //================================
  case 270:
  m=y; y=x+2; x=m;
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0xC8); //��������� ����� ���� � � � - ����� ������ ���� �������
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  break;
  //================================
  default:
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48); //��������� ����� ���� � � � - ����� ������� ���� �������
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  //=================================
 };
}

//===============================================================
//   ������� ������ ������, ������������� � ram ��� ����� ����
//===============================================================
void LCD_Puts_Shadow(char *str, int x, int y,  int t_color, char zoom_width, char zoom_height, int rot)
{
 unsigned char i=0;

 if(zoom_width == 0)   zoom_width = 1;
 if(zoom_height == 0)  zoom_height = 1;

 while (str[i]) //x � y - ����� ������� ��������� �������; � ����������� ���������� i ����� ������ ���������� ������� ��������� �� i*6 (��� ������������ ������� �������)
 {
  LCD_Putchar_Shadow(str[i], x+(i*6*zoom_width), y, t_color, zoom_width, zoom_height, rot);
  i++;
 }
}

//===============================================================
//                  ������� ������ ������
//===============================================================
void LCD_FillScreen (unsigned int color)
{
 unsigned int x;
 SetArea( 0, 100, 0, 80 );   //������� ����� ������
 for (x = 0; x < 8080; x++)
 {
  Send_to_lcd( DAT, color ); //������ - ����� ���� �������
 }
}

#ifdef _GEOMETRICAL
//===============================================================
//                      ���������� �����
//===============================================================
void LCD_DrawLine (char x1, char y1, char x2, char y2, int color)
{
 short  x, y, d, dx, dy, i, i1, i2, kx, ky;
 signed char flag;

 dx = x2 - x1;
 dy = y2 - y1;
 if (dx == 0 && dy == 0) Put_Pixel(x1, y1, color);  //�����
 else      //�����
 {
  kx = 1;
  ky = 1;
  if( dx < 0 )
  {
   dx = -dx;
   kx = -1;
  }
  else
  if(dx == 0) kx = 0;
  if(dy < 0)
  {
   dy = -dy;
   ky = -1;
  }
  if(dx < dy)
  {
   flag = 0;
   d = dx;
   dx = dy;
   dy = d;
  }
  else flag = 1;
  i1 = dy + dy;
  d = i1 - dx;
  i2 = d - dx;
  x = x1;
  y = y1;

  for(i=0; i < dx; i++)
  {
   Put_Pixel(x, y, color);
   if(flag) x += kx;
   else y += ky;
   if( d < 0 ) d += i1;
   else
   {
    d += i2;
    if(flag) y += ky;
    else x += kx;
   }
  }
  Put_Pixel(x, y, color);
 }
}

//===============================================================
//			���������� �����
//===============================================================
void LCD_DrawRect (char x1, char y1, char width, char height, char size, int color)
{
 unsigned int i;
 char x2=x1+(width-1), y2=y1+(height-1); //�������� ������� ����� �� ���� � � �
 for( i=1; i<=size; i++)   // size - ������� �����
 {
  LCD_DrawLine(x1, y1, x1, y2, color);
  LCD_DrawLine(x2, y1, x2, y2, color);
  LCD_DrawLine(x1, y1, x2, y1, color);
  LCD_DrawLine(x1, y2, x2, y2, color);
  x1++; // ���������� ������� �����, ���� ��� ������
  y1++;
  x2--;
  y2--;
 }
}

//===============================================================
//              ��������� ������������� ������ COLOR
//===============================================================
// ����������! ��� 12-�� ������� ������ ��������� ������������ width * height ������ ���� ������ 2!
void LCD_FillRect (char x1, char y1, char width, char height, int color)
{
 unsigned int x, y;

 SetArea( x1, x1+(width-1), y1, y1+(height-1) );
 y = width * height;	        //���������� �������� � ��������������
 for (x = 0; x < y; x++)
 {
  Send_to_lcd( DAT, color ); Send_to_lcd( DAT, color ); //������ - ����� ���� �������
 }
}

//===============================================================
#endif
