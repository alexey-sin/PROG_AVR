@echo off
set PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%
set SRC_FILES=%CD%\src
set TFT_FILES=%~d0\PROG_AVR_2\_lib_tft_ili9486
set FONT_FILES=%~d0\PROG_AVR_2\_fonts
set OBJ_FILES=%CD%\build

set MCU=atmega32
set F_CPU=16000000
set OPT=s

REM del /Q %OBJ_FILES%\*.*

set C_FLAGS= -mmcu=%MCU% -I. -I%SRC_FILES% -DF_CPU=%F_CPU%UL -O%OPT% -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -std=gnu99 -MMD -MP -Wall

REM set TARGET=Font_Tahoma20
REM echo Compiling C: %TARGET%.c
REM avr-gcc -c %C_FLAGS% %FONT_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
REM if errorlevel 1 goto err

REM set TARGET=Font_Tahoma10
REM echo Compiling C: %TARGET%.c
REM avr-gcc -c %C_FLAGS% %FONT_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
REM if errorlevel 1 goto err

REM set TARGET=Font_Tahoma36num
REM echo Compiling C: %TARGET%.c
REM avr-gcc -c %C_FLAGS% %FONT_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
REM if errorlevel 1 goto err

REM set TARGET=tft_ili9486
REM echo Compiling C: %TARGET%.c
REM avr-gcc -c %C_FLAGS% %TFT_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
REM if errorlevel 1 goto err

set TARGET=main
echo Compiling C: ../src/%TARGET%.c
avr-gcc -c %C_FLAGS% %SRC_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
if errorlevel 1 goto err

echo Linking.
avr-gcc %C_FLAGS% --output %OBJ_FILES%\main.elf ^
	%OBJ_FILES%\tft_ili9486.o ^
	%OBJ_FILES%\Font_Tahoma10.o ^
	%OBJ_FILES%\Font_Tahoma20.o ^
	%OBJ_FILES%\main.o
if errorlevel 1 goto err
	
	REM %OBJ_FILES%\Font_Tahoma36num.o ^
	
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





