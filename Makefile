build:
	avr-gcc -Wall -g -Os -mmcu=attiny85 -o main.bin main.c
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex

flash:
	avrdude -p attiny85 -c usbtiny -U flash:w:main.hex:i -F -P usb

clean:
	rm -f *.hex *.bin
