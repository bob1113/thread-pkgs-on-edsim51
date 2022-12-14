#
# makefile for testing preemptive multithreading
#
# This assumes you have SDCC installed and this targets EdSim51.
# The generated .hex file should be one that can be loaded and run
# directly.
#
# Author: Pai Chou
# CS 3423 Fall 2018
#

CC = sdcc
CFLAGS = -c 
LDFLAGS = 
#--stack-after-data --stack-loc 0x39 --data-loc 0x20

C_OBJECTS = test3threads.rel preemptive.rel

all: test3threads.hex

test3threads.hex:   $(C_OBJECTS) $(ASM_OBJECTS)
				$(CC) $(LDFLAGS) -o test3threads.hex $(C_OBJECTS)

clean:
	rm *.hex *.ihx *.lnk *.lst *.map *.mem *.rel *.rst *.sym   *.lk *.asm

# use in windows env	
clear:
	del *.hex *.ihx *.lnk *.lst *.map *.mem *.rel *.rst *.sym   *.lk *.asm
%.rel:  %.c    preemptive.h Makefile
	$(CC) $(CFLAGS) $<