//---Библиотека дисплея Siemens А60,C60,М55,MC60,S55---
//           Драйвер на LPH9116 (LPH8731)
//                  101х80 пикселей
//Начальный адрес осей Х и У - левый верхний угол дисплея
//          ==== Кизим Игорь ====
//                  v 1.1
//-----------------------------------------------------


#include "Symbols.h"  //Подключаем файл с шрифтами
#include <util/delay.h>

#define _GEOMETRICAL  //Использование функций вывода геометрических фигур


//===============================================================
//                    Определение цветов
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
//Назначение выводов порта
#define LCD_CS_0  PORTD &= ~(1 << 0) //Выбор чипа
#define LCD_CS_1  PORTD |= (1 << 0)
#define LCD_RESET_0  PORTD &= ~(1 << 1) //Сброс
#define LCD_RESET_1	 PORTD |= (1 << 1)
#define LCD_RS_0  PORTD &= ~(1 << 4) //CD - тип передаваемых данных
#define LCD_RS_1  PORTD |= (1 << 4)
#define LCD_CLK_0  PORTD &= ~(1 << 5) //Синхронизация
#define LCD_CLK_1  PORTD |= (1 << 5)
#define LCD_DATA_0  PORTD &= ~(1 << 6) //Данные
#define LCD_DATA_1 	PORTD |= (1 << 6)

//*************************************************************
//Команда/Данные
#define CMD 	0
#define DAT 	1
char RS_old;

//*************************************************************
unsigned char com_row=0, com_column=0; //Переменные для хранения положения "курсора" (номера строки и столбца), необходимы для автоматического определения адреса при выводе массивов


//*************************************************************
//ПРОТОТИПЫ
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
//                           СБРОС
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
//                          ИНИЦИАЛИЗАЦИЯ
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
 Send_to_lcd(CMD,0x36); //Memory access control - Метод доступа к дисплейной памяти (установка нулевого адреса страницы и столбца, направление записи в память дисплея)
 Send_to_lcd(DAT,0x48); //6-й бит, установленный в "1" делает адресацию столбца - слева-направо, а 3-й бит в "1" - порядок RGB (а в даташите - ошибка!!!)
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
 Send_to_lcd(CMD,0x25); //Установка контраста
 Send_to_lcd(DAT,0x61); //
 Send_to_lcd(CMD,0xbe); //Power control
 Send_to_lcd(DAT,0x58); //

 Send_to_lcd(CMD,0x3a); //Информация о выводе пикселя
 Send_to_lcd(DAT,0x02); //8-ми битный цвет
 Send_to_lcd(CMD,0x03); //Booster voltage ON
 _delay_ms(40);
 Send_to_lcd(CMD,0x11); //Выход из спящего режима

 _delay_ms(20); 		//Перед включением ждем 20 миллисекунд
 Send_to_lcd(CMD,0x29); //Включение дисплея
}

//===============================================================
//Функция записи команды/данных в LCD (RS==0 - команда, RS==1 - данные)
//===============================================================
void Send_to_lcd (unsigned char RS, unsigned char data)
{
 //unsigned char count;
 LCD_CLK_0;
 LCD_DATA_0;
 if ((RS_old != RS) || (!RS_old && !RS)) //проверяем старое значение RS (если поступают одни команды то дергаем CS)
 {
  LCD_CS_1;	// Установка CS
  if (RS == 0){
	  LCD_RS_0;
  }else{
	  LCD_RS_1;
  }
  LCD_CS_0;	// Сброс CS
 }
 //******************************************************************************
 //Такой цикл обеспечивает более компактный код при записи байта в дисплей
 //******************************************************************************
 unsigned char count;
 for (count = 0; count < 8; count++) //Цикл передачи данных
 {
  if(data&0x80)		LCD_DATA_1;
  else			LCD_DATA_0;
  LCD_CLK_1;
  data <<= 1;
  LCD_CLK_0;
 }
}

//===============================================================
//              Задание прямоугольной области экрана
//===============================================================
void SetArea(char x1, char x2, char y1, char y2)
{
 Send_to_lcd( CMD, 0x2A );  //задаем область по X
 Send_to_lcd( DAT, x1 );    //начальная
 Send_to_lcd( DAT, x2 );    //конечная

 Send_to_lcd( CMD, 0x2B );  //задаем область по Y
 Send_to_lcd( DAT, y1+1 );  //начальная (у этого контроллера Y отсчитывается от 1, а не от 0)
 Send_to_lcd( DAT, y2+1 );  //конечная

 Send_to_lcd( CMD, 0x2C );  //отправляем команду на начало записи в память и начинаем посылать данные
}

//===============================================================
//   			 Рисуем точку
//===============================================================
void Put_Pixel (char x, char y, unsigned int color)
{
 SetArea( x, x, y, y );
 Send_to_lcd( DAT, color ); //Данные - задаём цвет пикселя
}

//===============================================================
//     Функция прорисовки символа на дисплее без цвета фона
//===============================================================
void Send_Symbol_Shadow (unsigned char symbol, char x, char y, int t_color, char zoom_width, char zoom_height, int rot)
{
 volatile unsigned char temp_symbol = 0, a = 0, b = 0, zw = 0, zh = 0, mask = 0;
 char m = 0, n = 0;
 m=x;
 n=y;
 if (symbol>127) symbol-=64;    //Убираем отсутствующую часть таблицы ASCII
 for ( a = 0; a < 5; a++) //Перебираю 5 байт, составляющих символ
 {
  temp_symbol = pgm_read_byte(&(font_5x8[symbol-32][a]));
  zw = 0;
  while(zw != zoom_width) //Вывод байта выполняется zw раз
  {
   switch(rot)
   {
    case 0: case 180: n=y; break;
    case 90: case 270: m=x; break;
   }
   mask=0x01;
   for ( b = 0; b < 8; b++ ) //Цикл перебирания 8 бит байта
   {
    zh = 0; //в zoom_height раз увеличится высота символа
    while(zh != zoom_height) //Вывод пикселя выполняется z раз
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
      break; //Получить адрес начального пикселя по оси y для вывода очередного байта
     }
     zh++;
    }
    mask<<=1; //Смещаю содержимое mask на 1 бит влево;
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
// Функция вывода одного символа ASCII-кода без цвета фона
//===============================================================
void LCD_Putchar_Shadow (char symbol, char x, char y, int t_color, char zoom_width, char zoom_height, int rot)
{
 unsigned char m;
 if(zoom_width == 0)   zoom_width = 1;
 if(zoom_height == 0)  zoom_height = 1;
 switch (rot)
 {
  case 0:  //Начальный адрес осей Х и У - левый верхний угол дисплея
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  break;
  //================================
  case 90:
  m=y+3; y=x; x=m;
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x08); //Начальный адрес осей Х и У - правый верхний угол дисплея
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  break;
  //================================
  case 180:
  x+=3; y+=2;
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x88); //Начальный адрес осей Х и У - правый нижний угол дисплея
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  break;
  //================================
  case 270:
  m=y; y=x+2; x=m;
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0xC8); //Начальный адрес осей Х и У - левый нижний угол дисплея
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  break;
  //================================
  default:
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48); //Начальный адрес осей Х и У - левый верхний угол дисплея
  Send_Symbol_Shadow( symbol, x, y, t_color, zoom_width, zoom_height, rot);
  Send_to_lcd(CMD, 0x36);
  Send_to_lcd(DAT, 0x48);
  //=================================
 };
}

//===============================================================
//   Функция вывода строки, расположенной в ram без цвета фона
//===============================================================
void LCD_Puts_Shadow(char *str, int x, int y,  int t_color, char zoom_width, char zoom_height, int rot)
{
 unsigned char i=0;

 if(zoom_width == 0)   zoom_width = 1;
 if(zoom_height == 0)  zoom_height = 1;

 while (str[i]) //x и y - адрес пикселя начальной позиции; с увеличением переменной i адрес вывода очередного символа смещается на i*6 (чем организуются столбцы дисплея)
 {
  LCD_Putchar_Shadow(str[i], x+(i*6*zoom_width), y, t_color, zoom_width, zoom_height, rot);
  i++;
 }
}

//===============================================================
//                  ЗАЛИВКА ЭКРАНА ЦВЕТОМ
//===============================================================
void LCD_FillScreen (unsigned int color)
{
 unsigned int x;
 SetArea( 0, 100, 0, 80 );   //Область всего экрана
 for (x = 0; x < 8080; x++)
 {
  Send_to_lcd( DAT, color ); //Данные - задаём цвет пикселя
 }
}

#ifdef _GEOMETRICAL
//===============================================================
//                      НАРИСОВАТЬ ЛИНИЮ
//===============================================================
void LCD_DrawLine (char x1, char y1, char x2, char y2, int color)
{
 short  x, y, d, dx, dy, i, i1, i2, kx, ky;
 signed char flag;

 dx = x2 - x1;
 dy = y2 - y1;
 if (dx == 0 && dy == 0) Put_Pixel(x1, y1, color);  //Точка
 else      //Линия
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
//			НАРИСОВАТЬ РАМКУ
//===============================================================
void LCD_DrawRect (char x1, char y1, char width, char height, char size, int color)
{
 unsigned int i;
 char x2=x1+(width-1), y2=y1+(height-1); //Конечные размеры рамки по осям х и у
 for( i=1; i<=size; i++)   // size - толщина рамки
 {
  LCD_DrawLine(x1, y1, x1, y2, color);
  LCD_DrawLine(x2, y1, x2, y2, color);
  LCD_DrawLine(x1, y1, x2, y1, color);
  LCD_DrawLine(x1, y2, x2, y2, color);
  x1++; // Увеличиваю толщину рамки, если это задано
  y1++;
  x2--;
  y2--;
 }
}

//===============================================================
//              ЗАПОЛНИТЬ ПРЯМОУГОЛЬНИК ЦВЕТОМ COLOR
//===============================================================
// Примечание! Для 12-ти битного режима результат произведения width * height должен быть кратен 2!
void LCD_FillRect (char x1, char y1, char width, char height, int color)
{
 unsigned int x, y;

 SetArea( x1, x1+(width-1), y1, y1+(height-1) );
 y = width * height;	        //Количество пикселей в прямоугольнике
 for (x = 0; x < y; x++)
 {
  Send_to_lcd( DAT, color ); Send_to_lcd( DAT, color ); //Данные - задаём цвет пикселя
 }
}

//===============================================================
#endif
