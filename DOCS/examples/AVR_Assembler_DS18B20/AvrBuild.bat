@ECHO OFF
"C:\Program Files (x86)\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "D:\AVR Assembler - DS18B20\labels.tmp" -fI -W+ie -C V2E -o "D:\AVR Assembler - DS18B20\Test.hex" -d "D:\AVR Assembler - DS18B20\Test.obj" -e "D:\AVR Assembler - DS18B20\Test.eep" -m "D:\AVR Assembler - DS18B20\Test.map" "D:\AVR Assembler - DS18B20\Test.asm"
