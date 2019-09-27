@echo off
PATH=%~d0\WinAVR\bin;%~d0\WinAVR\utils\bin;%PATH%

REM cd ./build
make -f ./Makefile clean
make -f ./Makefile all
pause
exit





