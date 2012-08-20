# AVR-GCC Makefile
PROJECT=engg2800_micro
SOURCES=main.c common.c display.c inout.c timeutil.c control.c
PIN_DEF=arduino_pins.c
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=atmega1280
PORT=COM12
PROGRAM=arduino
DEVICE=m1280

CFLAGS=-mmcu=$(MMCU) -Os -Wall
	
program: $(PROJECT).hex
	avrdude -p ${DEVICE} -b 57600 -c ${PROGRAM} -P ${PORT} -U flash:w:$(PROJECT).hex
	
	

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -j .data -O ihex $(PROJECT).out $(PROJECT).hex
	
$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT).out $(SOURCES) $(PIN_DEF)	

clean:
	rm -rf *.o
	rm -rf *.hex
	rm -rf *.out
	rm -rf *.inc

	