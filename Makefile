
DEVICE     = atmega32
CLOCK      = 4000000UL

all:
	avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -I. -c main.c -o main.o 
	avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -I. -c lcd_lib.c -o lcd_lib.o
	avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -o main.elf main.o lcd_lib.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

clean:
	rm -rf *.o *.hex
program:
	sudo avrdude -c usbasp -pm32 -U flash:w:main.hex