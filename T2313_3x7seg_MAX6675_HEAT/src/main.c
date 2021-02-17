#include "main.h"
//===============================================================================
int main(void){
    init();
	LoadValuesFromEEPROM();

    while(1){
		if(BitIsSet(FLAGS1,FL1_BLC1)){	//тик ~1mc
			CNT_TEMP_START++;
			if(CNT_TEMP_START == TEMP_START){	// ~1sec
				CNT_TEMP_START = 0;
				ReadMAX6675();
			}
			if(BitIsSet(FLAGS1,FL1_MODE_EDIT)){
				EDIT_OUT_CNT++;
				if(EDIT_OUT_CNT >= EDIT_OUT){	//режим коррекции уставки. здесь отслеживаем выход
					ClrBit(FLAGS1,FL1_MODE_EDIT);
					ClrBit(FLAGS1,FL1_MODE_PID);
					UpdateValuesFromEEPROM();	//сохраним изменения
				}
			}else if(STATUS_TC == 1) NumberToButeBuffer(VALUE_TC);
			else{
				SEGM_RAZR[0] = 10;
				SEGM_RAZR[1] = 10;
				SEGM_RAZR[2] = 10;
			}
			CheckButtons();
			if(BUTTON) ProcessingButtons();
			Display();
			ClrBit(FLAGS1,FL1_BLC1);
		}
 		if(BitIsSet(FLAGS1,FL1_BLC8)){	//тик ~8mc
			ProcessingPID();
			ClrBit(FLAGS1,FL1_BLC8);
		}
   }
    return 0;
}
//===============================================================================
void init(void){
	SetBit(ACSR, ACD);//отключим аналоговый компаратор	
	// порты
	//DDRx: 0 - вход: 1 - выход		PORTx: 
	// Port A
	DDRA = (1 << PinA_HEAT)|(1 << PinA_SCK);
	PORTA = 0;
	
	// Port B
	DDRB = 0b11111111;
	PORTB = 0;
	
	// Port D
	DDRD |= (1 << PinD_RAZR_1)|(1 << PinD_RAZR_2)|(1 << PinD_RAZR_3)|(1 << PinD_CS);
	DDRD &= ~((1 << PinD_KnPLS)|(1 << PinD_KnMIN)|(1 << PinD_SO));
	PORTD = (1 << PinD_KnPLS)|(1 << PinD_KnMIN)|(1 << PinD_RAZR_1)|(1 << PinD_RAZR_2)|(1 << PinD_RAZR_3);
	// 0(8бит)таймер
	// вариант использования прерывания при переполнении счетного регистра TCCR0A
	// при этом используем прерывание ISR(TIMER0_OVF_vect)
	TCCR0A = 0;
	TIMSK |= (1 << TOIE0);   // разрешить прерывание по переполнению 2(8разр) счетчика
	// TCCR0B = (1 << CS01);   // (8М)0,125мкс*8*256 = ~0,256мс (переполнение счетчика)
	// TCCR0B = (1 << CS00)|(1 << CS01);   // (8М)0,125мкс*64*256 = ~2мс (переполнение счетчика)
	TCCR0B = (1 << CS02);   // (8М)0,125мкс*256*256 = ~8мс (переполнение счетчика)
	// TCCR0B = (1 << CS00)|(1 << CS02);   // (8М)0,125мкс*1024*256 = ~32,7мс (переполнение счетчика)
	
	//1(16бит)таймер
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS10 | 1 << CS11);	//(8М)0,125мкс*64*128 = ~1.024mc
	OCR1AH = 0;
	OCR1AL = 0x0080;
	TIMSK |= (1 << OCIE1A);   // разрешить прерывание по совпадению А 1(16разр) счетчика

	// Настройка UART на 38400 bps(бод)
	// UBRRH = 0;
	// // UBRRL = 12;	//UBRR = 8000000/(16 * 38400) - 1 = 12,02 //при U2X = 0
	// UBRRL = 51;	//UBRR = 8000000/(16 * 9600) - 1 = 51,08 //при U2X = 0
	// UCSRB = (1 << RXEN)|(1 << RXCIE)|(1 << TXCIE);	// вкл приемник(RXEN=1),вкл передатчик(TXEN=1),вкл прерывание по приему(RXCIE=1),вкл прерывание по передаче(TXCIE=1)
	// // Для доступа к регистру UCSRC ОБЯЗАТЕЛЬНО выставить бит URSEL!!!
	// UCSRC = (3 << UCSZ0);	// асинхронный режим(UMSEL=0), 8 бит(UCSZ2-0=011), 1 стоп-бит(USBS=0),без контроля четности(UPM1-0=00)
	
	// (1 << UPM1)| четный(UPM1-0=10)		
	
	//Прерывание по растущему фронту INT0
	// MCUCR |= (1 << ISC01)|(1 << ISC00);
	// GIMSK |= (1 << INT0);
	
	sei();
}
ISR(TIMER1_COMPA_vect){	//~1mc
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	
	SetBit(FLAGS1, FL1_BLC1);
}
ISR(TIMER0_OVF_vect){	//~8mc
	SetBit(FLAGS1, FL1_BLC8);
}
void NumberToButeBuffer(uint16_t number){
	SEGM_RAZR[2] = number / 100;
	number = number % 100;
	SEGM_RAZR[1] = number / 10;
	SEGM_RAZR[0] = number % 10;
	if(BitIsSet(FLAGS1, FL1_MODE_PID)) SEGM_RAZR[2] = 11;
}
void Display(void){
	IND_RAZR++;
	if(IND_RAZR == 3) IND_RAZR = 0;
	
	PORTB = 0;	//гасим все сегменты
	
	switch(IND_RAZR){
		case 0:{
			SetBit(PORTD,PinD_RAZR_1);
			ClrBit(PORTD,PinD_RAZR_3);
			break;
		}
		case 1:{
			SetBit(PORTD,PinD_RAZR_3);
			ClrBit(PORTD,PinD_RAZR_2);
			break;
		}
		case 2:{
			SetBit(PORTD,PinD_RAZR_2);
			ClrBit(PORTD,PinD_RAZR_1);
			break;
		}
	}
	if(IND_RAZR == 2 && SEGM_RAZR[2] == 0){}	//гашение нуля старшего разряда
	else PORTB = pgm_read_byte(&(DigSeg[SEGM_RAZR[IND_RAZR]]));
}
void CheckButtons(void){
	uint8_t bit_button = PIND & ((1 << PinD_KnPLS)|(1 << PinD_KnMIN));
	
	if(bit_button == ((1 << PinD_KnPLS)|(1 << PinD_KnMIN))){	// || bit_button == 0
		SHAKE_CNT = 0;
		EDIT_CNT = 0;
		if(BitIsSet(BUTTON,BTN_SHORT_MAKED)) BUTTON = 0;
	}else{	//что-то нажато
		SHAKE_CNT++;
		if(SHAKE_CNT > BUTTON_PUSH_SHORT_CNT && SHAKE_CNT <= BUTTON_PUSH_LONG_CNT){	//диапазон короткого нажатия
			if(BitIsClr(PIND,PinD_KnPLS)) SetBit(BUTTON, BTN_PLS);
			if(BitIsClr(PIND,PinD_KnMIN)) SetBit(BUTTON, BTN_MIN);
		}else if(SHAKE_CNT > BUTTON_PUSH_LONG_CNT){	//диапазон длинного нажатия
			SetBit(BUTTON, BTN_LONG);
			SHAKE_CNT--;
		}
	}
}
void ProcessingButtons(void){
	EDIT_OUT_CNT = 0;
	
	if(BitIsSet(BUTTON,BTN_PLS) && BitIsSet(BUTTON,BTN_MIN)) SetBit(FLAGS1, FL1_MODE_PID);
	if(BitIsClr(FLAGS1,FL1_MODE_EDIT)){
		EDIT_OUT_CNT = 0;
		SetBit(FLAGS1, FL1_MODE_EDIT);
		SetBit(BUTTON, BTN_SHORT_MAKED);
	}else{
		if(BitIsClr(BUTTON, BTN_LONG) && BitIsClr(BUTTON, BTN_SHORT_MAKED)){
			if(BitIsSet(BUTTON,BTN_PLS)) ChangeVALUE(CHANGE_PLS);	//PLS
			if(BitIsSet(BUTTON,BTN_MIN)) ChangeVALUE(CHANGE_MIN);	//MIN
			SetBit(BUTTON, BTN_SHORT_MAKED);
		}
		
		if(BitIsSet(BUTTON, BTN_LONG)){
			EDIT_CNT++;
			if(EDIT_CNT >= EDIT_CHANGE){
				EDIT_CNT = 0;
				if(BitIsSet(BUTTON,BTN_PLS)) ChangeVALUE(CHANGE_PLS);	//PLS
				if(BitIsSet(BUTTON,BTN_MIN)) ChangeVALUE(CHANGE_MIN);	//MIN
			}
		}
	}
	if(BitIsSet(FLAGS1, FL1_MODE_PID)) NumberToButeBuffer(SETVAL_PID); else NumberToButeBuffer(SETVAL_TEMP);
}
void ChangeVALUE(uint8_t ditect){
	if(BitIsSet(FLAGS1, FL1_MODE_PID)){
		if(ditect == CHANGE_PLS){	//PLS
			SETVAL_PID += DEF_SETVAL_PID_STEP;
			if(SETVAL_PID > DEF_SETVAL_PID_MAX) SETVAL_PID = DEF_SETVAL_PID_MAX;
		}else if(ditect == CHANGE_MIN){	//MIN
			SETVAL_PID -= DEF_SETVAL_PID_STEP;
			if(SETVAL_PID < DEF_SETVAL_PID_MIN) SETVAL_PID = DEF_SETVAL_PID_MIN;
		}
	}else{
		if(ditect == CHANGE_PLS){	//PLS
			SETVAL_TEMP += DEF_SETVAL_TEMP_STEP;
			if(SETVAL_TEMP > DEF_SETVAL_TEMP_MAX) SETVAL_TEMP = DEF_SETVAL_TEMP_MAX;
		}else if(ditect == CHANGE_MIN){	//MIN
			SETVAL_TEMP -= DEF_SETVAL_TEMP_STEP;
			if(SETVAL_TEMP < DEF_SETVAL_TEMP_MIN) SETVAL_TEMP = DEF_SETVAL_TEMP_MIN;
		}
	}
}
void LoadValuesFromEEPROM(void){
	SETVAL_PID = eeprom_read_byte(&EE_SETVAL_PID);
	SETVAL_TEMP = eeprom_read_word(&EE_SETVAL_TEMP);	
}
void UpdateValuesFromEEPROM(void){
	eeprom_update_byte(&EE_SETVAL_PID, SETVAL_PID);
	eeprom_update_word(&EE_SETVAL_TEMP, SETVAL_TEMP);	
}
void ReadMAX6675(void){	// данные сохраняются в свои переменные VALUE_TC и STATUS_TC
/**
PinA_SCK	выдаем изначально без подтяжки
PinD_SO		читаем без подтяжки
PinD_CS		выдаем изначально с подтяжкой

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
	
	ClrBit(PORTD, PinD_CS);
	
	i = 16;
	while(i--){
		SetBit(PORTA, PinA_SCK);
		tmp <<= 1;
		if(BitIsSet(PIND, PinD_SO)) tmp |= 0x0001; else tmp &= 0xFFFE;
		ClrBit(PORTA, PinA_SCK);
	}
	SetBit(PORTD, PinD_CS);
	
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
void CalculatePID(void){
	if(STATUS_TC != 1){
		PID_VAL = 0;
		return;
	}
	int16_t err = SETVAL_TEMP - VALUE_TC;
	err *= SETVAL_PID;
	if(err < 0) PID_VAL = 0;
	else if(err > 255) PID_VAL = 255;
	else PID_VAL = err;
}
void ProcessingPID(void){
	PID_CNT++;
	if(PID_CNT == 255) CalculatePID();
	if(PID_VAL == 0){
		ClrBit(PORTA,PinA_HEAT);	//выключение нагревателя
		return;
	}
	if(PID_CNT > 0) SetBit(PORTA,PinA_HEAT);	//включение нагревателя
	if(PID_CNT > PID_VAL) ClrBit(PORTA,PinA_HEAT);	//выключение нагревателя
}
//===============================================================================
		// ClrBit(PORTA,PinA_HEAT);
		// SetBit(PORTA,PinA_HEAT);


	// uint8_t button_current;
	// uint8_t *pSt;
	
	// //определим текущее состояние кнопок
	// if(BitIsClr(PIND,PinD_KnPLS) && BitIsClr(PIND,PinD_KnMIN)) button_current = 1;
	// else if(BitIsClr(PIND,PinD_KnPLS)) button_current = 2;
	// else if(BitIsClr(PIND,PinD_KnMIN)) button_current = 3;
	// else button_current = 0;
	
	// switch(button_current){
		// case 0: break;
		// case 1: pSt = (uint8_t*)&BTN_BOTH_STATUS; break;
		// case 2: pSt = (uint8_t*)&BTN_PLS_STATUS; break;
		// case 3: pSt = (uint8_t*)&BTN_MIN_STATUS; break;
	// }
	// if(button_current && button_current == BUTTON_OLD){	//что-то нажато
		// SHAKE_CNT++;
		// if(SHAKE_CNT > BUTTON_PUSH_SHORT_CNT && SHAKE_CNT <= BUTTON_PUSH_LONG_CNT){	//диапазон короткого нажатия
			// SetBit(*pSt, BTN_SHORT);
		// }else if(SHAKE_CNT > BUTTON_PUSH_LONG_CNT && SHAKE_CNT <= BUTTON_PUSH_SUPERLONG_CNT){	//диапазон длинного нажатия
			// SetBit(*pSt, BTN_LONG);
		// }else{
			// SHAKE_CNT--;
		// }
	// }else{
		// SHAKE_CNT = 0;
		// BTN_BOTH_STATUS = 0;
		// BTN_PLS_STATUS = 0;
		// BTN_MIN_STATUS = 0;
	// }
	// BUTTON_OLD = button_current;

// //=============================================================
	// if(BitIsSet(BTN_MIN_STATUS,BTN_SHORT) && BitIsClr(BTN_MIN_STATUS,BTN_SHORT_MAKED)){//MIN короткое
		// SetBit(BTN_MIN_STATUS, BTN_SHORT_MAKED);
		// switch(MODE){
			// case MODE_VALUE:{	//режим индикации текущей температуры
				// SetMODE(MODE_TARGET);
			// } break;
			// case MODE_TARGET:{	//режим показа уставки
				// SetMODE(MODE_EDIT);
			// } break;
			// case MODE_EDIT:{	//режим коррекции уставки
				// SETVAL_TEMP -= DEF_SETVAL_TEMP_STEP;
				// if(SETVAL_TEMP < DEF_SETVAL_TEMP_MIN) SETVAL_TEMP = DEF_SETVAL_TEMP_MIN;
				// NumberToButeBuffer(SETVAL_TEMP);
			// } break;
		// }
	// }
	// if(BitIsSet(BTN_MIN_STATUS,BTN_LONG) && BitIsClr(BTN_MIN_STATUS,BTN_LONG_MAKED)){//MIN длинное
		// SetBit(BTN_MIN_STATUS, BTN_LONG_MAKED);
		// switch(MODE){
			// case MODE_VALUE:{	//режим индикации текущей температуры
				// SetMODE(MODE_EDIT);
			// } break;
			// case MODE_TARGET:{	//режим показа уставки
				// SetMODE(MODE_EDIT);
			// } break;
			// case MODE_EDIT:{	//режим коррекции уставки
				// EDIT_CNT++;
				// if(EDIT_CNT == EDIT_CHANGE){
					// EDIT_CNT = 0;
					// SETVAL_TEMP -= DEF_SETVAL_TEMP_STEP;
					// if(SETVAL_TEMP < DEF_SETVAL_TEMP_MIN) SETVAL_TEMP = DEF_SETVAL_TEMP_MIN;
					// NumberToButeBuffer(SETVAL_TEMP);
				// }
			// } break;
		// }
	// }

	// if(BitIsSet(BTN_PLS_STATUS,BTN_SHORT) && BitIsClr(BTN_PLS_STATUS,BTN_SHORT_MAKED)){//PLS короткое
		// SetBit(BTN_PLS_STATUS, BTN_SHORT_MAKED);
		// switch(MODE){
			// case MODE_VALUE:{	//режим индикации текущей температуры
				// SetMODE(MODE_TARGET);
			// } break;
			// case MODE_TARGET:{	//режим показа уставки
				// SetMODE(MODE_EDIT);
			// } break;
			// case MODE_EDIT:{	//режим коррекции уставки
				// SETVAL_TEMP += DEF_SETVAL_TEMP_STEP;
				// if(SETVAL_TEMP > DEF_SETVAL_TEMP_MAX) SETVAL_TEMP = DEF_SETVAL_TEMP_MAX;
				// NumberToButeBuffer(SETVAL_TEMP);
			// } break;
		// }
	// }
	// if(BitIsSet(BTN_PLS_STATUS,BTN_LONG) && BitIsClr(BTN_PLS_STATUS,BTN_LONG_MAKED)){//PLS длинное
		// SetBit(BTN_PLS_STATUS, BTN_LONG_MAKED);
		// switch(MODE){
			// case MODE_VALUE:{	//режим индикации текущей температуры
				// SetMODE(MODE_EDIT);
			// } break;
			// case MODE_TARGET:{	//режим показа уставки
				// SetMODE(MODE_EDIT);
			// } break;
			// case MODE_EDIT:{	//режим коррекции уставки
				// EDIT_CNT++;
				// if(EDIT_CNT == EDIT_CHANGE){
					// EDIT_CNT = 0;
					// SETVAL_TEMP += DEF_SETVAL_TEMP_STEP;
					// if(SETVAL_TEMP > DEF_SETVAL_TEMP_MAX) SETVAL_TEMP = DEF_SETVAL_TEMP_MAX;
					// NumberToButeBuffer(SETVAL_TEMP);
				// }
			// } break;
		// }
