.include "..\_AVR_ASSEMBLER\DEFs\m16def.inc"
.equ 	t1			=$60
.equ 	t2			=$61
.equ 	t3			=$62

.dseg
.org 0x063
buff_command:	.byte 7		;����� �������� ������
byte_count: 	.byte 1		;���-�� �������� ������
byte_type_command: 	.byte 1		;��� ������������ ������� ��� ���
	;6 - ������� 6 ������� �������
	;3 - �������� 3 ������� �������

.def RES_ST	= r3		;������� ������� ��������� ������ �� ��
	;RES_ST: 7��� - 1= ��������� ������ ����� �� ��(������� ������), 0= ��������,����� � ������
	;RES_ST: 6��� - 1= ������� ����� ��� ������� �� ��
	;RES_ST: 5��� - 1= ���������� ��������� � �� ����� PING
	;RES_ST: 4��� - 1= ���������� ��������� ������� � ���
	;RES_ST: 3��� - 1= ���������� ��������� � �� ������ - ��������� ����� TXD ��� � ������ ��������� ����� 2��
	;RES_ST: 2��� - 1= ���������� ���������� ��������� ����������
	;RES_ST: 1��� - 1= ���������� ���������� ��������� ��������� ��������
	;RES_ST: 0��� - 1= ������
	
	
.cseg
.org 0x000
	rjmp start
.org INT0addr	;0x002
	rjmp Receive_RS232        ;��� ������������� ���������� INT0 RS232 - ����� � �����������
.org INT1addr	;0x004
	rjmp Receive_ASU     	 ;��� ������������� ���������� INT1 RS232 - ����� � ���
.org OVF2addr	;0x008
	rjmp over_82			;������������ 2-�� 8-���������� �������� (��� ���������� ����� � ������ ��������� ����� �����) ~2��
.org OVF0addr	;0x012
	rjmp over_8				;��� ������������ �������� ~2�� �� ������ ������� ����� (����� �� 8 ���� = 1,4��) ������ ���������
; ----------------------------------------------
; ��� F=8��� (���������� RC ���������)
; �����: ��� USBASP 1 => ������������� ��� => ����� ���
;					0 => ���������� ��� => �������� ������
; ��� ���� LOCK �����������
;---- Fuse High Byte ----
; OCDEN - 1
; JTAGEN - 1
; SPIEN - 0 ����������
; CKOPT - 1
; EESAVE - 1
; BOOTSZ1 - 0
; BOOTSZ0 - 0
; BOOTRST - 1
;---- Fuse Low Byte ----
; BODLEVEL - 1
; BODEN - 1
; SUT1 - 1
; SUT0 - 0
; CKSEL3 - 0
; CKSEL2 - 1
; CKSEL1 - 0
; CKSEL0 - 0
; ----------------------------------------------
.org 0x030
start:

	ldi r16,low(RAMEND)
	out spl,r16
	ldi r16,high(RAMEND)
	out sph,r16

;rcall Transmit_RS232
	; rcall d_800ms

	clr r10					;R10 - 00000000
	ser r16
	mov r11,r16				;R11 - 11111111

;===============================================
;��������� ������
; ���� A
	out DDRA,r11			;��� ������ �������� ��� �����
	; ldi r16,0b10001110
	; out PORTA,r16			;
	out PORTA,r11			;

; ���� B
	out DDRB,r11			;
	ldi r16,0b00000111
	out PORTB,r16

; ���� C
	out DDRC,r11			;��� ������ �������� ��� �����
	out PORTC,r10			;��� ������������� ���������� �.�. 0

; ���� D
	ldi r16,0b00000011
	out DDRD,r16
	ldi r16,0b00001111
	out PORTD,r16
;===============================================

	ldi r16,0b00000110		; ��������� ���������� �� ������� ������ INT0
	out MCUCR,r16			; ��� 0b00000110 �� ������� ������ INT0 � ������ ��������� �� INT1
	ldi r16,0b11000000		; ��������� ���������� �� INT0
	out GICR,r16			; ��� 0b11000000 �� ������� ������ INT0 � INT1

	ldi r16,0b01000001		;0b00000001 ��������� ���������� �� ������������ 0(8����) ��������
	out TIMSK,r16			;��� 0b01000001 ��������� ���������� �� ������������ 0(8����) � 2(8����) ���������

	ldi xh,high(byte_count)
	ldi xl,low(byte_count)
	st x,r10	 			; ������� ���-�� �������� ������
	clr RES_ST				; ������� ������� ������� ��������� ������ �� ��

	sei
;-------------------------------------------------------
loop:

				sbrc RES_ST,6		;���������� ���� �� ������� ����� �� RS232 �� ��
				rcall analizRS232

				sbrc RES_ST,5		;���������� ���� ������ ���������� � �� ����� PING
				rcall sendPING
					
				sbrc RES_ST,4		;���������� ���� ������ ���������� ������� � ���
				rcall sendASU
		
				sbrc RES_ST,3		;���������� ���� ������ ���������� ��������� �� ������ - ��������� ����� TXD ���
				rcall sendERR_RXD
		
				sbrc RES_ST,2		;���������� ���� ������ ���������� ��������� ����������
				rcall setGEN

				sbrc RES_ST,1		;���������� ���� ������ ���������� ��������� ��������� ��������
				rcall setUM

					; sbi PORTB,4
					; rcall d_200ms ���������� ���������� ��������� ����������
					; cbi PORTB,4
					; rcall d_200ms
rjmp loop
;-------------------------------------------------------
setGEN:
				mov r16,RES_ST
				cbr r16,0b00000100			;(7 ���)����� ������������� ���������� ��������� ����������
				mov RES_ST,r16

				ldi zh,high(buff_command)
				ldi zl,low(buff_command)
				ld r16,z+					;�������� �� ������� �����
				ld r16,z+					;��������� 2 ���� (�������,������� ������� ���������� - ������� ����, ������� �� ������������)

				ldi r17,0b11110011			;(1) ��
				out PORTA,r17
				rcall strobGen

				cbr r16,0b00000101
				sbr r16,0b00001010
				out PORTA,r16				;�������,������� ������� ����������
				rcall strobGen

				ld r16,z+					;��������� 3 ���� (�������: ������� ���� - 1(��) ������, ������� - 2 ������)
				mov r17,r16
				cbr r17,0b00000100
				sbr r17,0b00001011
				out PORTA,r17
				rcall strobGen

				swap r16
				cbr r16,0b00000011
				sbr r16,0b00001100
				out PORTA,r16
				rcall strobGen
				
				ld r16,z+					;��������� 4 ���� (�������: ������� ���� - 3 ������, ������� - 4(��) ������)
				mov r17,r16
				cbr r17,0b00000010
				sbr r17,0b00001101
				out PORTA,r17
				rcall strobGen

				swap r16
				cbr r16,0b00000001
				sbr r16,0b00001110
				out PORTA,r16
				rcall strobGen
				
				ld r16,z+					;��������� 5 ���� (�������,������� ���������� ���������� - ������� ����, ������� �� ������������)
				cbr r16,0b00001001
				sbr r16,0b00000110
				out PORTA,r16
				rcall strobGen
				
				ld r16,z+					;��������� 6 ���� (����������: ������� ���� - 1(��) ������, ������� - 2 ������)
				mov r17,r16
				cbr r17,0b00001000
				sbr r17,0b00000111
				out PORTA,r17
				rcall strobGen

				swap r16
				cbr r16,0b00000111
				sbr r16,0b00001000
				out PORTA,r16
				rcall strobGen
				
				ld r16,z+					;��������� 7 ���� (����������: ������� ���� - 3(��) ������, ������� �� ������������)
				cbr r16,0b00000110
				sbr r16,0b00001001
				out PORTA,r16
				rcall strobGen

				out PORTA,r11				;������� � ��������
ret
;-------------------------------------------------------
strobGen:
				rcall d_500mks
				cbi PORTB,0
				rcall d_1ms
				sbi PORTB,0
				rcall d_500mks
ret
;-------------------------------------------------------
setUM:
				mov r16,RES_ST
				cbr r16,0b00000010			;(7 ���)����� ������������� ���������� ��������� ��������� ��������
				mov RES_ST,r16

				ldi zh,high(buff_command)
				ldi zl,low(buff_command)
				ld r16,z+				;�������� �� ������� �����

				ld r16,z+				;�������� ��������� ����
				;����������
				sbrc r16,0				;���������� ���� ��� ������� (0)
				sbi PORTB,2				;������� 1
				sbrs r16,0				;���������� ���� ��� ���������� (1)
				cbi PORTB,2				;������� 0
				;�����/������
				sbrc r16,1				;���������� ���� ��� ������� (0)
				sbi PORTB,1				;������� 1
				sbrs r16,1				;���������� ���� ��� ���������� (1)
				cbi PORTB,1				;������� 0

				ld r16,z+				;�������� �������������� ����
				out PORTC,r16			;����� ������������
ret
;-------------------------------------------------------
Receive_RS232:	;����� ������ � ��
				cli				;(1)��������� ��� ����������
				push r17		;(2)
				push r16		;(2)
				push xh			;(2)
				push xl			;(2)
				push yh			;(2)
				push yl			;(2)

;��� �������� 57600 ������������ ���� 1/57600=17,361(1)���
;����������� ������������ ���������� ���� � �������� ������� (��������) ����
;�� ������ ���������� �� ������ ����� ����� ������ ������ �������� 
;26��� = 208 �������� ������ (��� f=8MHz ����=0,125���)
				ldi r17,8			;(1)������� �����
				clr r16				;(1)�������� ����
				rcall d_st_rs232	;6.625+1.875=8.5��� ~(8.68=�������� ����)

r_bit:	
				rcall d_bit_rs232	;16,375+0.875=17.25��� ~(17,361=������������ ����)
				clc					;(1)������� ���� �������� �
				sbic PIND,2			;���������� ���� ��� � ����� ������
				sec					;(2)
				ror	r16				;(1)����������� ����� ������ ����� ���� �������� �
				dec r17				;(1)
				brne r_bit			;(2)

				rcall d_bit_rs232	;�������� �� �������� ��������� ����
				
				ldi xh,high(byte_count)
				ldi xl,low(byte_count)
				ld r17,x			; �������� ���-�� �������� ������
				cpi r17,7			; ������� � ����������
				brpl rec2			; ������� ���� ��� r17-8 �������� ������������� �����
				ldi yh,high(buff_command)
				ldi yl,low(buff_command)
				add yl,r17			; ��������� ����� ����� � ������
				brcc rec1			; ���� ��������� �������
				inc yh				; ������������� ������� ������
rec1:			st y,r16			; �������� �������� ���� � ������
				inc r17				; ������������� ���-�� �������� ������
				st x,r17			; � �������� ���

rec2:			ldi r16,0b11100000	; ������� ������ ���������� �� INT0 � INT1 � INT2
				out GIFR,r16

				sbrc RES_ST,7		;���������� ��� ������ �����
				rjmp rec4
				
				mov r16,RES_ST
				sbr r16,0b10000000			;��������� ��������� ������ �����
				mov RES_ST,r16
				;������ ��������
				out TCNT0,r10		;������� ������� �������
				ldi r16,0b00000011	;(���� = 0,125���) * 64 * 256 = 2.048 ��
				out TCCR0,r16		;���� �������

rec4:
				pop yl
				pop yh
				pop xl
				pop xh
				pop r16
				pop r17
				sei
reti
;-------------------------------------------------------
over_8:
				push r17
				push r16
				push xh
				push xl

				out TCCR0,r10	;���� �������
				mov r16,RES_ST
				cbr r16,0b10000000			;(7 ���)����� ��������� ������
				mov RES_ST,r16

				ldi xh,high(byte_count)
				ldi xl,low(byte_count)
				ld r16,x			; �������� ���-�� �������� ������
				st x,r10			;������� ���������� �������� ������

				cpi r16,7
				brne out_over_8		;������� ���� �� ����� 0
					
				mov r16,RES_ST
				sbr r16,0b01000000			;(6 ���)������� ����� � ���������� �������������
				mov RES_ST,r16

out_over_8:	
				pop xl
				pop xh
				pop r16
				pop r17
reti
; ----------------------------------------------
analizRS232:
				mov r16,RES_ST
				cbr r16,0b01000000			;(6 ���)����� ������������� �������
				mov RES_ST,r16

				ldi xh,high(buff_command)
				ldi xl,low(buff_command)
				ld r17,x					;�������� ������ ���� �������

				cpi r17,$55					;������ PING
				brne anlz1
				;�������� �� PING
					mov r16,RES_ST
					sbr r16,0b00100000			;(5 ���)���������� ��������� � �� ����� PING
					mov RES_ST,r16
				ret
anlz1:
				cpi r17,$11					;������� ��� ��� 6 �������
				brne anlz2
				;����������� ������� � ���
					ldi zh,high(byte_type_command)
					ldi zl,low(byte_type_command)
					ldi r16,6
					st z,r16				;������� ������
					mov r16,RES_ST
					sbr r16,0b00010000			;(4 ���)���������� ��������� ������� � ���
					mov RES_ST,r16
				ret
anlz2:
				cpi r17,$12					;������� ��� ��� �������� 3 �������
				brne anlz3
				;����������� ������� � ���
					ldi zh,high(byte_type_command)
					ldi zl,low(byte_type_command)
					ldi r16,3
					st z,r16				;������� ������
					mov r16,RES_ST
					sbr r16,0b00010000			;(4 ���)���������� ��������� ������� � ���
					mov RES_ST,r16
				ret
anlz3:	
				cpi r17,$22					;������� ��� ����������
				brne anlz4
				;����������� ������� � ���������
					mov r16,RES_ST
					sbr r16,0b00000100			;(2 ���) ���������� ���������� ��������� ����������
					mov RES_ST,r16
				ret
anlz4:	
				cpi r17,$33					;������� ��� ��������� ��������
				brne anlz5
				;����������� ������� � ��������� ��������
					mov r16,RES_ST
					sbr r16,0b00000010			;(1 ���) ���������� ���������� ��������� ��������� ��������
					mov RES_ST,r16
				ret
anlz5:	
				cpi r17,$45					;������
				brne anlz6
				;
					sbi PORTB,4
					rcall d_200ms
					cbi PORTB,4
					; rcall d_200ms
					; rcall d_200ms
	
				ret
anlz6:	
	
	
	
ret
; ----------------------------------------------
.cseg	;��������� ���� ������, �� �� �������� ���������� �������� ���������� :(
world_1: .DB $55,$55,$55,$4F,$4B,$55,$55,$00			; ����� PING OK
world_2: .DB $45,$52,$52,$4F,$52,$52,$58,$00			; ����� ERROR RXD
; world_3: .DB 7,$45,$52,$52,$4F,$52,$0D,$0A			;"ERROR"
; world_4: .DB 4,$05,$01,$04,$06						;BFB �����
; ----------------------------------------------
sendPING:
				mov r16,RES_ST
				cbr r16,0b00100000		;(5 ���)����� ������������� ���������� � �� ����� PING
				mov RES_ST,r16

				ldi r18,7				; ������� ������
				ldi zh,high(world_1*2)
				ldi zl,low(world_1*2)

t_byte:
				lpm r16,z+
				ldi r17,8				; ������� �����

				cbi PORTD,0				; ��������� ���
				rcall d_bit_rs232

t_bit:			sbrc r16,0		
				sbi PORTD,0
				sbrs r16,0
				cbi PORTD,0
				ror r16
				rcall d_bit_rs232
				dec r17
				brne t_bit

				sbi PORTD,0
				rcall d_bit_rs232		; �������� ���
	
				dec r18
				brne t_byte
		
ret
; ----------------------------------------------
sendERR_RXD:
				mov r16,RES_ST
				cbr r16,0b00001000		;(3 ���)����� ������������� ���������� ��������� �� ������ - ��������� ����� TXD ���
				mov RES_ST,r16

				ldi r18,7				; ������� ������
				ldi zh,high(world_2*2)
				ldi zl,low(world_2*2)

t_byte0:
				lpm r16,z+
				ldi r17,8				; ������� �����

				cbi PORTD,0				; ��������� ���
				rcall d_bit_rs232

t_bit0:			sbrc r16,0		
				sbi PORTD,0
				sbrs r16,0
				cbi PORTD,0
				ror r16
				rcall d_bit_rs232
				dec r17
				brne t_bit0

				sbi PORTD,0
				rcall d_bit_rs232		; �������� ���
	
				dec r18
				brne t_byte0
		
ret
; ----------------------------------------------
sendASU:
				mov r16,RES_ST
				cbr r16,0b00010000		;(4 ���)����� ������������� ���������� ������� � ���
				mov RES_ST,r16

				cbi PORTD,1				;�������� ���������� ���
				rcall d_2ms
				sbi PORTD,1
				rcall d_20ms

				ldi zh,high(byte_type_command)
				ldi zl,low(byte_type_command)
				ld r18,z				;������� ������
				ldi zh,high(buff_command)
				ldi zl,low(buff_command)
				ld r16,z+				;�������� �� ������� �����

t_byte1:
				ld r16,z+
				ldi r17,8				; ������� �����

				cbi PORTD,1				; ��������� ���
				rcall d_bit_rs232

t_bit1:			sbrc r16,0		
				sbi PORTD,1
				sbrs r16,0
				cbi PORTD,1
				ror r16
				rcall d_bit_rs232
				dec r17
				brne t_bit1

				sbi PORTD,1
				rcall d_bit_rs232		; �������� ���
				
				dec r18
				brne t_byte1

ret
; ----------------------------------------------
; Transmit_RS232:
		; push r17

		; ldi r17,8			;������� �����
		; cbi PORTD,0
		; rcall d_bit_rs232
; t_bit:	sbrc r24,0
		; sbi PORTD,0
		; sbrs r24,0
		; cbi PORTD,0
		; ror r24
		; rcall d_bit_rs232
		; dec r17
		; brne t_bit

		; sbi PORTD,0
		; rcall d_bit_rs232

		; mov r17,RES_ST
		; cbr r17,0b01000000			;(6 ���)
		; mov RES_ST,r17


		; pop r17
		; ret
; ----------------------------------------------
Receive_ASU:	;����� ������ � ���
				cli
				push r16
				
				sbic PIND,3			;���������� ���� ��� � ����� ������
				rjmp rcasu_1
				;������ ������� �� ����� (0)
				cbi PORTD,0

				;�������� ������� ���������� ��������� ����� � ������ ���������
				out TCNT2,r10		;������� ������� �������
				ldi r16,0b00000011	;(���� = 0,125���) * 64 * 256 = 2.048 ��
				out TCCR2,r16		;���� �������
			
			
				rjmp rcasu_exit

rcasu_1:		;������� ������� �� ����� (1)
				sbi PORTD,0
				
				;��������� ������� ���������� ��������� ����� � ������ ���������
				out TCCR2,r10	;���� �������




rcasu_exit:		
				pop r16
				sei
reti
; ----------------------------------------------
over_82:	;������������ 2-�� 8-���������� �������� (��� ���������� ����� � ������ ��������� ����� �����)
				cli
				push r16

				out TCCR2,r10	;���� �������
				sbi PORTD,0				;��������� ����� TXD (�� ����� � ��)

				mov r16,RES_ST
				sbr r16,0b00001000		;(3 ���)���������� ��������� � �� ��������� �� ������ ��������� ����� RXD ���
				mov RES_ST,r16

				pop r16
				sei
reti
; ----------------------------------------------
; Transmit_ASU:

		; cbi PORTD,0			; �������� ���
		; rcall d_bit_rs232
		; sbi PORTD,0	
		; rcall d_20mks		; ��������  ��������

		; ldi r16,1
		; ldi xh,high(byte_count)
		; ldi xl,low(byte_count)
		; ld r17,x			; �������� ���-�� �������� ������
		; ldi yh,high(buff_command)
		; ldi yl,low(buff_command)
; tr_2:	add yl,r16			; ��������� ����� ����� � ������
		; brcc tr_1			; ���� ��������� �������
		; inc yh				; ������������� ������� ������
; tr_1:	ld r25,y			; �������� ���� �� ������
		; rcall Send_ASU
		
		; inc r16
		; cp r17,r16
		; brne tr_2

		; mov r17,RES_ST
		; cbr r17,7			; ����� ����������, ������� ���
		; mov RES_ST,r17
		; st x,r10			; ������� ���-�� �������� ������
		; ret
; ; ----------------------------------------------
; Send_ASU:
		; push r17

		; ldi r17,8			;������� �����
		; cbi PORTD,0
		; rcall d_bit_rs232
; t_b11:	sbrc r25,0
		; sbi PORTD,1
		; sbrs r25,0
		; cbi PORTD,0
		; ror r25
		; rcall d_bit_rs232
		; dec r17
		; brne t_b11

		; sbi PORTD,0
		; rcall d_bit_rs232

		; pop r17
		; ret
; ----------------------------------------------
;���� ��������		������ �������� �������� ���� � ����� �� ������������ ��������
; ----------------------------------------------
; d_1mks:	; 3(rcall)+1+4(ret) = 8*0,125 = 1.0 ���
		; nop
		; ret
; ----------------------------------------------
d_st_rs232:	; 3(rcall)+2+1+ X(1+2)-1 +2+4(ret) = 11+X*3 = 11+14*3 = 53		53*0,125 = 6.625���
		push r16	;2
		ldi r16,14	;1	X
del4:	dec r16		;1
		brne del4	;2/1
		pop r16		;2
		ret			;4
; ----------------------------------------------
d_bit_rs232:	; 3(rcall)+2+1+ 40(1+2)-1 +2+4(ret) = 131		131*0,125 = 16,375���
		push r16
		ldi r16,40
del5:	dec r16
		brne del5
		pop r16
		ret
; ----------------------------------------------
d_20mks:
		push r16
		ldi r16,1
		sts t1,r16
		ldi r16,1
		sts t2,r16
		ldi r16,39
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_500mks:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 4028*0.125 = 503,5���
		push r16
		ldi r16,1
		sts t1,r16
		ldi r16,6
		sts t2,r16
		ldi r16,220
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_1ms:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 8012*0.125 = 1001,5���
		push r16
		ldi r16,1
		sts t1,r16
		ldi r16,12
		sts t2,r16
		ldi r16,220
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_2ms:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } =
		push r16
		ldi r16,1
		sts t1,r16
		ldi r16,24
		sts t2,r16
		ldi r16,220
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_20ms:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 160765*0.125 = 20095,625��� = 20,096��
		push r16
		ldi r16,1
		sts t1,r16
		ldi r16,21
		sts t2,r16
		ldi r16,254
		sts t3,r16
		rcall delay
		pop r16
		ret
; ----------------------------------------------
d_200ms:
		push r16
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
d_800ms:	; 3(rcall)+2+1+2+1+2+1+2+3(rcall)+[2+2+2+2+{ t1*(5+t2*(5+(t3*3-1))-1)-1 }+2+2+2+4(ret)]+2+4(ret) = X*T(0.125) = 
			; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 41+ 34*188504-1 = 6409176*0.125 = 801147��� = 801,147��
		push r16	;2
		ldi r16,34	;1
		sts t1,r16	;2
		ldi r16,250	;1
		sts t2,r16	;2
		ldi r16,250	;1
		sts t3,r16	;2
		rcall delay	;3
		pop r16		;2
		ret			;4
; ----------------------------------------------
;������������� ��. ����. �������(1,1,1)= 41+{1*(5+1*(5+(1*3-1))-1)-1 } = 51*0.125 = 6.375���
delay:	
		push r13	;2
		push r14	;2
		push r15	;2
		lds r15,t1	;2
del3:	lds r14,t2	;2
del2:	lds r13,t3	;2
del1:	dec r13		;1
		brne del1	;2/1
		dec r14		;1
		brne del2	;2/1
		dec r15		;1
		brne del3	;2/1
		pop r15		;2
		pop r14		;2
		pop r13		;2
		ret			;4
; ----------------------------------------------
