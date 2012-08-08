# AVR-GCC Makefile
PROJECT=tutorial_1
SOURCES=main.c common.c inout.c arduino_pins.c
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=atmega1280
PORT=COM12

CFLAGS=-mmcu=$(MMCU) -Os -Wall
	
program: $(PROJECT).hex
	avrdude -p m1280 -b 57600 -c arduino -P ${PORT} -U flash:w:$(PROJECT).hex

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex
	
$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).out $(SOURCES)	

clean:
	rm -rf *.o
	rm -rf *.hex
	rm -rf *.out
	rm -rf *.inc

	