//определяем порт и бит к которому подключено устройство 1-wire
#include <util/delay.h>

#define W1_PORT PORTC
#define W1_DDR DDRC
#define W1_PIN PINC
#define W1_BIT 4

//функция определяет есть ли устройство на шине
unsigned char w1_find()
{
	unsigned char device;
	W1_DDR |= 1<<W1_BIT;//логический "0"
	_delay_us(485);//ждем минимум 480мкс
	W1_DDR &= ~(1<<W1_BIT);//отпускаем шину
	_delay_us(65);//ждем минимум 60мкс и смотрим что на шине
	
	if((W1_PIN & (1<<W1_BIT)) ==0x00)
		device = 1;
	else
		device = 0;
	_delay_us(420);//ждем оставшееся время до 480мкс
	return device;
}

//функция посылает команду на устройство 1-wire
void w1_sendcmd(unsigned char cmd)
{
   unsigned char i;
	for(i = 0; i < 8; i++)//в цикле посылаем побитно
	{
		if((cmd & (1<<i)) == 1<<i)//если бит=1 посылаем 1
		{
			W1_DDR |= 1<<W1_BIT;
			_delay_us(2);
			W1_DDR &= ~(1<<W1_BIT);			
			_delay_us(65);
		} 
		else//иначе посылаем 0
		{
			
			W1_DDR |= 1<<W1_BIT;
			_delay_us(65);
			W1_DDR &= ~(1<<W1_BIT);
			_delay_us(5);			
		}
	}
}

//функция читает один байт с устройства 1-wire
unsigned char w1_receive_byte()
{
	unsigned char data=0, i;
	for(i = 0; i < 8; i++)//в цикле смотрим что на шине и сохраняем значение
	{	
		W1_DDR |= 1<<W1_BIT;
		_delay_us(2);
		W1_DDR &= ~(1<<W1_BIT) ;
		_delay_us(7);
		if((W1_PIN & (1<<W1_BIT)) == 0x00)
			data &= ~(1<<i);
		else
			data |= 1<<i;
		_delay_us(50);//задержка до окончания тайм-слота
	}
	return data;
}