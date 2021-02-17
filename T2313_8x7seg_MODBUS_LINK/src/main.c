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
		// {	//
			// uint8_t i;
			// for(i = 0;i < 8;i++) SEGM_RAZR[i] = 0;
			// // SEGM_RAZR[1] = DigToSeg(4);
			
		// }
		// {	//
			// SEGM_RAZR[1] = DigToSeg(5);
			
		// }
		// {	//
			// SEGM_RAZR[1] = DigToSeg(6);
			
		// }
	// SEGM_RAZR[6] = DigToSeg(9);
	// SEGM_RAZR[5] = DigToSeg(10);
	// SEGM_RAZR[4] = DigToSeg(11);
	// SEGM_RAZR[3] = DigToSeg(12);
	// SEGM_RAZR[2] = DigToSeg(13);
	// SEGM_RAZR[0] = DigToSeg(15);
		// if(BitIsClr(PINKnMNU,PinKnMNU)) SEGM_RAZR[1] = DigToSeg(14);
	
    while(1){
		if(BitIsSet(FLAGS1,FL1_0_OVER)){	//�������� ���������� ���� ���������
			AnalizBUFFER();
			RX_cnt = 0;
			ClrBit(FLAGS1,FL1_0_OVER);
		}
		if(BitIsSet(FLAGS1,FL1_BLC1)){	//��� ~1.024mc ����� ���������� ������� ���������
			Display();
			ClrBit(FLAGS1,FL1_BLC1);
		}
		if(BitIsSet(FLAGS1,FL1_TX)){	//��������� �������� �����
			if(TX_cnt == TX_size){	//�������� ������� ���������
				StartTX_RS232(0);	//��������� ADM485 � ����� ������
			}else{	//�������� ��������� ����
				while(BitIsClr(UCSRA,UDRE)){};
				UDR = TX_BUFFER[TX_cnt];
				TX_cnt++;
			}
			ClrBit(FLAGS1,FL1_TX);
		}
    }
    return 0;
}
//===============================================================================
void init(void){
	SetBit(ACSR, ACD);//�������� ���������� ����������	
	// �����
	//DDRx: 0 - ����: 1 - �����		PORTx: 
	// Port A
	DDRA |= (1 << PinTxRx);
	DDRA &= ~(1 << PinKnMIN);
	PORTA = (1 << PinKnMIN)|(1 << PinTxRx);
	PORTA &= ~(1 << PinTxRx);
	
	// Port B
	DDRB = 0b11111111;
	PORTB = 0;
	
	// Port D
	DDRD |= (1 << PinSEGM_C)|(1 << PinSEGM_D)|(1 << PinSEGM_E)|(1 << PinKnMNU)|(1 << PinKnPLS);
	DDRD &= ~((1 << PinKnMNU)|(1 << PinKnPLS)|(1 << PinTx)|(1 << PinRx));
	PORTD = (1 << PinKnMNU)|(1 << PinKnPLS);
	//0(8���)������ - �������� ���������� ��������� ���� ~2��
	TCCR0A = 0;
	TCCR0B = 0;	//���� �����
	TIMSK |= (1 << TOIE0);   // ��������� ���������� �� ������������ 2(8����) ��������
	// ��������� ����� �����: TCCR0B = (1 << CS02);   // (8�)0,125���*256*256 = ~8�� (������������ ��������)
	// ��������� ����� �����: TCCR0B = (1 << CS00)|(1 << CS01);   // (8�)0,125���*64*256 = ~2�� (������������ ��������)
	
	//1(16���)������ - ������������ ���������� ������� ~1.024mc = 100�� ��� 8 ��������
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = (1 << CS10 | 1 << CS11);	//(8�)0,125���*64*128 = ~1.024mc
	OCR1AH = 0;
	OCR1AL = 0x0080;
	TIMSK |= (1 << OCIE1A);   // ��������� ���������� �� ���������� � 1(16����) ��������

	// ��������� UART �� 38400 bps(���)
	UBRRH = 0;
	// UBRRL = 12;	//UBRR = 8000000/(16 * 38400) - 1 = 12,02 //��� U2X = 0
	UBRRL = 51;	//UBRR = 8000000/(16 * 9600) - 1 = 51,08 //��� U2X = 0
	UCSRB = (1 << RXEN)|(1 << RXCIE)|(1 << TXCIE);	// ��� ��������(RXEN=1),��� ����������(TXEN=1),��� ���������� �� ������(RXCIE=1),��� ���������� �� ��������(TXCIE=1)
	// ��� ������� � �������� UCSRC ����������� ��������� ��� URSEL!!!
	UCSRC = (3 << UCSZ0);	// ����������� �����(UMSEL=0), 8 ���(UCSZ2-0=011), 1 ����-���(USBS=0),��� �������� ��������(UPM1-0=00)
	
	// (1 << UPM1)| ������(UPM1-0=10)		
	
	//���������� �� ��������� ������ INT0
	// MCUCR |= (1 << ISC01)|(1 << ISC00);
	// GIMSK |= (1 << INT0);
	
	sei();
}
ISR(USART_RX_vect){	// ����� ����� ��������
	TCNT0 = 0;
	if(RX_cnt == RX_BUFFER_SIZE){	//�������
		RX_cnt = 0;
		TCCR0B = 0;   //���� ��������
		return;
	}
	RX_BUFFER[RX_cnt] = UDR;
	RX_cnt++;
	TCCR0B = (1 << CS00)|(1 << CS01);   //������ (����������) �������� ~2mc
	// TCCR0B = (1 << CS02);   //������ (����������) �������� ~8mc
}
ISR(USART_TX_vect){	// �������� ����� ���������
	SetBit(FLAGS1, FL1_TX);
}
ISR(TIMER0_OVF_vect){	//~8��/~2mc
	TCCR0B = 0;   //���� ��������
	SetBit(FLAGS1, FL1_0_OVER);
}
ISR(TIMER1_COMPA_vect){	//~1.024mc
	TCNT1H = 0;			// ����� ������� ���������
	TCNT1L = 0;
	
	SetBit(FLAGS1, FL1_BLC1);
	BLINK250++;
	if(BLINK250 == 250){
		BLINK250 = 0;
		InvBit(FLAGS1, FL1_BLC250);
	}
}
void AnalizBUFFER(void){
	uint16_t crc;
	
	crc = CalkCRC16(RX_BUFFER, RX_cnt);
	if(crc != 0) return;
	if(RX_BUFFER[0] != MY_ADDR) return;
	if(RX_BUFFER[1] != 0x17) return;	// 23 ������� ������/������ � ������ ���������
	if(RX_cnt != 29) return;	//������� �� ��������� �� ����� ������
	
	if(RX_BUFFER[2] != 0x15) return;	//��.���� �������� ������ (����� ������ �������� ��� ������ 0x15AA (�������))
	if(RX_BUFFER[3] != 0xAA) return;	//��.���� �������� ������
	
	if(RX_BUFFER[4] != 0x00) return;	//����� ��������� ������ ��.������ (� ��� ����� ������ 00 03)
	if(RX_BUFFER[5] != 0x03) return;	//����� ��������� ������ ��.������
		
	if(RX_BUFFER[6] != 0x17) return;	//��.���� �������� ������ (����� ������ �������� ��� ������ 0x1755 (�������))
	if(RX_BUFFER[7] != 0x55) return;	//��.���� �������� ������
		
	if(RX_BUFFER[8] != 0x00) return;	//����� ��������� ������ ��.������ (� ��� ����� ������ 00 08)
	if(RX_BUFFER[9] != 0x08) return;	//����� ��������� ������ ��.������
		
	if(RX_BUFFER[10] != 0x10) return;	//����� ���� ������ (� ��� ����� ������ 0x10)
	
	if(RX_BUFFER[11] & 0x02) RAZR_REGIM[0] = 1; else RAZR_REGIM[0] = 0;
	if(RX_BUFFER[11] & 0x01) SEGM_RAZR[0] = RX_BUFFER[12]; else SEGM_RAZR[0] = DigToSeg(RX_BUFFER[12]);
	
	if(RX_BUFFER[13] & 0x02) RAZR_REGIM[1] = 1; else RAZR_REGIM[1] = 0;
	if(RX_BUFFER[13] & 0x01) SEGM_RAZR[1] = RX_BUFFER[14]; else SEGM_RAZR[1] = DigToSeg(RX_BUFFER[14]);
	
	if(RX_BUFFER[15] & 0x02) RAZR_REGIM[2] = 1; else RAZR_REGIM[2] = 0;
	if(RX_BUFFER[15] & 0x01) SEGM_RAZR[2] = RX_BUFFER[16]; else SEGM_RAZR[2] = DigToSeg(RX_BUFFER[16]);
	
	if(RX_BUFFER[17] & 0x02) RAZR_REGIM[3] = 1; else RAZR_REGIM[3] = 0;
	if(RX_BUFFER[17] & 0x01) SEGM_RAZR[3] = RX_BUFFER[18]; else SEGM_RAZR[3] = DigToSeg(RX_BUFFER[18]);
	
	if(RX_BUFFER[19] & 0x02) RAZR_REGIM[4] = 1; else RAZR_REGIM[4] = 0;
	if(RX_BUFFER[19] & 0x01) SEGM_RAZR[4] = RX_BUFFER[20]; else SEGM_RAZR[4] = DigToSeg(RX_BUFFER[20]);
	
	if(RX_BUFFER[21] & 0x02) RAZR_REGIM[5] = 1; else RAZR_REGIM[5] = 0;
	if(RX_BUFFER[21] & 0x01) SEGM_RAZR[5] = RX_BUFFER[22]; else SEGM_RAZR[5] = DigToSeg(RX_BUFFER[22]);
	
	if(RX_BUFFER[23] & 0x02) RAZR_REGIM[6] = 1; else RAZR_REGIM[6] = 0;
	if(RX_BUFFER[23] & 0x01) SEGM_RAZR[6] = RX_BUFFER[24]; else SEGM_RAZR[6] = DigToSeg(RX_BUFFER[24]);
	
	if(RX_BUFFER[25] & 0x02) RAZR_REGIM[7] = 1; else RAZR_REGIM[7] = 0;
	if(RX_BUFFER[25] & 0x01) SEGM_RAZR[7] = RX_BUFFER[26]; else SEGM_RAZR[7] = DigToSeg(RX_BUFFER[26]);
		
	// ��������� �����
	TX_BUFFER[0] = MY_ADDR;
	TX_BUFFER[1] = 0x17;
	TX_BUFFER[2] = 0x06;
	// �������� ��������� ������
	if(BitIsClr(PINKnMNU,PinKnMNU)){TX_BUFFER[3] = 0xFF; TX_BUFFER[4] = 0xFF;}else{TX_BUFFER[3] = 0x00; TX_BUFFER[4] = 0x00;}
	if(BitIsClr(PINKnPLS,PinKnPLS)){TX_BUFFER[5] = 0xFF; TX_BUFFER[6] = 0xFF;}else{TX_BUFFER[5] = 0x00; TX_BUFFER[6] = 0x00;}
	if(BitIsClr(PINKnMIN,PinKnMIN)){TX_BUFFER[7] = 0xFF; TX_BUFFER[8] = 0xFF;}else{TX_BUFFER[7] = 0x00; TX_BUFFER[8] = 0x00;}
	crc = CalkCRC16(TX_BUFFER, 9);
	TX_BUFFER[9] = (uint8_t)crc;
	TX_BUFFER[10] = (uint8_t)(crc >> 8);
	
	TX_cnt = 0;
	TX_size = 11;
	
	//�������� ��������
	StartTX_RS232(1);
		
}
uint16_t CalkCRC16(uint8_t* buffer, uint8_t cnt){
	uint8_t i;
	uint16_t crc = 0xFFFF;
	
	while(cnt--){
		crc ^= *buffer++;
		for(i = 0;i < 8;i++){
			if(crc & 0x0001) crc = (crc >> 1) ^ 0xA001;
			else crc = (crc >> 1);
		}
	}
	return crc;
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
���� - ��������
	7  6  5  4  3  2  1  0
	A  B  C  D  E  F  dt G
*/
}
void Display(void){
	uint8_t i,byte;
	
	IND_RAZR++;
	if(IND_RAZR == 8) IND_RAZR = 0;
	
	byte = SEGM_RAZR[IND_RAZR];
	PORTB = 0;	//����� ��� �������
	for(i = 0;i < 8;i++){
		ClrBit(PortSEGM,PinSEGM_C);	//�������� CLK
		if(byte & 0b00000001) SetBit(PortSEGM,PinSEGM_D); else ClrBit(PortSEGM,PinSEGM_D);	//������������� ������
		SetBit(PortSEGM,PinSEGM_C);	//��������� CLK
		ClrBit(PortSEGM,PinSEGM_C);	//�������� CLK
		byte >>= 1;
	}
	SetBit(PortSEGM,PinSEGM_E);	//��������� CE
	ClrBit(PortSEGM,PinSEGM_E);	//�������� CE
	
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
void StartTX_RS232(uint8_t tx){
	if(tx){
		UCSRB |= (1 << TXEN);
		UCSRB &= ~(1 << RXEN);
		SetBit(PortTxRx, PinTxRx);	//��������� ADM485 � ����� ��������
		SetBit(FLAGS1, FL1_TX);
	}else{
		UCSRB |= (1 << RXEN);
		UCSRB &= ~(1 << TXEN);
		ClrBit(PortTxRx, PinTxRx);	//��������� ADM485 � ����� ������
	}
}
//===============================================================================



