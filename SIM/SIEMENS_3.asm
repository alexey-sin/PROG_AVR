.include "m16def.inc"

.equ 	t1			=$60
.equ 	t2			=$61
.equ 	t3			=$62

.eseg
.org 0x000
smsc:	.db 11, 7, 9,1,6, 8,9,9, 9,1, 0,0	;������� SMS ������ ������ (1����- ���-�� ������ � ������)
.org 0x010
da:		.db 11, 7, 9,1,5, 9,6,3, 3,6, 8,5	;������� ���������� (1����- ���-�� ������ � ������)
.org 0x020
ds_1990_1:	.db $8F,$00,$00,$07,$0F,$A1,$E7,$01;����:CRC,��.���,.......,��.���,01H

.dseg
.org 0x063
buff_sms_ascii: .byte 100	;����� ��� � ASCII ��� ��������
buff_sms_c: 	.byte 1		;���-�� �������� � ���������

ds1820_buffer:	.byte 9	;0�-CRC;......;8�-��.�.����
temperature_0:	.byte 4	;0�-����;1�-�������;2�-�������;3�-������� 
temperature_1:	.byte 4	;0�-����;1�-�������;2�-�������;3�-������� 
ds1990_buffer:	.byte 8	;0�-CRC;......;8=01H

adc_v: .byte 2				;����� ������ ��� � ASCII �������
adc_dv: .byte 2				;������� ������ ��� � ASCII �������

.def TMPR_ST	= r2		;������� ������� ��������� �����������
	;TMPR_ST: 7��� - 1= ���������� ������ �� ����� PC1(����� 23);0- PC0(����� 22)
	;TMPR_ST: 6��� - 1= ������, ��������� �� LCD 1-PC1(����� 23);0- PC0(����� 22)
	;TMPR_ST: 5��� - ������
	;TMPR_ST: 4��� - ������
	;TMPR_ST: 3-0���� - �����,���������� ������� ��������� ��������� �����������

.def SMS_ST		= r3		;���� ��������� ������� SMS
	;SMS_ST: 7��� - 1= ����������� �� ������ PDU ����� ��������
	;SMS_ST: 6��� - 1= ������ ������ �� ������� ���, ���� �����������
	;SMS_ST: 5��� - 1= ���� ����� ��������� ����� � ������ ��������
	;SMS_ST: 4��� - 1= ��������� ��������� ������ �� ������� ���
	;SMS_ST: 3��� - 1= ��� �� ������ �������
	;SMS_ST: 2��� - 1= PDU ������, ���� ����� ��������
	;SMS_ST: 1��� - 1= ��� �� ������ ������

.def BAT_ST		= r4		;���� ��������� �������
	;BAT_ST: 7��� - 1= ������������� ���: ���������� ���� 11,0V (1 ���)
	;BAT_ST: 6��� - 1= ��������� �� 1 ���� ����������

	;BAT_ST: 5��� - 1= ������������� ���: ���������� ���� 10,6V (2 ���)
	;BAT_ST: 4��� - 1= ��������� �� 2 ���� ����������

	;BAT_ST: 3��� - 1= ������������� ���: ���������� ���� 10,2V (3 ���)
	;BAT_ST: 2��� - 1= ��������� �� 3 ���� ����������

	;BAT_ST: 1��� - 1= ����:/0/-1���./1/-2���./1/-3���.
	;BAT_ST: 0��� - 1=      /1/      /0/      /1/

.def SECR_ST	= r5		;���� ��������� ������
	;SECR_ST: 7��� - 1= ������� ������ ��������

	;SECR_ST: 6��� - 1= �������� 1 �������	(PA1)
	;SECR_ST: 5��� - 1= ��������� �� 1 ������� ����������

	;SECR_ST: 4��� - 1= �������� 2 �������	(PA2)
	;SECR_ST: 3��� - 1= ��������� �� 2 ������� ����������

	;SECR_ST: 2��� - 1= �������� 3 �������	(PA3)
	;SECR_ST: 1��� - 1= ��������� �� 3 ������� ����������

.def POZ_LCD	= r6		;������� ������� � ����������  00-09
.def SYM_LCD	= r7		;������ ��������� � ���������
.def IDENT		= r8		;������� ����������� �������� �� ������� ���
	;IDENT:=0 �� ������� ����������
	;7 6 5 4 3 2 1 0
	;---------------
	;0 0 0 0 0 0 0 1	OK
	;0 0 0 0 0 0 1 0	ERROR
	;0 0 0 0 0 0 1 1	">"

;.def ERR_ST		= r9		;���� ������� �� PDU
.def KEY_ST		= r12		;������� ��������� ����������
.def ERR_DS		= r9		;������� ������ ������ DS1990

.cseg
.org 0x000
	rjmp start
.org OVF1addr	;0x010
	rjmp over_16			;LD_on
.org OVF0addr	;0x012
	rjmp over_8				;LD_off
.org URXCaddr	;0x016
	rjmp Receive_Int        ;��� ������������� ���������� RXC
; ----------------------------------------------
; ��� F=8���
; ----------------------------------------------
.org 0x030
start:
	ldi r16,low(RAMEND)
	out spl,r16
	ldi r16,high(RAMEND)
	out sph,r16

	rcall dj_200ms

	clr r10					;R0 - 00000000
	ser r16
	mov r11,r16				;R11 - 11111111

	ldi r16,0b11110000
	out DDRA,r16
	out PORTA,r10

	out DDRB,r10
	out PORTB,r11

	ldi r16,0b11111100
	out DDRC,r16
	out PORTC,r10

	out DDRD,r10
	out PORTD,r10

	rcall initlcd
	rcall pause

	ldi yh,high(buff_sms_ascii)
	ldi yl,low(buff_sms_ascii)
	st Y,r10   			;���������� ��������� ��������� � ����	

	clr TMPR_ST			;������� ������� ������������� ���������
	clr BAT_ST			;������� ��������� �������
	clr SMS_ST			;������� ��������� ������� SMS
	clr SECR_ST			;������� ����� � ������
;	clr ERR_ST			;������� ������
	clr KEY_ST			;��������� ��������� ����������
	clr IDENT			;������� ��������� ������������� �������� ����
	clr ERR_DS			;������� ������� ������ ������ DS1990

	ldi zh,high(world_1_c)
	ldi zl,low(world_1_c)
	st z,r10			;������� ��������� ��������� �������� ������
	ldi zh,high(world_2_c)
	ldi zl,low(world_2_c)
	st z,r10			;������� ��������� ��������� �������� ������
	ldi zh,high(world_3_c)
	ldi zl,low(world_3_c)
	st z,r10			;������� ��������� ��������� �������� ������


	out EEARH,r10


	ldi r16,25
	out UBRRL,r16			;19200 bod
	ldi r16,0b10011000		;RXCIE=1(���������� �� ������);RXEN=1;TXEN=1
	out UCSRB,r16			;on USART
;�������� ���
	ldi r16,(1<<ADEN)|(1<<ADSC)|(7<<ADPS0)	;/128
	out ADCSRA,r16
	ldi r16,0b11100000		;������������ �� ����� �������
	out ADMUX,r16

		out TCNT1H,r10
		out TCNT1L,r10			;
		ldi r16,0b00000101		;��������� ���������� �� ������������ 1(16����)� 0(8����) ���������
		out TIMSK,r16
		ldi r16,0b00000101
		out TCCR1B,r16
		out TCCR0,r10

	sei

		rcall ds_error
		mov TMPR_ST,r11
		rcall ds_error
		clr TMPR_ST

		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms
		rcall initlcd

		rcall out_volt		;�������� ������� � ����� ����������
		rcall set_BAT_ST_of_situation
		rcall set_SECR_ST_of_situation
;-------------------------------------------------------
loop:
		sbrs SECR_ST,7		;���������� ���� ��� �������
		rcall test_key		;�������� ����������

		rcall test_sensors	;�������� ��������
		rcall out_volt		;�������� ������� � ����� ����������
;-------------------------------------------------------
		tst IDENT
		breq k10
		mov r16,IDENT
		
k5:		cpi r16,0b00000011	;">"
		brne k6
		mov r17,SMS_ST
		cbr r17,0b01000000
		sbr r17,0b10000000
		mov SMS_ST,r17
		rcall beep_25ms
		rjmp k8
;--------
k6:		cpi r16,0b00000001	;"OK"
		brne k7
		rcall beep_25ms
		rjmp k8
;--------
k7:		cpi r16,0b00000010	;"ERROR"
		brne k8
		rcall beep_200ms

k8:		mov IDENT,r10

;--------
k10:	sbrc SMS_ST,7
		rcall send_pdu

		sbrc SMS_ST,4
		rcall send_query
;--------

		mov r16,BAT_ST
		sbrs BAT_ST,7			;U=11,0-10,6
		rjmp kb1
		sbrc BAT_ST,6			;��� ��� �� ����������
		rjmp kb1
		sbr r16,0b00000001
		cbr r16,0b00000010
		mov BAT_ST,r16			;1 ��� ����������� ���
		mov r16,SMS_ST
		sbr r16,0b00001000		;��� �� ������ �������	
		mov SMS_ST,r16
		sbrc SMS_ST,6			;���������� ���� ��� ������ ������ �� ������� ���
		rjmp kb1
		sbrc SECR_ST,7			;���������� ���� �� ��� �������
		rcall sms_bat_1
		sbrs SECR_ST,7			;���������� ���� ��� �������
		rcall signal

kb1:
		mov r16,BAT_ST
		sbrs BAT_ST,5			;U=10,6-10,2
		rjmp kb2
		sbrc BAT_ST,4			;��� ��� �� ����������
		rjmp kb2
		sbr r16,0b00000010
		cbr r16,0b00000001
		mov BAT_ST,r16			;2 ��� ����������� ���
		mov r16,SMS_ST
		sbr r16,0b00001000		;��� �� ������ �������	
		mov SMS_ST,r16
		sbrc SMS_ST,6			;���������� ���� ��� ������ ������ �� ������� ���
		rjmp kb2
		sbrc SECR_ST,7			;���������� ���� �� ��� �������
		rcall sms_bat_1
		sbrs SECR_ST,7			;���������� ���� ��� �������
		rcall signal


kb2:
		mov r16,BAT_ST
		sbrs BAT_ST,3			;U=<10,2
		rjmp kb4
		sbrc BAT_ST,2			;��� ��� �� ����������
		rjmp kb3
		sbr r16,0b00000001
		sbr r16,0b00000010
		mov BAT_ST,r16			;3 ��� ����������� ���
		mov r16,SMS_ST
		sbr r16,0b00001000		;��� �� ������ �������	
		mov SMS_ST,r16
		sbrc SMS_ST,6			;���������� ���� ��� ������ ������ �� ������� ���
		rjmp kb4
		sbrc SECR_ST,7			;���������� ���� �� ��� �������
		rcall sms_bat_2
		sbrs SECR_ST,7			;���������� ���� ��� �������
		rcall signal
		rjmp kb4

kb3:	sbi PORTC,2				;�������� ��������� �������

;--------
kb4:
		sbrs SECR_ST,7			;���������� ���� �� ��� �������
		rjmp ks1

		rcall ds1990

		sbrs SECR_ST,6			;���������� ���� �������� 1 ������
		rjmp ks2
		sbrc SECR_ST,5			;���������� ���� sms �� 1 ������� ��� ����������
		rjmp ks2
		sbrc SMS_ST,6			;���������� ���� ��� ������ ������ �� ������� ���
		rjmp ks2
		sbrc SMS_ST,2			;PDU ������, ���� ����� ��������
		rjmp ks2
		rcall sms_secr

ks2:	sbrs SECR_ST,4			;���������� ���� �������� 2 ������
		rjmp ks3
		sbrc SECR_ST,3			;���������� ���� sms �� 2 ������� ��� ����������
		rjmp ks3
		sbrc SMS_ST,6			;���������� ���� ��� ������ ������ �� ������� ���
		rjmp ks3
		sbrc SMS_ST,2			;PDU ������, ���� ����� ��������
		rjmp ks3
		rcall sms_secr

ks3:	sbrs SECR_ST,2			;���������� ���� �������� 3 ������
		rjmp ks1
		sbrc SECR_ST,1			;���������� ���� sms �� 3 ������� ��� ����������
		rjmp ks1
		sbrc SMS_ST,6			;���������� ���� ��� ������ ������ �� ������� ���
		rjmp ks1
		sbrc SMS_ST,2			;PDU ������, ���� ����� ��������
		rjmp ks1
		rcall sms_secr
;--------
ks1:	clr POZ_LCD
		rcall out_lcd_poz		;����� �� �����

		sbrs TMPR_ST,6			;����� ������ �������
		rjmp ks6
		ldi yh,high(temperature_1)
		ldi yl,low(temperature_1)
		ldi r16,2
		mov SYM_LCD,r16
		rcall symbol 
		rjmp ks7
ks6:	ldi yh,high(temperature_0)
		ldi yl,low(temperature_0)
		ldi r16,1
		mov SYM_LCD,r16
		rcall symbol 

ks7:	ld r16,y+
		mov SYM_LCD,r16
		rcall symbol 			;����� �����
		
		clt
		ld r16,y+				;�������, ���� 0->����������
		tst r16
		breq ks8
		set
		mov SYM_LCD,r16
		rcall symbol 

ks8:	ld r16,y+				;�������
		cpi r16,28
		breq ks9
		subi r16,-16			;������� �����
ks9:	mov SYM_LCD,r16
		rcall symbol 
		
		ld r16,y				;�������
		mov SYM_LCD,r16
		rcall symbol

		brts ks10
		clr SYM_LCD
		rcall out_lcd_sym

ks10:	ldi r16,6				;����� �� ����� ���������� ������
		mov POZ_LCD,r16
		rcall out_lcd_poz

		clr SYM_LCD
		sbrs SECR_ST,7			;���������� ���� ��� �������
		rjmp ks4
		ldi r16,71
		mov SYM_LCD,r16
ks4:	rcall out_lcd_sym



rjmp loop
;-------------------------------------------------------
ds1990:

		sbi DDRD,2		;post0
		rcall d_500mks
		cbi DDRD,2		;post1
		rcall d_100mks
		sbic PIND,2
		ret
		rcall d_500mks

		push xh
		push xl
		push r16
		push r17
		push r18
		push r19
		push r20

		ldi r16,$33
		rcall post_ds1990
		rcall d_20mks
		rcall read_ds1990_ROM
		rcall check_crc_ds1990_buffer
		brtc ai1
		rjmp ai100

;�������� �� ���������� �������� ������
ai1:	clr r17				;����� ����� ������� 00
		clr r19				;����� ����� ������� FF
		ldi r16,8
		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
ai4:	ld r18,x+
		tst r18
		brne ai2
		inc r17
ai2:	inc r18
		tst r18
		brne ai3
		inc r19
ai3:	dec r16
		brne ai4
		cpi r17,8
		brne ai5		;��������� ���� 00
		rjmp ai100
ai5:	cpi r19,8
		brne ai6		;��������� ���� FF
		rjmp ai100
;����� ��������
ai6:
;�������� �����
		ldi r18,8
		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
	    ldi r19,low(ds_1990_1)
		
ai7:	out EEARL,r19
		inc r19
		ld r17,x+
		sbi EECR,EERE
		in r16,EEDR
		cp r16,r17
		brne ai9
		dec r18
		brne ai7
;����� �������� �����
ai8:	sbi PORTC,5		;���������
		rcall beep_25ms	;beep
		rcall d_800ms
		cbi PORTC,5		;���������
		rcall gard_off
		rjmp ai100

ai9:	;������� ������� �����
		sbi PORTC,5		;���������
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall d_800ms
		cbi PORTC,5		;���������
		mov r16,ERR_DS
		inc r16
		mov ERR_DS,r16
		cpi r16,3
		brne ai100
		rcall selecting_key
		clr ERR_DS

ai100:
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
ret
;-------------------------------------------------------
gard_off:			;������ � ������
		clr SECR_ST
		clr ERR_DS
ret
;-------------------------------------------------------
read_ds1990_ROM:
		push r16
		push r17
		push r18
		push r19
		push zh
		push zl
		ldi r17,8		;������� ������
i910:	ldi r18,8		;������� ���
		clr r19
i911:	sbi DDRD,2		;post0		;���0 �� �����
		rcall d_1mks
		cbi DDRD,2		;post1		;���������� �� ����� �� ���0
		rcall d_10mks
		in r16,PIND		;
		sbrc r16,2
		sbr r19,$80
		clc
		ror r19
		rcall d_65mks
		dec r18
		brne i911
		rol r19		;��������� ����� ������ ��� ������
		dec r17
		ldi zh,high(ds1990_buffer)
		ldi zl,low(ds1990_buffer)
		add zl,r17
		brcc i912
		inc zh
i912:	st z,r19
		cpi r17,0
		brne i910
		pop zl
		pop zh
		pop r19
		pop r18
		pop r17
		pop r16
		ret
;-------------------------------------------------------
check_crc_ds1990_buffer:	;���� ������, ��� � ��������������� � 1, ���� ��� ��������� - � 0
		push zh
		push zl
		push r16
		push r17
		push r18
		push r19
		push r20
        clr r19
		ldi r17,8
		ldi zh,high(ds1990_buffer)
		ldi zl,low(ds1990_buffer+8)
jmp92:	ld r20,-z		
        ldi r18,8			;����������� �����
		push r20
jmp94:	eor r20,r19
		ror r20
		mov r20,r19
		brcc jmp91
		ldi r16,$18
		eor r20,r16
jmp91:	ror r20
		mov r19,r20
		pop r20
		lsr r20
		push r20
		dec r18
		brne jmp94
		pop r20
		dec r17
		brne jmp92
		tst r19
		brne jmp93
		clt
		rjmp jmp90
jmp93:	set
jmp90:
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
		pop zl
		pop zh
ret
;-------------------------------------------------------
post_ds1990:		;�������� ����� PIND,
		push r17
		push r16

		ldi r17,8
	dk1:sbrs r16,0
		rcall post_0_ds1990
		sbrc r16,0
		rcall post_1_ds1990
		rcall d_10mks
		ror r16
		dec r17
		brne dk1

		pop r16
		pop r17
ret
post_1_ds1990:
		sbi DDRD,2		;post0
		rcall d_10mks
		cbi DDRD,2		;post1
		rcall d_80mks
ret
post_0_ds1990:	
		sbi DDRD,2		;post0
		rcall d_80mks
		rcall d_10mks
		cbi DDRD,2		;post1
ret
;-------------------------------------------------------
test_key:	;�������� ����������
		tst KEY_ST
		brne k400

		sbic PINB,0			;������ 0 "������"
		rjmp k000
		rcall key_busy
		rcall set_security

k000:
		sbic PINB,1			;������ 1 "TEST"
		rjmp k100
		rcall key_busy
		rcall key_1

k100:
		sbic PINB,2			;������ 2 � �������������� �������
		rjmp k200
		rcall key_busy
		rcall key_2

k200:
		sbic PINB,3			;������ 3
		rjmp k300
		rcall key_busy
		rcall key_3

k300:
		sbic PINB,4			;������ 4	��������� ����� ������
		rjmp k400
		rcall key_busy
		rcall set_key_DS1990
k400:	ret
;-------------------------------------------------------
key_busy:
		rcall beep_25ms
		out TCNT0,r10
		ldi r16,0b00000101
		out TCCR0,r16
		ldi r16,0b00001010
		mov KEY_ST,r16
ret
; ----------------------------------------------
over_8:
		push r16
		out TCCR0,r10	;���� �������
		mov r16,KEY_ST
		dec r16
		tst r16
		breq ovr8_1
		mov KEY_ST,r16
		out TCNT0,r10
		ldi r16,0b00000101
		out TCCR0,r16	;���� �������
		rjmp ovr8_2
ovr8_1:	clr KEY_ST
ovr8_2:	pop r16
reti
; ----------------------------------------------
set_security:
		push r16
		sbrc SECR_ST,7
		rjmp ss3

		ldi r16,30
ss2:	sbi PORTC,5		;���������
		rcall d_200ms
		rcall test_sensors
		cbi PORTC,5		;���������
		sbis PINB,0
		rjmp ss1
		rcall d_200ms
		sbis PINB,0
		rjmp ss1
		dec r16
		brne ss2

		tst SECR_ST
		breq ss4
		rcall signal_long
		rcall set_SECR_ST_of_situation
ss4:	mov r16,SECR_ST
		sbr r16,0b10000000
		mov SECR_ST,r16

ss1:	rcall beep_25ms
ss3:	rcall d_800ms
		pop r16
ret
; ----------------------------------------------
test_sensors:
		push r16
		mov r16,SECR_ST

		sbis PINA,1			;������ 1 (������, �����)
		cbr r16,0b01000000
		sbic PINA,1
		sbr r16,0b01000000

		sbis PINA,2			;������ 2
		cbr r16,0b00010000
		sbic PINA,2
		sbr r16,0b00010000

		sbis PINA,3			;������ 3
		cbr r16,0b00000100
		sbic PINA,3
		sbr r16,0b00000100

		cp r16,SECR_ST
		breq ts1
		rcall beep_25ms
		mov SECR_ST,r16
ts1:	pop r16
ret
; ----------------------------------------------
signal:
		rcall signal_long
		rcall set_BAT_ST_of_situation
ret
; ----------------------------------------------
signal_long:
		push r16
		sbi PORTC,5		;���������
		ldi r16,3
sg1:	rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall d_800ms
		dec r16
		brne sg1
		cbi PORTC,5		;���������
		pop r16
ret
; ----------------------------------------------
selecting_key:		;������� ������� �����
		push zh
		push zl
		push xh
		push xl
		push r16
		push r17
		push r18

		ldi zh,high(s_key_1*2)
		ldi zl,low(s_key_1*2)
		ldi xh,high(buff_sms_ascii)
		ldi xl,low(buff_sms_ascii)
		lpm r16,z+
		mov r17,r16

sk_1:	lpm r18,z+
		st x+,r18
		dec r16
		brne sk_1

		ldi zh,high(s_bat_2*2)
		ldi zl,low(s_bat_2*2)
		lpm r16,z+
		add r17,r16

sk_2:	lpm r18,z+
		st x+,r18
		dec r16
		brne sk_2

		ldi zh,high(adc_v)
		ldi zl,low(adc_v)
		subi r17,-5

		ld r18,z+
		cpi r18,$30
		brne sk_4
		dec r17
		rjmp sk_5

sk_4:	st x+,r18
sk_5:	ld r18,z+
		st x+,r18

		ldi r18,','
		st x+,r18

		ldi zh,high(adc_dv)
		ldi zl,low(adc_dv)
		ld r18,z+
		st x+,r18
		ld r18,z
		st x+,r18

		ldi zh,high(s_bat_3*2)
		ldi zl,low(s_bat_3*2)
		lpm r16,z+
		add r17,r16

sk_3:	lpm r18,z+
		st x+,r18
		dec r16
		brne sk_3

		ldi xh,high(buff_sms_c)
		ldi xl,low(buff_sms_c)
		st x,r17

		mov r16,SMS_ST
		sbr r16,0b00010000		;��������� ���� �� ������� �������
		mov SMS_ST,r16

		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
		pop zl
		pop zh
ret
; ----------------------------------------------
s_key_1: .DB 22,'�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�',' ','�','�','�','�','�','\r',0
; ----------------------------------------------
key_1:			;"TEST"
		push zh
		push zl
		push xh
		push xl
		push r16
		push r17
		push r18

		ldi zh,high(sms_1*2)
		ldi zl,low(sms_1*2)
		ldi xh,high(buff_sms_ascii)
		ldi xl,low(buff_sms_ascii)
		lpm r16,z+
		mov r17,r16

k1_1:	lpm r18,z+
		st x+,r18
		dec r16
		brne k1_1

		ldi zh,high(s_bat_2*2)
		ldi zl,low(s_bat_2*2)
		lpm r16,z+
		add r17,r16

k1_2:	lpm r18,z+
		st x+,r18
		dec r16
		brne k1_2

		ldi zh,high(adc_v)
		ldi zl,low(adc_v)
		subi r17,-5

		ld r18,z+
		cpi r18,$30
		brne k1_4
		dec r17
		rjmp k1_5

k1_4:	st x+,r18
k1_5:	ld r18,z+
		st x+,r18

		ldi r18,','
		st x+,r18

		ldi zh,high(adc_dv)
		ldi zl,low(adc_dv)
		ld r18,z+
		st x+,r18
		ld r18,z
		st x+,r18

		ldi zh,high(s_bat_3*2)
		ldi zl,low(s_bat_3*2)
		lpm r16,z+
		add r17,r16

k1_3:	lpm r18,z+
		st x+,r18
		dec r16
		brne k1_3

		ldi xh,high(buff_sms_c)
		ldi xl,low(buff_sms_c)
		st x,r17

		mov r16,SMS_ST
		sbr r16,0b00010000		;��������� ���� �� ������� �������
		mov SMS_ST,r16

		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
		pop zl
		pop zh
ret
; ----------------------------------------------
sms_1: .DB 5,'T','E','S','T','\r'
; ----------------------------------------------
key_2:	;����������� �������
		mov r16,TMPR_ST
		sbrs TMPR_ST,6
		sbr r16,0b01000000

		sbrc TMPR_ST,6
		cbr r16,0b01000000
		mov TMPR_ST,r16
ret
; ----------------------------------------------
key_3:

		sbi PORTC,2				;�������� ��������� �������

ret
; ----------------------------------------------
set_key_DS1990:
		rcall initlcd
		clr POZ_LCD
		rcall out_lcd_poz		;����� �� �����
		ldi r16,$CD
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,$8F
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,$87
		mov SYM_LCD,r16
		rcall out_lcd_sym

set_key_loop:
		tst KEY_ST
		brne skl_0

		sbic PINB,4			;������ 4 "�����"
		rjmp skl_0
		rcall key_busy
		ret

skl_0:
		sbi DDRD,2		;post0
		rcall d_500mks
		cbi DDRD,2		;post1
		rcall d_100mks
		sbic PIND,2
		rjmp set_key_loop
		rcall d_500mks

		push xh
		push xl
		push r16
		push r17
		push r18
		push r19
		push r20

		ldi r16,$33
		rcall post_ds1990
		rcall d_20mks
		rcall read_ds1990_ROM
		rcall check_crc_ds1990_buffer
		brtc skl_1
		rjmp skl_100

;�������� �� ���������� �������� ������
skl_1:	clr r17				;����� ����� ������� 00
		clr r19				;����� ����� ������� FF
		ldi r16,8
		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
skl_4:	ld r18,x+
		tst r18
		brne skl_2
		inc r17
skl_2:	inc r18
		tst r18
		brne skl_3
		inc r19
skl_3:	dec r16
		brne skl_4
		cpi r17,8
		brne skl_5		;��������� ���� 00
		rjmp skl_100
skl_5:	cpi r19,8
		brne skl_6		;��������� ���� FF
		rjmp skl_100
;����� ��������
skl_6:
		ldi r19,8
		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
	    ldi r18,high(ds_1990_1)
	    ldi r17,low(ds_1990_1)

skl_7:	sbic EECR,1
		rjmp skl_7
		out EEARH,r18
		out EEARL,r17
		ld r16,x+
		out EEDR,r16
		sbi EECR,2
		nop
		sbi EECR,1
		
		inc r17
		tst r17
		brne skl_8
		inc r18
skl_8:	dec r19
		tst r19
		brne skl_7

		sbi PORTC,5		;���������
		rcall beep_25ms	;beep
		cbi PORTC,5		;���������
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
		ret

skl_100:
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
		pop xl
		pop xh

rjmp set_key_loop
; ----------------------------------------------
sms_secr:
		push zh
		push zl
		push xh
		push xl
		push r16
		push r17
		push r18

		ldi zh,high(s_secr_1*2)
		ldi zl,low(s_secr_1*2)
		ldi xh,high(buff_sms_ascii)
		ldi xl,low(buff_sms_ascii)
		lpm r16,z+
		mov r17,r16

ssc_1:	lpm r18,z+
		st x+,r18
		dec r16
		brne ssc_1

		ldi zh,high(s_secr_2*2)
		ldi zl,low(s_secr_2*2)
		lpm r16,z+
		add r17,r16

ssc_2:	lpm r18,z+
		st x+,r18
		dec r16
		brne ssc_2

		inc r17

		sbrs SECR_ST,6			;���������� ���� �������� 1 ������
		rjmp ssc_3
		sbrc SECR_ST,5			;���������� ���� sms �� 1 ������� ��� ����������
		rjmp ssc_3
		ldi r18,'1'
		st x+,r18

ssc_3:	sbrs SECR_ST,4			;���������� ���� �������� 2 ������
		rjmp ssc_4
		sbrc SECR_ST,3			;���������� ���� sms �� 2 ������� ��� ����������
		rjmp ssc_4
		ldi r18,'2'
		st x+,r18

ssc_4:	sbrs SECR_ST,2			;���������� ���� �������� 3 ������
		rjmp ssc_5
		sbrc SECR_ST,1			;���������� ���� sms �� 3 ������� ��� ����������
		rjmp ssc_5
		ldi r18,'3'
		st x+,r18

ssc_5:	ldi zh,high(s_secr_3*2)
		ldi zl,low(s_secr_3*2)
		lpm r16,z+
		add r17,r16

ssc_6:	lpm r18,z+
		st x+,r18
		dec r16
		brne ssc_6

		sbrs SECR_ST,6			;���������� ���� �������� 1 ������
		rjmp ssc_7
		sbrc SECR_ST,5			;���������� ���� sms �� 1 ������� ��� ����������
		rjmp ssc_7

		ldi zh,high(s_secr_4*2)
		ldi zl,low(s_secr_4*2)
		lpm r16,z+
		add r17,r16

ssc_8:	lpm r18,z+
		st x+,r18
		dec r16
		brne ssc_8

ssc_7:	ldi xh,high(buff_sms_c)
		ldi xl,low(buff_sms_c)
		st x,r17

		mov r16,SMS_ST
		sbr r16,0b00010010		;��������� ���� �� ������� �������
		mov SMS_ST,r16

		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
		pop zl
		pop zh
ret
; ----------------------------------------------
s_secr_1: .DB 8,'�','�','�','�','�','�','�','\r',0
s_secr_2: .DB 9,'�','�','�','�','�','�','�','�',' '
s_secr_3: .DB 8,' ','�','�','�','�','�','�','\r',0
s_secr_4: .DB 15,'(','�','�','�','�','�','�',',',' ','�','�','�','�','�',')'
; ----------------------------------------------
sms_bat_1:
		push zh
		push zl
		push xh
		push xl
		push r16
		push r17
		push r18

		ldi zh,high(s_bat_1*2)
		ldi zl,low(s_bat_1*2)
		ldi xh,high(buff_sms_ascii)
		ldi xl,low(buff_sms_ascii)
		lpm r16,z+
		mov r17,r16

sb1_1:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb1_1

		ldi zh,high(s_bat_2*2)
		ldi zl,low(s_bat_2*2)
		lpm r16,z+
		add r17,r16

sb1_2:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb1_2

		ldi zh,high(adc_v)
		ldi zl,low(adc_v)
		subi r17,-5

		ld r18,z+
		cpi r18,$30
		brne sb1_4
		dec r17
		rjmp sb1_5

sb1_4:	st x+,r18
sb1_5:	ld r18,z+
		st x+,r18

		ldi r18,','
		st x+,r18

		ldi zh,high(adc_dv)
		ldi zl,low(adc_dv)
		ld r18,z+
		st x+,r18
		ld r18,z
		st x+,r18

		ldi zh,high(s_bat_3*2)
		ldi zl,low(s_bat_3*2)
		lpm r16,z+
		add r17,r16

sb1_3:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb1_3

		ldi xh,high(buff_sms_c)
		ldi xl,low(buff_sms_c)
		st x,r17

		mov r16,SMS_ST
		sbr r16,0b00010000		;��������� ���� �� ������� �������
		mov SMS_ST,r16

		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
		pop zl
		pop zh
ret
; ----------------------------------------------
sms_bat_2:
		push zh
		push zl
		push xh
		push xl
		push r16
		push r17
		push r18

		ldi zh,high(s_bat_1*2)
		ldi zl,low(s_bat_1*2)
		ldi xh,high(buff_sms_ascii)
		ldi xl,low(buff_sms_ascii)
		lpm r16,z+
		mov r17,r16

sb2_1:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb2_1

		ldi zh,high(s_bat_2*2)
		ldi zl,low(s_bat_2*2)
		lpm r16,z+
		add r17,r16

sb2_2:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb2_2

		ldi zh,high(adc_v)
		ldi zl,low(adc_v)
		subi r17,-5

		ld r18,z+
		cpi r18,$30
		brne sb2_4
		dec r17
		rjmp sb2_5

sb2_4:	st x+,r18
sb2_5:	ld r18,z+
		st x+,r18

		ldi r18,','
		st x+,r18

		ldi zh,high(adc_dv)
		ldi zl,low(adc_dv)
		ld r18,z+
		st x+,r18
		ld r18,z
		st x+,r18

		ldi zh,high(s_bat_3*2)
		ldi zl,low(s_bat_3*2)
		lpm r16,z+
		add r17,r16

sb2_3:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb2_3

		ldi zh,high(s_bat_4*2)
		ldi zl,low(s_bat_4*2)
		lpm r16,z+
		add r17,r16

sb2_6:	lpm r18,z+
		st x+,r18
		dec r16
		brne sb2_6

		ldi xh,high(buff_sms_c)
		ldi xl,low(buff_sms_c)
		st x,r17

		mov r16,SMS_ST
		sbr r16,0b00010000		;��������� ���� �� ������� �������
		mov SMS_ST,r16

		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
		pop zl
		pop zh
ret
; ----------------------------------------------
s_bat_1: .DB 18,'�','�','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','\r',0
s_bat_2: .DB 3,'U','=',' '
s_bat_3: .DB 3,' ','�','\r'
s_bat_4: .DB 27,'�','�','�','�','�','�','�','�','�','�',' ','�','�','�','�','�',' ','�','�','�','�','�','�','�','�','�','!'
; ----------------------------------------------
out_volt:
		ldi r16,7
		mov POZ_LCD,r16
		rcall out_lcd_poz

 		sbi ADCSRA,ADSC
ov1:	sbis ADCSRA,ADIF
		rjmp ov1
		in r16,ADCH
; ����� ����� �������� ���������� �������� � ��������� ����������
		mov r17,BAT_ST

		cpi r16,198			;10,2v
		brcc ov11
		cbr r17,0b10000000
		cbr r17,0b00100000
		sbr r17,0b00001000
		rjmp ov9

ov11:	cpi r16,205			;10,6v
		brcc ov12
		cbr r17,0b10000000
		sbr r17,0b00100000
		cbr r17,0b00001000
		rjmp ov9

ov12:	cpi r16,214			;11,0v
		brcc ov13
		sbr r17,0b10000000
		cbr r17,0b00100000
		cbr r17,0b00001000
		rjmp ov9

ov13:	;clr r17			;�������� �������� ��� �������� �������
		
ov9:	mov BAT_ST,r17		;�������� ��������� �������� � ����� BAT_ST

		rcall adc_calc		;r16-����; (r17:r16) - ����� ��������� ���������� � �������� �����
; ������� �� �����
		ldi r18,-1
ov2:	inc r18
		subi r16,100
        brsh ov2
		subi r17,1
		brsh ov2
		subi r16,-100

		mov r19,r16

		ldi r16,-1		;������� ������� �����
ov3:	inc r16
		subi r18,10
        brsh ov3
		subi r18,-10
		tst r16			;�������� �� ����
		brne ov7		;������� ���� ���
		clr SYM_LCD		;���� �� ������� �����(������)
		rcall out_lcd_sym
		rjmp ov8
ov7:	mov SYM_LCD,r16
		rcall symbol
ov8:	subi r16,-$30
		ldi yh,high(adc_v)
		ldi yl,low(adc_v)
		st y+,r16
		
		ldi r16,-1		;������� ������� �����
ov4:	inc r16
		subi r18,1
        brsh ov4
		subi r18,-1
		subi r16,-16
		mov SYM_LCD,r16
		rcall symbol
		subi r16,16
		subi r16,-$30
		st y,r16

		ldi r16,-1		;������� �������
ov5:	inc r16
		subi r19,10
        brsh ov5
		subi r19,-10
		mov SYM_LCD,r16
		rcall symbol
		subi r16,-$30
		ldi yh,high(adc_dv)
		ldi yl,low(adc_dv)
		st y+,r16

		ldi r16,-1		;������� �����
ov10:	inc r16
		subi r19,1
        brsh ov10
		subi r19,-1
		subi r16,-$30
		st y,r16
ret
; ----------------------------------------------
c_adc: .DB 5,20,17,0		;����������� ��������� ��� �������� ��� (5,2017)
; ----------------------------------------------
adc_calc:					;r16 - ������� ���������� �����
		push zh
		push zl
		push r20
		push r21
		push r22
		push r23
		push r24
		push r25

		ldi zh,high(c_adc*2)
		ldi zl,low(c_adc*2)
		lpm r17,z+			;��������� ������ ��������� - ����� �����
		mul r16,r17			;��������
		movw r24,r0			;��������� ��������� � (r25:r24) - ������� ������������� ���������
		lpm r17,z+			;��������� ������ ��� ����� ����� ������� �������� ���������
		mul r16,r17			;��������
		movw r22,r0			;��������� � (r23:r22) - �������� ��� ������� �� 100
		rcall subi_100		;�������� �� 100
		add r24,r20
		brcc ac3
		inc r25
ac3:	add r25,r21			;������� ���������� � ������ ����������
		lpm r17,z			;��������� ������ � ��������� ����� ����� ������� �������� ���������
		mul r16,r17			;��������
		movw r22,r0			;��������� � (r23:r22) - �������� ��� ������� �� 100
		rcall subi_100		;�������� �� 100
		movw r22,r20		;��������� ��� ���������� �������
		rcall subi_100		;�������� �� 100
		add r24,r20
		brcc ac4
		inc r25
ac4:	add r25,r21			;������� ���������� � ������ ����������
		movw r16,r24		;��������� ��� ������ �� ������������

		pop r25
		pop r24
		pop r23
		pop r22
		pop r21
		pop r20
		pop zl
		pop zh
ret
; ----------------------------------------------
subi_100:					;(r23:r22) - �������� ��� ������� �� 100;  (r21:r20) - ��������� �������
		ldi r20,-1
		ldi r21,-1
ac1:	subi r20,-1
		brlo ac2
		inc r21
ac2:	subi r22,100
        brsh ac1
		subi r23,1
		brsh ac1
ret
; ----------------------------------------------
dj_200ms:
		rcall d_200ms
ret
dj_800ms:
		rcall d_800ms
ret
; ----------------------------------------------
send_query:
		ldi r16,low(smsc)
		out EEARL,r16
		sbi EECR,EERE
		in r17,EEDR
		sbrc r17,0
		inc r17
		lsr r17

	    ldi r16,low(da)
		out EEARL,r16
		sbi EECR,EERE
		in r16,EEDR
		sbrc r16,0
		inc r16
		lsr r16
		
		add r17,r16
		ldi zh,high(buff_sms_c)
		ldi zl,low(buff_sms_c)
		ld r16,Z
		lsl r16
		add r17,r16
		subi r17,-2
		mov r18,r17

		push r17
		push r18
		ldi zh,high(at_cmgs*2)
		ldi zl,low(at_cmgs*2)
		lpm r18,z+
s_m1:	lpm r17,z+
		rcall out_byte
		dec r18
		brne s_m1
		pop r18
		pop r17

		ldi r17,-1
s_m2:	inc r17
		subi r18,100
       	brsh s_m2
		subi r18,-100
		tst r17
		breq s_m3
		subi r17,-$30
		rcall out_byte

s_m3:	ldi r17,-1
s_m4:	inc r17
		subi r18,10
       	brsh s_m4
		subi r18,-10
		subi r17,-$30
		rcall out_byte
	
		ldi r17,-1
s_m5:	inc r17
		subi r18,1
       	brsh s_m5
		subi r17,-$30
		rcall out_byte

		ldi r17,$0D	
		rcall out_byte

		mov r16,SMS_ST
		sbr r16,0b01000000		;������ ������ �� ������� ���
		cbr r16,0b00010000		;c������ ���� ������
		mov SMS_ST,r16

ret
; ----------------------------------------------
out_double_byte:	;r19-����, �����:r17
		push r17

		mov r17,r19
		swap r17
		cbr r17,0b11110000
		sbr r17,0b00110000
		rcall check_alfa
		rcall out_byte
		mov r17,r19
		cbr r17,0b11110000
		sbr r17,0b00110000
		rcall check_alfa
		rcall out_byte

		pop r17
ret
; ----------------------------------------------
out_double_byte_rus:	;r19-����, �����:r19
		push r19
		push r18
		push r16

		ldi r16,$03
		ldi r18,$50
		add r18,r19
		brcc odbr
		inc r16
odbr:	mov r19,r16
		rcall out_double_byte
		mov r19,r18
		rcall out_double_byte

		pop r16
		pop r18
		pop r19
ret
; ----------------------------------------------
out_double_byte_eng:	;r19-����, �����:r19
		push r19
		push r18
		mov r18,r19
		ldi r19,0
		rcall out_double_byte
		mov r19,r18
		rcall out_double_byte
		pop r18
		pop r19
ret
; ----------------------------------------------
check_alfa:
		cpi r17,$3a
		brne ca1
		ldi r17,$41
		ret
ca1:	cpi r17,$3b
		brne ca2
		ldi r17,$42
		ret
ca2:	cpi r17,$3c
		brne ca3
		ldi r17,$43
		ret
ca3:	cpi r17,$3d
		brne ca4
		ldi r17,$44
		ret
ca4:	cpi r17,$3e
		brne ca5
		ldi r17,$45
		ret
ca5:	cpi r17,$3f
		brne ca6
		ldi r17,$46
ca6:	ret
; ----------------------------------------------
at_cmgs: .DB 8,'A','T','+','C','M','G','S','=',0
; ----------------------------------------------
send_pdu:
	    ldi r16,low(smsc)
		out EEARL,r16
		sbi EECR,EERE
		in r20,EEDR
		mov r17,r20
		sbrs r17,0
		rjmp sp1
		inc r17
sp1:	lsr r17
		inc r17
		mov r19,r17
		rcall out_double_byte
		rcall send_number

		ldi r19,$11
		rcall out_double_byte
		clr r19
		rcall out_double_byte

	    ldi r16,low(da)
		out EEARL,r16
		sbi EECR,EERE
		in r20,EEDR
		mov r19,r20
		rcall out_double_byte
		rcall send_number

		clr r19
		rcall out_double_byte
		ldi r19,$08
		rcall out_double_byte
		ser r19
		rcall out_double_byte

		ldi xh,high(buff_sms_c)
		ldi xl,low(buff_sms_c)
		ld r19,X
		mov r20,r19
		lsl r19
		rcall out_double_byte
		
		ldi xh,high(buff_sms_ascii)
		ldi xl,low(buff_sms_ascii)
sp2:	ld r19,X+
		cpi r19,$C0
		brsh sp3
		rcall out_double_byte_eng
		rjmp sp4
		
sp3:	rcall out_double_byte_rus	
		
sp4:	dec r20
		brne sp2
		
		ldi r17,$1a
		rcall out_byte

		sbrc SMS_ST,3			;��� �� ������ �������
		rcall set_BAT_ST_of_situation
		sbrc SMS_ST,1			;��� �� ������ ������
		rcall set_SECR_ST_of_situation
		clr SMS_ST

ret
; ----------------------------------------------
set_SECR_ST_of_situation:
		push r16
		
		mov r16,SECR_ST
		sbrc r16,6				;���������� ���� �� �������� 1 ������
		sbr r16,0b00100000	

		sbrc r16,4				;���������� ���� �� �������� 2 ������
		sbr r16,0b00001000	

		sbrc r16,2				;���������� ���� �� �������� 3 ������
		sbr r16,0b00000010
		mov SECR_ST,r16

		pop r16
ret
; ----------------------------------------------
set_BAT_ST_of_situation:
		push r16
		
		mov r16,BAT_ST
		sbrc r16,7
		sbr r16,0b01000000	

		sbrc r16,5
		sbr r16,0b00010000	

		sbrc r16,3
		sbr r16,0b00000100
		cbr r16,0b00000011
		mov BAT_ST,r16

		pop r16
ret
; ----------------------------------------------
send_number:
		ldi r19,$91
		rcall out_double_byte
		clr r18
sn1:    in r16,EEARL
		inc r16
		out EEARL,r16
		sbi EECR,EERE
		in r17,EEDR
		inc r18 
		sbrc r18,0
		rjmp sn2		;��������
		swap r17
		or r19,r17
		rcall out_double_byte
		rjmp sn3
sn2:	mov r19,r17
sn3:	dec r20
		brne sn1
		sbrs r18,0
		ret
		ori r19,$f0
		rcall out_double_byte
ret
; ----------------------------------------------
;�� ��������� ���������� ��� ����������� ���������� ����� � �������� UART
Receive_Int:
		push zh
		push zl
		push xh
		push xl
		push r19
		push r18
		push r17
		push r16

		in r16,UDR             ;������� ���� �� �������� ������ ���������
		rcall identify

		pop r16
		pop r17
		pop r18
		pop r19
		pop xl
		pop xh
		pop zl
		pop zh

        reti
; ----------------------------------------------
identify:		;r16 - �������� ����

		ldi zh,high(world_1*2)
		ldi zl,low(world_1*2)
		lpm r18,z+				;r18 - ���-�� ���� � �����, (������ �� ������ �����)
		ldi xh,high(world_1_c)
		ldi xl,low(world_1_c)
		ld r17,x				;r17 - ���-�� ��������� �������� ������ ������ � ������ �����
		add zl,r17
		brcc id01
		inc zh
id01:	lpm r19,z
		cp r19,r16
		breq id02
		st x,r10			;�������� ������� ������ ����� �����
		rjmp id10
id02:	inc r17				;���� �������� ���� ������
		st x,r17
		cp r17,r18
		brne id10
		st x,r10
		ldi r17,0b00000011
		mov IDENT,r17
;--------------
id10:	ldi zh,high(world_2*2)
		ldi zl,low(world_2*2)
		lpm r18,z+				;r18 - ���-�� ���� � �����, (������ �� ������ �����)
		ldi xh,high(world_2_c)
		ldi xl,low(world_2_c)
		ld r17,x				;r17 - ���-�� ��������� �������� ������ ������ � ������ �����
		add zl,r17
		brcc id11
		inc zh
id11:	lpm r19,z
		cp r19,r16
		breq id12
		st x,r10			;�������� ������� ������ ����� �����
		rjmp id20
id12:	inc r17				;���� �������� ���� ������
		st x,r17
		cp r17,r18
		brne id20
		st x,r10
		ldi r17,0b00000001
		mov IDENT,r17
;--------------
id20:	ldi zh,high(world_3*2)
		ldi zl,low(world_3*2)
		lpm r18,z+				;r18 - ���-�� ���� � �����, (������ �� ������ �����)
		ldi xh,high(world_3_c)
		ldi xl,low(world_3_c)
		ld r17,x				;r17 - ���-�� ��������� �������� ������ ������ � ������ �����
		add zl,r17
		brcc id21
		inc zh
id21:	lpm r19,z
		cp r19,r16
		breq id22
		st x,r10			;�������� ������� ������ ����� �����
		rjmp id30
id22:	inc r17				;���� �������� ���� ������
		st x,r17
		cp r17,r18
		brne id30
		st x,r10
		ldi r17,0b00000010
		mov IDENT,r17
;--------------
id30:	;ldi zh,high(world_4*2)
;		ldi zl,low(world_4*2)
;		lpm r18,z+				;r18 - ���-�� ���� � �����, (������ �� ������ �����)
;		ldi xh,high(world_4_c)
;		ldi xl,low(world_4_c)
;		ld r17,x				;r17 - ���-�� ��������� �������� ������ ������ � ������ �����
;		add zl,r17
;		brcc id31
;		inc zh
;id31:	lpm r19,z
;		cp r19,r16
;		breq id32
;		st x,r10			;�������� ������� ������ ����� �����
;		rjmp id40
;id32:	inc r17				;���� �������� ���� ������
;		st x,r17
;		cp r17,r18
;		brne id40
;		st x,r10
;		ldi xh,high(ident)
;		ldi xl,low(ident)
;		ld r17,x
;		sbr r17,0b01000000
;		st x,r17

;id40:

ret
; ----------------------------------------------
.dseg				;�������� ������ � �����
world_1_c: .byte 1
world_2_c: .byte 1
world_3_c: .byte 1
;world_4_c: .byte 1
; ----------------------------------------------
.cseg
world_1: .DB 3,$0A,$3E,$20							;">"
world_2: .DB 3,$4F,$4B,$0D							;"OK"
world_3: .DB 7,$45,$52,$52,$4F,$52,$0D,$0A			;"ERROR"
;world_4: .DB 4,$05,$01,$04,$06						;BFB �����
; ----------------------------------------------
;tel_off:
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms

;		rcall initlcd

;		ldi zh,high(at_off*2)
;		ldi zl,low(at_off*2)
;		rcall out_uart

;		rcall d_800ms
;		out UCSRB,r10			;off USART
;		ldi r16,0b01100000
;		out MCUCR,r16
;		SLEEP
; ----------------------------------------------
;at_off: .DB 7,'A','T','^','S','M','S','O'
; ----------------------------------------------
;tel_on:
;		sbi PORTC,3			;��������� ����������
;	ldi r16,8
;	out UBRRL,r16			;57600 bod
;		rcall d_800ms
;		rcall d_800ms
;
;		ldi zh,high(at_on1*2)
;		ldi zl,low(at_on1*2)
;		rcall out_uart
;
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		cbi PORTC,3			;���������� ����������
;	ldi r16,25
;	out UBRRL,r16			;19200 bod
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;
;		ldi zh,high(at_on2*2)
;		ldi zl,low(at_on2*2)
;		rcall out_uart
;
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;	ldi r16,8
;	out UBRRL,r16			;57600 bod
;		rcall d_800ms
;
;		ldi zh,high(at_on3*2)
;		ldi zl,low(at_on3*2)
;		rcall out_uart
;
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		rcall d_800ms
;		ldi zh,high(at_on4*2)
;		ldi zl,low(at_on4*2)
;		rcall out_uart
;	ldi r16,25
;	out UBRRL,r16			;19200 bod
;		ldi zh,high(at_on5*2)
;		ldi zl,low(at_on5*2)
;		rcall out_uart

;ret
; ----------------------------------------------
;tel_ping:		;���������� �=1 ���� ������� �������� � �=0 ���� ���
;		push zh
;		push zl
;		push r16
;		push xh
;		push xl
;
;		mov IDENT,r10
;		ldi zh,high(t_ping*2)
;;		ldi zl,low(t_ping*2)
;;		rcall out_uart
;		rcall d_800ms
;		clt
;		sbrc IDENT,0
;		set
;		mov IDENT,r10
;
;		pop xl
;		pop xh
;		pop r16
;		pop zl
;		pop zh
;ret
; ----------------------------------------------
;t_ping: .DB 6,'A','T','+','G','M','R',0
; ----------------------------------------------
;at_on1: .DB 5,$05,$02,$07,$06,$0C
;at_on2: .DB 9,'A','T','^','S','Q','W','E','=','1'
;at_on3: .DB 5,$05,$02,$07,$06,$01
;at_on4: .DB 12,$06,$0A,$0C,'A','T','^','S','Q','W','E','=','0',0
;at_on5: .DB 9,'A','T','^','S','Q','W','E','=','0'
; ----------------------------------------------
out_uart:
		push r16
		push r17
		lpm r16,z+
ou:		lpm r17,z+
		rcall out_byte
		dec r16
		brne ou
		ldi r17,$0D	;������� �������
		rcall out_byte
		pop r17
		pop r16
ret
; ----------------------------------------------
out_byte:
ou_b:	sbis UCSRA,UDRE
		rjmp ou_b
		out UDR,r17
		ret
; ----------------------------------------------
beep_25ms:
		push r16

		ldi r16,128
bp1:	rcall d_100mks
		sbi PORTC,4		;������ ���
		rcall d_100mks
		cbi PORTC,4		;������ ����
		dec r16
		brne bp1

		pop r16
ret
; ----------------------------------------------
beep_200ms:
		push r17
		push r16

		ldi r17,4
bp3:	ldi r16,255
bp2:	rcall d_100mks
		sbi PORTC,4		;������ ���
		rcall d_100mks
		cbi PORTC,4		;������ ����
		dec r16
		brne bp2
		dec r17
		brne bp3

		pop r16
		pop r17
ret
; ----------------------------------------------
over_16:	;read_temperature:
		push r16
		push xh
		push xl
	
		out TCCR1B,r10			;���� �������
; -----
		mov r16,TMPR_ST
		cbr r16,0b11110000


		cpi r16,0				;��������.1�����,�������� 0
		brne ovr_01
		rcall ds_post0
		out TCNT1H,r11
		ldi r16,$FA
		out TCNT1L,r16	;~600mks
		mov r16,TMPR_ST
		cbr r16,0b00001111
		ori r16,1
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out

ovr_01:
		cpi r16,1				;1�����,0 ������, �������� 1
		brne ovr_02
		rcall ds_post1
		out TCNT1H,r11
		ldi r16,$FA
		out TCNT1L,r16	;~600mks
		mov r16,TMPR_ST
		cbr r16,0b00001111
		ori r16,2
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out
		
ovr_02:
		cpi r16,2				;1����� ���������, �������� $CC � $44
		brne ovr_03
		ldi r16,$CC
		rcall post_ds1820
		ldi r16,$44
		rcall post_ds1820
		ldi r16,$E7
		out TCNT1H,r16
		ldi r16,$95
		out TCNT1L,r16	;~800ms
		mov r16,TMPR_ST
		cbr r16,0b00001111
		ori r16,3
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out
		
ovr_03:
		cpi r16,3				;2�����,�������� 0
		brne ovr_04
		rcall ds_post0
		out TCNT1H,r11
		ldi r16,$FA
		out TCNT1L,r16	;~600mks
		mov r16,TMPR_ST
		cbr r16,0b00001111
		ori r16,4
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out
		
ovr_04:
		cpi r16,4				;2�����,0 ������, �������� 1
		brne ovr_05
		rcall ds_post1
		out TCNT1H,r11
		ldi r16,$FA
		out TCNT1L,r16	;~600mks
		mov r16,TMPR_ST
		cbr r16,0b00001111
		ori r16,5
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out
		
ovr_05:
		cpi r16,5				;2����� ���������, �������� $CC � $BE,������ � �������
		brne ovr_06
		ldi r16,$CC
		rcall post_ds1820
		ldi r16,$BE
		rcall post_ds1820
		rcall read_ds1820_ROM
		rcall calculate	
		ldi r16,$76
		out TCNT1H,r16
		ldi r16,$AB
		out TCNT1L,r16	;~4,5s
		mov r16,TMPR_ST
		cbr r16,0b00001111
		ori r16,6
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out
		
ovr_06:
		cpi r16,6				;��� ����������,�������� 8-������ �������,����������� ������,���������� �������
		brne ovr_09
		mov r16,TMPR_ST
		sbrs r16,7
		rjmp ovr_07
		cbr r16,0b10000000
		rjmp ovr_08
ovr_07:	sbr r16,0b10000000
ovr_08:	cbr r16,0b00001111
		ori r16,7
		mov TMPR_ST,r16
		sbrc SECR_ST,7
		sbi PORTC,5		;���������
		ldi r16,$FE
		out TCNT1H,r16
		ldi r16,$70
		out TCNT1L,r16	;~51ms
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		rjmp ovr_out

ovr_09:
		cpi r16,7				;��� � ��������
		brne ovr_out
		sbrc SECR_ST,7
		cbi PORTC,5		;���������
		out TCNT1H,r11
		ldi r16,$F8
		out TCNT1L,r16	;~1ms
		mov r16,TMPR_ST
		cbr r16,0b00001111
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;����� �������
		
; -----
ovr_out:pop xl
		pop xh
		pop r16
reti
; ----------------------------------------------
calculate:
		push xh
		push xl
		push yh
		push yl
		push r16
		push r17
		push r18
		push r19
		push r20
		rcall check_crc_ds1820_buffer
		brtc cl6
cl10:	rcall ds_error		;CRC �� �������,�����
		rjmp cl7
;�������� �� ���������� �������� ������
cl6:	clr r17				;����� ����� ������� 00
		clr r19				;����� ����� ������� FF
		ldi r16,9
		ldi xh,high(ds1820_buffer)
		ldi xl,low(ds1820_buffer)
cl8:	ld r18,x+
		tst r18
		brne cl13
		inc r17
cl13:	inc r18
		tst r18
		brne cl14
		inc r19
cl14:	dec r16
		brne cl8
		cpi r17,9
		brne cl9		;��������� ���� 00
		rjmp cl10
cl9:	cpi r19,9
		brne cl15		;��������� ���� FF
		rjmp cl10
;����� ��������
cl15:
		ldi xh,high(ds1820_buffer+7)
		ldi xl,low(ds1820_buffer+7)
		ld r20,x+		;�������� ��������, ������� ����
		ld r19,x		;�������� ��������, ������� ����

		ldi r16,29		;������
		sbrs r20,7
		rjmp cl5
		ldi r16,28		;"-"
		com r19
		inc r19
cl5:	sbrs TMPR_ST,7
		rjmp cl11
		ldi yh,high(temperature_1)
		ldi yl,low(temperature_1)
		rjmp cl12
cl11:	ldi yh,high(temperature_0)
		ldi yl,low(temperature_0)
cl12:	st y+,r16
		clc
		ror r19

		ldi xh,high(ds1820_buffer+1)
		ldi xl,low(ds1820_buffer+1)
		ld r20,x+		;count_per_c
		ld r18,x		;count_remain
		mov r17,r20
		sub r17,r18
		ldi r16,10
		clr r18
cl3:	add r18,r17
		dec r16
		brne cl3
		ldi r16,-1
cl4:	inc r16
		sub r18,r20
		brsh cl4

		ldi r17,-1		;������� �������
cl1:	inc r17
		subi r19,10
        brsh cl1
		subi r19,-10

		ldi r18,-1		;������� �������
cl2:	inc r18
		subi r19,1
        brsh cl2
		st y+,r17		;1�-������� ASCII
		st y+,r18		;2�-������� ASCII
		st y,r16		;3�-������� �������,����� ��� ������������� ����� ������ ��������� ��������
cl7:	
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
		pop yl
		pop yh
		pop xl
		pop xh
ret
; ----------------------------------------------
ds_error:
		push yh
		push yl
		push r16
		sbrs r2,7
		rjmp de1
		ldi yh,high(temperature_1)
		ldi yl,low(temperature_1)
		rjmp de2
de1:	ldi yh,high(temperature_0)
		ldi yl,low(temperature_0)
de2:	ldi r16,29
		st y+,r16
		ldi r16,28
		st y+,r16
		st y+,r16
		ldi r16,29
		st y,r16
		pop r16
		pop yl
		pop yh
		ret
;-------------------------------------------------------
post_ds1820:
		push r17
		push r16
		ldi r17,8
	nk1:sbrs r16,0
		rcall post_0_ds1820
		sbrc r16,0
		rcall post_1_ds1820
		rcall d_2mks
		ror r16
		dec r17
		brne nk1
		pop r16
		pop r17
ret
post_1_ds1820:
		rcall ds_post0
		rcall d_2mks
		rcall ds_post1
		rcall d_65mks
ret
post_0_ds1820:	
		rcall ds_post0
		rcall d_65mks
		rcall ds_post1
ret
ds_post1:
		sbrc TMPR_ST,7
		cbi DDRC,1
		sbrs TMPR_ST,7
		cbi DDRC,0
ret
ds_post0:
		sbrc TMPR_ST,7
		sbi DDRC,1
		sbrs TMPR_ST,7
		sbi DDRC,0
ret
;-------------------------------------------------------
read_ds1820_ROM:
		push r16
		push r17
		push r18
		push r19
		push zh
		push zl
		ldi r17,9		;������� ������
i110:	ldi r18,8		;������� ���
		clr r19
i111:	rcall ds_post0	;���0 �� �����
		rcall d_2mks
		rcall ds_post1	;���������� �� ����� �� ���0
		rcall d_10mks
		in r16,PINC		;
		sbrc TMPR_ST,7
		lsr r16
		sbrc r16,0
		sbr r19,$80
		clc
		ror r19
		rcall d_65mks
		dec r18
		brne i111
		rol r19		;��������� ����� ������ ��� ������
		dec r17
		ldi zh,high(ds1820_buffer)
		ldi zl,low(ds1820_buffer)
		add zl,r17
		st z,r19
		cpi r17,0
		brne i110
		pop zl
		pop zh
		pop r19
		pop r18
		pop r17
		pop r16
		ret
;-------------------------------------------------------
check_crc_ds1820_buffer:	;���� ������, ��� � ��������������� � 1, ���� ��� ��������� - � 0
        clr r19
		ldi r17,9
		ldi zh,high(ds1820_buffer)
		ldi zl,low(ds1820_buffer+9)
jmp12:	ld r20,-z		
        ldi r18,8			;����������� �����
		push r20
jmp14:	eor r20,r19
		ror r20
		mov r20,r19
		brcc jmp11
		ldi r16,$18
		eor r20,r16
jmp11:	ror r20
		mov r19,r20
		pop r20
		lsr r20
		push r20
		dec r18
		brne jmp14
		pop r20
		dec r17
		brne jmp12
		tst r19
		brne jmp13
		clt
		rjmp jmp10
jmp13:	set
jmp10:
ret
;-------------------------------------------------------
sim: .DB $EE,$60,$2F,$6D,$E1,$CD,$CF,$68,$EF,$ED,$EB,$C7,$8E,$67,$8F,$8B,$FE,$70,$3F,$7D,$F1,$DD,$DF,$78,$FF,$FD,$46,$A9,$01,0
;		   0   1   2   3   4   5   6   7   8   9   A   b   C   d   E   F   0.  1.  2.  3.  4.  5.  6.  7.  8.  9.  u  ��   -
;-------------------------------------------------------
symbol: push r16
		push zl
		push zh


		mov r16,SYM_LCD
		ldi zh,high(sim*2)
		ldi zl,low(sim*2)
		add zl,r16
		brcc sy1
		inc zh
sy1:	lpm r16,z
		mov SYM_LCD,r16
		rcall out_lcd_sym 

		pop zh
		pop zl
		pop r16
		ret
;-------------------------------------------------------
initlcd:
		push r16

		ldi r16,$0F
		mov POZ_LCD,r16			;������ ����������
		rcall out_lcd_poz
		mov SYM_LCD,r11
		rcall out_lcd_sym 

		clr POZ_LCD				;������� ������
		rcall out_lcd_poz
		clr SYM_LCD
		clr r16
il1:	rcall out_lcd_sym 
		inc r16
		cpi r16,10
		brne il1

		pop r16
ret
; ----------------------------------------------
pause:
		push r16
		clr POZ_LCD
		rcall out_lcd_poz		;������� ������
		mov SYM_LCD,r11
		clr r16
paus:	rcall out_lcd_sym 
		inc r16
		cpi r16,10
		brne paus
		pop r16
ret
; ----------------------------------------------
out_lcd_poz:
		push r16
		cbi PORTC,6 	;A0=0
		mov r16,POZ_LCD
		rcall pin_lcd		
		sbi PORTC,7		;WR1=1
		cbi PORTC,7		;WR1=0
		pop r16
ret
; ----------------------------------------------
out_lcd_sym:
		push r16
		mov r16,SYM_LCD
		sbi PORTC,6 	;A0=1
		rcall pin_lcd		
		sbi PORTC,7		;WR1=1
		cbi PORTC,7		;WR1=0
		swap r16
		rcall pin_lcd		
		sbi PORTC,7		;WR1=1
		cbi PORTC,7		;WR1=0
		pop r16
ret
; ----------------------------------------------
pin_lcd:
		out PORTA,r10
		sbrc r16,3
		sbi PORTA,7
		sbrc r16,2
		sbi PORTA,6
		sbrc r16,1
		sbi PORTA,5
		sbrc r16,0
		sbi PORTA,4
ret
; ----------------------------------------------
;���� ��������
; ----------------------------------------------
d_1mks:	nop
		ret
; ----------------------------------------------
d_2mks:	push r16
		ldi r16,2
del4:	dec r16
		brne del4
		pop r16
		ret
; ----------------------------------------------
d_10mks:push r16
		ldi r16,1
		sts t1,r16
		ldi r16,1
		sts t2,r16
		ldi r16,15
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_20mks:push r16
		ldi r16,1
		sts t1,r16
		ldi r16,1
		sts t2,r16
		ldi r16,41
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_65mks:push r16
		ldi r16,1
		sts t1,r16
		ldi r16,1
		sts t2,r16
		ldi r16,162
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_80mks:push r16
		ldi r16,1
		sts t1,r16
		ldi r16,1
		sts t2,r16
		ldi r16,201
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_100mks:push r16
		ldi r16,1
		sts t1,r16
		ldi r16,2
		sts t2,r16
		ldi r16,126
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_500mks:push r16
		ldi r16,1
		sts t1,r16
		ldi r16,10
		sts t2,r16
		ldi r16,131
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_5ms:	push r16
		ldi r16,1
		sts t1,r16
		ldi r16,52
		sts t2,r16
		ldi r16,255
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_25ms:	push r16
		ldi r16,2
		sts t1,r16
		ldi r16,130
		sts t2,r16
		ldi r16,255
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_200ms:push r16
		ldi r16,9
		sts t1,r16
		ldi r16,232
		sts t2,r16
		ldi r16,255
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_800ms:push r16
		ldi r16,34
		sts t1,r16
		ldi r16,250
		sts t2,r16
		ldi r16,250
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
delay:	lds r15,t1
del3:	lds r14,t2
del2:	lds r13,t3
del1:	dec r13
		brne del1
		dec r14
		brne del2
		dec r15
		brne del3
		ret
; ----------------------------------------------