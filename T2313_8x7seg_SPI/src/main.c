#include "main.h"
//===============================================================================
int main(void){  
    init();

	// _delay_ms(500);
	// SEGM_RAZR[2] = 0b00000100;
	// SEGM_RAZR[3] = 0b00001000;
	// SEGM_RAZR[7] = DigToSeg(0);
	// SEGM_RAZR[6] = DigToSeg(1);
	// SEGM_RAZR[5] = DigToSeg(2); RAZR_REGIM[5] = 1;
	// SEGM_RAZR[4] = DigToSeg(3);
	// SEGM_RAZR[3] = DigToSeg(4);
	// SEGM_RAZR[2] = DigToSeg(5);
	// SEGM_RAZR[1] = DigToSeg(6);
	// SEGM_RAZR[0] = DigToSeg(7);
	
	// SEGM_RAZR[7] = DigToSeg(8);
	// SEGM_RAZR[6] = DigToSeg(9);
	// SEGM_RAZR[5] = DigToSeg(10);
	// SEGM_RAZR[4] = DigToSeg(11);
	// SEGM_RAZR[3] = DigToSeg(12);
	// SEGM_RAZR[2] = DigToSeg(13);
	// SEGM_RAZR[1] = DigToSeg(14);
	// SEGM_RAZR[0] = DigToSeg(15);
	
    while(1){
		if(BitIsSet(FLAGS1,FL1_0_OVER)){	//ожидание следующего бита превышено
			CNT_BITS = 0;
			BUFFER = 0;
			ClrBit(FLAGS1,FL1_0_OVER);
		}
		if(BitIsSet(FLAGS1,FL1_REC_BITS)){	//Принято нужное колличество бит
			AnalizBUFFER();
			CNT_BITS = 0;
			ClrBit(FLAGS1,FL1_REC_BITS);
		}
		if(BitIsSet(FLAGS1,FL1_BLC1)){	//тик ~1.024mc вывод очередного разряда индикации
			Display();
			ClrBit(FLAGS1,FL1_BLC1);
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
	DDRA = 0;
	PORTA = 0;
	
	// Port B
	DDRB = 0b11111111;
	PORTB = 0;
	
	// Port D
	DDRD |= (1 << PinSEGM_C)|(1 << PinSEGM_D)|(1 << PinSEGM_E);
	DDRD &= ~((1 << PinIN_C)|(1 << PinIN_D));
	PORTD = 0;
	//0(8бит)таймер - ожидание следующего приемного бита ~2мс
	TCCR0A = 0;
	TCCR0B = 0;	//пуск потом
	TIMSK |= (1 << TOIE0);   // разрешить прерывание по переполнению 2(8разр) счетчика
	// запускать будем потом: TCCR0B = (1 << CS01);   // (1М)1мкс*256*8 = ~2мс (переполнение счетчика)
	//1(16бит)таймер - высвечивание очередного разряда ~1.024mc = 100Гц для 8 разрядов
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS10 | 1 << CS11);	//(1М)1мкс*64*16 = ~1.024mc
	OCR1AH = 0;
	OCR1AL = 0x0010;
	TIMSK |= (1 << OCIE1A);   // разрешить прерывание по совпадению А 1(16разр) счетчика

	//Прерывание по растущему фронту INT0
	MCUCR |= (1 << ISC01)|(1 << ISC00);
	GIMSK |= (1 << INT0);
	
	sei();
}
ISR(INT0_vect){	//Растущий фронт CLK
	TCNT0 = 0;
	TCCR0B |= (1 << CS01);   //запуск (перезапуск) счетчика
	
	BUFFER <<= 1;
	if(BitIsSet(PIND,PinIN_D)) BUFFER += 1;
	CNT_BITS++;
	if(CNT_BITS >= 24) SetBit(FLAGS1, FL1_REC_BITS);
}
ISR(TIMER0_OVF_vect){	//~2мс
	TCCR0B = 0;   //стоп счетчика
	SetBit(FLAGS1, FL1_0_OVER);
}
ISR(TIMER1_COMPA_vect){	//~1.024mc
	TCNT1H = 0;			// сброс счетных регистров
	TCNT1L = 0;
	SetBit(FLAGS1, FL1_BLC1);
	BLINK250++;
	if(BLINK250 == 250){
		BLINK250 = 0;
		InvBit(FLAGS1, FL1_BLC250);
	}
}
void AnalizBUFFER(void){
	// SEGM_RAZR[6] = DigToSeg(CNT_BITS / 10);
	// SEGM_RAZR[5] = DigToSeg(CNT_BITS % 10);
	// SEGM_RAZR[2] = DigToSeg(BUFFER & 0x000000FF);
	
	
	
	// return;
	
	
	uint8_t cb;	//командный байт
	uint8_t db;	//байт данных
	uint8_t ks;	//контрольная сумма
	uint8_t rz;	//разряд индикатора
	uint8_t frm;	//формат данных
	
	cb = (BUFFER >> 16) & 0x000000FF;
	db = (BUFFER >> 8) & 0x000000FF;
	ks = BUFFER & 0x000000FF;
	BUFFER = 0;
	if((uint8_t)(cb + db) != ks) return;
	rz = cb & 0b00000111;
	if(cb & 0b00001000) RAZR_REGIM[rz] = 1; else RAZR_REGIM[rz] = 0;
	frm = (cb >> 4) & 0b00001111;
	switch(frm){
		case 0:{
			SEGM_RAZR[rz] = db;
			break;
		}
		case 1:{
			SEGM_RAZR[rz] = DigToSeg(db);
			break;
		}
	}
}
uint8_t DigToSeg(uint8_t di){
	switch(di){
		case 0: return(0b11111100);		// 0
		case 1: return(0b01100000);		// 1
		case 2: return(0b11011001);		// 2
		case 3: return(0b11110001);		// 3
		case 4: return(0b01100101);		// 4
		case 5: return(0b10110101);		// 5
		case 6: return(0b10111101);		// 6
		case 7: return(0b11100000);		// 7
		case 8: return(0b11111101);		// 8
		case 9: return(0b11110101);		// 9
		case 10: return(0b11101101);	// A
		case 11: return(0b00111101);	// b
		case 12: return(0b10011100);	// C
		case 13: return(0b01111001);	// d
		case 14: return(0b10011101);	// E
		case 15: return(0b10001101);	// F
		case 16: return(0b11001101);	// P
		default: return(0b11111111);	// 
	}
/**      
Биты - Сегменты
	7  6  5  4  3  2  1  0
	A  B  C  D  E  F  dt G
*/
}
void Display(void){
	uint8_t i,byte;
	
	IND_RAZR++;
	if(IND_RAZR == 8) IND_RAZR = 0;
	
	byte = SEGM_RAZR[IND_RAZR];
	PORTB = 0;	//гасим все разряды
	for(i = 0;i < 8;i++){
		ClrBit(PortSEGM,PinSEGM_C);	//опускаем CLK
		if(byte & 0b00000001) SetBit(PortSEGM,PinSEGM_D); else ClrBit(PortSEGM,PinSEGM_D);	//устанавливаем данные
		SetBit(PortSEGM,PinSEGM_C);	//поднимаем CLK
		ClrBit(PortSEGM,PinSEGM_C);	//опускаем CLK
		byte >>= 1;
	}
	SetBit(PortSEGM,PinSEGM_E);	//поднимаем CE
	ClrBit(PortSEGM,PinSEGM_E);	//опускаем CE
	
	switch(IND_RAZR){
		case 0:{
			if(RAZR_REGIM[0] == 0 || (RAZR_REGIM[0] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_8);
			break;
		}
		case 1:{
			if(RAZR_REGIM[1] == 0 || (RAZR_REGIM[1] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_7);
			break;
		}
		case 2:{
			if(RAZR_REGIM[2] == 0 || (RAZR_REGIM[2] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_6);
			break;
		}
		case 3:{
			if(RAZR_REGIM[3] == 0 || (RAZR_REGIM[3] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_5);
			break;
		}
		case 4:{
			if(RAZR_REGIM[4] == 0 || (RAZR_REGIM[4] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_4);
			break;
		}
		case 5:{
			if(RAZR_REGIM[5] == 0 || (RAZR_REGIM[5] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_3);
			break;
		}
		case 6:{
			if(RAZR_REGIM[6] == 0 || (RAZR_REGIM[6] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_2);
			break;
		}
		case 7:{
			if(RAZR_REGIM[7] == 0 || (RAZR_REGIM[7] == 1 && BitIsClr(FLAGS1, FL1_BLC250))) SetBit(PortRAZR,PinRAZR_1);
			break;
		}
	}
}
//===============================================================================
	// SetBit(FLAGS1, FL1_BLC2);
// void CheckBVK(void){
	// //джампер одет - 0 => на соответствующем входе если 1 - сигнальное состояние
	
	// if(BitIsClr(PortPinBVK,PinBVK_TJ)) if(BitIsSet(PortPinBVK,PinBVK_T)) BVK_T = 0; else BVK_T = 1;
	// else if(BitIsClr(PortPinBVK,PinBVK_T)) BVK_T = 0; else BVK_T = 1;

	// if(BitIsClr(PortPinBVK,PinBVK_LJ)) if(BitIsSet(PortPinBVK,PinBVK_L)) BVK_L = 0; else BVK_L = 1;
	// else if(BitIsClr(PortPinBVK,PinBVK_L)) BVK_L = 0; else BVK_L = 1;

	// if(BitIsClr(PortPinBVK,PinBVK_RJ)) if(BitIsSet(PortPinBVK,PinBVK_R)) BVK_R = 0; else BVK_R = 1;
	// else if(BitIsClr(PortPinBVK,PinBVK_R)) BVK_R = 0; else BVK_R = 1;
// }
// void ExecuteBVK(void){
	// if((LEFT == 1 && RIGTH == 1) || (BVK_L == 1 && BVK_R == 1)){	//ошибка. Оба положения или оба движения вместе недопустимы.
		// LEFT = 0;
		// RIGTH = 0;
		// STATUS = 0;
		// ClrBit(PortKL,PinKL_L);
		// ClrBit(PortKL,PinKL_R);
		// return;
	// }
	// if(BVK_L == 0 && BVK_R == 0) return;	//шток не дошел до крайнего положения
	// if(STATUS == 0){	//если не "в работе"
		// if(LEFT == 0 && RIGTH == 0 && BVK_T == 1){
			// if(BVK_L == 1){
				// RIGTH = 1;
				// STATUS = 1;
				// SetBit(PortKL,PinKL_R);
			// }
			// if(BVK_R == 1){
				// LEFT = 1;
				// STATUS = 1;
				// SetBit(PortKL,PinKL_L);
			// }
		// }
	// }else if(STATUS == 1){	//если "в работе"
		// if(RIGTH == 1 && BVK_R == 1){
			// _delay_ms(500);
			// STATUS = 2;	//"отработал ход"
			// RIGTH = 0;
			// ClrBit(PortKL,PinKL_R);
			// _delay_ms(500);
		// }
		// if(LEFT == 1 && BVK_L == 1){
			// _delay_ms(500);
			// STATUS = 2;	//"отработал ход"
			// LEFT = 0;
			// ClrBit(PortKL,PinKL_L);
			// _delay_ms(500);
		// }
	// }else{	//если "отработал ход" ждем когда выключится БВК транспортера
		// if(BVK_T == 0){
			// STATUS = 0;	//цикл завершился, начинаем сначала
			// _delay_ms(500);
		// }
	// }
// }	

		// _delay_ms(250);
		// ClrBit(PortKL,PinKL_L);
		// SetBit(PortKL,PinKL_R);
		// _delay_ms(250);
		// ClrBit(PortKL,PinKL_R);
		// SetBit(PortKL,PinKL_L);
		
	// if(BVK_L == 1) SetBit(PortKL,PinKL_L); else ClrBit(PortKL,PinKL_L);
	// if(BVK_R == 1) SetBit(PortKL,PinKL_R); else ClrBit(PortKL,PinKL_R);
	// if(BVK_T == 1) {
		// SetBit(PortKL,PinKL_L);
		// SetBit(PortKL,PinKL_R);
	// }else{
		// ClrBit(PortKL,PinKL_L);
		// ClrBit(PortKL,PinKL_R);
	// }
		



