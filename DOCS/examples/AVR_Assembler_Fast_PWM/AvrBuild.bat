@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "D:\www\avr\AVR - Fast PWM\labels.tmp" -fI -W+ie -C V2E -o "D:\www\avr\AVR - Fast PWM\Test.hex" -d "D:\www\avr\AVR - Fast PWM\Test.obj" -e "D:\www\avr\AVR - Fast PWM\Test.eep" -m "D:\www\avr\AVR - Fast PWM\Test.map" "D:\www\avr\AVR - Fast PWM\Test.asm"
