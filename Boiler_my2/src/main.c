#include "main.h"
//===============================================================================
int main(void){  
	LoadValuesFromEEPROM();
	
    init();
	InitLCD();	//ориентацию экрана см. tft_ili9486_config.h X_MAX и Y_MAX

	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);
	// DrawRectFill(0, 319, 300, 424, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	
	DrawRectFill(0, 319, 300, 302, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(0, 319, 337, 339, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	
	DrawRectFill(0, 1, 303, 424, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(318, 319, 303, 424, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawString("Режим:   ", 15, 303);		//str, poz X, poz Y
	//кнопки меню
	DrawRectFill(0, 319, 425, 426, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(0, 319, 478, 479, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	
	DrawRectFill(0, 1, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(105, 106, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(210, 211, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(318, 319, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	
	DrawString("НАСТР", 12, 436);		//str, poz X, poz Y
	
	DS18B20_convertTemp(1);
	DS18B20_convertTemp(2);
	DS18B20_convertTemp(3);
	DS18B20_convertTemp(4);
	OutFullInfoToLCD();
	
	// _delay_ms(500);
	FlapTravel(FLAP_INIT, 0);	//начальное положение заслонки - закрытое

	// SetBit(PORT_BEEP, B_BEEP);
	// _delay_ms(100);
	// ClrBit(PORT_BEEP, B_BEEP);

// // __asm volatile ("nop");	
	
    while(1){
		CheckButton();
		if(BitIsSet(STATUS,ST_BTN)){
			ExecuteButton();
			ClrBit(STATUS,ST_BTN);
		}
		if(BitIsSet(STATUS,ST_BLC)){
			ReadMAX6675();
			DS18B20_getTemp(1);
			DS18B20_convertTemp(1);
			DS18B20_getTemp(2);
			DS18B20_convertTemp(2);
			DS18B20_getTemp(3);
			DS18B20_convertTemp(3);
			DS18B20_getTemp(4);
			DS18B20_convertTemp(4);
			if(BitIsClr(STATUS,ST_MENU)){
				OutVarInfoToLCD();
				Work();
			}else{
				CountMUL_MENU_WAIT--;
					uint16_t nx;
					SetFont(font_Tahoma10);
					SetColor(VGA_YELLOW);
					nx = DrawString("Выход из меню:  ", 70, 282);
					nx = OutNumber(CountMUL_MENU_WAIT, nx, 282);
					DrawString(" сек     ", nx, 282);
				if(CountMUL_MENU_WAIT == 0){	//выходим из меню
					ClrBit(STATUS,ST_MENU);
					BOILER_STATUS = BOILER_STATUS_IDLE;
					UpdateValuesFromEEPROM();
					OutFullInfoToLCD();
					OutVarInfoToLCD();
				}
			}
			ClrBit(STATUS,ST_BLC);
		}
   }
	
	
		// _delay_ms(10);
   return 0;
}
//===============================================================================
void init(void){
	SetBit(ACSR, ACD);//отключим аналоговый компаратор
	
	// Кнопки
	DDR_PLS &= ~(1 << B_PLS);
	PORT_PLS |= (1 << B_PLS);
	DDR_MIN &= ~(1 << B_MIN);
	PORT_MIN |= (1 << B_MIN);
	
	// м/с управления шаговым двигателем заслонки(FLAP) TLE4729G
	DDR_FLAP_FH |= (1 << B_FLAP_FH);
	PORT_FLAP_FH &= ~(1 << B_FLAP_FH);

	DDR_FLAP_FVR |= (1 << B_FLAP_FVR);
	PORT_FLAP_FVR &= ~(1 << B_FLAP_FVR);

	DDR_FLAP_REV |= (1 << B_FLAP_REV);
	PORT_FLAP_REV &= ~(1 << B_FLAP_REV);

	// Датчики DS18B20
	//Out	Уличный
	DDR_DS_OUT &= ~(1 << B_DS_OUT);
	PORT_DS_OUT &= ~(1 << B_DS_OUT);

	//Room	Комната
	DDR_DS_ROOM &= ~(1 << B_DS_ROOM);
	PORT_DS_ROOM &= ~(1 << B_DS_ROOM);

	//Serve (температура подачи)
	DDR_DS_SRV &= ~(1 << B_DS_SRV);
	PORT_DS_SRV &= ~(1 << B_DS_SRV);

	//Return (температура обратки)
	DDR_DS_RTN &= ~(1 << B_DS_RTN);
	PORT_DS_RTN &= ~(1 << B_DS_RTN);

	// Датчик дымовых газов - термопара мс MAX6675
	DDR_TC_SO &= ~(1 << B_TC_SO);
	PORT_TC_SO &= ~(1 << B_TC_SO);

	DDR_TC_CS |= (1 << B_TC_CS);
	PORT_TC_CS |= (1 << B_TC_CS);

	DDR_TC_SCK |= (1 << B_TC_SCK);
	PORT_TC_SCK &= ~(1 << B_TC_SCK);

	//Beep Зуммер активный
	DDR_BEEP |= (1 << B_BEEP);
	PORT_BEEP &= ~(1 << B_BEEP);
	
	//Preheating реле включения ТЭНов подогрева
	DDR_PHEAT |= (1 << B_PHEAT);
	PORT_PHEAT &= ~(1 << B_PHEAT);
	
	// // Настройка UART на 38400 bps(бод)
	// UBRRH = 0;
	// UBRRL = 26;	//UBRR = 16000000/(16 * 38400) - 1 = 26,04 //при U2X = 0
	// UCSRB = (1 << TXEN);	// вкл приемник(RXEN=1),вкл передатчик(TXEN=1),вкл прерывание по приему(RXCIE=1),вкл прерывание по передаче(TXCIE=1)
	// // Для доступа к регистру UCSRC ОБЯЗАТЕЛЬНО выставить бит URSEL(кроме ATTiny2313, там его нет)!!!
	// UCSRC = (1 << URSEL)|(3 << UCSZ0);	// асинхронный режим(UMSEL=0), 8 бит(UCSZ2-0=011), 1 стоп-бит(USBS=0),без контроля четности(UPM1-0=00)

    //1(16бит)таймер - основной тик ~1сек
    SFIOR |= (1 << PSR10 | 1 << PSR2);   // сброс прескалера таймеров 1 и 0
    TIMSK |= (1 << OCIE1A);   // разрешить прерывание по совпадению А 1(16разр) счетчика
    
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS12);// Делитель на 256  1/16000000 = 62,5нс * 256 = 16мкс
	//0xF424 это 62500 * 0,000016 = 1,0 сек
	OCR1AH = ((0xF424 & 0xFF00) >> 8);
	OCR1AL = (0xF424 & 0x00FF);
	
	//2(8бит)таймер - блокировка клавиатуры (снятие)
	TIMSK |= (1 << TOIE2);   // разрешить прерывание по переполнению 2(8разр) счетчика
	// запускать будем потом
	// TCNT2 = 0;
	// TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (8М)0,125мкс*256*1024 = 32мс (переполнение счетчика)
	// TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (16М)0,0625мкс*256*1024 = 16.4мс (переполнение счетчика)
	
	sei();
}
ISR(TIMER1_COMPA_vect){
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	SetBit(STATUS, ST_BLC);
}
ISR(TIMER2_OVF_vect){
	CountMUL_BTN_WAIT--;
	if(CountMUL_BTN_WAIT == 0){
		TCCR2 = 0;	// таймер 2 стоп
		ClrBit(STATUS, ST_BTN_WAIT);
	}
}
void CheckButton(void){
	if(BitIsSet(STATUS,ST_BTN_WAIT)) return;

	if(BitIsClr(PIN_PLS, B_PLS) && BitIsClr(PIN_MIN, B_MIN)){
		SetBit(BUTTON, MMM);
		SetBit(STATUS, ST_BTN);
	}else if(BitIsClr(PIN_PLS, B_PLS)){
		SetBit(BUTTON, PLS);
		SetBit(STATUS, ST_BTN);
	}else if(BitIsClr(PIN_MIN, B_MIN)){
		SetBit(BUTTON, MIN);
		SetBit(STATUS, ST_BTN);
	}
	
	CountMUL_BTN_WAIT = MUL_BTN_WAIT;
	TCNT2 = 0;
	TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (16М)0,0625мкс*256*1024 = 16.4мс (переполнение счетчика)
	SetBit(STATUS, ST_BTN_WAIT);
}
void ExecuteButton(void){
	if(BitIsSet(STATUS,ST_MENU)) CountMUL_MENU_WAIT = MUL_MENU_WAIT;
	
	if(BitIsSet(BUTTON,MMM)){
		if(BitIsClr(STATUS,ST_MENU)){	//запуск состояния меню
			MENU = 0;
			CountMUL_MENU_WAIT = MUL_MENU_WAIT;
			
			OutMenuToLCD();
			SetBit(STATUS,ST_MENU);
		}else{
			switch(MENU){
				case 0: if(SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_AUTO) MENU = 3; break;
				case 2: if(SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_MAN) MENU = 13; break;
				case 13: if(SETVAL_LIGHT_DISPLAY == DEF_SETVAL_LIGHT_ALL_ON) MENU = 15; break;
				case 15: if(SETVAL_ALARM_FUEL_ON == DEF_SETVAL_ALARM_FUEL_OFF) MENU = 17; break;
				case 17:{
					if(SETVAL_TEN_MODE == DEF_SETVAL_TEN_MODE_OFF) MENU = 22;
					if(SETVAL_TEN_MODE == DEF_SETVAL_TEN_MODE_HEAT_ROOM) MENU = 20;
				} break;
				case 19: MENU = 22; break;

				default: MENU++;
			}
			if(MENU == 27) MENU = 0;
			OutMenuToLCD();
		}
		ClrBit(BUTTON,MMM);
	}
	if(BitIsSet(BUTTON,PLS)){
		
		if(BitIsSet(STATUS,ST_MENU)){	//состояние меню
			switch(MENU){
				case 0:{	// Автоматическая регулировка / Ручная	
					SETVAL_AUTO_CONTROL = DEF_SETVAL_AUTO_CONTROL_AUTO;
				} break;
				case 1:{	// Включение/Выключение ТЭНов в ручном режиме
					SetBit(PORT_PHEAT,B_PHEAT);	//Включить подогрев
				} break;
				case 2:{	// Количество шагов двигателя заслонки в ручном режиме
					if(SETVAL_FLAP_MANUAL_STEP < DEF_SETVAL_FLAP_MANUAL_STEP_MAX)
						SETVAL_FLAP_MANUAL_STEP += DEF_SETVAL_FLAP_MANUAL_STEP_STEP;
				} break;
				case 3:{	// Зона нечувствительности датчика дымохода в каждую сторону
					if(SETVAL_SMOKE_DEAD_BAND < DEF_SETVAL_SMOKE_DEAD_BAND_MAX)
						SETVAL_SMOKE_DEAD_BAND += DEF_SETVAL_SMOKE_DEAD_BAND_STEP;
				} break;
				case 4:{	// Количество шагов двигателя заслонки в автоматическом режиме
					if(SETVAL_FLAP_AUTO_STEP < DEF_SETVAL_FLAP_AUTO_STEP_MAX)
						SETVAL_FLAP_AUTO_STEP += DEF_SETVAL_FLAP_AUTO_STEP_STEP;
				} break;
				case 5:{	// Стартовое положение заслонки в %
					if(SETVAL_FLAP_START_STEPS < DEF_SETVAL_FLAP_START_STEPS_MAX)
						SETVAL_FLAP_START_STEPS += DEF_SETVAL_FLAP_START_STEPS_STEP;
				} break;
				case 6:{	// температура газов дымохода
					if(SETVAL_TEMP_SMOKE < DEF_SETVAL_TEMP_SMOKE_MAX)
						SETVAL_TEMP_SMOKE += DEF_SETVAL_TEMP_SMOKE_STEP;
				} break;
				case 7:{	// максимальная температура подачи
					if(SETVAL_MAX_TEMP_SERVE < DEF_SETVAL_MAX_TEMP_SERVE_MAX)
						SETVAL_MAX_TEMP_SERVE += DEF_SETVAL_MAX_TEMP_SERVE_STEP;
				} break;
				case 8:{	// Гистерезис температуры подачи
					if(SETVAL_HYSTER_TEMP_SERVE < DEF_SETVAL_HYSTER_TEMP_SERVE_MAX)
						SETVAL_HYSTER_TEMP_SERVE += DEF_SETVAL_HYSTER_TEMP_SERVE_STEP;
				} break;
				case 9:{	// максимальная температура комнаты
					if(SETVAL_MAX_TEMP_ROOM < DEF_SETVAL_MAX_TEMP_ROOM_MAX)
						SETVAL_MAX_TEMP_ROOM += DEF_SETVAL_MAX_TEMP_ROOM_STEP;
				} break;
				case 10:{	// Гистерезис температуры комнаты
					if(SETVAL_HYSTER_TEMP_ROOM < DEF_SETVAL_HYSTER_TEMP_ROOM_MAX)
						SETVAL_HYSTER_TEMP_ROOM += DEF_SETVAL_HYSTER_TEMP_ROOM_STEP;
				} break;
				case 11:{	// Температура дымохода отключения управления котла
					if(SETVAL_TEMP_BOILER_OFF < DEF_SETVAL_TEMP_BOILER_OFF_MAX)
						SETVAL_TEMP_BOILER_OFF += DEF_SETVAL_TEMP_BOILER_OFF_STEP;
				} break;
				case 12:{	// Гистерезис температуры дымохода отключения управления котла
					if(SETVAL_HYSTER_TEMP_BOILER_OFF < DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MAX)
						SETVAL_HYSTER_TEMP_BOILER_OFF += DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_STEP;
				} break;
				case 13:{	// Подсветка индикатора
					SETVAL_LIGHT_DISPLAY = DEF_SETVAL_LIGHT_ALL_ON;
				} break;
				case 14:{	// Время работы подсветки индикатора а автоматич. режиме
					if(SETVAL_TIME_LIGHT < DEF_SETVAL_TIME_LIGHT_MAX)
						SETVAL_TIME_LIGHT += DEF_SETVAL_TIME_LIGHT_STEP;
				} break;
				case 15:{	// Сигнализация прогорания топлива
					SETVAL_ALARM_FUEL_ON = DEF_SETVAL_ALARM_FUEL_ON;
				} break;
				case 16:{	// Температура сигнализации прогорания топлива
					if(SETVAL_TEMP_ALARM_FUEL < DEF_SETVAL_TEMP_ALARM_FUEL_MAX)
						SETVAL_TEMP_ALARM_FUEL += DEF_SETVAL_TEMP_ALARM_FUEL_STEP;
				} break;
				case 17:{	// Режим работы ТЕНов
					if(SETVAL_TEN_MODE < DEF_SETVAL_TEN_MODE_HEAT_ROOM) SETVAL_TEN_MODE++; else SETVAL_TEN_MODE = DEF_SETVAL_TEN_MODE_OFF;
				} break;
				case 18:{	// температура включения ТЕНов аварийный подогрев
					if(SETVAL_TEMP_TEN_NOTFROZE_ON < DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MAX && ((SETVAL_TEMP_TEN_NOTFROZE_ON + 2) < SETVAL_TEMP_TEN_NOTFROZE_OFF))
						SETVAL_TEMP_TEN_NOTFROZE_ON += DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_STEP;
				} break;
				case 19:{	// температура выключения ТЕНов аварийный подогрев
					if(SETVAL_TEMP_TEN_NOTFROZE_OFF < DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MAX)
						SETVAL_TEMP_TEN_NOTFROZE_OFF += DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_STEP;
				} break;
				case 20:{	// температура включения ТЕНов по нагреву помещения
					if(SETVAL_TEMP_TEN_HEAT_ROOM_ON < DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MAX && ((SETVAL_TEMP_TEN_HEAT_ROOM_ON + 2) < SETVAL_TEMP_TEN_HEAT_ROOM_OFF))
						SETVAL_TEMP_TEN_HEAT_ROOM_ON += DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_STEP;
				} break;
				case 21:{	// температура выключения ТЕНов по нагреву помещения
					if(SETVAL_TEMP_TEN_HEAT_ROOM_OFF < DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MAX)
						SETVAL_TEMP_TEN_HEAT_ROOM_OFF += DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_STEP;
				} break;
				case 22:{	// Мощность ТЕНов (кВт)
					if(SETVAL_TEN_POWER < DEF_SETVAL_TEN_POWER_MAX)
						SETVAL_TEN_POWER += DEF_SETVAL_TEN_POWER_STEP;
				} break;
				case 23:{	// Задержка в регулировании в режиме рауза
					if(SETVAL_TIME_DELAY_PAUSE < DEF_SETVAL_TIME_DELAY_PAUSE_MAX)
						SETVAL_TIME_DELAY_PAUSE += DEF_SETVAL_TIME_DELAY_PAUSE_STEP;
				} break;
				case 24:{	// Производительность насоса циркуляции (л/мин)
					if(SETVAL_PUMP_PERFORMANCE < DEF_SETVAL_PUMP_PERFORMANCE_MAX)
						SETVAL_PUMP_PERFORMANCE += DEF_SETVAL_PUMP_PERFORMANCE_STEP;
				} break;
				case 25:{	// Сброс счетчика тепловой энергии
				} break;
				case 26:{	// Сброс счетчика электрической энергии
				} break;
			}
			OutMenuToLCDValue();
		}else if(BOILER_STATUS == BOILER_STATUS_MANUAL){
			FlapTravel(FLAP_FORVARD, SETVAL_FLAP_MANUAL_STEP);
		}else if(BOILER_STATUS == BOILER_STATUS_AUTO){
			SetBit(STATUS,ST_WORK_PAUSE_HIGH);
		// }else if(BOILER_STATUS == BOILER_STATUS_PAUSE){
			// SetBit(STATUS,ST_WORK_PAUSE_STOP);
		}
		ClrBit(BUTTON,PLS);
	}
	if(BitIsSet(BUTTON,MIN)){
		
		if(BitIsSet(STATUS,ST_MENU)){	//состояние меню
			switch(MENU){
				case 0:{	// Автоматическая регулировка / Ручная
					SETVAL_AUTO_CONTROL = DEF_SETVAL_AUTO_CONTROL_MAN;
				} break;
				case 1:{	// Включение/Выключение ТЭНов в ручном режиме
					ClrBit(PORT_PHEAT,B_PHEAT);	//Выключить подогрев
				} break;
				case 2:{	// Количество шагов двигателя заслонки в ручном режиме
					if(SETVAL_FLAP_MANUAL_STEP > DEF_SETVAL_FLAP_MANUAL_STEP_MIN)
						SETVAL_FLAP_MANUAL_STEP -= DEF_SETVAL_FLAP_MANUAL_STEP_STEP;
					else SETVAL_FLAP_MANUAL_STEP = DEF_SETVAL_FLAP_MANUAL_STEP_MIN;
				} break;
				case 3:{	// Зона нечувствительности датчика дымохода в каждую сторону
					if(SETVAL_SMOKE_DEAD_BAND > DEF_SETVAL_SMOKE_DEAD_BAND_MIN)
						SETVAL_SMOKE_DEAD_BAND -= DEF_SETVAL_SMOKE_DEAD_BAND_STEP;
					else SETVAL_SMOKE_DEAD_BAND = DEF_SETVAL_SMOKE_DEAD_BAND_MIN;
				} break;
				case 4:{	// Количество шагов двигателя заслонки в автоматическом режиме
					if(SETVAL_FLAP_AUTO_STEP > DEF_SETVAL_FLAP_AUTO_STEP_MIN)
						SETVAL_FLAP_AUTO_STEP -= DEF_SETVAL_FLAP_AUTO_STEP_STEP;
					else SETVAL_FLAP_AUTO_STEP = DEF_SETVAL_FLAP_AUTO_STEP_MIN;
				} break;
				case 5:{	// Стартовое положение заслонки в %
					if(SETVAL_FLAP_START_STEPS > DEF_SETVAL_FLAP_START_STEPS_MIN)
						SETVAL_FLAP_START_STEPS -= DEF_SETVAL_FLAP_START_STEPS_STEP;
					else SETVAL_FLAP_START_STEPS = DEF_SETVAL_FLAP_START_STEPS_MIN;
				} break;
				case 6:{	// температура газов дымохода
					if(SETVAL_TEMP_SMOKE > DEF_SETVAL_TEMP_SMOKE_MIN)
						SETVAL_TEMP_SMOKE -= DEF_SETVAL_TEMP_SMOKE_STEP;
					else SETVAL_TEMP_SMOKE = DEF_SETVAL_TEMP_SMOKE_MIN;
				} break;
				case 7:{	// максимальная температура подачи
					if(SETVAL_MAX_TEMP_SERVE > DEF_SETVAL_MAX_TEMP_SERVE_MIN)
						SETVAL_MAX_TEMP_SERVE -= DEF_SETVAL_MAX_TEMP_SERVE_STEP;
					else SETVAL_MAX_TEMP_SERVE = DEF_SETVAL_MAX_TEMP_SERVE_MIN;
				} break;
				case 8:{	// Гистерезис температуры подачи
					if(SETVAL_HYSTER_TEMP_SERVE > DEF_SETVAL_HYSTER_TEMP_SERVE_MIN)
						SETVAL_HYSTER_TEMP_SERVE -= DEF_SETVAL_HYSTER_TEMP_SERVE_STEP;
					else SETVAL_HYSTER_TEMP_SERVE = DEF_SETVAL_HYSTER_TEMP_SERVE_MIN;
				} break;
				case 9:{	// максимальная температура комнаты
					if(SETVAL_MAX_TEMP_ROOM > DEF_SETVAL_MAX_TEMP_ROOM_MIN)
						SETVAL_MAX_TEMP_ROOM -= DEF_SETVAL_MAX_TEMP_ROOM_STEP;
					else SETVAL_MAX_TEMP_ROOM = DEF_SETVAL_MAX_TEMP_ROOM_MIN;
				} break;
				case 10:{	// Гистерезис температуры комнаты
					if(SETVAL_HYSTER_TEMP_ROOM > DEF_SETVAL_HYSTER_TEMP_ROOM_MIN)
						SETVAL_HYSTER_TEMP_ROOM -= DEF_SETVAL_HYSTER_TEMP_ROOM_STEP;
					else SETVAL_HYSTER_TEMP_ROOM = DEF_SETVAL_HYSTER_TEMP_ROOM_MIN;
				} break;
				case 11:{	// Температура дымохода отключения управления котла
					if(SETVAL_TEMP_BOILER_OFF > DEF_SETVAL_TEMP_BOILER_OFF_MIN)
						SETVAL_TEMP_BOILER_OFF -= DEF_SETVAL_TEMP_BOILER_OFF_STEP;
					else SETVAL_TEMP_BOILER_OFF = DEF_SETVAL_TEMP_BOILER_OFF_MIN;
				} break;
				case 12:{	// Гистерезис температуры дымохода отключения управления котла
					if(SETVAL_HYSTER_TEMP_BOILER_OFF > DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MIN)
						SETVAL_HYSTER_TEMP_BOILER_OFF -= DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_STEP;
					else SETVAL_HYSTER_TEMP_BOILER_OFF = DEF_SETVAL_HYSTER_TEMP_BOILER_OFF_MIN;
				} break;
				case 13:{	// Подсветка индикатора
					SETVAL_LIGHT_DISPLAY = DEF_SETVAL_LIGHT_AUTO;
				} break;
				case 14:{	// Время работы подсветки индикатора а автоматич. режиме
					if(SETVAL_TIME_LIGHT > DEF_SETVAL_TIME_LIGHT_MIN)
						SETVAL_TIME_LIGHT -= DEF_SETVAL_TIME_LIGHT_STEP;
					else SETVAL_TIME_LIGHT = DEF_SETVAL_TIME_LIGHT_MIN;
				} break;
				case 15:{	// Сигнализация прогорания топлива
					SETVAL_ALARM_FUEL_ON = DEF_SETVAL_ALARM_FUEL_OFF;
				} break;
				case 16:{	// Температура сигнализации прогорания топлива
					if(SETVAL_TEMP_ALARM_FUEL > DEF_SETVAL_TEMP_ALARM_FUEL_MIN)
						SETVAL_TEMP_ALARM_FUEL -= DEF_SETVAL_TEMP_ALARM_FUEL_STEP;
					else SETVAL_TEMP_ALARM_FUEL = DEF_SETVAL_TEMP_ALARM_FUEL_MIN;
				} break;
				case 17:{	// Режим работы ТЕНов
					if(SETVAL_TEN_MODE > DEF_SETVAL_TEN_MODE_OFF) SETVAL_TEN_MODE--;
					else SETVAL_TEN_MODE = DEF_SETVAL_TEN_MODE_HEAT_ROOM;
				} break;
				case 18:{	// температура включения ТЕНов аварийный подогрев
					if(SETVAL_TEMP_TEN_NOTFROZE_ON > DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MIN)
						SETVAL_TEMP_TEN_NOTFROZE_ON -= DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_STEP;
					else SETVAL_TEMP_TEN_NOTFROZE_ON = DEF_SETVAL_TEMP_TEN_NOTFROZE_ON_MIN;
				} break;
				case 19:{	// температура выключения ТЕНов аварийный подогрев
					if(SETVAL_TEMP_TEN_NOTFROZE_OFF > DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_MIN && ((SETVAL_TEMP_TEN_NOTFROZE_ON + 2) < SETVAL_TEMP_TEN_NOTFROZE_OFF))
						SETVAL_TEMP_TEN_NOTFROZE_OFF -= DEF_SETVAL_TEMP_TEN_NOTFROZE_OFF_STEP;
				} break;
				case 20:{	// температура включения ТЕНов по нагреву помещения
					if(SETVAL_TEMP_TEN_HEAT_ROOM_ON > DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MIN)
						SETVAL_TEMP_TEN_HEAT_ROOM_ON -= DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_STEP;
					else SETVAL_TEMP_TEN_HEAT_ROOM_ON = DEF_SETVAL_TEMP_TEN_HEAT_ROOM_ON_MIN;
				} break;
				case 21:{	// температура выключения ТЕНов по нагреву помещения
					if(SETVAL_TEMP_TEN_HEAT_ROOM_OFF > DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_MIN && ((SETVAL_TEMP_TEN_HEAT_ROOM_ON + 2) < SETVAL_TEMP_TEN_HEAT_ROOM_OFF))
						SETVAL_TEMP_TEN_HEAT_ROOM_OFF -= DEF_SETVAL_TEMP_TEN_HEAT_ROOM_OFF_STEP;
				} break;
				case 22:{	// Мощность ТЕНов (кВт)
					if(SETVAL_TEN_POWER > DEF_SETVAL_TEN_POWER_MIN)
						SETVAL_TEN_POWER -= DEF_SETVAL_TEN_POWER_STEP;
					else SETVAL_TEN_POWER = DEF_SETVAL_TEN_POWER_MIN;
				} break;
				case 23:{	// Задержка в регулировании в режиме рауза
					if(SETVAL_TIME_DELAY_PAUSE > DEF_SETVAL_TIME_DELAY_PAUSE_MIN)
						SETVAL_TIME_DELAY_PAUSE -= DEF_SETVAL_TIME_DELAY_PAUSE_STEP;
					else SETVAL_TIME_DELAY_PAUSE = DEF_SETVAL_TIME_DELAY_PAUSE_MIN;
				} break;
				case 24:{	// Производительность насоса циркуляции (л/мин)
					if(SETVAL_PUMP_PERFORMANCE > DEF_SETVAL_PUMP_PERFORMANCE_MIN)
						SETVAL_PUMP_PERFORMANCE -= DEF_SETVAL_PUMP_PERFORMANCE_STEP;
					else SETVAL_PUMP_PERFORMANCE = DEF_SETVAL_PUMP_PERFORMANCE_MIN;
				} break;
				case 25:{	// Сброс счетчика тепловой энергии
				} break;
				case 26:{	// Сброс счетчика электрической энергии
				} break;
			}
			OutMenuToLCDValue();
		}else if(BOILER_STATUS == BOILER_STATUS_MANUAL){
			FlapTravel(FLAP_REVERCE, SETVAL_FLAP_MANUAL_STEP);
		}else if(BOILER_STATUS == BOILER_STATUS_AUTO){
			SetBit(STATUS,ST_WORK_PAUSE_LOW);
		}else if(BOILER_STATUS == BOILER_STATUS_PAUSE){
			SetBit(STATUS,ST_WORK_PAUSE_STOP);
		}
		ClrBit(BUTTON,MIN);
	}
}
void FlapTravel(uint8_t direct, uint16_t steps){	//direct = 0 : Reverce; direct = 1 : Forvard
	uint8_t dir = FLAP_REVERCE;
	
	if(direct == FLAP_INIT){
		dir = FLAP_REVERCE;
		steps = FLAP_STEP_MAX + 10;
		FLAP_STEPS = 0;
	}else if(direct == FLAP_START){
		dir = FLAP_REVERCE;
		steps = FLAP_STEPS + 10;
		FLAP_STEPS = 0;
	}else if(direct == FLAP_FORVARD){
		if((FLAP_STEPS + steps) > FLAP_STEP_MAX) steps = FLAP_STEP_MAX - FLAP_STEPS;
		FLAP_STEPS += steps;
		dir = FLAP_FORVARD;
	}else if(direct == FLAP_REVERCE){
		if((FLAP_STEPS - (int16_t)steps) < 0){
			steps = FLAP_STEPS;
			FLAP_STEPS = 0;
		}else FLAP_STEPS -= steps;
		dir = FLAP_REVERCE;
	}

	SetBit(PORT_FLAP_FH,B_FLAP_FH);	//Normal mode
	if(dir == FLAP_FORVARD){
		while(steps--){
			switch(FLAP_STATE){
				case 0:{
					SetBit(PORT_FLAP_FVR,B_FLAP_FVR);
					_delay_ms(time_flap);
					FLAP_STATE = 2;
				}break;
				case 1:{
					ClrBit(PORT_FLAP_REV,B_FLAP_REV);
					_delay_ms(time_flap);
					FLAP_STATE = 0;
				}break;
				case 2:{
					SetBit(PORT_FLAP_REV,B_FLAP_REV);
					_delay_ms(time_flap);
					FLAP_STATE = 3;
				}break;
				case 3:{
					ClrBit(PORT_FLAP_FVR,B_FLAP_FVR);
					_delay_ms(time_flap);
					FLAP_STATE = 1;
				}break;
			}
		}
	}else if(dir == FLAP_REVERCE){
		while(steps--){
			switch(FLAP_STATE){
				case 0:{
					SetBit(PORT_FLAP_REV,B_FLAP_REV);
					_delay_ms(time_flap);
					FLAP_STATE = 1;
				}break;
				case 1:{
					SetBit(PORT_FLAP_FVR,B_FLAP_FVR);
					_delay_ms(time_flap);
					FLAP_STATE = 3;
				}break;
				case 2:{
					ClrBit(PORT_FLAP_FVR,B_FLAP_FVR);
					_delay_ms(time_flap);
					FLAP_STATE = 0;
				}break;
				case 3:{
					ClrBit(PORT_FLAP_REV,B_FLAP_REV);
					_delay_ms(time_flap);
					FLAP_STATE = 2;
				}break;
			}
		}
	}
	ClrBit(PORT_FLAP_FH,B_FLAP_FH);	//No current
}
void OutFullInfoToLCD(void){	//вывод оперативной инфомации на дисплей (рисование полного экрана)
	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);
	//очистка рабочей области экрана
	DrawRectFill(0, 319, 0, 299, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	
	DrawString("Улица: ", 5, 5);		//str, poz X, poz Y
	DrawString("Комната: ", 5, 37);	//str, poz X, poz Y
	
	DrawString("Подача: ", 5, 101);	//str, poz X, poz Y
	DrawString("Обратка: ", 5, 133);	//str, poz X, poz Y
	
	DrawString("Дымоход: ", 5, 197);	//str, poz X, poz Y
	
	DrawString("Заслонка тяги: ", 5, 261);		//str, poz X, poz Y
	
}
void OutVarInfoToLCD(void){	//вывод оперативной инфомации на дисплей (только переменные)

	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);

	OutTemperatureDS(1, 200, 5);
	OutTemperatureDS(2, 200, 37);
	
	OutTemperatureDS(3, 200, 101);
	OutTemperatureDS(4, 200, 133);
	
	OutTemperatureTC(200, 197);
	
	OutPercent(FLAP_STEPS, FLAP_STEP_MAX, 200, 261);

}
void OutTemperatureDS(uint8_t dat, uint16_t x, uint16_t y){
	uint8_t d, fnam = 0, min = 0, ival, ival_t, status = 0;
	char buf[10];
	uint8_t ib = 0;
	double fval = 0.0;
	uint16_t nx;

	switch(dat){
		case 1:{
			status = STATUS_DS_OUT;
			fval = dbDS_OUT;
		} break;
		case 2:{
			status = STATUS_DS_ROOM;
			fval = dbDS_ROOM;
		} break;
		case 3:{
			status = STATUS_DS_SERVE;
			fval = dbDS_SERVE;
		} break;
		case 4:{
			status = STATUS_DS_RETURN;
			fval = dbDS_RETURN;
		} break;
		default: break;
	}
	if(fval < 0){
		min = 1;
		fval *= -1;
	}
	ival = ival_t = (int8_t)fval;
	
	if(status == 1){
		if(min){buf[ib] = '-'; ib++;}

		d = ival / 100;
		if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
		ival = ival % 100;
		
		d = ival / 10;
		if(d != 0){buf[ib] = (d + 0x30); ib++;}
		else if(fnam == 1){buf[ib] = 0x30; ib++;}
		d = ival % 10;
		
		buf[ib] = (d + 0x30); ib++;
		buf[ib] = '.'; ib++;
		
		fval = (fval - ival_t) * 10.0;
		d = (int8_t)fval;
		buf[ib] = (d + 0x30); ib++;
		
		buf[ib] = '`'; ib++;
		buf[ib] = 'C'; ib++;
	}else{
		while(LabelBRK[ib] != 0){
			buf[ib] = LabelBRK[ib];
			ib++;
		}
	}
	buf[ib] = 0;
	nx = DrawString(buf, x, y);
	DrawRectFill(nx, 319, y, y + 32, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
}
void ReadMAX6675(void){	//данные сохраняются в VALUE_TC и STATUS_TC
/**
PC7 SCL	выдаем изначально без подтяжки
PC4 SO	читаем без подтяжки
PC6 CE	1 датчик	выдаем изначально с подтяжкой
PC5 CE	2 датчик	выдаем изначально с подтяжкой

Чтение данных из АЦП MAX6675:
Опускаем CE в 0, выдаем клоки на SCL 16 бит и в когда SCL = 1 читаем биты с вывода MISO.
Байты идут старшим байтом и старшим битом вперед
В конце CE поднимаем  1, SCL должен остаться в 0.

0 бит - 3-е состояние - не учитыаем
1 бит - всегда 0 - признак MAX6675
2 бит 0 если все хорошо, 1 если термопара в обрыве
3-14 биты (12 штук) значение температуры, 2 младших разряда - дробная часть (х0,25)
	все нули = 0 гр. по цельсию
	все единицы - +1023,75
15 бит - знак температуры = всегда 0.
*/
	uint8_t i;
	uint16_t tmp = 0;
	uint8_t status = 0;
	
	ClrBit(PORT_TC_CS, B_TC_CS);
	
	i = 16;
	while(i--){
		SetBit(PORT_TC_SCK, B_TC_SCK);
		tmp <<= 1;
		if(BitIsSet(PIN_TC_SO, B_TC_SO)) tmp |= 0x0001; else tmp &= 0xFFFE;
		ClrBit(PORT_TC_SCK, B_TC_SCK);
	}
	SetBit(PORT_TC_CS, B_TC_CS);
	
	if((tmp & (1 << 1 | 1 << 2 | 1 << 15))){
		status = 2;
		tmp = 0;
	}
	if(status == 0){
		tmp >>= 5;	//оставим только целое значение температуры
		if(tmp > (uint16_t)999) status = 3;
		else status = 1;
	}
	STATUS_TC = status;
	VALUE_TC = tmp;
}
void OutTemperatureTC(uint16_t x, uint16_t y){
	uint16_t nx, value = VALUE_TC;
	uint8_t d, fnam;
	char buf[10];
	uint8_t ib = 0;
	
	switch(STATUS_TC){
		case 0:{
			while(LabelUNC[ib] != 0){
				buf[ib] = LabelUNC[ib];
				ib++;
			}
			break;
		}
		case 1:{
			fnam = 0;
			d = value / 100;
			if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
			value = value % 100;
			
			d = value / 10;
			if(d != 0){buf[ib] = (d + 0x30); ib++;}
			else if(fnam == 1){buf[ib] = 0x30; ib++;}
			d = value % 10;
			
			buf[ib] = (d + 0x30); ib++;
			
			buf[ib] = '`'; ib++;
			buf[ib] = 'C'; ib++;
			break;
		}
		case 2:{
			while(LabelBRK[ib] != 0){
				buf[ib] = LabelBRK[ib];
				ib++;
			}
			break;
		}
		case 3:{
			while(LabelOVR[ib] != 0){
				buf[ib] = LabelOVR[ib];
				ib++;
			}
			break;
		}
		default:{
			while(LabelERR[ib] != 0){
				buf[ib] = LabelERR[ib];
				ib++;
			}
		}
	}
	buf[ib] = 0;
	nx = DrawString(buf, x, y);
	DrawRectFill(nx, 319, y, y + 32, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
}
void OutPercent(uint16_t value, uint16_t max_value, uint16_t x, uint16_t y){	//вывод на дисплей значений в процентах: value - входное значение; max_value - значение при 100%
	uint8_t d, fnam;
	uint16_t nx, tmp;
	char buf[10];
	uint8_t ib = 0;
	
	tmp = value * 100 / max_value;
	fnam = 0;
	d = tmp / 100;
	if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
	tmp = tmp % 100;
	
	d = tmp / 10;
	if(d != 0){buf[ib] = (d + 0x30); ib++;}
	else if(fnam == 1){buf[ib] = 0x30; ib++;}
	d = tmp % 10;
	
	buf[ib] = (d + 0x30); ib++;
	buf[ib] = '%'; ib++;

	buf[ib] = 0;
	nx = DrawString(buf, x, y);
	DrawRectFill(nx, 319, y, y + 32, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
}
void LoadValuesFromEEPROM(void){
	SETVAL_AUTO_CONTROL = eeprom_read_byte(&EE_SETVAL_AUTO_CONTROL);
	SETVAL_FLAP_MANUAL_STEP = eeprom_read_byte(&EE_SETVAL_FLAP_MANUAL_STEP);
	SETVAL_SMOKE_DEAD_BAND = eeprom_read_byte(&EE_SETVAL_SMOKE_DEAD_BAND);
	SETVAL_FLAP_AUTO_STEP = eeprom_read_byte(&EE_SETVAL_FLAP_AUTO_STEP);
	SETVAL_FLAP_START_STEPS = eeprom_read_byte(&EE_SETVAL_FLAP_START_STEPS);
	SETVAL_TEMP_SMOKE = eeprom_read_byte(&EE_SETVAL_TEMP_SMOKE);
	SETVAL_MAX_TEMP_SERVE = eeprom_read_byte(&EE_SETVAL_MAX_TEMP_SERVE);
	SETVAL_HYSTER_TEMP_SERVE = eeprom_read_byte(&EE_SETVAL_HYSTER_TEMP_SERVE);
	SETVAL_MAX_TEMP_ROOM = eeprom_read_byte(&EE_SETVAL_MAX_TEMP_ROOM);
	SETVAL_HYSTER_TEMP_ROOM = eeprom_read_byte(&EE_SETVAL_HYSTER_TEMP_ROOM);
	SETVAL_TEMP_BOILER_OFF = eeprom_read_byte(&EE_SETVAL_TEMP_BOILER_OFF);
	SETVAL_HYSTER_TEMP_BOILER_OFF = eeprom_read_byte(&EE_SETVAL_HYSTER_TEMP_BOILER_OFF);
	SETVAL_LIGHT_DISPLAY = eeprom_read_byte(&EE_SETVAL_LIGHT_DISPLAY);
	SETVAL_TIME_LIGHT = eeprom_read_byte(&EE_SETVAL_TIME_LIGHT);
	SETVAL_ALARM_FUEL_ON = eeprom_read_byte(&EE_SETVAL_ALARM_FUEL_ON);
	SETVAL_TEMP_ALARM_FUEL = eeprom_read_byte(&EE_SETVAL_TEMP_ALARM_FUEL);
	SETVAL_TEN_MODE = eeprom_read_byte(&EE_SETVAL_TEN_MODE);
	SETVAL_TEMP_TEN_NOTFROZE_ON = eeprom_read_byte(&EE_SETVAL_TEMP_TEN_NOTFROZE_ON);
	SETVAL_TEMP_TEN_NOTFROZE_OFF = eeprom_read_byte(&EE_SETVAL_TEMP_TEN_NOTFROZE_OFF);
	SETVAL_TEMP_TEN_HEAT_ROOM_ON = eeprom_read_byte(&EE_SETVAL_TEMP_TEN_HEAT_ROOM_ON);
	SETVAL_TEMP_TEN_HEAT_ROOM_OFF = eeprom_read_byte(&EE_SETVAL_TEMP_TEN_HEAT_ROOM_OFF);
	SETVAL_TEN_POWER = eeprom_read_byte(&EE_SETVAL_TEN_POWER);
	SETVAL_TIME_DELAY_PAUSE = eeprom_read_byte(&EE_SETVAL_TIME_DELAY_PAUSE);
	SETVAL_PUMP_PERFORMANCE = eeprom_read_byte(&EE_SETVAL_PUMP_PERFORMANCE);
}
void UpdateValuesFromEEPROM(void){
	eeprom_update_byte(&EE_SETVAL_AUTO_CONTROL, SETVAL_AUTO_CONTROL);
	eeprom_update_byte(&EE_SETVAL_FLAP_MANUAL_STEP, SETVAL_FLAP_MANUAL_STEP);
	eeprom_update_byte(&EE_SETVAL_SMOKE_DEAD_BAND, SETVAL_SMOKE_DEAD_BAND);
	eeprom_update_byte(&EE_SETVAL_FLAP_AUTO_STEP, SETVAL_FLAP_AUTO_STEP);
	eeprom_update_byte(&EE_SETVAL_FLAP_START_STEPS, SETVAL_FLAP_START_STEPS);
	eeprom_update_byte(&EE_SETVAL_TEMP_SMOKE, SETVAL_TEMP_SMOKE);
	eeprom_update_byte(&EE_SETVAL_MAX_TEMP_SERVE, SETVAL_MAX_TEMP_SERVE);
	eeprom_update_byte(&EE_SETVAL_HYSTER_TEMP_SERVE, SETVAL_HYSTER_TEMP_SERVE);
	eeprom_update_byte(&EE_SETVAL_MAX_TEMP_ROOM, SETVAL_MAX_TEMP_ROOM);
	eeprom_update_byte(&EE_SETVAL_HYSTER_TEMP_ROOM, SETVAL_HYSTER_TEMP_ROOM);
	eeprom_update_byte(&EE_SETVAL_TEMP_BOILER_OFF, SETVAL_TEMP_BOILER_OFF);
	eeprom_update_byte(&EE_SETVAL_HYSTER_TEMP_BOILER_OFF, SETVAL_HYSTER_TEMP_BOILER_OFF);
	eeprom_update_byte(&EE_SETVAL_LIGHT_DISPLAY, SETVAL_LIGHT_DISPLAY);
	eeprom_update_byte(&EE_SETVAL_TIME_LIGHT, SETVAL_TIME_LIGHT);
	eeprom_update_byte(&EE_SETVAL_ALARM_FUEL_ON, SETVAL_ALARM_FUEL_ON);
	eeprom_update_byte(&EE_SETVAL_TEMP_ALARM_FUEL, SETVAL_TEMP_ALARM_FUEL);
	eeprom_update_byte(&EE_SETVAL_TEN_MODE, SETVAL_TEN_MODE);
	eeprom_update_byte(&EE_SETVAL_TEMP_TEN_NOTFROZE_ON, SETVAL_TEMP_TEN_NOTFROZE_ON);
	eeprom_update_byte(&EE_SETVAL_TEMP_TEN_NOTFROZE_OFF, SETVAL_TEMP_TEN_NOTFROZE_OFF);
	eeprom_update_byte(&EE_SETVAL_TEMP_TEN_HEAT_ROOM_ON, SETVAL_TEMP_TEN_HEAT_ROOM_ON);
	eeprom_update_byte(&EE_SETVAL_TEMP_TEN_HEAT_ROOM_OFF, SETVAL_TEMP_TEN_HEAT_ROOM_OFF);
	eeprom_update_byte(&EE_SETVAL_TEN_POWER, SETVAL_TEN_POWER);
	eeprom_update_byte(&EE_SETVAL_TIME_DELAY_PAUSE, SETVAL_TIME_DELAY_PAUSE);
	eeprom_update_byte(&EE_SETVAL_PUMP_PERFORMANCE, SETVAL_PUMP_PERFORMANCE);
}
void OutMenuToLCD(void){	//вывод меню и уставки на дисплей
	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);
	if(BitIsClr(STATUS,ST_MENU)){
		//очистка рабочей области экрана
		DrawRectFill(0, 319, 0, 299, VGA_BLACK);	//start X, stop X, start Y, stop Y, color

		DrawString("НАСТРОЙКА", 85, 5);		//str, poz X, poz Y
		DrawRectFill(5, 309, 38, 40, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
		
		//кнопки
		DrawRectFill(2, 104, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(107, 209, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(212, 317, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		
		DrawString("ДАЛЕЕ", 12, 436);		//str, poz X, poz Y
		DrawString("+", 147, 436);		//str, poz X, poz Y
		DrawString("-", 258, 436);		//str, poz X, poz Y
		
	}else{
		DrawRectFill(0, 319, 41, 281, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	}
	
	uint16_t y1 = 43, y2 = 75;
	switch(MENU){
		case 0:{
			DrawString("Режим работы котла", 2, y1);		//str, poz X, poz Y
			DrawString("(Ручной/Автоматический)", 2, y2);		//str, poz X, poz Y
		} break;
		case 1:{
			DrawString("Шагов заслонки", 2, y1);		//str, poz X, poz Y
			DrawString("в ручном режиме", 2, y2);		//str, poz X, poz Y
		} break;
		case 2:{
			DrawString("Зона нечувствительности", 2, y1);		//str, poz X, poz Y
			DrawString("датчика дымохода", 2, y2);		//str, poz X, poz Y
		} break;
		case 3:{
			DrawString("Шагов заслонки", 2, y1);		//str, poz X, poz Y
			DrawString("в автомате", 2, y2);		//str, poz X, poz Y
		} break;
		case 4:{
			DrawString("Стартовое положение", 2, y1);		//str, poz X, poz Y
			DrawString("заслонки тяги", 2, y2);		//str, poz X, poz Y
		} break;
		case 5:{
			DrawString("Температура газов", 2, y1);		//str, poz X, poz Y
			DrawString("дымохода", 50, y2);		//str, poz X, poz Y
		} break;
		case 6:{
			DrawString("Макс. температура", 2, y1);		//str, poz X, poz Y
			DrawString("выхода теплоносителя", 2, y2);		//str, poz X, poz Y
		} break;
		case 7:{
			DrawString("Макс. температура", 2, y1);		//str, poz X, poz Y
			DrawString("комнаты", 50, y2);		//str, poz X, poz Y
		} break;
		case 8:{
			DrawString("Температура дымохода", 2, y1);		//str, poz X, poz Y
			DrawString("отключения котла", 2, y2);		//str, poz X, poz Y
		} break;
		case 9:{
			DrawString("Режим работы табло", 2, y1);		//str, poz X, poz Y
		} break;
		case 10:{
			DrawString("Сигнализация прогорания", 2, y1);		//str, poz X, poz Y
			DrawString("топлива", 50, y2);		//str, poz X, poz Y
		} break;
		case 11:{
			DrawString("Температура сигнализ.", 2, y1);		//str, poz X, poz Y
			DrawString("прогорания топлива", 25, y2);		//str, poz X, poz Y
		} break;
		case 12:{
			DrawString("Автоматический подогрев", 2, y1);		//str, poz X, poz Y
			DrawString("электрическими ТЕНами", 2, y2);		//str, poz X, poz Y
		} break;
		case 13:{
			DrawString("Температура включения", 2, y1);		//str, poz X, poz Y
			DrawString("электрических ТЕНов", 2, y2);		//str, poz X, poz Y
		} break;
		case 14:{
			DrawString("Температура выключения", 2, y1);		//str, poz X, poz Y
			DrawString("электрических ТЕНов", 2, y2);		//str, poz X, poz Y
		} break;
		case 15:{
			DrawString("Задержка в режиме", 2, y1);		//str, poz X, poz Y
			DrawString("пауза", 2, y2);		//str, poz X, poz Y
		} break;
	}
	OutMenuToLCDValue();
}
void OutMenuToLCDValue(){	//вывод переменных значений уставок на дисплей
	uint16_t x, y = 139;
	
	DrawRectFill(0, 319, 107, 281, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	SetFont(font_Tahoma20);
	SetColor(VGA_AQUA);
	switch(MENU){
		case 0:{	// Автоматическая регулировка / Ручная
			if(SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_AUTO) DrawString("Автоматический", 25, y);		//str, poz X, poz Y
			else DrawString("Ручной", 25, y);		//str, poz X, poz Y
		} break;
		case 1:{	// Количество шагов двигателя заслонки в ручном режиме
			OutNumber(SETVAL_FLAP_MANUAL_STEP, 100, y);
		} break;
		case 2:{	// Зона нечувствительности датчика дымохода в каждую сторону
			x = OutNumber(SETVAL_SMOKE_DEAD_BAND, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 3:{	// Количество шагов двигателя заслонки в автоматическом режиме
			OutNumber(SETVAL_FLAP_AUTO_STEP, 100, y);
		} break;
		case 4:{	// Стартовое положение заслонки в шагах двигателя
			x = OutNumber(SETVAL_FLAP_START_STEPS, 100, y);
			DrawString("%", x, y);		//str, poz X, poz Y
		} break;
		case 5:{	// температура газов дымохода
			x = OutNumber(SETVAL_TEMP_SMOKE, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 6:{	// максимальная температура подачи
			x = OutNumber(SETVAL_MAX_TEMP_SERVE, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 7:{	// максимальная температура комнаты 
			x = OutNumber(SETVAL_MAX_TEMP_ROOM, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 8:{	// Температура дымохода отключения управления котла
			x = OutNumber(SETVAL_TEMP_BOILER_OFF, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 9:{	// Работа индикатора
			if(SETVAL_LIGHT_ALL_ON == DEF_SETVAL_LIGHT_ALL_ON) DrawString("Всегда включен", 25, y);		//str, poz X, poz Y
			else DrawString("Экономичный", 25, y);		//str, poz X, poz Y
		} break;
		case 10:{	// Сигнализация прогорания топлива
			if(SETVAL_ALARM_FUEL_ON == DEF_SETVAL_ALARM_FUEL_ON) DrawString("Включена", 25, y);		//str, poz X, poz Y
			else DrawString("Выключена", 25, y);		//str, poz X, poz Y
		} break;
		case 11:{	// Температура сигнализации прогорания топлива
			x = OutNumber(SETVAL_TEMP_ALARM_FUEL, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 12:{	// Автоматический подогрев ТЕНами
			if(SETVAL_AUTO_HEAT == DEF_SETVAL_AUTO_HEAT_ON) DrawString("Включен", 25, y);		//str, poz X, poz Y
			else DrawString("Выключен", 25, y);		//str, poz X, poz Y
		} break;
		case 13:{	// температура включения ТЕНов аварийный подогрев
			x = OutNumber(SETVAL_TEMP_TEN_ON, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 14:{	// температура выключения ТЕНов аварийный подогрев
			x = OutNumber(SETVAL_TEMP_TEN_OFF, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 15:{	// Задержка в регулировании в режиме рауза
			x = OutNumber(SETVAL_DELAY_PAUSE, 100, y);
			DrawString("мин.", x, y);		//str, poz X, poz Y
		} break;
	}
}
uint16_t OutNumber(uint8_t val, uint16_t x, uint16_t y){
	uint8_t d, fnam = 0;
	char buf[10];
	uint8_t ib = 0;

	d = val / 100;
	if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
	val = val % 100;
	
	d = val / 10;
	if(d != 0){buf[ib] = (d + 0x30); ib++;}
	else if(fnam == 1){buf[ib] = 0x30; ib++;}
	d = val % 10;
	
	buf[ib] = (d + 0x30); ib++;

	// while(ib < 9){
		// buf[ib] = ' ';
		// ib++;
	// }
	buf[ib] = 0;
	return DrawString(buf, x, y);
}
uint16_t OutBigNumber(uint16_t val, uint16_t x, uint16_t y){
	uint8_t d, fnam = 0;
	// uint16_t d;
	char buf[10];
	uint8_t ib = 0;

	d = val / 10000;
	if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
	val = val % 10000;
	
	d = val / 1000;
	if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
	else if(fnam == 1){buf[ib] = 0x30; ib++;}
	val = val % 1000;
	
	d = val / 100;
	if(d != 0){buf[ib] = (d + 0x30); ib++; fnam = 1;}
	else if(fnam == 1){buf[ib] = 0x30; ib++;}
	val = val % 100;
	
	d = val / 10;
	if(d != 0){buf[ib] = (d + 0x30); ib++;}
	else if(fnam == 1){buf[ib] = 0x30; ib++;}
	d = val % 10;
	
	buf[ib] = (d + 0x30); ib++;

	buf[ib] = 0;
	return DrawString(buf, x, y);
}
void Work(void){	//анализ и работа
	uint8_t new = 0, NEW_BOILER_STATUS = BOILER_STATUS_UNKNOWN;
	uint16_t nx;
	
	while(1){
		// if(SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_MAN){
			// NEW_BOILER_STATUS = BOILER_STATUS_MANUAL;	//РУЧНОЙ
			// break;
		// }
		// if(STATUS_TC != 1 || STATUS_DS_ROOM != 1 || STATUS_DS_SERVE != 1 || STATUS_DS_RETURN != 1){
			// NEW_BOILER_STATUS = BOILER_STATUS_ERROR;	//ОШИБКА
			// break;
		// }
		// if(dbDS_SERVE >= SETVAL_MAX_TEMP_SERVE || dbDS_ROOM >= SETVAL_MAX_TEMP_ROOM){
			// NEW_BOILER_STATUS = BOILER_STATUS_STOP;	//ОСТАНОВЛЕН
			// break;
		// }
		// if(VALUE_TC <= (SETVAL_TEMP_BOILER_OFF - 10)){
			// NEW_BOILER_STATUS = BOILER_STATUS_IDLE;	//ОЖИДАНИЕ
			// break;
		// }
		// if(BOILER_STATUS == BOILER_STATUS_PAUSE){
			// NEW_BOILER_STATUS = BOILER_STATUS_PAUSE;
			// break;
		// }
		// if(BitIsSet(STATUS,ST_WORK_PAUSE_HIGH) || BitIsSet(STATUS,ST_WORK_PAUSE_LOW)){
			// ClrBit(STATUS,ST_WORK_PAUSE_HIGH);
			// ClrBit(STATUS,ST_WORK_PAUSE_LOW);
			// NEW_BOILER_STATUS = BOILER_STATUS_PAUSE;
			// break;
		// }
		
		// if(VALUE_TC > SETVAL_TEMP_BOILER_OFF) NEW_BOILER_STATUS = BOILER_STATUS_AUTO;	//РАБОТА АВТОМАТ
		break;
	}
	if(NEW_BOILER_STATUS != BOILER_STATUS){	//только индикация
		// new = 1;
		// nx = 120;
		
		// SetFont(font_Tahoma20);
		// SetColor(VGA_WHITE);
		// DrawRectFill(nx, 317, 303, 336, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		// DrawRectFill(2, 317, 340, 424, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		// //очистка полей кнопок
		// DrawRectFill(2, 104, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		// DrawRectFill(107, 209, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		// DrawRectFill(212, 317, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		// DrawString("НАСТР", 12, 436);		//str, poz X, poz Y
		
		// switch(NEW_BOILER_STATUS){
			// case BOILER_STATUS_UNKNOWN:
			// case BOILER_STATUS_IDLE:{
				// DrawString("ОЖИДАНИЕ", nx, 303);		//str, poz X, poz Y
				// DrawRectFill(2, 317, 340, 424, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
			// } break;
			// case BOILER_STATUS_AUTO:{
				// DrawString("АВТОМАТ", nx, 303);		//str, poz X, poz Y
				// //кнопки
				// SetFont(font_Tahoma20);
				// DrawString("ПАУЗА", 115, 428);		//str, poz X, poz Y
				// DrawString("ПАУЗА", 222, 428);		//str, poz X, poz Y
				// SetFont(font_Tahoma10);
				// DrawString("ВЕРХ", 147, 460);		//str, poz X, poz Y
				// DrawString("НИЗ", 252, 460);		//str, poz X, poz Y
			// } break;
			// case BOILER_STATUS_STOP:{
				// DrawString("ОСТАНОВЛЕН", nx, 303);		//str, poz X, poz Y
				// SetFont(font_Tahoma20);
				// SetColor(VGA_YELLOW);
				// if(dbDS_SERVE >= SETVAL_MAX_TEMP_SERVE) DrawString("Перегрев теплоносителя", 5, 341);		//str, poz X, poz Y
				// if(dbDS_ROOM >= SETVAL_MAX_TEMP_ROOM) DrawString("Перегрев комнаты", 5, 373);		//str, poz X, poz Y
			// } break;
			// case BOILER_STATUS_ERROR:{
				// DrawString("ОШИБКА", nx, 303);		//str, poz X, poz Y
				// if(STATUS_TC != 1 || STATUS_DS_ROOM != 1 || STATUS_DS_SERVE != 1 || STATUS_DS_RETURN != 1){
					// SetFont(font_Tahoma20);
					// SetColor(VGA_RED);
					// DrawString("Неисправность датчиков", 15, 341);
				// }
			// } break;
			// case BOILER_STATUS_PAUSE:{
				// DrawString("ПАУЗА", nx, 303);		//str, poz X, poz Y
				// //кнопки
				// SetFont(font_Tahoma20);
				// // DrawString("ПАУЗА", 115, 428);		//str, poz X, poz Y
				// DrawString("СТОП", 229, 428);		//str, poz X, poz Y
				// SetFont(font_Tahoma10);
				// // DrawString("ВЕРХ", 147, 460);		//str, poz X, poz Y
				// DrawString("ПАУЗА", 242, 460);		//str, poz X, poz Y
			// } break;
			// case BOILER_STATUS_MANUAL:{
				// DrawString("РУЧНОЙ", nx, 303);		//str, poz X, poz Y
				// //кнопки
				// SetFont(font_Tahoma20);
				// DrawString("ОТКР", 125, 428);		//str, poz X, poz Y
				// DrawString("ЗАКР", 232, 428);		//str, poz X, poz Y
				// SetFont(font_Tahoma10);
				// DrawString("1 шаг", 142, 460);		//str, poz X, poz Y
				// DrawString("1 шаг", 247, 460);		//str, poz X, poz Y
			// } break;
		// }
	}
	//анализируем переход в новое состояние
	// if(new == 1 && NEW_BOILER_STATUS == BOILER_STATUS_AUTO){	//если новое состояние - "авто"
		// FlapTravel(FLAP_START, 0);
		// double d = ((double)FLAP_STEP_MAX / 100) * (double)SETVAL_FLAP_START_STEPS;
		// FlapTravel(FLAP_FORVARD,(uint8_t)d);
		// EXTREMUM = SETVAL_TEMP_SMOKE;
		// ALARM_TRIGGER = ALARM_TRIGGER_OFF;
		// //пискнем
		// SetBit(PORT_BEEP, B_BEEP);
		// _delay_ms(100);
		// ClrBit(PORT_BEEP, B_BEEP);
	// }
	// if(new == 1 && NEW_BOILER_STATUS != BOILER_STATUS_AUTO){	//если выход из состояния - "авто"
		// FlapTravel(FLAP_START, 0);
	// }
	// if(new == 1 && NEW_BOILER_STATUS == BOILER_STATUS_PAUSE){	//если новое состояние - "пауза"
		// Count_PAUSE_SEC = (uint16_t)SETVAL_DELAY_PAUSE * 60 + 1;
		// FlapTravel(FLAP_START, 0);
	// }
	// if(new == 1){
		// BOILER_STATUS = NEW_BOILER_STATUS;	//приняли новый статус
		// return;
	// }
	
	// если состояние не новое обрабатываем текущее состояние
	// if(BOILER_STATUS == BOILER_STATUS_IDLE){	//ОЖИДАНИЕ
		// if(SETVAL_AUTO_HEAT == DEF_SETVAL_AUTO_HEAT_ON){
			// SetFont(font_Tahoma20);
			// if(dbDS_RETURN <= SETVAL_TEMP_TEN_ON && BitIsClr(PIN_PHEAT,B_PHEAT)) SetBit(PORT_PHEAT,B_PHEAT);	//Включить подогрев
			// if(dbDS_RETURN >= SETVAL_TEMP_TEN_OFF && BitIsSet(PIN_PHEAT,B_PHEAT)) ClrBit(PORT_PHEAT,B_PHEAT);	//Выключить подогрев
			// if(BitIsSet(PIN_PHEAT,B_PHEAT)){
				// SetColor(VGA_RED);
				// DrawString("Включены ТЭНы.", 5, 341);		//str, poz X, poz Y
			// }else{
				// SetColor(VGA_GREEN);
				// DrawString("ТЭНы выключены.", 5, 341);		//str, poz X, poz Y
			// }
			// return;
		// }
	// }else if(BitIsSet(PIN_PHEAT,B_PHEAT)) ClrBit(PORT_PHEAT,B_PHEAT);	//Выключить подогрев
	
	// if(BOILER_STATUS == BOILER_STATUS_AUTO){	//РАБОТА АВТОМАТ
		// uint8_t high = 0, low = 0;
		// //определим текущий уровень температуры
		// if(VALUE_TC < (SETVAL_TEMP_SMOKE - SETVAL_SMOKE_DEAD_BAND)){	//падающее
			// low = 1;
			// //поправим экстремум если он не в той стороне
			// if(EXTREMUM > SETVAL_TEMP_SMOKE) EXTREMUM = SETVAL_TEMP_SMOKE - SETVAL_SMOKE_DEAD_BAND;
		// }
		// if(VALUE_TC > (SETVAL_TEMP_SMOKE + SETVAL_SMOKE_DEAD_BAND)){	//растущее
			// high = 1;
			// //поправим экстремум если он не в той стороне
			// if(EXTREMUM < SETVAL_TEMP_SMOKE) EXTREMUM = SETVAL_TEMP_SMOKE + SETVAL_SMOKE_DEAD_BAND;
		// }
		// //проверим триггер сигнализации
		// if(ALARM_TRIGGER == ALARM_TRIGGER_OFF && VALUE_TC >= SETVAL_TEMP_SMOKE) ALARM_TRIGGER = ALARM_TRIGGER_ON;
		// //управляем задвижкой
		// if(low && VALUE_TC <= (EXTREMUM - DELTA_EXTREMUM)){
			// EXTREMUM = VALUE_TC;
			// FlapTravel(FLAP_FORVARD, SETVAL_FLAP_AUTO_STEP);
		// }
		// if(high && VALUE_TC >= (EXTREMUM + DELTA_EXTREMUM)){
			// EXTREMUM = VALUE_TC;
			// FlapTravel(FLAP_REVERCE, SETVAL_FLAP_AUTO_STEP);
		// }
		// //сигнализация
		// if(SETVAL_ALARM_FUEL_ON == DEF_SETVAL_ALARM_FUEL_ON && ALARM_TRIGGER == ALARM_TRIGGER_ON){
			// if(VALUE_TC <= (uint16_t)SETVAL_TEMP_ALARM_FUEL){
				// uint8_t c = 3;
				// while(c--){
					// SetBit(PORT_BEEP, B_BEEP);
					// _delay_ms(100);
					// ClrBit(PORT_BEEP, B_BEEP);
					// _delay_ms(100);
				// }
				// ALARM_TRIGGER = ALARM_TRIGGER_OFF;
			// }
		// }
	// }
	
	// if(BOILER_STATUS == BOILER_STATUS_PAUSE){
		// Count_PAUSE_SEC--;
		// if(Count_PAUSE_SEC == 0 || BitIsSet(STATUS,ST_WORK_PAUSE_STOP)){	//выходим из паузы
			// BOILER_STATUS = BOILER_STATUS_UNKNOWN;
			// ClrBit(STATUS,ST_WORK_PAUSE_STOP);
		// }
		// uint16_t nx;
		// SetFont(font_Tahoma10);
		// SetColor(VGA_YELLOW);
		// nx = DrawString("Выход из паузы:  ", 70, 400);
		// nx = OutNumber(Count_PAUSE_SEC, nx, 400);
		// DrawString(" сек     ", nx, 400);
	// }
	
// SetFont(font_Tahoma10);
// SetColor(VGA_YELLOW);
// nx = DrawString("EXTREMUM = ", 30, 400);
// nx = OutBigNumber(EXTREMUM, nx, 400);
// DrawString("    ", nx, 400);

}





//===============================================================================
/**БИБЛИОТЕКА ДЛЯ РАБОТЫ С ДАТЧИКОМ ТЕМПЕРАТУРЫ DS18B20
начальное состояние работы датчика 12-битное и для конвертироания температуры датчику надо 750мс
значит опрашивать надо не чаще 800мс
9-битное конвертирование - 94мс, точность 0,5гр

процедура работы с датчиком:
отправка команды преобразования температуры:
	инициализация (проверка ответа)
	отправка команды DS18B20_SKIP_ROM		0xCC Обращение к единственному на шине устройству без указания его адреса
	отправка команды DS18B20_CONVERT_T		0x44 Старт преобразования температуры

чтение результатов:
	инициализация (проверка ответа)
	отправка команды DS18B20_SKIP_ROM		0xCC Обращение к единственному на шине устройству без указания его адреса
	отправка команды DS18B20_R_SCRATCHPAD	0xBE Чтение внутреннего буфера (регистров)
	чтение 
	обработка результата

*/
uint8_t DS18B20_init(uint8_t dat){	//Out: 0 - OK; 1 - нет датчика
	uint8_t i,d;
	
	DDR_Reg = &DDR_DS_OUT;
	PIN_Reg = &PIN_DS_OUT;
	uint8_t DS_BIT = B_DS_OUT;
	
	switch(dat){
		case 2:{
			DDR_Reg = &DDR_DS_ROOM;
			PIN_Reg = &PIN_DS_ROOM;
			DS_BIT = B_DS_ROOM;
		} break;
		case 3:{
			DDR_Reg = &DDR_DS_SRV;
			PIN_Reg = &PIN_DS_SRV;
			DS_BIT = B_DS_SRV;
		} break;
		case 4:{
			DDR_Reg = &DDR_DS_RTN;
			PIN_Reg = &PIN_DS_RTN;
			DS_BIT = B_DS_RTN;
		} break;
		default: break;
	}
	
	ClrBit((*DDR_Reg), DS_BIT);
	
	if(BitIsClr((*PIN_Reg), DS_BIT)) return 1; //проверим наличие 1 в линии, если нет - это error
	SetBit((*DDR_Reg), DS_BIT);	//давим линию в 0
	_delay_us(500);
	ClrBit((*DDR_Reg), DS_BIT);	//отпускаем линию
	_delay_us(60);
	
	d = 0;
	i = 240;
	while(i--){
		_delay_us(1);
		if(BitIsClr((*PIN_Reg), DS_BIT)){d = 1; break;}	//PRESENCE от датчика получен
	}
	if(d == 0) return 1;
	_delay_us(480);
	return 0;
}
uint8_t DS18B20_send(uint8_t dat, uint8_t sbyte){	//если делать ВЫВОД 0хFF, то на выходе будет ПРИНЯТЫЙ байт
	uint8_t i = 8, ans = 0;
	
	DDR_Reg = &DDR_DS_OUT;
	PIN_Reg = &PIN_DS_OUT;
	uint8_t DS_BIT = B_DS_OUT;
	
	switch(dat){
		case 2:{
			DDR_Reg = &DDR_DS_ROOM;
			PIN_Reg = &PIN_DS_ROOM;
			DS_BIT = B_DS_ROOM;
		} break;
		case 3:{
			DDR_Reg = &DDR_DS_SRV;
			PIN_Reg = &PIN_DS_SRV;
			DS_BIT = B_DS_SRV;
		} break;
		case 4:{
			DDR_Reg = &DDR_DS_RTN;
			PIN_Reg = &PIN_DS_RTN;
			DS_BIT = B_DS_RTN;
		} break;
		default: break;
	}

	while(i--){
		SetBit((*DDR_Reg), DS_BIT);	//давим линию в 0
		_delay_us(1);
		if(sbyte & 0x01) ClrBit((*DDR_Reg), DS_BIT);	//отпускаем линию
		sbyte >>= 1;
		_delay_us(10);
		ans >>= 1;
		if(BitIsSet((*PIN_Reg), DS_BIT)) ans |= 0x80;
		_delay_us(90);
		ClrBit((*DDR_Reg), DS_BIT);	//отпускаем линию
		_delay_us(10);
	}
	return ans;
}
uint8_t DS18B20_read(uint8_t dat){	//считывает 9 байтов в буфер, подсчитывает CRC. Out: 0 - OK, 1 - error CRC
	uint8_t i,rb;
	uint8_t DS_crc8 = CRC8INIT;
	
	for(i = 0; i < 9;i++){
		rb = DS18B20_send(dat, 0xFF);
		DS_TempData[i] = rb;
		DS_crc8 = DS_calcCRC8(rb, DS_crc8);
	}
	if(DS_crc8 != CRC8INIT) return 1;
	return 0;
}
uint8_t DS_calcCRC8(uint8_t data, uint8_t crc){	//Out: CRC
// перед первым вызовом CRC необходимо обнулить
// Для серийного номера вызывать 8 раз
// Для данных вызвать 9 раз
// Если в результате crc == 0, то чтение успешно
	uint8_t i, feedback_bit;
	
	i = 8;
	while(i--){
		feedback_bit = (crc ^ data) & 0x01;
		if(feedback_bit == 0x01) crc = crc ^ CRC8POLY;
		crc = (crc >> 1) & 0x7F;
		if(feedback_bit == 0x01) crc = crc | 0x80;
		data >>= 1;
	}
	return crc;
}
// void DS18B20_SetResolution(uint8_t numBit){	//установка разрешения датчика, по умолчанию используется разрешение 12 бит
	// DS18B20_init();
	// DS18B20_send(DS18B20_SKIP_ROM);
	// DS18B20_send(DS18B20_W_SCRATCHPAD);
	// DS18B20_send(0xFF);
	// DS18B20_send(0xFF);
	// DS18B20_send(numBit);

	// DS18B20_init();
	// DS18B20_send(DS18B20_SKIP_ROM);
	// DS18B20_send(DS18B20_C_SCRATCHPAD);
// }
void DS18B20_convertTemp(uint8_t dat){	//отправка команды преобразования температуры.
	if(DS18B20_init(dat) != 0){
		switch(dat){
			case 1: STATUS_DS_OUT = 0; break;
			case 2: STATUS_DS_ROOM = 0; break;
			case 3: STATUS_DS_SERVE = 0; break;
			case 4: STATUS_DS_RETURN = 0; break;
		}
		return;
	}
	DS18B20_send(dat, DS18B20_SKIP_ROM);
	DS18B20_send(dat,DS18B20_CONVERT_T);
}
void DS18B20_getTemp(uint8_t dat){	//запрос, получение и вычисление температуры
	double DS_Temp = 0.0;
	uint8_t status = 1;
	
	while(1){
		if(DS18B20_init(dat) != 0){status = 0; break;}
		
		DS18B20_send(dat, DS18B20_SKIP_ROM);
		DS18B20_send(dat, DS18B20_R_SCRATCHPAD);
		if(DS18B20_read(dat) != 0){status = 0; break;}

		int16_t t = (DS_TempData[1] << 8) + DS_TempData[0];
		DS_Temp = t / 16.0;
		
		break;
	}
	switch(dat){
		case 1: {STATUS_DS_OUT = status; dbDS_OUT = DS_Temp;} break;
		case 2: {STATUS_DS_ROOM = status; dbDS_ROOM = DS_Temp;} break;
		case 3: {STATUS_DS_SERVE = status; dbDS_SERVE = DS_Temp;} break;	//DS_Temp
		case 4: {STATUS_DS_RETURN = status; dbDS_RETURN = DS_Temp;} break;
	}
}
//===============================================================================
//===============================================================================
//===========================================================================
//===========================================================================
//===========================================================================
//===========================================================================


