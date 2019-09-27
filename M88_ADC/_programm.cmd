@echo off
PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%~d0\PROG_AVR\_AVRISPmkII\STK500;%PATH%

cd ./build
REM make -f ./Makefile program
call Stk500.exe -dATmega88 -cUSB -ms -e -if"main.hex" -pf -vf
call Stk500.exe -dATmega88 -cUSB -ms -ie"main.eep" -pe -ve
REM -ie"main.eep"

REM хелп
REM call Stk500.exe -h

REM список поддерживаемых устройств
REM call Stk500.exe -?

pause
exit





