/* 
 * file: testcoop.c 
 */
#include <8051.h>
#include "cooperative.h"

/* 
 * TODO: [2pt] 
 * declare your global variables here, for the shared buffer 
 * between the producer and consumer.  
 * Hint: you may want to manually designate the location for the 
 * variable.  you can use
 *        __data __at (0x30) type var; 
 * to declare a variable var of the type
 */ 

__data __at (0x20) char buf[10];
__data __at (0x2A) char BUF_SIZE;
__data __at (0x2B) char BUF_IN;
__data __at (0x2C) char BUF_OUT;

/* TODO: [8 pts] for this function
 * the producer in this test program generates one characters at a
 * time from 'A' to 'Z' and starts from 'A' again. The shared buffer
 * must be empty in order for the Producer to write.
 */
char make_item(void) {
	static char c = 'A'; 
	if (c > 'Z') { 
		c = 'A';
	}
	return c++;
}

void Producer(void) {
	/*
	 * TODO: [2 pt]
	 * initialize producer data structure, and then enter
	 * an infinite loop (does not return)
	 */
	char next_produced;
	while (1) {
		/* TODO: [6 pt]
		* wait for the buffer to be available, 
		* and then write the new data into the buffer */
		next_produced = make_item();
		while((BUF_IN + 1) % BUF_SIZE == BUF_OUT) { // buffer is full
			ThreadYield();
		}
		buf[BUF_IN] = next_produced;
		BUF_IN = (BUF_IN + 1) % BUF_SIZE;
	}
	
}

/* [10 pts for this function]
 * the consumer in this test program gets the next item from
 * the queue and consume it and writes it to the serial port.
 * The Consumer also does not return.
 */
void Consumer(void) {
	/* TODO: [2 pt] initialize Tx for polling */
	TMOD = 0x20;
	TH1 = -6;
	SCON = 0x50;
	TR1 = 1;
	while (1) {
		/* TODO: [2 pt] wait for new data from producer
		 * TODO: [6 pt] write data to serial port Tx, 
		 * poll for Tx to finish writing (TI),
		 * then clear the flag
		 */
		while(BUF_IN % BUF_SIZE == BUF_OUT) { // buffer is empty
			ThreadYield();
		}
		while(!TI) ThreadYield();
		SBUF = buf[BUF_OUT];
		TI = 0;
		BUF_OUT = (BUF_OUT + 1) % BUF_SIZE;	
	}
}

/* [5 pts for this function]
 * main() is started by the thread bootstrapper as thread-0.
 * It can create more thread(s) as needed:
 * one thread can act as producer and another as consumer.
 */
void main(void) {
	/* 
	 * TODO: [1 pt] initialize globals 
	 * TODO: [4 pt] set up Producer and Consumer.
	 * Because both are infinite loops, there is no loop
	 * in this function and no return.
	 */

	// FIXME: buffer initialization fails
	// buf[10] = {0};
	BUF_SIZE = 10;
	BUF_IN = 0;
	BUF_OUT = 0;

	// FIXME: uncertain about this
	ThreadID temp_id = ThreadCreate(Producer);
	Consumer();
}

void _sdcc_gsinit_startup(void) {
	__asm
		ljmp  _Bootstrap
	__endasm;
}

void _mcs51_genRAMCLEAR(void) {}
void _mcs51_genXINIT(void) {}
void _mcs51_genXRAMCLEAR(void) {}
