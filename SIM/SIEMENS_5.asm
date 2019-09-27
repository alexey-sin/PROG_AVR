.include "m16def.inc"

.equ 	t1			=$60
.equ 	t2			=$61
.equ 	t3			=$62

.eseg
.org 0x000
smsc:	.db $07,$91,$97,$01,$94,$99,$91,$F3 ;7 916 899 91 00 телефон SMS сервис центра (1байт- кол-во знаков в номере)
.org 0x010
da:		.db $07,$91,$97,$08,$56,$70,$46,$F8 ;7 980 650 76 48 телефон получателя (1байт- кол-во знаков в номере)
.org 0x020
ds_1990_1:	.db $8F,$00,$00,$07,$0F,$A1,$E7,$01;ключ:CRC,мл.бит,.......,ст.бит,01H
ds_1990_2:	.db $8F,$00,$00,$07,$0F,$A1,$E7,$01;ключ:CRC,мл.бит,.......,ст.бит,01H
ds_1990_3:	.db $8F,$00,$00,$07,$0F,$A1,$E7,$01;ключ:CRC,мл.бит,.......,ст.бит,01H
STATUS:		.db 0b11111111
	;STATUS: 7бит - 1= разрешено звонить (0-запрещено)
	;STATUS: 6бит - 1= разрешено отправление SMS (0-запрещено)

.dseg
.org 0x063
buff_sms_ascii: .byte 100	;буфер смс в ASCII для передачи
buff_sms_c: 	.byte 1		;кол-во символов в сообщении

ds1820_buffer:	.byte 9	;0б-CRC;......;8б-мл.б.темп
temperature_0:	.byte 4	;0б-знак;1б-десятки;2б-единицы;3б-десятые 
temperature_1:	.byte 4	;0б-знак;1б-десятки;2б-единицы;3б-десятые 
ds1990_buffer:	.byte 8	;0б-CRC;......;8=01H

adc_v: .byte 2				;целые вольты АЦП в ASCII формате
adc_dv: .byte 2				;дробные вольты АЦП в ASCII формате

.def TMPR_ST	= r2		;регистр статуса измерения температуры
	;TMPR_ST: 7бит - 1= измеряемый датчик по входу PC1(вывод 23);0- PC0(вывод 22)
	;TMPR_ST: 6бит - 1= датчик, выводимый на LCD 1-PC1(вывод 23);0- PC0(вывод 22)
	;TMPR_ST: 5бит - резерв
	;TMPR_ST: 4бит - резерв
	;TMPR_ST: 3-0биты - число,отражающее текущее состояние изтерения температуры

.def SMS_ST		= r3		;байт состояния посылок SMS
	;SMS_ST: 7бит - 1= приглашение на выдачу PDU слова получено
	;SMS_ST: 6бит - 1= послан запрос на посылку смс, ждем приглашения
	;SMS_ST: 5бит - 1= есть новое опознаное слово в ответе телефона
	;SMS_ST: 4бит - 1= небходимо отправить запрос на посылку смс
	;SMS_ST: 3бит - 1= смс по поводу батареи
	;SMS_ST: 2бит - 1= PDU выдано, ждем ответ телефона
	;SMS_ST: 1бит - 1= смс по поводу охраны

.def BAT_ST		= r4		;байт состояния батареи
	;BAT_ST: 7бит - 1= устанавливает АЦП: напряжение ниже 11,0V (1 тип)
	;BAT_ST: 6бит - 1= сообщение по 1 типу отправлено

	;BAT_ST: 5бит - 1= устанавливает АЦП: напряжение ниже 10,6V (2 тип)
	;BAT_ST: 4бит - 1= сообщение по 2 типу отправлено

	;BAT_ST: 3бит - 1= устанавливает АЦП: напряжение ниже 10,2V (3 тип)
	;BAT_ST: 2бит - 1= сообщение по 3 типу отправлено

	;BAT_ST: 1бит - 1= типы:/0/-1тип./1/-2тип./1/-3тип.
	;BAT_ST: 0бит - 1=      /1/      /0/      /1/

.def SECR_ST	= r5		;байт состояния охраны
	;SECR_ST: 7бит - 1= система охраны включена

	;SECR_ST: 6бит - 1= сработка 1 датчика	(PA1)
	;SECR_ST: 5бит - 1= сообщение по 1 датчику отправлено

	;SECR_ST: 4бит - 1= сработка 2 датчика	(PA2)
	;SECR_ST: 3бит - 1= сообщение по 2 датчику отправлено

	;SECR_ST: 2бит - 1= сработка 3 датчика	(PA3)
	;SECR_ST: 1бит - 1= сообщение по 3 датчику отправлено

.def POZ_LCD	= r6		;позиция символа в индикаторе  00-09
.def SYM_LCD	= r7		;символ выводимый в индикатор
.def IDENT		= r8		;принятые команды из телефона
	;IDENT:=0 не принято совпадений
	;7 6 5 4 3 2 1 0
	;---------------
;1	;0 0 0 0 0 0 0 1	OK
;2	;0 0 0 0 0 0 1 0	ERROR
;3	;0 0 0 0 0 0 1 1	">"
;4	;0 0 0 0 0 1 0 0	входящий звонок с номером
;5	;0 0 0 0 0 1 0 1	начало приема номера SMS центра
;6	;0 0 0 0 0 1 1 0	звонок
;7	;0 0 0 0 0 1 1 1	вызов отклонен или истекло время ожидания ответа
;8	;0 0 0 0 1 0 0 0	абонент недоступен

.def ERR_DS		= r9		;счетчик ошибок чтения DS1990
.def KEY_ST		= r12		;регистр занятости клавиатуры

.cseg
.org 0x000
	rjmp start
.org OVF2addr	;0x008
	rjmp j_over_8_2			;
.org OVF1addr	;0x010
	rjmp j_over_16			;
.org OVF0addr	;0x012
	rjmp over_8				;
.org URXCaddr	;0x016
	rjmp Receive_Int        ;при возникновении прерывания RXC
; ----------------------------------------------
; для F=8МГц
; ----------------------------------------------
.org 0x030
start:
	ldi r16,low(RAMEND)
	out spl,r16
	ldi r16,high(RAMEND)
	out sph,r16
;	rcall ssp10
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

	rcall j_initlcd
	rcall j_pause

	ldi yh,high(buff_sms_ascii)
	ldi yl,low(buff_sms_ascii)
	st Y,r10   			;Установить указатель сообщения в ноль	

	clr TMPR_ST			;очистим регистр температурных измерений
	clr BAT_ST			;обнулим состояние Батареи
	clr SMS_ST			;обнулим состояние посылок SMS
	clr SECR_ST			;система снята с охраны
	clr KEY_ST			;начальное состояние клавиатуры
	clr IDENT			;обнулим состояние идентификации принятых слов
	clr ERR_DS			;обнулим счетчик ошибок чтения DS1990

	rcall clear_world_c	;обнулим состояние правильно принятых байтов

	out EEARH,r10


	ldi r16,25
	out UBRRL,r16			;19200 bod
	ldi r16,0b10011000		;RXCIE=1(прерывание по приему);RXEN=1;TXEN=1
	out UCSRB,r16			;on USART
;Настроим АЦП
	ldi r16,(1<<ADEN)|(1<<ADSC)|(7<<ADPS0)	;/128
	out ADCSRA,r16
	ldi r16,0b11100000		;выравнивание по левой границе
	out ADMUX,r16

		out TCNT1H,r10
		out TCNT1L,r10			;
		ldi r16,0b01000101		;разрешить прерывание по переполнению 1(16разр),2(8разр)и 0(8разр) счетчиков
		out TIMSK,r16
		ldi r16,0b00000101
		out TCCR1B,r16
		out TCCR0,r10
		out TCCR2,r10

	sei

		rcall j_ds_error
		mov TMPR_ST,r11
		rcall j_ds_error
		clr TMPR_ST

		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms
		rcall j_initlcd

		rcall out_volt		;проверка батареи и вывод напряжения
		rcall set_BAT_ST_of_situation
		rcall set_SECR_ST_of_situation
;-------------------------------------------------------
loop:
		sbrs SECR_ST,7		;пропустить если под охраной
		rcall test_key		;проверка клавиатуры

		rcall test_sensors	;проверка датчиков
		rcall out_volt		;проверка батареи и вывод напряжения
;-------------------------------------------------------
		tst IDENT
		breq k10
		mov r16,IDENT

		cpi r16,3			;">"
		brne k0
		mov r17,SMS_ST
		cbr r17,0b01000000
		sbr r17,0b10000000
		mov SMS_ST,r17
		rcall beep_25ms
		rjmp k8
;--------
k0:		cpi r16,1			;"OK"
		brne k1
		rcall beep_25ms
		rjmp k8
;--------
k1:		cpi r16,2			;"ERROR"
		brne k2
		rcall beep_200ms
		rjmp k8
;--------
k2:		cpi r16,6			;"RING"
		brne k8
		ldi zh,high(AT_H*2)
		ldi zl,low(AT_H*2)
		rcall out_uart			;посылка "ОТБОЙ"


k8:		mov IDENT,r10

;--------
k10:	sbrc SMS_ST,7
		rcall send_pdu

		sbrs SMS_ST,4		;событие=>отправка запроса СМС или звонок или все вместе
		rjmp k9

	    ldi r24,high(STATUS)
	    ldi r25,low(STATUS)
		rcall load_e		;чтение из EEPROM состояния байта STATUS
		mov r23,r16
		sbrs r23,7			;пропустить если надо звонить
		rjmp k11
;звонок
		ldi zh,high(AT_D*2)
		ldi zl,low(AT_D*2)
		lpm r16,z+
k14:	lpm r17,z+
		rcall out_byte
		dec r16
		brne k14

	    ldi r24,high(da)
	    ldi r25,low(da)
		rcall load_e		;чтение из EEPROM номера телефона
		mov r19,r16
		rcall load_e		;загрузка $91, по сути просто увеличение адреса EE
		dec r19

k12:	rcall load_e
		mov r18,r16
		cbr r16,0b11110000
		subi r16,-$30
		mov r17,r16
		rcall out_byte
		swap r18
		cbr r18,0b11110000
		subi r18,-$30
		mov r17,r18
		cpi r17,$3F
		breq k15
		rcall out_byte
k15:	dec r19
		brne k12
		ldi r17,$0D	;перевод каретки
		rcall out_byte
;отправили команду, ждем ответ
k13:	mov r16,IDENT			;ждем от телефона ответа о номере SMSC
		cpi r16,7
		breq k11
		cpi r16,8
		breq k11
		rjmp k13
;ответ получен





k11:	mov IDENT,r10
		sbrc r23,6			;пропустить если ненадо отправить СМС
		rcall send_query

		sbrc SMS_ST,3			;смс по поводу батареи
		rcall set_BAT_ST_of_situation
		sbrc SMS_ST,1			;смс по поводу охраны
		rcall set_SECR_ST_of_situation

		mov r16,SMS_ST
		cbr r16,0b00011010		;cнимаем флаг задач и типа СМС
		mov SMS_ST,r16
; ----------------------------------------------
AT_D: .DB 4,'A','T','D','+',0
; ----------------------------------------------
k9:		mov r16,BAT_ST
		sbrs BAT_ST,7			;U=11,0-10,6
		rjmp kb1
		sbrc BAT_ST,6			;смс еще не посылалось
		rjmp kb1
		sbr r16,0b00000001
		cbr r16,0b00000010
		mov BAT_ST,r16			;1 тип батарейного смс
		mov r16,SMS_ST
		sbr r16,0b00001000		;смс по поводу батареи	
		mov SMS_ST,r16
		sbrc SMS_ST,6			;пропустить если уже послан запрос на посылку смс
		rjmp kb1
		sbrc SECR_ST,7			;пропустить если не под охраной
		rcall sms_bat_1
		sbrs SECR_ST,7			;пропустить если под охраной
		rcall signal

kb1:
		mov r16,BAT_ST
		sbrs BAT_ST,5			;U=10,6-10,2
		rjmp kb2
		sbrc BAT_ST,4			;смс еще не посылалось
		rjmp kb2
		sbr r16,0b00000010
		cbr r16,0b00000001
		mov BAT_ST,r16			;2 тип батарейного смс
		mov r16,SMS_ST
		sbr r16,0b00001000		;смс по поводу батареи	
		mov SMS_ST,r16
		sbrc SMS_ST,6			;пропустить если уже послан запрос на посылку смс
		rjmp kb2
		sbrc SECR_ST,7			;пропустить если не под охраной
		rcall sms_bat_1
		sbrs SECR_ST,7			;пропустить если под охраной
		rcall signal


kb2:
		mov r16,BAT_ST
		sbrs BAT_ST,3			;U=<10,2
		rjmp kb4
		sbrc BAT_ST,2			;смс еще не посылалось
		rjmp kb3
		sbr r16,0b00000001
		sbr r16,0b00000010
		mov BAT_ST,r16			;3 тип батарейного смс
		mov r16,SMS_ST
		sbr r16,0b00001000		;смс по поводу батареи	
		mov SMS_ST,r16
		sbrc SMS_ST,6			;пропустить если уже послан запрос на посылку смс
		rjmp kb4
		sbrc SECR_ST,7			;пропустить если не под охраной
		rcall sms_bat_2
		sbrs SECR_ST,7			;пропустить если под охраной
		rcall signal
		rjmp kb4

kb3:	sbi PORTC,2				;выключим поддержку питания

;--------
kb4:
		sbrs SECR_ST,7			;пропустить если не под охраной
		rjmp ks1

		rcall ds1990

		sbrs SECR_ST,6			;пропустить если сработал 1 датчик
		rjmp ks2
		sbrc SECR_ST,5			;пропустить если sms по 1 датчику уже отправлено
		rjmp ks2
		sbrc SMS_ST,6			;пропустить если уже послан запрос на посылку смс
		rjmp ks2
		sbrc SMS_ST,2			;PDU выдано, ждем ответ телефона
		rjmp ks2
		rcall sms_secr

ks2:	sbrs SECR_ST,4			;пропустить если сработал 2 датчик
		rjmp ks3
		sbrc SECR_ST,3			;пропустить если sms по 2 датчику уже отправлено
		rjmp ks3
		sbrc SMS_ST,6			;пропустить если уже послан запрос на посылку смс
		rjmp ks3
		sbrc SMS_ST,2			;PDU выдано, ждем ответ телефона
		rjmp ks3
		rcall sms_secr

ks3:	sbrs SECR_ST,2			;пропустить если сработал 3 датчик
		rjmp ks1
		sbrc SECR_ST,1			;пропустить если sms по 3 датчику уже отправлено
		rjmp ks1
		sbrc SMS_ST,6			;пропустить если уже послан запрос на посылку смс
		rjmp ks1
		sbrc SMS_ST,2			;PDU выдано, ждем ответ телефона
		rjmp ks1
		rcall sms_secr
;--------
ks1:	clr POZ_LCD
		rcall j_out_lcd_poz		;вывод на экран

		sbrs TMPR_ST,6			;вывод номера датчика
		rjmp ks6
		ldi yh,high(temperature_1)
		ldi yl,low(temperature_1)
		ldi r16,2
		mov SYM_LCD,r16
		rcall j_symbol 
		rjmp ks7
ks6:	ldi yh,high(temperature_0)
		ldi yl,low(temperature_0)
		ldi r16,1
		mov SYM_LCD,r16
		rcall j_symbol 

ks7:	ld r16,y+
		mov SYM_LCD,r16
		rcall j_symbol 			;вывод знака
		
		clt
		ld r16,y+				;десятки, если 0->пропускаем
		tst r16
		breq ks8
		set
		mov SYM_LCD,r16
		rcall j_symbol 

ks8:	ld r16,y+				;единицы
		cpi r16,28
		breq ks9
		subi r16,-16			;добавим точку
ks9:	mov SYM_LCD,r16
		rcall j_symbol 
		
		ld r16,y				;десятые
		mov SYM_LCD,r16
		rcall j_symbol

		brts ks10
		clr SYM_LCD
		rcall j_out_lcd_sym

ks10:	ldi r16,6				;вывод на экран индикатора охраны
		mov POZ_LCD,r16
		rcall j_out_lcd_poz

		clr SYM_LCD
		sbrs SECR_ST,7			;пропустить если под охраной
		rjmp ks4
		ldi r16,71
		mov SYM_LCD,r16
ks4:	rcall j_out_lcd_sym



rjmp loop
;-------------------------------------------------------
ds1990:

		sbi DDRD,2		;post0
		rcall dj_500mks
		cbi DDRD,2		;post1
		rcall dj_100mks
		sbic PIND,2
		ret
		rcall dj_500mks

		push xh
		push xl
		push r16
		push r17
		push r18
		push r19
		push r20
		in r16,SREG
		push r16

		ldi r16,$33
		rcall post_ds1990
		rcall dj_20mks
		rcall read_ds1990_ROM
		rcall check_crc_ds1990_buffer
		brtc ai1
		rjmp ai100

;проверка на валидность принятых данных
ai1:	clr r17				;здесь будем считать 00
		clr r19				;здесь будем считать FF
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
		brne ai5		;прочитаны одни 00
		rjmp ai100
ai5:	cpi r19,8
		brne ai6		;прочитаны одни FF
		rjmp ai100
;конец проверки
ai6:
;проверка ключа
		clt 					;начальная установка бита T
		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
	    ldi r24,high(ds_1990_1)
	    ldi r25,low(ds_1990_1)
		rcall valid_key

		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
	    ldi r24,high(ds_1990_2)
	    ldi r25,low(ds_1990_2)
		rcall valid_key

		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
	    ldi r24,high(ds_1990_3)
	    ldi r25,low(ds_1990_3)
		rcall valid_key

		brtc ai9				;перейти если все же ключа нет в списке
;конец проверки ключа
ai8:	sbi PORTC,5		;светодиод
		rcall beep_25ms	;beep
		rcall dj_800ms
		cbi PORTC,5		;светодиод
		rcall gard_off
		rjmp ai100

ai9:	;попытка подбора ключа
		sbi PORTC,5		;светодиод
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall dj_800ms
		cbi PORTC,5		;светодиод
		mov r16,ERR_DS
		inc r16
		mov ERR_DS,r16
		cpi r16,3
		brne ai100
		rcall selecting_key
		clr ERR_DS

ai100:	pop r16
		out SREG,r16
		pop r20
		pop r19
		pop r18
		pop r17
		pop r16
		pop xl
		pop xh
ret
; ----------------------------------------------
valid_key:				;проверка ключа DS1990 на наличие номера в памяти
		ldi r18,8
v_k1:	rcall load_e
		ld r17,x+
		cp r16,r17
		brne v_k2
		dec r18
		brne v_k1
		set
v_k2:	ret
; ----------------------------------------------
load_e:	out EEARH,r24		;чтение из EEPROM с постинкрементом r24,r25
		out EEARL,r25		;вход r24-(H);r25-(L-1)=>адрес; r16-данные
		sbi EECR,EERE
		in r16,EEDR
		inc r25
		brne l_e1
		inc r24
l_e1:ret
;-------------------------------------------------------
gard_off:			;снятие с охраны
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
		ldi r17,8		;счетчик байтов
i910:	ldi r18,8		;счетчик бит
		clr r19
i911:	sbi DDRD,2		;post0		;инт0 на выход
		rcall dj_1mks
		cbi DDRD,2		;post1		;настроимся на прием по инт0
		rcall dj_10mks
		in r16,PIND		;
		sbrc r16,2
		sbr r19,$80
		clc
		ror r19
		rcall dj_65mks
		dec r18
		brne i911
		rol r19		;последний сдвиг вправо был лишним
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
check_crc_ds1990_buffer:	;если ошибка, бит Т устанавливается в 1, если все правильно - в 0
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
        ldi r18,8			;колличество битов
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
post_ds1990:		;передаем через PIND,
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
		rcall dj_10mks
		cbi DDRD,2		;post1
		rcall dj_80mks
ret
post_0_ds1990:	
		sbi DDRD,2		;post0
		rcall dj_80mks
		rcall dj_10mks
		cbi DDRD,2		;post1
ret
;-------------------------------------------------------
test_key:	;проверка клавиатуры
		tst KEY_ST
		brne k400

		sbic PINB,0			;кнопка 0 "ОХРАНА"
		rjmp k000
		rcall key_busy
		rcall set_security

k000:
		sbic PINB,1			;кнопка 1 "TEST"
		rjmp k100
		rcall key_busy
		rcall key_1

k100:
		sbic PINB,2			;кнопка 2 № температурного датчика
		rjmp k200
		rcall key_busy
		rcall key_2

k200:
		sbic PINB,3			;кнопка 3
		rjmp k300
		rcall key_busy
		rcall key_3

k300:
		sbic PINB,4			;кнопка 4	SET (установка ключа охраны)
		rjmp k400
		rcall key_busy
		rcall set_key_DS1990
		rcall j_initlcd
k400:	ret
;-------------------------------------------------------
key_busy:
		rcall beep_25ms
		out TCNT0,r10		;обнулим счетчик таймера
		ldi r16,0b00000101
		out TCCR0,r16		;пуск счетчик
		ldi r16,0b00001010
		mov KEY_ST,r16
ret
; ----------------------------------------------
over_8:
		push r16
		out TCCR0,r10	;стор счетчик
		mov r16,KEY_ST
		dec r16
		tst r16
		breq ovr8_1
		mov KEY_ST,r16
		out TCNT0,r10
		ldi r16,0b00000101
		out TCCR0,r16	;пуск счетчик
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
ss2:	sbi PORTC,5		;светодиод
		rcall dj_200ms
		rcall test_sensors
		cbi PORTC,5		;светодиод
		sbis PINB,0
		rjmp ss1
		rcall dj_200ms
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

		ldi zh,high(AT_CNNI*2)
		ldi zl,low(AT_CNNI*2)
		rcall out_uart
		ldi zh,high(AT_CLIP*2)
		ldi zl,low(AT_CLIP*2)
		rcall out_uart

ss1:	rcall beep_25ms
ss3:	pop r16
ret
; ----------------------------------------------
AT_CNNI: .DB 17,'A','T','+','C','N','M','I','=','1',',','1',',','0',',','0',',','1'
; ----------------------------------------------
test_sensors:
		push r16
		mov r16,SECR_ST

		sbis PINA,1			;датчик 1 (ворота, дверь)
		cbr r16,0b01000000
		sbic PINA,1
		sbr r16,0b01000000

		sbis PINA,2			;датчик 2
		cbr r16,0b00010000
		sbic PINA,2
		sbr r16,0b00010000

		sbis PINA,3			;датчик 3
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
		sbi PORTC,5		;светодиод
		ldi r16,3
sg1:	rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall beep_200ms	;beep
		rcall d_800ms
		dec r16
		brne sg1
		cbi PORTC,5		;светодиод
		pop r16
ret
; ----------------------------------------------
selecting_key:		;сообщение о попытке подбора ключа
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
		sbr r16,0b00010000		;установим флаг на посылку запроса
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
s_key_1: .DB 22,'П','о','п','ы','т','к','а',' ','п','о','д','б','о','р','а',' ','к','л','ю','ч','а','\r',0
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
		sbr r16,0b00010000		;установим флаг на посылку запроса
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
key_2:	;переключаем датчики
		mov r16,TMPR_ST
		sbrs TMPR_ST,6
		sbr r16,0b01000000

		sbrc TMPR_ST,6
		cbr r16,0b01000000
		mov TMPR_ST,r16
ret
; ----------------------------------------------
key_3:

		sbi PORTC,2				;выключим поддержку питания

ret
; ----------------------------------------------
set_key_DS1990:
		ldi r23,1				;номер записываемого ключа

skl_8:	rcall set_key_i			;вывод на экран

set_key_loop:
		tst KEY_ST
		brne skl_0

		sbic PINB,4			;кнопка 4 "выход"
		rjmp skl_0
		rcall key_busy

		inc r23
		cpi r23,4
		brne skl_8
		rcall set_telefon
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
		push r24
		push r25

		ldi r16,$33
		rcall post_ds1990
		rcall d_20mks
		rcall read_ds1990_ROM
		rcall check_crc_ds1990_buffer
		brtc skl_1
		rjmp skl_100

;проверка на валидность принятых данных
skl_1:	clr r17				;здесь будем считать 00
		clr r19				;здесь будем считать FF
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
		brne skl_5		;прочитаны одни 00
		rjmp skl_100
skl_5:	cpi r19,8
		brne skl_6		;прочитаны одни FF
		rjmp skl_100
;конец проверки
skl_6:
		ldi r19,8
		ldi xh,high(ds1990_buffer)
		ldi xl,low(ds1990_buffer)
		
		cpi r23,1
		brne skl_9
	    ldi r24,high(ds_1990_1)
	    ldi r25,low(ds_1990_1)
		rjmp skl_7

skl_9:	cpi r23,2
		brne skl_10
	    ldi r24,high(ds_1990_2)
	    ldi r25,low(ds_1990_2)
		rjmp skl_7

skl_10:	ldi r24,high(ds_1990_3)
	    ldi r25,low(ds_1990_3)

skl_7:	ld r16,x+
		rcall save_e
		dec r19
		tst r19
		brne skl_7

		sbi PORTC,5		;светодиод
		rcall beep_25ms	;beep
		cbi PORTC,5		;светодиод
		pop r25
		pop r24
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
set_key_i:
		rcall j_initlcd
		clr POZ_LCD
		rcall out_lcd_poz
		ldi r16,$CD
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,$8F
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,$87
		mov SYM_LCD,r16
		rcall out_lcd_sym

		ldi r16,0
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,1
		mov SYM_LCD,r16
		rcall symbol
		ldi r16,28				;-
		mov SYM_LCD,r16
		rcall symbol
		mov r16,r23				;номер записываемого ключа
		mov SYM_LCD,r16
		rcall symbol
ret
; ----------------------------------------------
set_telefon:
		rcall j_initlcd
		clr POZ_LCD
		rcall out_lcd_poz		;вывод на экран
		ldi r16,$CD
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,$8F
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,$87
		mov SYM_LCD,r16
		rcall out_lcd_sym

		ldi r16,0
		mov SYM_LCD,r16
		rcall out_lcd_sym
		ldi r16,2
		mov SYM_LCD,r16
		rcall symbol

		ldi zh,high(AT_CLIP*2)
		ldi zl,low(AT_CLIP*2)
		rcall out_uart

set_tel_loop:
		tst KEY_ST
		brne stl_0

		sbic PINB,4			;кнопка 4 "выход"
		rjmp stl_0
		rcall key_busy
		ret
stl_0:
		mov r16,IDENT			;если принято входящий звонок с номером
		cpi r16,4
		brne set_tel_loop
; выключим прерывание по UART и принимаем номер
		mov IDENT,r10
		cli
		push xh
		push xl
		push zh
		push zl
		push r17
		push r24
		push r25

		clr r17
		ldi zh,high(buff_sms_ascii)
		ldi zl,low(buff_sms_ascii)
reseiv1:sbis UCSRA,RXC
		rjmp reseiv1
		in r16,UDR
		cpi r16,$22			;(")-конец приема номера
		breq stl_1
		inc r17
		subi r16,$30
		st z+,r16
		rjmp reseiv1

stl_1:	mov r18,r17
		sbrs r18,0
		rjmp stl_4
		inc r18
		ldi r16,$0F
		st z,r16

stl_4:	
		ror r18
		inc r18		;в r18 первый байт преобразованного номера (07)

		rcall j_initlcd
		clr POZ_LCD
		rcall out_lcd_poz		;вывод на экран

		subi r17,10
		ldi zh,high(buff_sms_ascii)
		ldi zl,low(buff_sms_ascii)
		add zl,r17
		brcc stl_2
		inc zh
stl_2:	
		ldi r17,10
stl_3:	ld r16,z+				;
		mov SYM_LCD,r16
		rcall symbol
		dec r17
		brne stl_3
		

	    ldi r24,high(da)
	    ldi r25,low(da)
		mov r16,r18
		rcall save_e
		ldi r16,$91
		rcall save_e

		ldi zh,high(buff_sms_ascii)
		ldi zl,low(buff_sms_ascii)

stl_5:	ld r16,z+
		swap r16
		ld r17,z+
		or r16,r17
		swap r16
		rcall save_e
		dec r18
		brne stl_5
		rcall dj_800ms
		ldi zh,high(AT_H*2)
		ldi zl,low(AT_H*2)
		rcall out_uart			;посылка "ОТБОЙ"
		rcall dj_800ms

		sei
		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms

		rcall beep_25ms

		mov IDENT,r10
		ldi zh,high(AT_CSCA*2)
		ldi zl,low(AT_CSCA*2)
		rcall out_uart

stl_6:	mov r16,IDENT			;ждем от телефона ответа о номере SMSC
		cpi r16,5
		brne stl_6
		cli
		mov IDENT,r10

		clr r17
		ldi zh,high(buff_sms_ascii)
		ldi zl,low(buff_sms_ascii)
reseiv2:sbis UCSRA,RXC
		rjmp reseiv2
		in r16,UDR
		cpi r16,$22			;(")-конец приема номера
		breq stl_7
		inc r17
		subi r16,$30
		st z+,r16
		rjmp reseiv2

stl_7:	mov r18,r17
		sbrs r18,0
		rjmp stl_8
		inc r18
		ldi r16,$0F
		st z,r16

stl_8:	
		ror r18
		inc r18		;в r18 первый байт преобразованного номера (07)

		rcall j_initlcd
		clr POZ_LCD
		rcall out_lcd_poz		;вывод на экран

		subi r17,10
		ldi zh,high(buff_sms_ascii)
		ldi zl,low(buff_sms_ascii)
		add zl,r17
		brcc stl_9
		inc zh
stl_9:	
		ldi r17,10
stl_10:	ld r16,z+				;
		mov SYM_LCD,r16
		rcall symbol
		dec r17
		brne stl_10
		

	    ldi r24,high(smsc)
	    ldi r25,low(smsc)
		mov r16,r18
		rcall save_e
		ldi r16,$91
		rcall save_e

		ldi zh,high(buff_sms_ascii)
		ldi zl,low(buff_sms_ascii)

stl_11:	ld r16,z+
		swap r16
		ld r17,z+
		or r16,r17
		swap r16
		rcall save_e
		dec r18
		brne stl_11

		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms
		rcall dj_800ms

		rcall beep_25ms
		rcall j_initlcd

		pop r25
		pop r24
		pop r17
		pop zl
		pop zh
		pop xl
		pop xh
		sei
		ret
rjmp set_tel_loop
; ----------------------------------------------
AT_CSCA:.DB 8,'A','T','+','C','S','C','A','?',0
; ----------------------------------------------
AT_H:	.DB 3,'A','T','H'
; ----------------------------------------------
save_e:	sbic EECR,1			;сохранение в EEPROM с постинкрементом r24,r25
		rjmp save_e			;вход r24-(H);r25-(L-1)=>адрес; r16-данные		out EEDR,r16
		out EEARH,r24
		out EEARL,r25
		out EEDR,r16
		sbi EECR,2
		nop
		sbi EECR,1
		inc r25
		brne s_e1
		inc r24
s_e1:ret
; ----------------------------------------------
AT_CLIP: .DB 9,'A','T','+','C','L','I','P','=','1'	;чтобы телефон выдавал номер вызываемого абонента
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

		sbrs SECR_ST,6			;пропустить если сработал 1 датчик
		rjmp ssc_3
		sbrc SECR_ST,5			;пропустить если sms по 1 датчику уже отправлено
		rjmp ssc_3
		ldi r18,'1'
		st x+,r18

ssc_3:	sbrs SECR_ST,4			;пропустить если сработал 2 датчик
		rjmp ssc_4
		sbrc SECR_ST,3			;пропустить если sms по 2 датчику уже отправлено
		rjmp ssc_4
		ldi r18,'2'
		st x+,r18

ssc_4:	sbrs SECR_ST,2			;пропустить если сработал 3 датчик
		rjmp ssc_5
		sbrc SECR_ST,1			;пропустить если sms по 3 датчику уже отправлено
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

		sbrs SECR_ST,6			;пропустить если сработал 1 датчик
		rjmp ssc_7
		sbrc SECR_ST,5			;пропустить если sms по 1 датчику уже отправлено
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
		sbr r16,0b00010010		;установим флаг на посылку запроса
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
dj_1mks:
		rcall d_1mks
ret
dj_10mks:
		rcall d_10mks
ret
dj_20mks:
		rcall d_20mks
ret
dj_65mks:
		rcall d_65mks
ret
dj_80mks:
		rcall d_80mks
ret
dj_100mks:
		rcall d_100mks
ret
dj_500mks:
		rcall d_500mks
ret
dj_200ms:
		rcall d_200ms
ret
dj_800ms:
		rcall d_800ms
ret
j_initlcd:
		rcall initlcd
ret
j_pause:
		rcall pause
ret
j_out_lcd_poz:
		rcall out_lcd_poz
ret
j_out_lcd_sym:
		rcall out_lcd_sym
ret
j_symbol:
		rcall symbol
ret
j_ds_error:
		rcall ds_error
ret
j_over_8_2:
		rcall over_8_2
ret
j_over_16:
		rcall over_16
ret
; ----------------------------------------------
s_secr_1: .DB 8,'Т','Р','Е','В','О','Г','А','\r',0
s_secr_2: .DB 9,'с','р','а','б','о','т','а','л',' '
s_secr_3: .DB 8,' ','д','а','т','ч','и','к','\r',0
s_secr_4: .DB 15,'(','в','о','р','о','т','а',',',' ','д','в','е','р','ь',')'
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
		sbr r16,0b00010000		;установим флаг на посылку запроса
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
		sbr r16,0b00010000		;установим флаг на посылку запроса
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
s_bat_1: .DB 18,'Б','А','Т','А','Р','Е','Я',' ','Р','А','З','Р','Я','Ж','Е','Н','А','\r',0
s_bat_2: .DB 3,'U','=',' '
s_bat_3: .DB 3,' ','в','\r'
s_bat_4: .DB 27,'У','С','Т','Р','О','Й','С','Т','В','О',' ','Б','У','Д','Е','Т',' ','В','Ы','К','Л','Ю','Ч','Е','Н','О','!'
; ----------------------------------------------
out_volt:
		ldi r16,7
		mov POZ_LCD,r16
		rcall out_lcd_poz

 		sbi ADCSRA,ADSC
ov1:	sbis ADCSRA,ADIF
		rjmp ov1
		in r16,ADCH
; здесь можно сравнить полученное значение с предельно допустимым
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

ov13:	;clr r17			;очищение регистра при возврате питания
		
ov9:	mov BAT_ST,r17		;сохраним состояние батареии в байте BAT_ST

		rcall adc_calc		;r16-вход; (r17:r16) - выход результат вычислений в двоичной форме
; выведем на экран
		ldi r18,-1
ov2:	inc r18
		subi r16,100
        brsh ov2
		subi r17,1
		brsh ov2
		subi r16,-100

		mov r19,r16

		ldi r16,-1		;будущие десятки целых
ov3:	inc r16
		subi r18,10
        brsh ov3
		subi r18,-10
		tst r16			;проверим на ноль
		brne ov7		;перейти если нет
		clr SYM_LCD		;если да вывести пусто(пробел)
		rcall out_lcd_sym
		rjmp ov8
ov7:	mov SYM_LCD,r16
		rcall symbol
ov8:	subi r16,-$30
		ldi yh,high(adc_v)
		ldi yl,low(adc_v)
		st y+,r16
		
		ldi r16,-1		;будущие единицы целых
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

		ldi r16,-1		;будущие десятые
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

		ldi r16,-1		;будущие сотые
ov10:	inc r16
		subi r19,1
        brsh ov10
		subi r19,-1
		subi r16,-$30
		st y,r16
ret
; ----------------------------------------------
c_adc: .DB 5,20,17,0		;коэффициент умножения для значений АЦП (5,2017)
; ----------------------------------------------
adc_calc:					;r16 - входное умножаемое число
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
		lpm r17,z+			;загружаем первый множитель - целое число
		mul r16,r17			;умножаем
		movw r24,r0			;переносим результат в (r25:r24) - будущий окончательный результат
		lpm r17,z+			;загружаем первые два знака после запятой дробного множителя
		mul r16,r17			;умножаем
		movw r22,r0			;переносим в (r23:r22) - регистры для деления на 100
		rcall subi_100		;поделили на 100
		add r24,r20
		brcc ac3
		inc r25
ac3:	add r25,r21			;сложили полученное к общему результату
		lpm r17,z			;загружаем третий и четвертый знаки после запятой дробного множителя
		mul r16,r17			;умножаем
		movw r22,r0			;переносим в (r23:r22) - регистры для деления на 100
		rcall subi_100		;поделили на 100
		movw r22,r20		;переносим для повторного деления
		rcall subi_100		;поделили на 100
		add r24,r20
		brcc ac4
		inc r25
ac4:	add r25,r21			;сложили полученное к общему результату
		movw r16,r24		;переносим для выноса из подпрограммы

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
subi_100:					;(r23:r22) - регистры для деления на 100;  (r21:r20) - результат деления
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
send_query:
		ldi r16,low(smsc)
		out EEARL,r16
		sbi EECR,EERE
		in r17,EEDR

	    ldi r16,low(da)
		out EEARL,r16
		sbi EECR,EERE
		in r16,EEDR
		
		add r17,r16
		ldi zh,high(buff_sms_c)
		ldi zl,low(buff_sms_c)
		ld r16,Z
		lsl r16
		add r17,r16
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
		sbr r16,0b01000000		;послан запрос на посылку смс
		mov SMS_ST,r16

ret
; ----------------------------------------------
out_double_byte:	;r19-вход, выход:r17
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
out_double_byte_rus:	;r19-вход, выход:r19
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
out_double_byte_eng:	;r19-вход, выход:r19
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
		rcall read_eerom	;вход: r16-адрес, выход: r19-данные
		mov r17,r19
		rcall out_double_byte

sp1:	inc r16
		rcall read_eerom	;вход: r16-адрес, выход: r19-данные
		rcall out_double_byte
		dec r17
		brne sp1

		ldi r19,$11
		rcall out_double_byte
		clr r19
		rcall out_double_byte


	    ldi r16,low(da)
		rcall read_eerom	;вход: r16-адрес, выход: r19-данные
		mov r17,r19
		add r16,r19
		rcall read_eerom	;вход: r16-адрес, выход: r19-данные
		mov r20,r17
		dec r20
		clc
		rol r20
		cbr r19,0b00001111
		cpi r19,0b11110000
		brne sp6

		dec r20
sp6:	mov r19,r20
		rcall out_double_byte
	    ldi r16,low(da)
sp5:	inc r16
		rcall read_eerom	;вход: r16-адрес, выход: r19-данные
		rcall out_double_byte
		dec r17
		brne sp5

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

		clr SMS_ST

ret
; ----------------------------------------------
read_eerom:		;вход: r16-адрес, выход: r19-данные
		out EEARL,r16
		sbi EECR,EERE
		in r19,EEDR
ret
; ----------------------------------------------
set_SECR_ST_of_situation:
		push r16
		
		mov r16,SECR_ST
		sbrc r16,6				;пропустить если не сработал 1 датчик
		sbr r16,0b00100000	

		sbrc r16,4				;пропустить если не сработал 2 датчик
		sbr r16,0b00001000	

		sbrc r16,2				;пропустить если не сработал 3 датчик
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
;ПП обработки прерывания при поступлении очередного байта в приемник UART
Receive_Int:
		push zh
		push zl
		push xh
		push xl
		push r19
		push r18
		push r17
		push r16
		in r16,SREG
		push r16

		in r16,UDR             ;Считать байт из регистра данных приемника
		rcall identify

		pop r16
		out SREG,r16
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
identify:		;r16 - принятый байт
		clt
		ldi zh,high(world_1*2)
		ldi zl,low(world_1*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_1_c)
		ldi xl,low(world_1_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id01
		inc zh
id01:	lpm r19,z
		cp r19,r16
		breq id02
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id10
id02:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id10
		st x,r10
		ldi r17,3			;принято ">"
		mov IDENT,r17
;--------------
id10:	ldi zh,high(world_2*2)
		ldi zl,low(world_2*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_2_c)
		ldi xl,low(world_2_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id11
		inc zh
id11:	lpm r19,z
		cp r19,r16
		breq id12
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id20
id12:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id20
		st x,r10
		ldi r17,1			;принято "OK"
		mov IDENT,r17
;--------------
id20:	ldi zh,high(world_3*2)
		ldi zl,low(world_3*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_3_c)
		ldi xl,low(world_3_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id21
		inc zh
id21:	lpm r19,z
		cp r19,r16
		breq id22
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id30
id22:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id30
		st x,r10
		ldi r17,2			;принято "ERROR"
		mov IDENT,r17
;--------------
id30:	ldi zh,high(world_4*2)
		ldi zl,low(world_4*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_4_c)
		ldi xl,low(world_4_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id31
		inc zh
id31:	lpm r19,z
		cp r19,r16
		breq id32
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id40
id32:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id40
		ldi r17,4			;принято входящий звонок с номером
		mov IDENT,r17
;--------------
id40:	ldi zh,high(world_5*2)
		ldi zl,low(world_5*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_5_c)
		ldi xl,low(world_5_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id41
		inc zh
id41:	lpm r19,z
		cp r19,r16
		breq id42
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id50
id42:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id50
		ldi r17,5			;начало приема номера SMS центра
		mov IDENT,r17
;--------------
id50:	ldi zh,high(world_6*2)
		ldi zl,low(world_6*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_6_c)
		ldi xl,low(world_6_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id51
		inc zh
id51:	lpm r19,z
		cp r19,r16
		breq id52
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id60
id52:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id60
		ldi r17,6			;звонок
		mov IDENT,r17
;--------------
id60:	ldi zh,high(world_7*2)
		ldi zl,low(world_7*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_7_c)
		ldi xl,low(world_7_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id61
		inc zh
id61:	lpm r19,z
		cp r19,r16
		breq id62
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id70
id62:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id70
		ldi r17,7			;вызов отклонен или истекло время ожидания ответа
		mov IDENT,r17
;--------------
id70:	ldi zh,high(world_8*2)
		ldi zl,low(world_8*2)
		lpm r18,z+				;r18 - кол-во байт в слове, (курсор на первом байте)
		ldi xh,high(world_8_c)
		ldi xl,low(world_8_c)
		ld r17,x				;r17 - кол-во правильно принятых байтов подряд с начала слова
		add zl,r17
		brcc id71
		inc zh
id71:	lpm r19,z
		cp r19,r16
		breq id72
		st x,r10			;обнуляем счетчик байтов этого слова
		rjmp id80
id72:	inc r17				;если принятый байт совпал
		st x,r17
		set					;в процессе приема слова
		cp r17,r18
		brne id80
		ldi r17,8			;абонент недоступен
		mov IDENT,r17
;--------------
id80:









		brts id1000			;если в процессе приема слова
	ret

id1000:	ldi r16,200
		out TCNT2,r16
		ldi r16,0b00000101
		out TCCR2,r16	;пуск счетчик
	ret
; ----------------------------------------------
.dseg				;счетчики байтов в слове
world_1_c: .byte 1
world_2_c: .byte 1
world_3_c: .byte 1
world_4_c: .byte 1
world_5_c: .byte 1
world_6_c: .byte 1
world_7_c: .byte 1
world_8_c: .byte 1
; ----------------------------------------------
.cseg
world_1: .DB 3,$0A,'>',$20							;">"
world_2: .DB 3,'O','K',$0D							;"OK"
world_3: .DB 7,'E','R','R','O','R',$0D,$0A			;"ERROR"
world_4: .DB 19,$0D,$0A,'R','I','N','G',$0D,$0A,$0D,$0A,'+','C','L','I','P',':',$20,'"','+'	;входящий звонок с номером
world_5: .DB 20,'A','T','+','C','S','C','A','?',$0D,$0D,$0A,'+','C','S','C','A',':',$20,'"','+',0	;начало приема номера SMS центра
world_6: .DB 8,$0D,$0A,'R','I','N','G',$0D,$0A,0	;звонок
world_7: .DB 3,$0D,$0A,'N','O',$20,'C','A','R','R','I','E','R',$0D,$0A,0	;вызов отклонен или истекло время ожидания ответа
world_8: .DB 7,$0D,$0A,'N','O',$20,'D','I','A','L','T','O','N','E',$0D,$0A	;абонент недоступен
; ----------------------------------------------
clear_world_c:
	push zh
	push zl
	ldi zh,high(world_1_c)
	ldi zl,low(world_1_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_2_c)
	ldi zl,low(world_2_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_3_c)
	ldi zl,low(world_3_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_4_c)
	ldi zl,low(world_4_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_5_c)
	ldi zl,low(world_5_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_6_c)
	ldi zl,low(world_6_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_7_c)
	ldi zl,low(world_7_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	ldi zh,high(world_8_c)
	ldi zl,low(world_8_c)
	st z,r10			;обнулим состояние правильно принятых байтов
	pop zl
	pop zh
ret
; ----------------------------------------------
over_8_2:
		out TCCR2,r10	;стоп счетчик
		rcall clear_world_c
reti
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
;		sbi PORTC,3			;включение подзарядки
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
;		cbi PORTC,3			;выключение подзарядки
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
;tel_ping:		;возвращает Т=1 если телефон отвечает и Т=0 если нет
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
		ldi r17,$0D	;перевод каретки
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
		sbi PORTC,4		;зуммер вкл
		rcall d_100mks
		cbi PORTC,4		;зуммер выкл
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
		sbi PORTC,4		;зуммер вкл
		rcall d_100mks
		cbi PORTC,4		;зуммер выкл
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
		in r16,SREG
		push r16
	
		out TCCR1B,r10			;стор счетчик
; -----
		mov r16,TMPR_ST
		cbr r16,0b11110000


		cpi r16,0				;стартуем.1сброс,посылаем 0
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
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out

ovr_01:
		cpi r16,1				;1сброс,0 послан, посылаем 1
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
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out
		
ovr_02:
		cpi r16,2				;1сброс отработан, посылаем $CC и $44
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
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out
		
ovr_03:
		cpi r16,3				;2сброс,посылаем 0
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
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out
		
ovr_04:
		cpi r16,4				;2сброс,0 послан, посылаем 1
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
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out
		
ovr_05:
		cpi r16,5				;2сброс отработан, посылаем $CC и $BE,читаем и считаем
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
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out
		
ovr_06:
		cpi r16,6				;все отработано,включаем 8-битный счетчик,переключаем датчик,сбрасываем регистр
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
		sbi PORTC,5		;светодиод
		ldi r16,$FE
		out TCNT1H,r16
		ldi r16,$70
		out TCNT1L,r16	;~51ms
		ldi r16,0b00000101
		out TCCR1B,r16	;старт таймера
		rjmp ovr_out

ovr_09:
		cpi r16,7				;все в исходное
		brne ovr_out
		sbrc SECR_ST,7
		cbi PORTC,5		;светодиод
		out TCNT1H,r11
		ldi r16,$F8
		out TCNT1L,r16	;~1ms
		mov r16,TMPR_ST
		cbr r16,0b00001111
		mov TMPR_ST,r16
		ldi r16,0b00000101
		out TCCR1B,r16	;старт таймера
		
; -----
ovr_out:pop r16
		out SREG,r16
		pop xl
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
cl10:	rcall ds_error		;CRC не сошлось,выход
		rjmp cl7
;проверка на валидность принятых данных
cl6:	clr r17				;здесь будем считать 00
		clr r19				;здесь будем считать FF
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
		brne cl9		;прочитаны одни 00
		rjmp cl10
cl9:	cpi r19,9
		brne cl15		;прочитаны одни FF
		rjmp cl10
;конец проверки
cl15:
		ldi xh,high(ds1820_buffer+7)
		ldi xl,low(ds1820_buffer+7)
		ld r20,x+		;двоичное значение, старший байт
		ld r19,x		;двоичное значение, младший байт

		ldi r16,29		;пробел
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

		ldi r17,-1		;будущие десятки
cl1:	inc r17
		subi r19,10
        brsh cl1
		subi r19,-10

		ldi r18,-1		;будущие единицы
cl2:	inc r18
		subi r19,1
        brsh cl2
		st y+,r17		;1б-десятки ASCII
		st y+,r18		;2б-единицы ASCII
		st y,r16		;3б-десятые градусы,здесь при необходимости можно ввести коррекцию точности
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
		ldi r17,9		;счетчик байтов
i110:	ldi r18,8		;счетчик бит
		clr r19
i111:	rcall ds_post0	;инт0 на выход
		rcall d_2mks
		rcall ds_post1	;настроимся на прием по инт0
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
		rol r19		;последний сдвиг вправо был лишним
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
check_crc_ds1820_buffer:	;если ошибка, бит Т устанавливается в 1, если все правильно - в 0
        clr r19
		ldi r17,9
		ldi zh,high(ds1820_buffer)
		ldi zl,low(ds1820_buffer+9)
jmp12:	ld r20,-z		
        ldi r18,8			;колличество битов
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
;		   0   1   2   3   4   5   6   7   8   9   A   b   C   d   E   F   0.  1.  2.  3.  4.  5.  6.  7.  8.  9.  u  гр   -
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
		mov POZ_LCD,r16			;снятие блокировки
		rcall out_lcd_poz
		mov SYM_LCD,r11
		rcall out_lcd_sym 

		clr POZ_LCD				;очистка экрана
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
		rcall out_lcd_poz		;очистка экрана
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
;БЛОК ЗАДЕРЖЕК
; ----------------------------------------------
d_1mks:	ret
; ----------------------------------------------
d_2mks:	push r16
		ldi r16,1
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
		ldi r16,13
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
		ldi r16,39
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
		ldi r16,160
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
		ldi r16,200
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
		ldi r16,124
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
delay:	
		push r13
		push r14
		push r15
		lds r15,t1
del3:	lds r14,t2
del2:	lds r13,t3
del1:	dec r13
		brne del1
		dec r14
		brne del2
		dec r15
		brne del3
		pop r15
		pop r14
		pop r13
		ret
; ----------------------------------------------
