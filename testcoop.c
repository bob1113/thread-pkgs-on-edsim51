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

__data __at (0x20) char BUF;
__data __at (0x21) char BUF_AVALIABLE;
__data __at (0x22) char next_produced;

/* TODO: [8 pts] for this function
 * the producer in this test program generates one characters at a
 * time from 'A' to 'Z' and starts from 'A' again. The shared buffer
 * must be empty in order for the Producer to write.
 */

void Producer(void) {
	/*
	 * TODO: [2 pt]
	 * initialize producer data structure, and then enter
	 * an infinite loop (does not return)
	 */
	next_produced = 'A';
	while (1) {
		/* TODO: [6 pt]
		* wait for the buffer to be available, 
		* and then write the new data into the buffer */
		while(BUF_AVALIABLE == 1) ThreadYield(); // buf is full
		BUF = next_produced;
		BUF_AVALIABLE = 1;
		if(next_produced == 'Z') next_produced = 'A';
		else next_produced++;
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
	TH1  = -6;
	SCON = 0x50;
	TR1  = 1;
	while (1) {
		/* TODO: [2 pt] wait for new data from producer
		 * TODO: [6 pt] write data to serial port Tx, 
		 * poll for Tx to finish writing (TI),
		 * then clear the flag
		 */
		while(BUF_AVALIABLE == 0) ThreadYield(); // buffer is empty
		
		SBUF = BUF;
		BUF_AVALIABLE = 0;
		
		while(!TI); // FIXME: something wrong here if call yield()
		TI = 0;
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
	// BUF[10] = {0};
	BUF_AVALIABLE = 0;

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
