@echo off
REM PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%

REM make -f ./Makefile program
PATH=%~d0\PROG_AVR\_AVRISPmkII\STK500;%PATH%
REM %~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;
cd ./build
REM make -f ./Makefile program
call Stk500.exe -dATmega32 -cUSB -ms -e -if"main.hex" -pf -vf
call Stk500.exe -dATmega32 -cUSB -ms -ie"main.eep" -pe -ve
REM -ie"main.eep"

REM ����
REM call Stk500.exe -h

REM ������ �������������� ���������
REM call Stk500.exe -?


pause
exit





