@echo off
PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%

make -f ./Makefile program
pause
exit





