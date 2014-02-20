all:
	avr-gcc *.c -mmcu=attiny84 -c -Os
	avr-gcc -g -mmcu=attiny84 -Os -o main.elf *.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	sudo avrdude -p t84 -c usbtiny -u -U flash:w:main.hex
