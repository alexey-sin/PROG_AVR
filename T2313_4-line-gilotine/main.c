#include "main.h"
//===============================================================================
int main(void){  
    init();

	_delay_ms(500);
	
    while(1){
		CheckBVK();
		ExecuteBVK();
    }
    return 0;
}
//===============================================================================
void init(void){
	SetBit(ACSR, ACD);//�������� ���������� ����������	
	// �����
	// Port A
	DDRA = 0;
	PORTA = 0;
	
	// Port B
	DDRB |= (1 << PinKL_L | 1 << PinKL_R);
	PORTB &= ~(1 << PinKL_L | 1 << PinKL_R);
	
	// Port D
	DDRD &= ~(1 << PinBVK_L | 1 << PinBVK_LJ | 1 << PinBVK_R | 1 << PinBVK_RJ | 1 << PinBVK_T | 1 << PinBVK_TJ);
	PORTD |= (1 << PinBVK_LJ | 1 << PinBVK_RJ | 1 << PinBVK_TJ);
	PORTD &= ~(1 << PinBVK_L | 1 << PinBVK_R | 1 << PinBVK_T);
}
void CheckBVK(void){
	//������� ���� - 0 => �� ��������������� ����� ���� 1 - ���������� ���������
	
	if(BitIsClr(PortPinBVK,PinBVK_TJ)) if(BitIsSet(PortPinBVK,PinBVK_T)) BVK_T = 0; else BVK_T = 1;
	else if(BitIsClr(PortPinBVK,PinBVK_T)) BVK_T = 0; else BVK_T = 1;

	if(BitIsClr(PortPinBVK,PinBVK_LJ)) if(BitIsSet(PortPinBVK,PinBVK_L)) BVK_L = 0; else BVK_L = 1;
	else if(BitIsClr(PortPinBVK,PinBVK_L)) BVK_L = 0; else BVK_L = 1;

	if(BitIsClr(PortPinBVK,PinBVK_RJ)) if(BitIsSet(PortPinBVK,PinBVK_R)) BVK_R = 0; else BVK_R = 1;
	else if(BitIsClr(PortPinBVK,PinBVK_R)) BVK_R = 0; else BVK_R = 1;
}
void ExecuteBVK(void){
	if((LEFT == 1 && RIGTH == 1) || (BVK_L == 1 && BVK_R == 1)){	//������. ��� ��������� ��� ��� �������� ������ �����������.
		LEFT = 0;
		RIGTH = 0;
		STATUS = 0;
		ClrBit(PortKL,PinKL_L);
		ClrBit(PortKL,PinKL_R);
		return;
	}
	if(BVK_L == 0 && BVK_R == 0) return;	//���� �� ����� �� �������� ���������
	if(STATUS == 0){	//���� �� "� ������"
		if(LEFT == 0 && RIGTH == 0 && BVK_T == 1){
			if(BVK_L == 1){
				RIGTH = 1;
				STATUS = 1;
				SetBit(PortKL,PinKL_R);
			}
			if(BVK_R == 1){
				LEFT = 1;
				STATUS = 1;
				SetBit(PortKL,PinKL_L);
			}
		}
	}else if(STATUS == 1){	//���� "� ������"
		if(RIGTH == 1 && BVK_R == 1){
			_delay_ms(500);
			STATUS = 2;	//"��������� ���"
			RIGTH = 0;
			ClrBit(PortKL,PinKL_R);
			_delay_ms(500);
		}
		if(LEFT == 1 && BVK_L == 1){
			_delay_ms(500);
			STATUS = 2;	//"��������� ���"
			LEFT = 0;
			ClrBit(PortKL,PinKL_L);
			_delay_ms(500);
		}
	}else{	//���� "��������� ���" ���� ����� ���������� ��� ������������
		if(BVK_T == 0){
			STATUS = 0;	//���� ����������, �������� �������
			_delay_ms(500);
		}
	}
}	
//===============================================================================

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
		



