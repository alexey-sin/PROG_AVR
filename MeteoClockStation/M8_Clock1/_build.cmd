@echo off
set PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%
set SRC_FILES=%CD%\src
set NRF24_FILES=%~d0\PROG_AVR_2\_lib_nrf24l01
set OBJ_FILES=%CD%\build

set MCU=atmega8
set F_CPU=8000000
set OPT=s

del /Q %OBJ_FILES%\*.*

set C_FLAGS= -mmcu=%MCU% -I. -I%SRC_FILES% -DF_CPU=%F_CPU%UL -O%OPT% -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -std=gnu99 -MMD -MP -Wall

set TARGET=nrf24
echo Compiling C: %TARGET%.c
avr-gcc -c %C_FLAGS% %NRF24_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
if errorlevel 1 goto err

set TARGET=main
echo Compiling C: ../src/%TARGET%.c
avr-gcc -c %C_FLAGS% %SRC_FILES%\%TARGET%.c -o %OBJ_FILES%\%TARGET%.o
if errorlevel 1 goto err

echo Linking.
avr-gcc %C_FLAGS% --output %OBJ_FILES%\main.elf ^
	%OBJ_FILES%\nrf24.o ^
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


REM set OUT_FILE=QuoteFinam

REM if exist %OUT_FILE%.exe del %OUT_FILE%.exe
REM windres "%SRC_FILES%\res.rc" "%OBJ_FILES%\res.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\asfunc.cpp" -o "%OBJ_FILES%\asfunc.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\asstatus.cpp" -o "%OBJ_FILES%\asstatus.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\asbutton.cpp" -o "%OBJ_FILES%\asbutton.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\asdatagrid.cpp" -o "%OBJ_FILES%\asdatagrid.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\itsocket.cpp" -o "%OBJ_FILES%\itsocket.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\main.cpp" -o "%OBJ_FILES%\main.o"
REM if errorlevel 1 goto err

REM g++ -o %OUT_FILE%.exe ^
	REM "%OBJ_FILES%\res.o" ^
	REM "%OBJ_FILES%\asfunc.o" ^
	REM "%OBJ_FILES%\asstatus.o" ^
	REM "%OBJ_FILES%\asdatagrid.o" ^
	REM "%OBJ_FILES%\main.o" ^
	REM -mwindows -static -lcomctl32 -lwininet
	REM "%OBJ_FILES%\itsocket.o" ^
	REM "%OBJ_FILES%\asbutton.o" ^
	
	REM "%OBJ_FILES%\itsocket.o" ^ -dllimport "%SRC_FILES%\SmartCOM4.dll
	REM "%OBJ_FILES%\asdatagrid.o" ^ -dllimport sqlite3.dll -lwsock32 -dllimport zlibwapi.dll
	REM "%OBJ_FILES%\itinvest.o" ^ -dllimport stcln-2 -lws2_32
	REM "%OBJ_FILES%\inflate.o" ^
	

REM if exist %OUT_FILE%.exe strip %OUT_FILE%.exe
REM if errorlevel 1 goto err

REM call %OUT_FILE%.exe
pause
exit

:err
pause
exit

REM g++ %C_FLAGS% "%SRC_FILES%\assocket.cpp" -o "%OBJ_FILES%\assocket.o"
REM if errorlevel 1 goto err

REM g++ %C_FLAGS% "%SRC_FILES%\asdatagrid.cpp" -o "%OBJ_FILES%\asdatagrid.o"
REM if errorlevel 1 goto err



REM ==================================================================================================
													REM -------- begin --------

													REM Cleaning project:
													REM rm -f ../main.hex
													REM rm -f ../main.eep
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
													REM avr-gcc -c -mmcu=attiny2313a -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigne
													REM d-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=ob
													REM j/main.lst  -std=gnu99 -MMD -MP -MF .dep/main.o.d ../src/main.c -o obj/main.o
													REM In file included from ../src/main.h:8,
																	 REM from ../src/main.c:1:
													REM ../src/bits_macros.h:23:1: warning: multi-line comment

													REM Linking: main.elf
													REM avr-gcc -mmcu=attiny2313a -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigned-b
													REM itfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=obj/m
													REM ain.o  -std=gnu99 -MMD -MP -MF .dep/main.elf.d obj/main.o --output main.elf -Wl,
													REM -Map=main.map,--cref     -lm

													REM Linking: ../main.elf
													REM avr-gcc -mmcu=attiny2313a -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigned-b
													REM itfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=obj/m
													REM ain.o  -std=gnu99 -MMD -MP -MF .dep/main.elf.d obj/main.o --output ../main.elf -
													REM Wl,-Map=main.map,--cref     -lm

													REM Creating load file for Flash: ../main.hex
													REM avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock ../main.elf ../main.hex

													REM Creating load file for EEPROM: ../main.eep
													REM avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
															REM --change-section-lma .eeprom=0 --no-change-warnings -O ihex ../main.elf
													REM ../main.eep || exit 0

													REM Creating Extended Listing: main.lss
													REM avr-objdump -h -S -z main.elf > main.lss

													REM Creating Symbol Table: main.sym
													REM avr-nm -n main.elf > main.sym

													REM Size after:
													REM AVR Memory Usage
													REM ----------------
													REM Device: attiny2313a

													REM Program:      66 bytes (3.2% Full)
													REM (.text + .data + .bootloader)

													REM Data:          0 bytes (0.0% Full)
													REM (.data + .bss + .noinit)



													REM -------- end --------

													REM rm ../main.elf
													REM Для продолжения нажмите любую клавишу . . .


REM ==========================================================================================


REM -------- begin --------

REM Cleaning project:
REM rm -f ../main.hex
REM rm -f ../main.eep
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
REM avr-gcc -c -mmcu=attiny2313a -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigne
REM d-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=ob
REM j/main.lst  -std=gnu99 -MMD -MP -MF .dep/main.o.d ../src/main.c -o obj/main.o
REM In file included from ../src/main.h:8,
                 REM from ../src/main.c:1:
REM ../src/bits_macros.h:23:1: warning: multi-line comment

REM Linking: main.elf
REM avr-gcc -mmcu=attiny2313a -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigned-b
REM itfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=obj/m
REM ain.o  -std=gnu99 -MMD -MP -MF .dep/main.elf.d obj/main.o --output main.elf -Wl,
REM -Map=main.map,--cref     -lm

REM Linking: ../main.elf
REM avr-gcc -mmcu=attiny2313a -I. -DF_CPU=8000000UL -Os -funsigned-char -funsigned-b
REM itfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes -Wa,-adhlns=obj/m
REM ain.o  -std=gnu99 -MMD -MP -MF .dep/main.elf.d obj/main.o --output ../main.elf -
REM Wl,-Map=main.map,--cref     -lm

REM Creating load file for Flash: ../main.hex
REM avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock ../main.elf ../main.hex

REM Creating load file for EEPROM: ../main.eep
REM avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" \
        REM --change-section-lma .eeprom=0 --no-change-warnings -O ihex ../main.elf
REM ../main.eep || exit 0

REM Creating Extended Listing: main.lss
REM avr-objdump -h -S -z main.elf > main.lss

REM Creating Symbol Table: main.sym
REM avr-nm -n main.elf > main.sym

REM Size after:
REM AVR Memory Usage
REM ----------------
REM Device: attiny2313a

REM Program:      66 bytes (3.2% Full)
REM (.text + .data + .bootloader)

REM Data:          0 bytes (0.0% Full)
REM (.data + .bss + .noinit)



REM -------- end --------

REM rm ../main.elf
REM Для продолжения нажмите любую клавишу . . .

