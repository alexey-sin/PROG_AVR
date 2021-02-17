@echo off
set PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%
set SRC_FILES=%CD%\src
set NRF24_FILES=%~d0\PROG_AVR_2\_lib_tft_st7735
set OBJ_FILES=%CD%\build

set MCU=atmega32
set F_CPU=8000000
set OPT=s

del /Q %OBJ_FILES%\*.*

set C_FLAGS= -mmcu=%MCU% -I. -I%SRC_FILES% -DF_CPU=%F_CPU%UL -O%OPT% -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -std=gnu99 -MMD -MP -Wall

set TARGET=tft_st7735
echo Compiling C: %TARGET%.c
avr-gcc -c %C_FLAGS% %NRF24_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
if errorlevel 1 goto err

set TARGET=main
echo Compiling C: ../src/%TARGET%.c
avr-gcc -c %C_FLAGS% %SRC_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
if errorlevel 1 goto err

echo Linking.
avr-gcc %C_FLAGS% --output %OBJ_FILES%\main.elf ^
	%OBJ_FILES%\tft_st7735.o ^
	%OBJ_FILES%\main.o
if errorlevel 1 goto err
	
	
 REM -Wl,-Map=%OBJ_FILES%\main.map,--cref -lm
 
 
echo Creating load file for Flash: main.hex
avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock %OBJ_FILES%\main.elf main.hex

echo Creating load file for EEPROM: main.eep
avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex ^
	%OBJ_FILES%\main.elf main.eep || exit 0

REM Creating Extended Listing: main.lss
REM avr-objdump -h -S -z %OBJ_FILES%\main.elf > main.lss

REM Creating Symbol Table: main.sym
REM avr-nm -n %OBJ_FILES%\main.elf > main.sym

echo.
avr-size --mcu=%MCU% --format=avr %OBJ_FILES%\main.elf



pause
exit

:err
pause
exit





