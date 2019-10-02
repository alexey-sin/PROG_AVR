@echo off
PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%~d0\PROG_AVR\_AVRISPmkII\STK500;%PATH%

cd ./build
REM -f High Low
call Stk500.exe -dATmega88 -cUSB -ms -fD9E9

REM хелп
REM call Stk500.exe -h

REM список поддерживаемых устройств
REM call Stk500.exe -?

pause
exit


REM калькулятор фьюзе бит: http://www.engbedded.com/fusecalc/


