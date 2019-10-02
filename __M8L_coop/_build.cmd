@echo off
PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%

set SRC_FILES=%CD%\src
set BUILD_FILES=%CD%\build
set OBJ_FILES=%CD%\build\obj

set MCU=atmega8
set F_CPU=1000000
set FORMAT=ihex

rem Optimisation value [0, 1, 2, 3, s]
set OPT=s

set TARGET=main

set CFLAGS= -c -mmcu=%MCU% -I. -DF_CPU=%F_CPU%UL -O%OPT% -funsigned-char -funsigned-bitfields ^
	-fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=%BUILD_FILES%\%TARGET%.lst ^
	-std=gnu99 -MMD -MP -MF 

set LFLAGS= -mmcu=%MCU% -I. -DF_CPU=%F_CPU%UL -O%OPT% -funsigned-char -funsigned-bitfields ^
	-fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=%OBJ_FILES%\%TARGET%.o ^
	-std=gnu99 -MMD -MP -MF

If Not Exist "%BUILD_FILES%\*.*" mkdir %BUILD_FILES%
If Not Exist "%OBJ_FILES%\*.*" mkdir %OBJ_FILES%

REM echo Compiling C: %TARGET%.c
avr-gcc %CFLAGS% %OBJ_FILES%\%TARGET%.o.d %SRC_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
if errorlevel 1 goto err

REM echo Linking: %TARGET%.elf
avr-gcc %LFLAGS% %OBJ_FILES%/%TARGET%.elf.d %OBJ_FILES%/%TARGET%.o --output %BUILD_FILES%\%TARGET%.elf -Wl,-Map=%BUILD_FILES%\%TARGET%.map,--cref -lm
if errorlevel 1 goto err

REM echo Creating load file for Flash: %TARGET%.hex
avr-objcopy -O %FORMAT% -R .eeprom -R .fuse -R .lock %BUILD_FILES%\%TARGET%.elf %BUILD_FILES%\%TARGET%.hex
if errorlevel 1 goto err

REM echo Creating load file for EEPROM: %TARGET%.eep
avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" ^
        --change-section-lma .eeprom=0 --no-change-warnings -O %FORMAT% %BUILD_FILES%\%TARGET%.elf %BUILD_FILES%\%TARGET%.eep
if errorlevel 1 goto err

REM echo Creating Extended Listing: %TARGET%.lss
avr-objdump -h -S -z %BUILD_FILES%\%TARGET%.elf > %BUILD_FILES%\%TARGET%.lss
if errorlevel 1 goto err

REM echo Creating Symbol Table: %TARGET%.sym
avr-nm -n %BUILD_FILES%\%TARGET%.elf > %BUILD_FILES%\%TARGET%.sym
if errorlevel 1 goto err

echo OK
echo.
avr-size --mcu=%MCU% --format=avr %BUILD_FILES%\%TARGET%.elf

REM cd ./build || exit 0



REM make -f ./Makefile clean
REM make -f ./Makefile all



pause
exit



:err
echo ERROR!
pause
exit





REM set SRC_FILES=%CD%\src
REM set OBJ_FILES=%CD%\obj
REM set OUT_FILE=NUMBERS

REM if exist %OUT_FILE%.exe del %OUT_FILE%.exe

REM set C_FLAGS= -c -s -O2
 
REM windres "%SRC_FILES%\res.rc" "%OBJ_FILES%\res.o"
REM if errorlevel 1 goto err

REM REM g++ %C_FLAGS% "%SRC_FILES%\asMyFunc.cpp" -o "%OBJ_FILES%\asMyFunc.o"
REM REM if errorlevel 1 goto err

REM REM g++ %C_FLAGS% "%SRC_FILES%\asStatus.cpp" -o "%OBJ_FILES%\asStatus.o"
REM REM if errorlevel 1 goto err

REM REM g++ %C_FLAGS% "%SRC_FILES%\asDataGrid.cpp" -o "%OBJ_FILES%\asDataGrid.o"
REM REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\main.cpp" -o "%OBJ_FILES%\main.o"
REM if errorlevel 1 goto err

REM g++ -o %OUT_FILE%.exe ^
	REM "%OBJ_FILES%\res.o" ^
	REM "%OBJ_FILES%\asMyFunc.o" ^
	REM "%OBJ_FILES%\asStatus.o" ^
	REM "%OBJ_FILES%\asDataGrid.o" ^
	REM "%OBJ_FILES%\main.o" ^
	REM -mwindows -static -lcomctl32 -lshlwapi -lole32

	

REM -------- begin --------

REM Cleaning project:
REM rm -f main.hex
REM rm -f main.eep
REM rm -f main.cof
REM rm -f main.elf
REM rm -f main.map
REM rm -f main.sym
REM rm -f main.lss
REM rm -f obj/main.o
REM rm -f obj/main.lst
REM rm -f main.s
REM rm -f main.d
REM rm -f main.i
REM rm -rf .dep
REM -------- end --------


REM -------- begin --------

REM Compiling C: ../src/main.c
REM avr-gcc -c -mmcu=atmega8 -I. -DF_CPU=1000000UL -Os -funsigned-char -funsigned-bi
REM tfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=obj/ma
REM in.lst  -std=gnu99 -MMD -MP -MF .dep/main.o.d ../src/main.c -o obj/main.o

REM Linking: main.elf
REM avr-gcc -mmcu=atmega8 -I. -DF_CPU=1000000UL -Os -funsigned-char -funsigned-bitfi
REM elds -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=obj/main.
REM o  -std=gnu99 -MMD -MP -MF .dep/main.elf.d obj/main.o --output main.elf -Wl,-Map
REM =main.map,--cref     -lm

REM Creating load file for Flash: main.hex
REM avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock main.elf main.hex

REM Creating load file for EEPROM: main.eep
REM avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
        REM --change-section-lma .eeprom=0 --no-change-warnings -O ihex main.elf mai
REM n.eep || exit 0

REM Creating Extended Listing: main.lss
REM avr-objdump -h -S -z main.elf > main.lss

REM Creating Symbol Table: main.sym
REM avr-nm -n main.elf > main.sym

REM Size after:
REM AVR Memory Usage
REM ----------------
REM Device: atmega8

REM Program:      86 bytes (1.0% Full)
REM (.text + .data + .bootloader)

REM Data:          0 bytes (0.0% Full)
REM (.data + .bss + .noinit)

