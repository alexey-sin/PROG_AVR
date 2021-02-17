#include "main.h"
//===============================================================================
int main(void){  
	LoadValuesFromEEPROM();
	
    init();
	InitLCD();	//���������� ������ ��. tft_ili9486_config.h X_MAX � Y_MAX

	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);
	// DrawRectFill(0, 319, 300, 424, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	
	DrawRectFill(0, 319, 300, 302, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(0, 319, 337, 339, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	
	DrawRectFill(0, 1, 303, 424, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(318, 319, 303, 424, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawString("�����:   ", 15, 303);		//str, poz X, poz Y
	//������ ����
	DrawRectFill(0, 319, 425, 426, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(0, 319, 478, 479, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	
	DrawRectFill(0, 1, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(105, 106, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(210, 211, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	DrawRectFill(318, 319, 427, 477, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
	
	DrawString("�����", 12, 436);		//str, poz X, poz Y
	
	DS18B20_convertTemp(1);
	DS18B20_convertTemp(2);
	DS18B20_convertTemp(3);
	DS18B20_convertTemp(4);
	OutFullInfoToLCD();
	
	// _delay_ms(500);
	FlapTravel(FLAP_INIT, 0);	//��������� ��������� �������� - ��������

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
					nx = DrawString("����� �� ����:  ", 70, 282);
					nx = OutNumber(CountMUL_MENU_WAIT, nx, 282);
					DrawString(" ���     ", nx, 282);
				if(CountMUL_MENU_WAIT == 0){	//������� �� ����
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
	SetBit(ACSR, ACD);//�������� ���������� ����������
	
	// ������
	DDR_PLS &= ~(1 << B_PLS);
	PORT_PLS |= (1 << B_PLS);
	DDR_MIN &= ~(1 << B_MIN);
	PORT_MIN |= (1 << B_MIN);
	
	// �/� ���������� ������� ���������� ��������(FLAP) TLE4729G
	DDR_FLAP_FH |= (1 << B_FLAP_FH);
	PORT_FLAP_FH &= ~(1 << B_FLAP_FH);

	DDR_FLAP_FVR |= (1 << B_FLAP_FVR);
	PORT_FLAP_FVR &= ~(1 << B_FLAP_FVR);

	DDR_FLAP_REV |= (1 << B_FLAP_REV);
	PORT_FLAP_REV &= ~(1 << B_FLAP_REV);

	// ������� DS18B20
	//Out	�������
	DDR_DS_OUT &= ~(1 << B_DS_OUT);
	PORT_DS_OUT &= ~(1 << B_DS_OUT);

	//Room	�������
	DDR_DS_ROOM &= ~(1 << B_DS_ROOM);
	PORT_DS_ROOM &= ~(1 << B_DS_ROOM);

	//Serve (����������� ������)
	DDR_DS_SRV &= ~(1 << B_DS_SRV);
	PORT_DS_SRV &= ~(1 << B_DS_SRV);

	//Return (����������� �������)
	DDR_DS_RTN &= ~(1 << B_DS_RTN);
	PORT_DS_RTN &= ~(1 << B_DS_RTN);

	// ������ ������� ����� - ��������� �� MAX6675
	DDR_TC_SO &= ~(1 << B_TC_SO);
	PORT_TC_SO &= ~(1 << B_TC_SO);

	DDR_TC_CS |= (1 << B_TC_CS);
	PORT_TC_CS |= (1 << B_TC_CS);

	DDR_TC_SCK |= (1 << B_TC_SCK);
	PORT_TC_SCK &= ~(1 << B_TC_SCK);

	//Beep ������ ��������
	DDR_BEEP |= (1 << B_BEEP);
	PORT_BEEP &= ~(1 << B_BEEP);
	
	//Preheating ���� ��������� ����� ���������
	DDR_PHEAT |= (1 << B_PHEAT);
	PORT_PHEAT &= ~(1 << B_PHEAT);
	
	// // ��������� UART �� 38400 bps(���)
	// UBRRH = 0;
	// UBRRL = 26;	//UBRR = 16000000/(16 * 38400) - 1 = 26,04 //��� U2X = 0
	// UCSRB = (1 << TXEN);	// ��� ��������(RXEN=1),��� ����������(TXEN=1),��� ���������� �� ������(RXCIE=1),��� ���������� �� ��������(TXCIE=1)
	// // ��� ������� � �������� UCSRC ����������� ��������� ��� URSEL(����� ATTiny2313, ��� ��� ���)!!!
	// UCSRC = (1 << URSEL)|(3 << UCSZ0);	// ����������� �����(UMSEL=0), 8 ���(UCSZ2-0=011), 1 ����-���(USBS=0),��� �������� ��������(UPM1-0=00)

    //1(16���)������ - �������� ��� ~1���
    SFIOR |= (1 << PSR10 | 1 << PSR2);   // ����� ���������� �������� 1 � 0
    TIMSK |= (1 << OCIE1A);   // ��������� ���������� �� ���������� � 1(16����) ��������
    
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS12);// �������� �� 256  1/16000000 = 62,5�� * 256 = 16���
	//0xF424 ��� 62500 * 0,000016 = 1,0 ���
	OCR1AH = ((0xF424 & 0xFF00) >> 8);
	OCR1AL = (0xF424 & 0x00FF);
	
	//2(8���)������ - ���������� ���������� (������)
	TIMSK |= (1 << TOIE2);   // ��������� ���������� �� ������������ 2(8����) ��������
	// ��������� ����� �����
	// TCNT2 = 0;
	// TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (8�)0,125���*256*1024 = 32�� (������������ ��������)
	// TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (16�)0,0625���*256*1024 = 16.4�� (������������ ��������)
	
	sei();
}
ISR(TIMER1_COMPA_vect){
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	SetBit(STATUS, ST_BLC);
}
ISR(TIMER2_OVF_vect){
	CountMUL_BTN_WAIT--;
	if(CountMUL_BTN_WAIT == 0){
		TCCR2 = 0;	// ������ 2 ����
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
	TCCR2 |= (1 << CS20 | 1 << CS21 | 1 << CS22);   // (16�)0,0625���*256*1024 = 16.4�� (������������ ��������)
	SetBit(STATUS, ST_BTN_WAIT);
}
void ExecuteButton(void){
	if(BitIsSet(STATUS,ST_MENU)) CountMUL_MENU_WAIT = MUL_MENU_WAIT;
	
	if(BitIsSet(BUTTON,MMM)){
		if(BitIsClr(STATUS,ST_MENU)){	//������ ��������� ����
			MENU = 0;
			CountMUL_MENU_WAIT = MUL_MENU_WAIT;
			
			OutMenuToLCD();
			SetBit(STATUS,ST_MENU);
		}else{
			if(MENU == 0 && SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_AUTO) MENU = 2;
			else if(MENU == 1 && SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_MAN) MENU = 9;
			else if(MENU == 9 && SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_MAN) MENU = 0;
			else if(MENU == 10 && SETVAL_ALARM_FUEL_ON == DEF_SETVAL_ALARM_FUEL_OFF) MENU = 12;
			else if(MENU == 12 && SETVAL_AUTO_HEAT == DEF_SETVAL_AUTO_HEAT_OFF) MENU = 15;
			else MENU++;
			
			if(MENU == 16) MENU = 0;
			OutMenuToLCD();
		}
		ClrBit(BUTTON,MMM);
	}
	if(BitIsSet(BUTTON,PLS)){
		
		if(BitIsSet(STATUS,ST_MENU)){	//��������� ����
			switch(MENU){
				case 0:{	// �������������� ����������� / ������	
					SETVAL_AUTO_CONTROL = DEF_SETVAL_AUTO_CONTROL_AUTO;
				} break;
				case 1:{	// ���������� ����� ��������� �������� � ������ ������
					if(SETVAL_FLAP_MANUAL_STEP == DEF_SETVAL_FLAP_MANUAL_STEP_MAX) break;
					SETVAL_FLAP_MANUAL_STEP += DEF_SETVAL_FLAP_MANUAL_STEP_STEP;
				} break;
				case 2:{	// ���� ������������������ ������� �������� � ������ ������� (����� �������)
					if(SETVAL_SMOKE_DEAD_BAND == DEF_SETVAL_SMOKE_DEAD_BAND_MAX) break;
					SETVAL_SMOKE_DEAD_BAND += DEF_SETVAL_SMOKE_DEAD_BAND_STEP;
				} break;
				case 3:{	// ���������� ����� ��������� �������� � �������������� ������
					if(SETVAL_FLAP_AUTO_STEP == DEF_SETVAL_FLAP_AUTO_STEP_MAX) break;
					SETVAL_FLAP_AUTO_STEP += DEF_SETVAL_FLAP_AUTO_STEP_STEP;
				} break;
				case 4:{	// ��������� ��������� �������� � ����� ���������
					if(SETVAL_FLAP_START_STEPS == DEF_SETVAL_FLAP_START_STEPS_MAX) break;
					SETVAL_FLAP_START_STEPS += DEF_SETVAL_FLAP_START_STEPS_STEP;
				} break;
				case 5:{	// ����������� ����� �������� (����� �������)
					if(SETVAL_TEMP_SMOKE == DEF_SETVAL_TEMP_SMOKE_MAX) break;
					SETVAL_TEMP_SMOKE += DEF_SETVAL_TEMP_SMOKE_STEP;
				} break;
				case 6:{	// ������������ ����������� ������ (����� �������)
					if(SETVAL_MAX_TEMP_SERVE == DEF_SETVAL_MAX_TEMP_SERVE_MAX) break;
					SETVAL_MAX_TEMP_SERVE += DEF_SETVAL_MAX_TEMP_SERVE_STEP;
				} break;
				case 7:{	// ������������ ����������� ������� (����� �������)
					if(SETVAL_MAX_TEMP_ROOM == DEF_SETVAL_MAX_TEMP_ROOM_MAX) break;
					SETVAL_MAX_TEMP_ROOM += DEF_SETVAL_MAX_TEMP_ROOM_STEP;
				} break;
				case 8:{	// ����������� �������� ���������� ���������� ����� (����� �������)
					if(SETVAL_TEMP_BOILER_OFF == DEF_SETVAL_TEMP_BOILER_OFF_MAX) break;
					SETVAL_TEMP_BOILER_OFF += DEF_SETVAL_TEMP_BOILER_OFF_STEP;
				} break;
				case 9:{	// ������ ����������
					SETVAL_LIGHT_ALL_ON = DEF_SETVAL_LIGHT_ALL_ON;
				} break;
				case 10:{	// ������������ ���������� �������
					SETVAL_ALARM_FUEL_ON = DEF_SETVAL_ALARM_FUEL_ON;
				} break;
				case 11:{	// ����������� ������������ ���������� ������� (����� �������)
					if(SETVAL_TEMP_ALARM_FUEL == DEF_SETVAL_TEMP_ALARM_FUEL_MAX) break;
					SETVAL_TEMP_ALARM_FUEL += DEF_SETVAL_TEMP_ALARM_FUEL_STEP;
				} break;
				case 12:{	// �������������� �������� ������ 
					SETVAL_AUTO_HEAT = DEF_SETVAL_AUTO_HEAT_ON;
				} break;
				case 13:{	// ����������� ��������� ����� ��������� �������� (����� �������)
					if(SETVAL_TEMP_TEN_ON == DEF_SETVAL_TEMP_TEN_ON_MAX || ((SETVAL_TEMP_TEN_ON + 2) > SETVAL_TEMP_TEN_OFF)) break;
					SETVAL_TEMP_TEN_ON += DEF_SETVAL_TEMP_TEN_ON_STEP;
				} break;
				case 14:{	// ����������� ���������� ����� ��������� �������� (����� �������)
					if(SETVAL_TEMP_TEN_OFF == DEF_SETVAL_TEMP_TEN_OFF_MAX) break;
					SETVAL_TEMP_TEN_OFF += DEF_SETVAL_TEMP_TEN_OFF_STEP;
				} break;
				case 15:{	// �������� � ������������� � ������ ����� (����� ������)
					if(SETVAL_DELAY_PAUSE == DEF_SETVAL_DELAY_PAUSE_MAX) break;
					SETVAL_DELAY_PAUSE += DEF_SETVAL_DELAY_PAUSE_STEP;
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
		
		if(BitIsSet(STATUS,ST_MENU)){	//��������� ����
			switch(MENU){
				case 0:{	// �������������� ����������� / ������
					SETVAL_AUTO_CONTROL = DEF_SETVAL_AUTO_CONTROL_MAN;
				} break;
				case 1:{	// ���������� ����� ��������� �������� � ������ ������
					if(SETVAL_FLAP_MANUAL_STEP == DEF_SETVAL_FLAP_MANUAL_STEP_MIN) break;
					SETVAL_FLAP_MANUAL_STEP -= DEF_SETVAL_FLAP_MANUAL_STEP_STEP;
				} break;
				case 2:{	// ���� ������������������ ������� �������� � ������ ������� (����� �������)
					if(SETVAL_SMOKE_DEAD_BAND == DEF_SETVAL_SMOKE_DEAD_BAND_MIN) break;
					SETVAL_SMOKE_DEAD_BAND -= DEF_SETVAL_SMOKE_DEAD_BAND_STEP;
				} break;
				case 3:{	// ���������� ����� ��������� �������� � �������������� ������
					if(SETVAL_FLAP_AUTO_STEP == DEF_SETVAL_FLAP_AUTO_STEP_MIN) break;
					SETVAL_FLAP_AUTO_STEP -= DEF_SETVAL_FLAP_AUTO_STEP_STEP;
				} break;
				case 4:{	// ��������� ��������� �������� � ����� ���������
					if(SETVAL_FLAP_START_STEPS == DEF_SETVAL_FLAP_START_STEPS_MIN) break;
					SETVAL_FLAP_START_STEPS -= DEF_SETVAL_FLAP_START_STEPS_STEP;
				} break;
				case 5:{	// ����������� ����� �������� (����� �������)
					if(SETVAL_TEMP_SMOKE == DEF_SETVAL_TEMP_SMOKE_MIN) break;
					SETVAL_TEMP_SMOKE -= DEF_SETVAL_TEMP_SMOKE_STEP;
				} break;
				case 6:{	// ������������ ����������� ������ (����� �������)
					if(SETVAL_MAX_TEMP_SERVE == DEF_SETVAL_MAX_TEMP_SERVE_MIN) break;
					SETVAL_MAX_TEMP_SERVE -= DEF_SETVAL_MAX_TEMP_SERVE_STEP;
				} break;
				case 7:{	// ������������ ����������� ������� (����� �������)
					if(SETVAL_MAX_TEMP_ROOM == DEF_SETVAL_MAX_TEMP_ROOM_MIN) break;
					SETVAL_MAX_TEMP_ROOM -= DEF_SETVAL_MAX_TEMP_ROOM_STEP;
				} break;
				case 8:{	// ����������� �������� ���������� ���������� ����� (����� �������)
					if(SETVAL_TEMP_BOILER_OFF == DEF_SETVAL_TEMP_BOILER_OFF_MIN) break;
					SETVAL_TEMP_BOILER_OFF -= DEF_SETVAL_TEMP_BOILER_OFF_STEP;
				} break;
				case 9:{	// ������ ����������
					SETVAL_LIGHT_ALL_ON = DEF_SETVAL_LIGHT_AUTO;
				} break;
				case 10:{	// ������������ ���������� �������
					SETVAL_ALARM_FUEL_ON = DEF_SETVAL_ALARM_FUEL_OFF;
				} break;
				case 11:{	// ����������� ������������ ���������� ������� (����� �������)
					if(SETVAL_TEMP_ALARM_FUEL == DEF_SETVAL_TEMP_ALARM_FUEL_MIN) break;
					SETVAL_TEMP_ALARM_FUEL -= DEF_SETVAL_TEMP_ALARM_FUEL_STEP;
				} break;
				case 12:{	// �������������� �������� ������
					SETVAL_AUTO_HEAT = DEF_SETVAL_AUTO_HEAT_OFF;
				} break;
				case 13:{	// ����������� ��������� ����� ��������� �������� (����� �������)
					if(SETVAL_TEMP_TEN_ON == DEF_SETVAL_TEMP_TEN_ON_MIN) break;
					SETVAL_TEMP_TEN_ON -= DEF_SETVAL_TEMP_TEN_ON_STEP;
				} break;
				case 14:{	// ����������� ���������� ����� ��������� �������� (����� �������)
					if(SETVAL_TEMP_TEN_OFF == DEF_SETVAL_TEMP_TEN_OFF_MIN || ((SETVAL_TEMP_TEN_ON + 2) > SETVAL_TEMP_TEN_OFF)) break;
					SETVAL_TEMP_TEN_OFF -= DEF_SETVAL_TEMP_TEN_OFF_STEP;
				} break;
				case 15:{	// �������� � ������������� � ������ ����� (����� ������)
					if(SETVAL_DELAY_PAUSE == DEF_SETVAL_DELAY_PAUSE_MIN) break;
					SETVAL_DELAY_PAUSE -= DEF_SETVAL_DELAY_PAUSE_STEP;
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
void OutFullInfoToLCD(void){	//����� ����������� ��������� �� ������� (��������� ������� ������)
	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);
	//������� ������� ������� ������
	DrawRectFill(0, 319, 0, 299, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	
	DrawString("�����: ", 5, 5);		//str, poz X, poz Y
	DrawString("�������: ", 5, 37);	//str, poz X, poz Y
	
	DrawString("������: ", 5, 101);	//str, poz X, poz Y
	DrawString("�������: ", 5, 133);	//str, poz X, poz Y
	
	DrawString("�������: ", 5, 197);	//str, poz X, poz Y
	
	DrawString("�������� ����: ", 5, 261);		//str, poz X, poz Y
	
}
void OutVarInfoToLCD(void){	//����� ����������� ��������� �� ������� (������ ����������)

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
void ReadMAX6675(void){	//������ ����������� � VALUE_TC � STATUS_TC
/**
PC7 SCL	������ ���������� ��� ��������
PC4 SO	������ ��� ��������
PC6 CE	1 ������	������ ���������� � ���������
PC5 CE	2 ������	������ ���������� � ���������

������ ������ �� ��� MAX6675:
�������� CE � 0, ������ ����� �� SCL 16 ��� � � ����� SCL = 1 ������ ���� � ������ MISO.
����� ���� ������� ������ � ������� ����� ������
� ����� CE ���������  1, SCL ������ �������� � 0.

0 ��� - 3-� ��������� - �� ��������
1 ��� - ������ 0 - ������� MAX6675
2 ��� 0 ���� ��� ������, 1 ���� ��������� � ������
3-14 ���� (12 ����) �������� �����������, 2 ������� ������� - ������� ����� (�0,25)
	��� ���� = 0 ��. �� �������
	��� ������� - +1023,75
15 ��� - ���� ����������� = ������ 0.
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
		tmp >>= 5;	//������� ������ ����� �������� �����������
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
void OutPercent(uint16_t value, uint16_t max_value, uint16_t x, uint16_t y){	//����� �� ������� �������� � ���������: value - ������� ��������; max_value - �������� ��� 100%
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
	SETVAL_MAX_TEMP_ROOM = eeprom_read_byte(&EE_SETVAL_MAX_TEMP_ROOM);
	SETVAL_TEMP_BOILER_OFF = eeprom_read_byte(&EE_SETVAL_TEMP_BOILER_OFF);
	SETVAL_LIGHT_ALL_ON = eeprom_read_byte(&EE_SETVAL_LIGHT_ALL_ON);
	SETVAL_ALARM_FUEL_ON = eeprom_read_byte(&EE_SETVAL_ALARM_FUEL_ON);
	SETVAL_TEMP_ALARM_FUEL = eeprom_read_byte(&EE_SETVAL_TEMP_ALARM_FUEL);
	SETVAL_AUTO_HEAT = eeprom_read_byte(&EE_SETVAL_AUTO_HEAT);
	SETVAL_TEMP_TEN_ON = eeprom_read_byte(&EE_SETVAL_TEMP_TEN_ON);
	SETVAL_TEMP_TEN_OFF = eeprom_read_byte(&EE_SETVAL_TEMP_TEN_OFF);
	SETVAL_DELAY_PAUSE = eeprom_read_byte(&EE_SETVAL_DELAY_PAUSE);
}
void UpdateValuesFromEEPROM(void){
	eeprom_update_byte(&EE_SETVAL_AUTO_CONTROL, SETVAL_AUTO_CONTROL);
	eeprom_update_byte(&EE_SETVAL_FLAP_MANUAL_STEP, SETVAL_FLAP_MANUAL_STEP);
	eeprom_update_byte(&EE_SETVAL_SMOKE_DEAD_BAND, SETVAL_SMOKE_DEAD_BAND);
	eeprom_update_byte(&EE_SETVAL_FLAP_AUTO_STEP, SETVAL_FLAP_AUTO_STEP);
	eeprom_update_byte(&EE_SETVAL_FLAP_START_STEPS, SETVAL_FLAP_START_STEPS);
	eeprom_update_byte(&EE_SETVAL_TEMP_SMOKE, SETVAL_TEMP_SMOKE);
	eeprom_update_byte(&EE_SETVAL_MAX_TEMP_SERVE, SETVAL_MAX_TEMP_SERVE);
	eeprom_update_byte(&EE_SETVAL_MAX_TEMP_ROOM, SETVAL_MAX_TEMP_ROOM);
	eeprom_update_byte(&EE_SETVAL_TEMP_BOILER_OFF, SETVAL_TEMP_BOILER_OFF);
	eeprom_update_byte(&EE_SETVAL_LIGHT_ALL_ON, SETVAL_LIGHT_ALL_ON);
	eeprom_update_byte(&EE_SETVAL_ALARM_FUEL_ON, SETVAL_ALARM_FUEL_ON);
	eeprom_update_byte(&EE_SETVAL_TEMP_ALARM_FUEL, SETVAL_TEMP_ALARM_FUEL);
	eeprom_update_byte(&EE_SETVAL_AUTO_HEAT, SETVAL_AUTO_HEAT);
	eeprom_update_byte(&EE_SETVAL_TEMP_TEN_ON, SETVAL_TEMP_TEN_ON);
	eeprom_update_byte(&EE_SETVAL_TEMP_TEN_OFF, SETVAL_TEMP_TEN_OFF);
	eeprom_update_byte(&EE_SETVAL_DELAY_PAUSE, SETVAL_DELAY_PAUSE);
}
void OutMenuToLCD(void){	//����� ���� � ������� �� �������
	SetFont(font_Tahoma20);
	SetColor(VGA_WHITE);
	if(BitIsClr(STATUS,ST_MENU)){
		//������� ������� ������� ������
		DrawRectFill(0, 319, 0, 299, VGA_BLACK);	//start X, stop X, start Y, stop Y, color

		DrawString("���������", 85, 5);		//str, poz X, poz Y
		DrawRectFill(5, 309, 38, 40, VGA_WHITE);	//start X, stop X, start Y, stop Y, color
		
		//������
		DrawRectFill(2, 104, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(107, 209, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(212, 317, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		
		DrawString("�����", 12, 436);		//str, poz X, poz Y
		DrawString("+", 147, 436);		//str, poz X, poz Y
		DrawString("-", 258, 436);		//str, poz X, poz Y
		
	}else{
		DrawRectFill(0, 319, 41, 281, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	}
	
	uint16_t y1 = 43, y2 = 75;
	switch(MENU){
		case 0:{
			DrawString("����� ������ �����", 2, y1);		//str, poz X, poz Y
			DrawString("(������/��������������)", 2, y2);		//str, poz X, poz Y
		} break;
		case 1:{
			DrawString("����� ��������", 2, y1);		//str, poz X, poz Y
			DrawString("� ������ ������", 2, y2);		//str, poz X, poz Y
		} break;
		case 2:{
			DrawString("���� ������������������", 2, y1);		//str, poz X, poz Y
			DrawString("������� ��������", 2, y2);		//str, poz X, poz Y
		} break;
		case 3:{
			DrawString("����� ��������", 2, y1);		//str, poz X, poz Y
			DrawString("� ��������", 2, y2);		//str, poz X, poz Y
		} break;
		case 4:{
			DrawString("��������� ���������", 2, y1);		//str, poz X, poz Y
			DrawString("�������� ����", 2, y2);		//str, poz X, poz Y
		} break;
		case 5:{
			DrawString("����������� �����", 2, y1);		//str, poz X, poz Y
			DrawString("��������", 50, y2);		//str, poz X, poz Y
		} break;
		case 6:{
			DrawString("����. �����������", 2, y1);		//str, poz X, poz Y
			DrawString("������ �������������", 2, y2);		//str, poz X, poz Y
		} break;
		case 7:{
			DrawString("����. �����������", 2, y1);		//str, poz X, poz Y
			DrawString("�������", 50, y2);		//str, poz X, poz Y
		} break;
		case 8:{
			DrawString("����������� ��������", 2, y1);		//str, poz X, poz Y
			DrawString("���������� �����", 2, y2);		//str, poz X, poz Y
		} break;
		case 9:{
			DrawString("����� ������ �����", 2, y1);		//str, poz X, poz Y
		} break;
		case 10:{
			DrawString("������������ ����������", 2, y1);		//str, poz X, poz Y
			DrawString("�������", 50, y2);		//str, poz X, poz Y
		} break;
		case 11:{
			DrawString("����������� ��������.", 2, y1);		//str, poz X, poz Y
			DrawString("���������� �������", 25, y2);		//str, poz X, poz Y
		} break;
		case 12:{
			DrawString("�������������� ��������", 2, y1);		//str, poz X, poz Y
			DrawString("�������������� ������", 2, y2);		//str, poz X, poz Y
		} break;
		case 13:{
			DrawString("����������� ���������", 2, y1);		//str, poz X, poz Y
			DrawString("������������� �����", 2, y2);		//str, poz X, poz Y
		} break;
		case 14:{
			DrawString("����������� ����������", 2, y1);		//str, poz X, poz Y
			DrawString("������������� �����", 2, y2);		//str, poz X, poz Y
		} break;
		case 15:{
			DrawString("�������� � ������", 2, y1);		//str, poz X, poz Y
			DrawString("�����", 2, y2);		//str, poz X, poz Y
		} break;
	}
	OutMenuToLCDValue();
}
void OutMenuToLCDValue(){	//����� ���������� �������� ������� �� �������
	uint16_t x, y = 139;
	
	DrawRectFill(0, 319, 107, 281, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
	SetFont(font_Tahoma20);
	SetColor(VGA_AQUA);
	switch(MENU){
		case 0:{	// �������������� ����������� / ������
			if(SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_AUTO) DrawString("��������������", 25, y);		//str, poz X, poz Y
			else DrawString("������", 25, y);		//str, poz X, poz Y
		} break;
		case 1:{	// ���������� ����� ��������� �������� � ������ ������
			OutNumber(SETVAL_FLAP_MANUAL_STEP, 100, y);
		} break;
		case 2:{	// ���� ������������������ ������� �������� � ������ �������
			x = OutNumber(SETVAL_SMOKE_DEAD_BAND, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 3:{	// ���������� ����� ��������� �������� � �������������� ������
			OutNumber(SETVAL_FLAP_AUTO_STEP, 100, y);
		} break;
		case 4:{	// ��������� ��������� �������� � ����� ���������
			x = OutNumber(SETVAL_FLAP_START_STEPS, 100, y);
			DrawString("%", x, y);		//str, poz X, poz Y
		} break;
		case 5:{	// ����������� ����� ��������
			x = OutNumber(SETVAL_TEMP_SMOKE, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 6:{	// ������������ ����������� ������
			x = OutNumber(SETVAL_MAX_TEMP_SERVE, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 7:{	// ������������ ����������� ������� 
			x = OutNumber(SETVAL_MAX_TEMP_ROOM, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 8:{	// ����������� �������� ���������� ���������� �����
			x = OutNumber(SETVAL_TEMP_BOILER_OFF, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 9:{	// ������ ����������
			if(SETVAL_LIGHT_ALL_ON == DEF_SETVAL_LIGHT_ALL_ON) DrawString("������ �������", 25, y);		//str, poz X, poz Y
			else DrawString("�����������", 25, y);		//str, poz X, poz Y
		} break;
		case 10:{	// ������������ ���������� �������
			if(SETVAL_ALARM_FUEL_ON == DEF_SETVAL_ALARM_FUEL_ON) DrawString("��������", 25, y);		//str, poz X, poz Y
			else DrawString("���������", 25, y);		//str, poz X, poz Y
		} break;
		case 11:{	// ����������� ������������ ���������� �������
			x = OutNumber(SETVAL_TEMP_ALARM_FUEL, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 12:{	// �������������� �������� ������
			if(SETVAL_AUTO_HEAT == DEF_SETVAL_AUTO_HEAT_ON) DrawString("�������", 25, y);		//str, poz X, poz Y
			else DrawString("��������", 25, y);		//str, poz X, poz Y
		} break;
		case 13:{	// ����������� ��������� ����� ��������� ��������
			x = OutNumber(SETVAL_TEMP_TEN_ON, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 14:{	// ����������� ���������� ����� ��������� ��������
			x = OutNumber(SETVAL_TEMP_TEN_OFF, 100, y);
			DrawString("`C", x, y);		//str, poz X, poz Y
		} break;
		case 15:{	// �������� � ������������� � ������ �����
			x = OutNumber(SETVAL_DELAY_PAUSE, 100, y);
			DrawString("���.", x, y);		//str, poz X, poz Y
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
void Work(void){	//������ � ������
	uint8_t new = 0, NEW_BOILER_STATUS = BOILER_STATUS_UNKNOWN;
	uint16_t nx;
	
	while(1){
		if(SETVAL_AUTO_CONTROL == DEF_SETVAL_AUTO_CONTROL_MAN){
			NEW_BOILER_STATUS = BOILER_STATUS_MANUAL;	//������
			break;
		}
		if(STATUS_TC != 1 || STATUS_DS_ROOM != 1 || STATUS_DS_SERVE != 1 || STATUS_DS_RETURN != 1){
			NEW_BOILER_STATUS = BOILER_STATUS_ERROR;	//������
			break;
		}
		if(dbDS_SERVE >= SETVAL_MAX_TEMP_SERVE || dbDS_ROOM >= SETVAL_MAX_TEMP_ROOM){
			NEW_BOILER_STATUS = BOILER_STATUS_STOP;	//����������
			break;
		}
		if(VALUE_TC <= (SETVAL_TEMP_BOILER_OFF - 10)){
			NEW_BOILER_STATUS = BOILER_STATUS_IDLE;	//��������
			break;
		}
		if(BOILER_STATUS == BOILER_STATUS_PAUSE){
			NEW_BOILER_STATUS = BOILER_STATUS_PAUSE;
			break;
		}
		if(BitIsSet(STATUS,ST_WORK_PAUSE_HIGH) || BitIsSet(STATUS,ST_WORK_PAUSE_LOW)){
			ClrBit(STATUS,ST_WORK_PAUSE_HIGH);
			ClrBit(STATUS,ST_WORK_PAUSE_LOW);
			NEW_BOILER_STATUS = BOILER_STATUS_PAUSE;
			break;
		}
		
		if(VALUE_TC > SETVAL_TEMP_BOILER_OFF) NEW_BOILER_STATUS = BOILER_STATUS_AUTO;	//������ �������
		break;
	}
	if(NEW_BOILER_STATUS != BOILER_STATUS){	//������ ���������
		new = 1;
		nx = 120;
		
		SetFont(font_Tahoma20);
		SetColor(VGA_WHITE);
		DrawRectFill(nx, 317, 303, 336, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(2, 317, 340, 424, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		//������� ����� ������
		DrawRectFill(2, 104, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(107, 209, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawRectFill(212, 317, 427, 477, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
		DrawString("�����", 12, 436);		//str, poz X, poz Y
		
		switch(NEW_BOILER_STATUS){
			case BOILER_STATUS_UNKNOWN:
			case BOILER_STATUS_IDLE:{
				DrawString("��������", nx, 303);		//str, poz X, poz Y
				DrawRectFill(2, 317, 340, 424, VGA_BLACK);	//start X, stop X, start Y, stop Y, color
			} break;
			case BOILER_STATUS_AUTO:{
				DrawString("�������", nx, 303);		//str, poz X, poz Y
				//������
				SetFont(font_Tahoma20);
				DrawString("�����", 115, 428);		//str, poz X, poz Y
				DrawString("�����", 222, 428);		//str, poz X, poz Y
				SetFont(font_Tahoma10);
				DrawString("����", 147, 460);		//str, poz X, poz Y
				DrawString("���", 252, 460);		//str, poz X, poz Y
			} break;
			case BOILER_STATUS_STOP:{
				DrawString("����������", nx, 303);		//str, poz X, poz Y
				SetFont(font_Tahoma20);
				SetColor(VGA_YELLOW);
				if(dbDS_SERVE >= SETVAL_MAX_TEMP_SERVE) DrawString("�������� �������������", 5, 341);		//str, poz X, poz Y
				if(dbDS_ROOM >= SETVAL_MAX_TEMP_ROOM) DrawString("�������� �������", 5, 373);		//str, poz X, poz Y
			} break;
			case BOILER_STATUS_ERROR:{
				DrawString("������", nx, 303);		//str, poz X, poz Y
				if(STATUS_TC != 1 || STATUS_DS_ROOM != 1 || STATUS_DS_SERVE != 1 || STATUS_DS_RETURN != 1){
					SetFont(font_Tahoma20);
					SetColor(VGA_RED);
					DrawString("������������� ��������", 15, 341);
				}
			} break;
			case BOILER_STATUS_PAUSE:{
				DrawString("�����", nx, 303);		//str, poz X, poz Y
				//������
				SetFont(font_Tahoma20);
				// DrawString("�����", 115, 428);		//str, poz X, poz Y
				DrawString("����", 229, 428);		//str, poz X, poz Y
				SetFont(font_Tahoma10);
				// DrawString("����", 147, 460);		//str, poz X, poz Y
				DrawString("�����", 242, 460);		//str, poz X, poz Y
			} break;
			case BOILER_STATUS_MANUAL:{
				DrawString("������", nx, 303);		//str, poz X, poz Y
				//������
				SetFont(font_Tahoma20);
				DrawString("����", 125, 428);		//str, poz X, poz Y
				DrawString("����", 232, 428);		//str, poz X, poz Y
				SetFont(font_Tahoma10);
				DrawString("1 ���", 142, 460);		//str, poz X, poz Y
				DrawString("1 ���", 247, 460);		//str, poz X, poz Y
			} break;
		}
	}
	//����������� ������� � ����� ���������
	if(new == 1 && NEW_BOILER_STATUS == BOILER_STATUS_AUTO){	//���� ����� ��������� - "����"
		FlapTravel(FLAP_START, 0);
		double d = ((double)FLAP_STEP_MAX / 100) * (double)SETVAL_FLAP_START_STEPS;
		FlapTravel(FLAP_FORVARD,(uint8_t)d);
		EXTREMUM = SETVAL_TEMP_SMOKE;
		ALARM_TRIGGER = ALARM_TRIGGER_OFF;
		//�������
		SetBit(PORT_BEEP, B_BEEP);
		_delay_ms(100);
		ClrBit(PORT_BEEP, B_BEEP);
	}
	if(new == 1 && NEW_BOILER_STATUS != BOILER_STATUS_AUTO){	//���� ����� �� ��������� - "����"
		FlapTravel(FLAP_START, 0);
	}
	if(new == 1 && NEW_BOILER_STATUS == BOILER_STATUS_PAUSE){	//���� ����� ��������� - "�����"
		Count_PAUSE_SEC = (uint16_t)SETVAL_DELAY_PAUSE * 60 + 1;
		FlapTravel(FLAP_START, 0);
	}
	if(new == 1){
		BOILER_STATUS = NEW_BOILER_STATUS;	//������� ����� ������
		return;
	}
	
	// ���� ��������� �� ����� ������������ ������� ���������
	if(BOILER_STATUS == BOILER_STATUS_IDLE){	//��������
		if(SETVAL_AUTO_HEAT == DEF_SETVAL_AUTO_HEAT_ON){
			SetFont(font_Tahoma20);
			if(dbDS_RETURN <= SETVAL_TEMP_TEN_ON && BitIsClr(PIN_PHEAT,B_PHEAT)) SetBit(PORT_PHEAT,B_PHEAT);	//�������� ��������
			if(dbDS_RETURN >= SETVAL_TEMP_TEN_OFF && BitIsSet(PIN_PHEAT,B_PHEAT)) ClrBit(PORT_PHEAT,B_PHEAT);	//��������� ��������
			if(BitIsSet(PIN_PHEAT,B_PHEAT)){
				SetColor(VGA_RED);
				DrawString("�������� ����.", 5, 341);		//str, poz X, poz Y
			}else{
				SetColor(VGA_GREEN);
				DrawString("���� ���������.", 5, 341);		//str, poz X, poz Y
			}
			return;
		}
	}else if(BitIsSet(PIN_PHEAT,B_PHEAT)) ClrBit(PORT_PHEAT,B_PHEAT);	//��������� ��������
	
	if(BOILER_STATUS == BOILER_STATUS_AUTO){	//������ �������
		uint8_t high = 0, low = 0;
		//��������� ������� ������� �����������
		if(VALUE_TC < (SETVAL_TEMP_SMOKE - SETVAL_SMOKE_DEAD_BAND)){	//��������
			low = 1;
			//�������� ��������� ���� �� �� � ��� �������
			if(EXTREMUM > SETVAL_TEMP_SMOKE) EXTREMUM = SETVAL_TEMP_SMOKE - SETVAL_SMOKE_DEAD_BAND;
		}
		if(VALUE_TC > (SETVAL_TEMP_SMOKE + SETVAL_SMOKE_DEAD_BAND)){	//��������
			high = 1;
			//�������� ��������� ���� �� �� � ��� �������
			if(EXTREMUM < SETVAL_TEMP_SMOKE) EXTREMUM = SETVAL_TEMP_SMOKE + SETVAL_SMOKE_DEAD_BAND;
		}
		//�������� ������� ������������
		if(ALARM_TRIGGER == ALARM_TRIGGER_OFF && VALUE_TC >= SETVAL_TEMP_SMOKE) ALARM_TRIGGER = ALARM_TRIGGER_ON;
		//��������� ���������
		if(low && VALUE_TC <= (EXTREMUM - DELTA_EXTREMUM)){
			EXTREMUM = VALUE_TC;
			FlapTravel(FLAP_FORVARD, SETVAL_FLAP_AUTO_STEP);
		}
		if(high && VALUE_TC >= (EXTREMUM + DELTA_EXTREMUM)){
			EXTREMUM = VALUE_TC;
			FlapTravel(FLAP_REVERCE, SETVAL_FLAP_AUTO_STEP);
		}
		//������������
		if(SETVAL_ALARM_FUEL_ON == DEF_SETVAL_ALARM_FUEL_ON && ALARM_TRIGGER == ALARM_TRIGGER_ON){
			if(VALUE_TC <= (uint16_t)SETVAL_TEMP_ALARM_FUEL){
				uint8_t c = 3;
				while(c--){
					SetBit(PORT_BEEP, B_BEEP);
					_delay_ms(100);
					ClrBit(PORT_BEEP, B_BEEP);
					_delay_ms(100);
				}
				ALARM_TRIGGER = ALARM_TRIGGER_OFF;
			}
		}
	}
	
	if(BOILER_STATUS == BOILER_STATUS_PAUSE){
		Count_PAUSE_SEC--;
		if(Count_PAUSE_SEC == 0 || BitIsSet(STATUS,ST_WORK_PAUSE_STOP)){	//������� �� �����
			BOILER_STATUS = BOILER_STATUS_UNKNOWN;
			ClrBit(STATUS,ST_WORK_PAUSE_STOP);
		}
		uint16_t nx;
		SetFont(font_Tahoma10);
		SetColor(VGA_YELLOW);
		nx = DrawString("����� �� �����:  ", 70, 400);
		nx = OutNumber(Count_PAUSE_SEC, nx, 400);
		DrawString(" ���     ", nx, 400);
	}
	
// SetFont(font_Tahoma10);
// SetColor(VGA_YELLOW);
// nx = DrawString("EXTREMUM = ", 30, 400);
// nx = OutBigNumber(EXTREMUM, nx, 400);
// DrawString("    ", nx, 400);

}





//===============================================================================
/**���������� ��� ������ � �������� ����������� DS18B20
��������� ��������� ������ ������� 12-������ � ��� �������������� ����������� ������� ���� 750��
������ ���������� ���� �� ���� 800��
9-������ ��������������� - 94��, �������� 0,5��

��������� ������ � ��������:
�������� ������� �������������� �����������:
	������������� (�������� ������)
	�������� ������� DS18B20_SKIP_ROM		0xCC ��������� � ������������� �� ���� ���������� ��� �������� ��� ������
	�������� ������� DS18B20_CONVERT_T		0x44 ����� �������������� �����������

������ �����������:
	������������� (�������� ������)
	�������� ������� DS18B20_SKIP_ROM		0xCC ��������� � ������������� �� ���� ���������� ��� �������� ��� ������
	�������� ������� DS18B20_R_SCRATCHPAD	0xBE ������ ����������� ������ (���������)
	������ 
	��������� ����������

*/
uint8_t DS18B20_init(uint8_t dat){	//Out: 0 - OK; 1 - ��� �������
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
	
	if(BitIsClr((*PIN_Reg), DS_BIT)) return 1; //�������� ������� 1 � �����, ���� ��� - ��� error
	SetBit((*DDR_Reg), DS_BIT);	//����� ����� � 0
	_delay_us(500);
	ClrBit((*DDR_Reg), DS_BIT);	//��������� �����
	_delay_us(60);
	
	d = 0;
	i = 240;
	while(i--){
		_delay_us(1);
		if(BitIsClr((*PIN_Reg), DS_BIT)){d = 1; break;}	//PRESENCE �� ������� �������
	}
	if(d == 0) return 1;
	_delay_us(480);
	return 0;
}
uint8_t DS18B20_send(uint8_t dat, uint8_t sbyte){	//���� ������ ����� 0�FF, �� �� ������ ����� �������� ����
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
		SetBit((*DDR_Reg), DS_BIT);	//����� ����� � 0
		_delay_us(1);
		if(sbyte & 0x01) ClrBit((*DDR_Reg), DS_BIT);	//��������� �����
		sbyte >>= 1;
		_delay_us(10);
		ans >>= 1;
		if(BitIsSet((*PIN_Reg), DS_BIT)) ans |= 0x80;
		_delay_us(90);
		ClrBit((*DDR_Reg), DS_BIT);	//��������� �����
		_delay_us(10);
	}
	return ans;
}
uint8_t DS18B20_read(uint8_t dat){	//��������� 9 ������ � �����, ������������ CRC. Out: 0 - OK, 1 - error CRC
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
// ����� ������ ������� CRC ���������� ��������
// ��� ��������� ������ �������� 8 ���
// ��� ������ ������� 9 ���
// ���� � ���������� crc == 0, �� ������ �������
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
// void DS18B20_SetResolution(uint8_t numBit){	//��������� ���������� �������, �� ��������� ������������ ���������� 12 ���
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
void DS18B20_convertTemp(uint8_t dat){	//�������� ������� �������������� �����������.
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
void DS18B20_getTemp(uint8_t dat){	//������, ��������� � ���������� �����������
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
// uint8_t OutBigNumber(uint16_t val){
	// uint8_t len_char = 0;
	// uint16_t d;
	// uint8_t fnam = 0;	//������ �������� �����
	
	// d = val / 10000;
	// if(d != 0){DrawChar((d + 0x30), WHITE, BLACK, X3); len_char++; fnam = 1;}
	// val = val % 10000;
	
	// d = val / 1000;
	// if(d != 0){DrawChar((d + 0x30), WHITE, BLACK, X3); fnam = 1;}
	// else if(fnam == 1) DrawChar(0x30, WHITE, BLACK, X3);
	// val = val % 1000;
	
	// d = val / 100;
	// if(d != 0){DrawChar((d + 0x30), WHITE, BLACK, X3); fnam = 1;}
	// else if(fnam == 1) DrawChar(0x30, WHITE, BLACK, X3);
	// val = val % 100;
	
	// d = val / 10;
	// if(d != 0){DrawChar((d + 0x30), WHITE, BLACK, X3); fnam = 1;}
	// else if(fnam == 1) DrawChar(0x30, WHITE, BLACK, X3);
	// d = val % 10;
	
	// DrawChar((d + 0x30), WHITE, BLACK, X3);
	// len_char += 4;
	
	// return len_char;
// }
// uint8_t CalculatePercent(uint8_t diskr){
	// double val;
	// val = (100.0 * diskr) / 255;
	// return (uint8_t)val;
// }
// void Work_T12_M12(void){
	// if(BitIsSet(STATUS, ST_M12_START)){	//������ ������������ �1 � �2
		// // �������� ��������� ������������
		// if(ui8_PWM2_LEVEL == 0){	//�2 ����������
			// if(ui8_PWM1_LEVEL == 0) ui8_M12_STATUS = 0;	//�1 � �2 �����������
			// else if(ui8_PWM1_LEVEL == 255) ui8_M12_STATUS = 2; //�1 � 100% ��������, �2 ����������
			// else ui8_M12_STATUS = 1;	//�1 � �������� �������, �2 ����������
		// }else{
			// if(ui8_PWM2_LEVEL == 255) ui8_M12_STATUS = 4;	//�1 � 100% ��������, �2 � 100% ��������
			// else ui8_M12_STATUS = 3;	//�1 � 100% ��������, �2 � �������� �������
		// }
		
		// switch(ui8_M12_STATUS){
			// case 0:{
				// dbl_PWM1_LEVEL = ui8_PWM1_Start_Discret;
				// ui8_PWM1_LEVEL = ui8_PWM1_Start_Discret;
				// PWM_send(PWM1, ui8_PWM1_LEVEL);
				// break;
			// }
			// case 1:{
				// dbl_PWM1_LEVEL += dbl_PWM1_Step_Start_Discret;
				// if(dbl_PWM1_LEVEL >= 255.0){
					// ui8_PWM1_LEVEL = 255;
					// ui16_M12_CNT = 0;
				// }else ui8_PWM1_LEVEL = (uint8_t)dbl_PWM1_LEVEL;
				// PWM_send(PWM1, ui8_PWM1_LEVEL);
				// break;
			// }
			// case 2:{
				// ui16_M12_CNT++;
				// if(ui16_M12_CNT >= M12_BETWEEN_START_PERIOD){
					// dbl_PWM2_LEVEL = ui8_PWM2_Start_Discret;
					// ui8_PWM2_LEVEL = ui8_PWM2_Start_Discret;
					// PWM_send(PWM2, ui8_PWM2_LEVEL);
					// ui16_M12_CNT = 0;
				// }
				// break;
			// }
			// case 3:{
				// dbl_PWM2_LEVEL += dbl_PWM2_Step_Discret;
				// if(dbl_PWM2_LEVEL >= 255.0){
					// ui8_PWM2_LEVEL = 255;
				// }else ui8_PWM2_LEVEL = (uint8_t)dbl_PWM2_LEVEL;
				// PWM_send(PWM2, ui8_PWM2_LEVEL);
				// break;
			// }
			// case 4:{
				// ClrBit(STATUS, ST_M12_START);
				// break;
			// }
			// default: break;
		// }
	// }
	// if(BitIsSet(STATUS, ST_M12_STOP)){	//��������� ������������ �1 � �2
		// // �������� ��������� ������������
		// if(ui8_PWM2_LEVEL == 0){	//�2 ����������
			// if(ui8_PWM1_LEVEL == 0) ui8_M12_STATUS = 3;	//�1 � �2 �����������
			// else if(ui8_PWM1_LEVEL == 255) ui8_M12_STATUS = 1; //�1 � 100% ��������, �2 ����������
			// else ui8_M12_STATUS = 2;	//�1 � �������� ����������, �2 ����������
		// }else ui8_M12_STATUS = 0;	//�1 � 100% ��������, �2 ��������

		// switch(ui8_M12_STATUS){
			// case 0:{
				// dbl_PWM2_LEVEL = 0.0;
				// ui8_PWM2_LEVEL = 0;
				// PWM_send(PWM2, ui8_PWM2_LEVEL);
				// ui16_M12_CNT = 0;
				// break;
			// }
			// case 1:{
				// ui16_M12_CNT++;
				// if(ui16_M12_CNT >= M12_BETWEEN_STOP_PERIOD){
					// ui16_M12_CNT = 0;
					// dbl_PWM1_LEVEL -= dbl_PWM1_Step_Stop_Discret;
					// ui8_PWM1_LEVEL = (uint8_t)dbl_PWM1_LEVEL;
					// if(ui8_PWM1_LEVEL == 255) ui8_PWM1_LEVEL -= 1;
					// PWM_send(PWM1, ui8_PWM1_LEVEL);
				// }
				// break;
			// }
			// case 2:{
				// dbl_PWM1_LEVEL -= dbl_PWM1_Step_Stop_Discret;
				// ui8_PWM1_LEVEL = (uint8_t)dbl_PWM1_LEVEL;
				// if(ui8_PWM1_LEVEL <= ui8_PWM1_Start_Discret){
					// dbl_PWM1_LEVEL = 0.0;
					// ui8_PWM1_LEVEL = 0;
				// }
				// PWM_send(PWM1, ui8_PWM1_LEVEL);
				// break;
			// }
			// case 3:{
				// ClrBit(STATUS, ST_M12_STOP);
				// break;
			// }
			// default: break;
		// }
	// }
	// if(STATUS_DS == 1 && STATUS_TC1 == 1 && ((VALUE_TC1 - (int16_t)DS_Temp) >= (int16_t)SETVAL_DELTA_TEMP_SHUTDOWN)){
		// SetBit(STATUS, ST_WORK_T12_M12);
		// uint8_t yesRoom, yesOven, noRoom, noOven;
		
		// if((int8_t)DS_Temp <= (SETVAL_TEMP_ROOM - SETVAL_DELTA_TEMP_ROOM)) yesRoom = 1; else yesRoom = 0;
		// if(VALUE_TC1 <= (SETVAL_TEMP_TOP_OVEN - SETVAL_DELTA_TEMP_TOP_OVEN)) yesOven = 1; else yesOven = 0;
		// if((int8_t)DS_Temp >= SETVAL_TEMP_ROOM) noRoom = 1; else noRoom = 0;
		// if(VALUE_TC1 >= SETVAL_TEMP_TOP_OVEN) noOven = 1; else noOven = 0;
		
		// //������� ������� ������������ 1 �������(�1 � �2)
		// if(yesRoom && yesOven){
			// SetBit(STATUS, ST_M12_START);
		// }else{
			// ClrBit(STATUS, ST_M12_START);
		// }
		// //������� ��������� ������������ 1 �������(�1 � �2)
		// if(noRoom || noOven){
			// SetBit(STATUS, ST_M12_STOP);
		// }else{
			// ClrBit(STATUS, ST_M12_STOP);
		// }
	// }else{
		// if(BitIsClr(STATUS, ST_M12_STOP) && ui8_PWM1_LEVEL != 0){
			// ClrBit(STATUS, ST_M12_START);
			// SetBit(STATUS, ST_M12_STOP);
		// }
		// ClrBit(STATUS, ST_WORK_T12_M12);
	// }
// }
// void Work_T3_M3(void){
	// if(STATUS_TC2 == 1 && VALUE_TC2 >= SETVAL_TEMP_PYROLYS_START){
		// uint16_t workTemp;
		// double diskret;
		// uint8_t level;
		
		// if(BitIsClr(STATUS, ST_WORK_T3_M3)) SetBit(STATUS, ST_WORK_T3_M3);
		// if(VALUE_TC2 >= SETVAL_TEMP_PYROLYS_FULL) diskret = 255.0;
		// else{
			// workTemp = VALUE_TC2 - SETVAL_TEMP_PYROLYS_START;
			// diskret = workTemp * dbl_PWM3_Step_Discret + ui8_PWM3_Start_Discret;
		// }
		// if(diskret > 255.0) level = 0xFF; else level = (uint8_t)diskret;
		// if(ui8_PWM3_LEVEL != level){
			// ui8_PWM3_LEVEL = level;
			// PWM_send(PWM3, ui8_PWM3_LEVEL);
		// }
	// }else{
		// if(ui8_PWM3_LEVEL != 0){
			// ui8_PWM3_LEVEL = 0;
			// PWM_send(PWM3, ui8_PWM3_LEVEL);
		// }
		// if(BitIsSet(STATUS, ST_WORK_T3_M3)) ClrBit(STATUS, ST_WORK_T3_M3);
	// }
// }
			// ClrScr();
			// ClrBit(PORT_CS, B_CS);
	
			// LCD_Write_COM(0x2A);		//Column
			// LCD_Write_DATA16(0x00,0x00);
			// LCD_Write_DATA16(0x01,0x3F);	//319
			// LCD_Write_COM(0x2B);		//Row
			// LCD_Write_DATA16(0x00,0x00);
			// LCD_Write_DATA16(0x01,0xDF);	//479
			// LCD_Write_COM(0x2C);
			
			// uint32_t i;
			// for(i = 0; i < 153600;i++){	//320*480=153600
				// LCD_Write_DATA16((VGA_WHITE >> 8),(VGA_WHITE & 0xFF));		
			// }
			// SetBit(PORT_CS, B_CS);
			// _delay_ms(1000);
			// // ClrBit(PORT_CS, B_CS);
	
			// // LCD_Write_COM(0x2C);
			
			// // uint16_t i;
			// // for(i = 0; i < 100;i++){	//320*480=153600
				// // // LCD_Write_DATA16((VGA_WHITE >> 8),(VGA_WHITE & 0xFF));		
				// // // LCD_Write_DATA16((VGA_BLACK >> 8),(VGA_BLACK & 0xFF));		
				// // LCD_Write_DATA16((VGA_SILVER >> 8),(VGA_SILVER & 0xFF));		
			// // }
			
			// // SetBit(PORT_CS, B_CS);
			// // uint8_t res;
			
			// // ClrBit(PORT_CS, B_CS);
			// // LCD_Write_COM(0xD3);
			// // DDR_DATA = 0x00;
			// // res = LCD_Read_DATA8();
			// // while(BitIsClr(UCSRA,UDRE)){};
			// // UDR = res;
			
			// // res = LCD_Read_DATA8();
			// // while(BitIsClr(UCSRA,UDRE)){};
			// // UDR = res;
			
			// // res = LCD_Read_DATA8();
			// // while(BitIsClr(UCSRA,UDRE)){};
			// // UDR = res;
			
			// // res = LCD_Read_DATA8();
			// // while(BitIsClr(UCSRA,UDRE)){};
			// // UDR = res;
			
			
			
			// // SetBit(PORT_CS, B_CS);
			// // DDR_DATA = 0xFF;
			
			// // _delay_ms(1000);
			// ClrBit(PORT_CS, B_CS);
	
			// LCD_Write_COM(0x2A);		//Column
			// LCD_Write_DATA16(0x00,0x00);
			// // LCD_Write_DATA16(0x01,0x3F);	//319
			// LCD_Write_DATA16(0x01,0x3F);	//319
			// LCD_Write_COM(0x2B);		//Row
			// LCD_Write_DATA16(0x00,0x00);
			// // LCD_Write_DATA16(0x00,0xEF);	//239
			// LCD_Write_DATA16(0x01,0xDF);	//479
			// LCD_Write_COM(0x2C);
			
			// // SetBit(PORT_RS, B_RS);
			// // PORT_DATA = 0xE2;
			// uint32_t i;
			// for(i = 0; i < 153600;i++){	//320*480=153600
				// // ClrBit(PORT_WR, B_WR);
				// // SetBit(PORT_WR, B_WR);
				// // LCD_Write_DATA16((VGA_WHITE >> 8),(VGA_WHITE & 0xFF));		
				// // LCD_Write_DATA16((VGA_BLACK >> 8),(VGA_BLACK & 0xFF));		
				// LCD_Write_DATA16((VGA_BLUE >> 8),(VGA_BLUE & 0xFF));		
				// // LCD_Write_DATA16(0x1F,0x04);				
				// // LCD_Write_DATA16(0x00,0x1F);				
				// // LCD_Write_DATA16(0,0x1F);
				// // LCD_Write_DATA16(0x0F,0xF0);
			// }
			// // LCD_Write_COM(0x2C);
			// // LCD_Write_COM(0x2C);
			// SetBit(PORT_CS, B_CS);
			
			// // uint16_t xx;
			// SetFont(font_Tahoma20);
			
			// char* str1 = "Hello baby!";
			// DrawString(str1, 10, 20);
			// char* str2 = "����������� +25`C";
			// SetFont(font_Tahoma20);
			// DrawString(str2, 10, 52);
			
			// SetFont(font_Tahoma10);
			// char* str3 = "������������� ������������";
			// DrawString(str3, 10, 100);
			// char* str4 = "����������� +25`C";
			// DrawString(str4, 10, 120);
			
			// SetFont(font_Tahoma36num);
			// char* str5 = "350";
			// DrawString(str5, 50, 150);
			
			// DrawRectFill(0, 320, 250, 255, VGA_RED);	//start X, stop X, start Y, stop Y, color
			// // SetFont(font_Tahoma10bold);
			// // DrawString(str1, 10, 70);
			// // DrawString(str2, 10, 86);
			
			// _delay_ms(1000);
// void PWM_send(uint8_t numPWM, uint8_t level){
	// cli();	//���� ����������
	// PWM_sendByte(numPWM);
	// PWM_sendByte(level);
	// PWM_sendByte((numPWM + level));
	// sei();	//�������� ����������
	// _delay_ms(1);
// }
// void PWM_sendByte(uint8_t data){
	// uint8_t i;
	
	// i = 8;
	// while(i--){
		// if(data & 0x80) SetBit(PortPWM,PinPWM_MOSI); else ClrBit(PortPWM,PinPWM_MOSI);
		// _delay_us(10);	
		// SetBit(PortPWM,PinPWM_SCK);
		// _delay_us(20);	
		// ClrBit(PortPWM,PinPWM_SCK);
		// data <<= 1;
		// _delay_us(10);	
	// }
	// // _delay_us(100);
// }
//===========================================================================
//===========================================================================


