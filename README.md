# pulseoximeter
Pulsioxymeter using Atmega328p, MAX30100 and SSD_1306
atmega328p has a SRAM of 2Kb 
SSD1306 oled display requires: WIDTH * ((HEIGHT + 7) / 8)) bytes of free ram to run 
Hence the code is completely done on 128x32 rather than 128x64
