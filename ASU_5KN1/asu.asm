.include "..\_AVR_ASSEMBLER\DEFs\m16def.inc"
.equ 	t1			=$60
.equ 	t2			=$61
.equ 	t3			=$62

.dseg
.org 0x063
buff_command:	.byte 7		;буфер принятых байтов
byte_count: 	.byte 1		;кол-во принятых байтов
byte_type_command: 	.byte 1		;тип передаваемой команды для асу
	;6 - обычная 6 байтная команда
	;3 - короткая 3 байтная команда

.def RES_ST	= r3		;регистр статуса приемника команд от ПК
	;RES_ST: 7бит - 1= состояние приема слова от ПК(запущен таймер), 0= свободен,готов к приему
	;RES_ST: 6бит - 1= принято слово для анализа от ПК
	;RES_ST: 5бит - 1= необходимо отправить в ПК ответ PING
	;RES_ST: 4бит - 1= необходимо отправить команду в АСУ
	;RES_ST: 3бит - 1= необходимо отправить в ПК ошибку - удержание линии TXD АСУ в низком состоянии более 2мс
	;RES_ST: 2бит - 1= необходимо установить настройки генератора
	;RES_ST: 1бит - 1= необходимо установить настройки усилителя мощности
	;RES_ST: 0бит - 1= резерв
	
	
.cseg
.org 0x000
	rjmp start
.org INT0addr	;0x002
	rjmp Receive_RS232        ;при возникновении прерывания INT0 RS232 - связь с компьютером
.org INT1addr	;0x004
	rjmp Receive_ASU     	 ;при возникновении прерывания INT1 RS232 - связь с АСУ
.org OVF2addr	;0x008
	rjmp over_82			;переполнение 2-го 8-разрядного счетчика (АСУ удерживает линию в низком состоянии очень долго) ~2мс
.org OVF0addr	;0x012
	rjmp over_8				;при переполнении счетчика ~2мс от начала первого байта (слово из 8 байт = 1,4мс) анализ принятого
; ----------------------------------------------
; для F=8МГц (внутренний RC генератор)
; Фьюзы: для USBASP 1 => установленный бит => галки нет
;					0 => сброшенный бит => отмечено галкой
; все биты LOCK установлены
;---- Fuse High Byte ----
; OCDEN - 1
; JTAGEN - 1
; SPIEN - 0 недоступен
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
;Настройка портов
; Порт A
	out DDRA,r11			;все выводы работают как выход
	; ldi r16,0b10001110
	; out PORTA,r16			;
	out PORTA,r11			;

; Порт B
	out DDRB,r11			;
	ldi r16,0b00000111
	out PORTB,r16

; Порт C
	out DDRC,r11			;все выводы работают как выход
	out PORTC,r10			;без подтягивающих резисторов т.е. 0

; Порт D
	ldi r16,0b00000011
	out DDRD,r16
	ldi r16,0b00001111
	out PORTD,r16
;===============================================

	ldi r16,0b00000110		; установка прерываний по заднему фронту INT0
	out MCUCR,r16			; или 0b00000110 по заднему фронту INT0 и любому изменению по INT1
	ldi r16,0b11000000		; установка прерываний по INT0
	out GICR,r16			; или 0b11000000 по заднему фронту INT0 и INT1

	ldi r16,0b01000001		;0b00000001 разрешить прерывание по переполнению 0(8разр) счетчика
	out TIMSK,r16			;или 0b01000001 разрешить прерывание по переполнению 0(8разр) и 2(8разр) счетчиков

	ldi xh,high(byte_count)
	ldi xl,low(byte_count)
	st x,r10	 			; обнулим кол-во принятых байтов
	clr RES_ST				; очистим регистр статуса приемника команд от ПК

	sei
;-------------------------------------------------------
loop:

				sbrc RES_ST,6		;пропустить если не принято слово по RS232 от ПК
				rcall analizRS232

				sbrc RES_ST,5		;пропустить если ненадо отправлять в ПК ответ PING
				rcall sendPING
					
				sbrc RES_ST,4		;пропустить если ненадо отправлять команду в АСУ
				rcall sendASU
		
				sbrc RES_ST,3		;пропустить если ненадо отправлять сообщение об ошибке - удержание линии TXD АСУ
				rcall sendERR_RXD
		
				sbrc RES_ST,2		;пропустить если ненадо установить настройки генератора
				rcall setGEN

				sbrc RES_ST,1		;пропустить если ненадо установить настройки усилителя мощности
				rcall setUM

					; sbi PORTB,4
					; rcall d_200ms необходимо установить настройки генератора
					; cbi PORTB,4
					; rcall d_200ms
rjmp loop
;-------------------------------------------------------
setGEN:
				mov r16,RES_ST
				cbr r16,0b00000100			;(7 бит)сброс необходимости установить настройки генератора
				mov RES_ST,r16

				ldi zh,high(buff_command)
				ldi zl,low(buff_command)
				ld r16,z+					;перейдем ко второму байту
				ld r16,z+					;загрузили 2 байт (степень,запятая частоты генератора - старшие биты, младшие не используются)

				ldi r17,0b11110011			;(1) НГ
				out PORTA,r17
				rcall strobGen

				cbr r16,0b00000101
				sbr r16,0b00001010
				out PORTA,r16				;степень,запятая частоты генератора
				rcall strobGen

				ld r16,z+					;загрузили 3 байт (частота: старшие биты - 1(ст) разряд, младшие - 2 разряд)
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
				
				ld r16,z+					;загрузили 4 байт (частота: старшие биты - 3 разряд, младшие - 4(мл) разряд)
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
				
				ld r16,z+					;загрузили 5 байт (степень,запятая напряжения генератора - старшие биты, младшие не используются)
				cbr r16,0b00001001
				sbr r16,0b00000110
				out PORTA,r16
				rcall strobGen
				
				ld r16,z+					;загрузили 6 байт (напряжение: старшие биты - 1(ст) разряд, младшие - 2 разряд)
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
				
				ld r16,z+					;загрузили 7 байт (напряжение: старшие биты - 3(мл) разряд, младшие не используются)
				cbr r16,0b00000110
				sbr r16,0b00001001
				out PORTA,r16
				rcall strobGen

				out PORTA,r11				;возврат в исходное
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
				cbr r16,0b00000010			;(7 бит)сброс необходимости установить настройки усилителя мощности
				mov RES_ST,r16

				ldi zh,high(buff_command)
				ldi zl,low(buff_command)
				ld r16,z+				;перейдем ко второму байту

				ld r16,z+				;загрузим командный байт
				;блокировка
				sbrc r16,0				;пропустить если бит сброшен (0)
				sbi PORTB,2				;вывести 1
				sbrs r16,0				;пропустить если бит установлен (1)
				cbi PORTB,2				;вывести 0
				;малая/полная
				sbrc r16,1				;пропустить если бит сброшен (0)
				sbi PORTB,1				;вывести 1
				sbrs r16,1				;пропустить если бит установлен (1)
				cbi PORTB,1				;вывести 0

				ld r16,z+				;загрузим поддиапазонный байт
				out PORTC,r16			;вывод поддиапазона
ret
;-------------------------------------------------------
Receive_RS232:	;прием данных с ПК
				cli				;(1)выключаем все прерывания
				push r17		;(2)
				push r16		;(2)
				push xh			;(2)
				push xl			;(2)
				push yh			;(2)
				push yl			;(2)

;для скорости 57600 длительность бита 1/57600=17,361(1)мкс
;отсчитываем длительность стартового бита и половину первого (младшего) бита
;от начала прерывания до чтения линии порта должно пройти примерно 
;26мкс = 208 машинных тактов (при f=8MHz такт=0,125мкс)
				ldi r17,8			;(1)счетчик битов
				clr r16				;(1)приемный байт
				rcall d_st_rs232	;6.625+1.875=8.5мкс ~(8.68=половина бита)

r_bit:	
				rcall d_bit_rs232	;16,375+0.875=17.25мкс ~(17,361=длительность бита)
				clc					;(1)очистим флаг переноса С
				sbic PIND,2			;пропустить если бит в порту очищен
				sec					;(2)
				ror	r16				;(1)циклический сдвиг вправо через флаг переноса С
				dec r17				;(1)
				brne r_bit			;(2)

				rcall d_bit_rs232	;подождем до середины стопового бита
				
				ldi xh,high(byte_count)
				ldi xl,low(byte_count)
				ld r17,x			; загрузим кол-во принятых байтов
				cpi r17,7			; сравним с максимумом
				brpl rec2			; перейти если при r17-8 осталось положительное число
				ldi yh,high(buff_command)
				ldi yl,low(buff_command)
				add yl,r17			; прирастим адрес байта в буфере
				brcc rec1			; если произошел перенос
				inc yh				; инкременируем старший разряд
rec1:			st y,r16			; сохраним принятый байт в буфере
				inc r17				; инкременируем кол-во принятых байтов
				st x,r17			; и сохраним его

rec2:			ldi r16,0b11100000	; очистка флагов прерываний по INT0 и INT1 и INT2
				out GIFR,r16

				sbrc RES_ST,7		;пропустить нет приема слова
				rjmp rec4
				
				mov r16,RES_ST
				sbr r16,0b10000000			;установим состояние приема слова
				mov RES_ST,r16
				;запуск счетчика
				out TCNT0,r10		;обнулим счетчик таймера
				ldi r16,0b00000011	;(такт = 0,125мкс) * 64 * 256 = 2.048 мс
				out TCCR0,r16		;пуск счетчик

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

				out TCCR0,r10	;стоп счетчик
				mov r16,RES_ST
				cbr r16,0b10000000			;(7 бит)сброс состояния приема
				mov RES_ST,r16

				ldi xh,high(byte_count)
				ldi xl,low(byte_count)
				ld r16,x			; загрузим кол-во принятых байтов
				st x,r10			;обнулим количество принятых байтов

				cpi r16,7
				brne out_over_8		;перейти если не равно 0
					
				mov r16,RES_ST
				sbr r16,0b01000000			;(6 бит)принято слово и необходимо анализировать
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
				cbr r16,0b01000000			;(6 бит)сброс необходимости анализа
				mov RES_ST,r16

				ldi xh,high(buff_command)
				ldi xl,low(buff_command)
				ld r17,x					;загрузим первый байт команды

				cpi r17,$55					;запрос PING
				brne anlz1
				;отвечаем на PING
					mov r16,RES_ST
					sbr r16,0b00100000			;(5 бит)необходимо отправить в ПК ответ PING
					mov RES_ST,r16
				ret
anlz1:
				cpi r17,$11					;Команда для АСУ 6 байтная
				brne anlz2
				;Транслируем команду в АСУ
					ldi zh,high(byte_type_command)
					ldi zl,low(byte_type_command)
					ldi r16,6
					st z,r16				;счетчик байтов
					mov r16,RES_ST
					sbr r16,0b00010000			;(4 бит)необходимо отправить команду в АСУ
					mov RES_ST,r16
				ret
anlz2:
				cpi r17,$12					;Команда для АСУ короткая 3 байтная
				brne anlz3
				;Транслируем команду в АСУ
					ldi zh,high(byte_type_command)
					ldi zl,low(byte_type_command)
					ldi r16,3
					st z,r16				;счетчик байтов
					mov r16,RES_ST
					sbr r16,0b00010000			;(4 бит)необходимо отправить команду в АСУ
					mov RES_ST,r16
				ret
anlz3:	
				cpi r17,$22					;Команда для генератора
				brne anlz4
				;Транслируем команду в генератор
					mov r16,RES_ST
					sbr r16,0b00000100			;(2 бит) необходимо установить настройки генератора
					mov RES_ST,r16
				ret
anlz4:	
				cpi r17,$33					;Команда для усилителя мощности
				brne anlz5
				;Транслируем команду в усилитель мощности
					mov r16,RES_ST
					sbr r16,0b00000010			;(1 бит) необходимо установить настройки усилителя мощности
					mov RES_ST,r16
				ret
anlz5:	
				cpi r17,$45					;резерв
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
.cseg	;последний байт лишний, но на нечетное количество ругается компилятор :(
world_1: .DB $55,$55,$55,$4F,$4B,$55,$55,$00			; ответ PING OK
world_2: .DB $45,$52,$52,$4F,$52,$52,$58,$00			; ответ ERROR RXD
; world_3: .DB 7,$45,$52,$52,$4F,$52,$0D,$0A			;"ERROR"
; world_4: .DB 4,$05,$01,$04,$06						;BFB ответ
; ----------------------------------------------
sendPING:
				mov r16,RES_ST
				cbr r16,0b00100000		;(5 бит)сброс необходимости отправлять в ПК ответ PING
				mov RES_ST,r16

				ldi r18,7				; счетчик байтов
				ldi zh,high(world_1*2)
				ldi zl,low(world_1*2)

t_byte:
				lpm r16,z+
				ldi r17,8				; счетчик битов

				cbi PORTD,0				; стартовый бит
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
				rcall d_bit_rs232		; стоповый бит
	
				dec r18
				brne t_byte
		
ret
; ----------------------------------------------
sendERR_RXD:
				mov r16,RES_ST
				cbr r16,0b00001000		;(3 бит)сброс необходимости отправлять сообщение об ошибке - удержание линии TXD АСУ
				mov RES_ST,r16

				ldi r18,7				; счетчик байтов
				ldi zh,high(world_2*2)
				ldi zl,low(world_2*2)

t_byte0:
				lpm r16,z+
				ldi r17,8				; счетчик битов

				cbi PORTD,0				; стартовый бит
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
				rcall d_bit_rs232		; стоповый бит
	
				dec r18
				brne t_byte0
		
ret
; ----------------------------------------------
sendASU:
				mov r16,RES_ST
				cbr r16,0b00010000		;(4 бит)сброс необходимости отправлять команду в АСУ
				mov RES_ST,r16

				cbi PORTD,1				;разбудим контроллер АСУ
				rcall d_2ms
				sbi PORTD,1
				rcall d_20ms

				ldi zh,high(byte_type_command)
				ldi zl,low(byte_type_command)
				ld r18,z				;счетчик байтов
				ldi zh,high(buff_command)
				ldi zl,low(buff_command)
				ld r16,z+				;перейдем ко второму байту

t_byte1:
				ld r16,z+
				ldi r17,8				; счетчик битов

				cbi PORTD,1				; стартовый бит
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
				rcall d_bit_rs232		; стоповый бит
				
				dec r18
				brne t_byte1

ret
; ----------------------------------------------
; Transmit_RS232:
		; push r17

		; ldi r17,8			;счетчик битов
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
		; cbr r17,0b01000000			;(6 бит)
		; mov RES_ST,r17


		; pop r17
		; ret
; ----------------------------------------------
Receive_ASU:	;прием данных с АСУ
				cli
				push r16
				
				sbic PIND,3			;пропустить если бит в порту очищен
				rjmp rcasu_1
				;низкий уровень на входе (0)
				cbi PORTD,0

				;запустим счетчик аварийного удержания линии в низком состоянии
				out TCNT2,r10		;обнулим счетчик таймера
				ldi r16,0b00000011	;(такт = 0,125мкс) * 64 * 256 = 2.048 мс
				out TCCR2,r16		;пуск счетчик
			
			
				rjmp rcasu_exit

rcasu_1:		;высокий уровень на входе (1)
				sbi PORTD,0
				
				;остановим счетчик аварийного удержания линии в низком состоянии
				out TCCR2,r10	;стоп счетчик




rcasu_exit:		
				pop r16
				sei
reti
; ----------------------------------------------
over_82:	;переполнение 2-го 8-разрядного счетчика (АСУ удерживает линию в низком состоянии очень долго)
				cli
				push r16

				out TCCR2,r10	;стоп счетчик
				sbi PORTD,0				;освободим линию TXD (от ТПАСУ к ПК)

				mov r16,RES_ST
				sbr r16,0b00001000		;(3 бит)необходимо отправить в ПК сообщение об ошибке удержания линии RXD АСУ
				mov RES_ST,r16

				pop r16
				sei
reti
; ----------------------------------------------
; Transmit_ASU:

		; cbi PORTD,0			; разбудим АСУ
		; rcall d_bit_rs232
		; sbi PORTD,0	
		; rcall d_20mks		; подождем  маленько

		; ldi r16,1
		; ldi xh,high(byte_count)
		; ldi xl,low(byte_count)
		; ld r17,x			; загрузим кол-во принятых байтов
		; ldi yh,high(buff_command)
		; ldi yl,low(buff_command)
; tr_2:	add yl,r16			; прирастим адрес байта в буфере
		; brcc tr_1			; если произошел перенос
		; inc yh				; инкременируем старший разряд
; tr_1:	ld r25,y			; загрузим байт из буфера
		; rcall Send_ASU
		
		; inc r16
		; cp r17,r16
		; brne tr_2

		; mov r17,RES_ST
		; cbr r17,7			; слово отправлено, сбросим бит
		; mov RES_ST,r17
		; st x,r10			; обнулим кол-во принятых байтов
		; ret
; ; ----------------------------------------------
; Send_ASU:
		; push r17

		; ldi r17,8			;счетчик битов
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
;БЛОК ЗАДЕРЖЕК		расчет задержек включает вход и выход из подпрограммы задержки
; ----------------------------------------------
; d_1mks:	; 3(rcall)+1+4(ret) = 8*0,125 = 1.0 мкс
		; nop
		; ret
; ----------------------------------------------
d_st_rs232:	; 3(rcall)+2+1+ X(1+2)-1 +2+4(ret) = 11+X*3 = 11+14*3 = 53		53*0,125 = 6.625мкс
		push r16	;2
		ldi r16,14	;1	X
del4:	dec r16		;1
		brne del4	;2/1
		pop r16		;2
		ret			;4
; ----------------------------------------------
d_bit_rs232:	; 3(rcall)+2+1+ 40(1+2)-1 +2+4(ret) = 131		131*0,125 = 16,375мкс
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
d_500mks:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 4028*0.125 = 503,5мкс
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
d_1ms:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 8012*0.125 = 1001,5мкс
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
d_20ms:	; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 160765*0.125 = 20095,625мкс = 20,096мс
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
			; 41+{ t1*(5+t2*(5+(t3*3-1))-1)-1 } = 41+ 34*188504-1 = 6409176*0.125 = 801147мкс = 801,147мс
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
;использование см. выше. минимум(1,1,1)= 41+{1*(5+1*(5+(1*3-1))-1)-1 } = 51*0.125 = 6.375мкс
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
