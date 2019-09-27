::avrdude.exe -p m16 -F -B 4800 -c apu_2 -P ft0 -U hfuse:w:217:m -U lfuse:w:228:m
avrdude.exe -p m16 -F -c apu_2 -P ft0 -U flash:w:Test.hex:a
Pause=null