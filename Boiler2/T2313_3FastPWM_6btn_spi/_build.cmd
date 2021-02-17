@rem если в программе используется EEPROM то используем следующую запись
@rem @call avrasm2.exe -fI -W+ie -o asu.hex -e asu.eep asu.asm

@rem короткая запись с автоматическим образованием hex файла с именем asu.hex
REM @call ..\_AVR_ASSEMBLER\avrasm2.exe -fI -W+ie -o main.hex main.avr

@call ..\..\_AVR_ASSEMBLER\avrasm2.exe -fI -W+ie -o main.hex .\src\main.avr

@pause
@exit


@rem avrasm2.exe -h help listing
@rem "C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\ASU\labels.tmp" -fI -W+ie -o "C:\ASU\ASU.hex" -d "C:\ASU\ASU.obj" -e "C:\ASU\ASU.eep" -m "C:\ASU\ASU.map" "C:\ASU\ASU.asm"
