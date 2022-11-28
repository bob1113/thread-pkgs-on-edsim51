/* 
 * file: testpreempt.c 
 */
#include <8051.h>
#include "preemptive.h"

/*
 * declare ISR
 */
void _mcs51_genRAMCLEAR(void) {}
void _mcs51_genXINIT(void)    {}
void _mcs51_genXRAMCLEAR(void){}

void timer0_ISR(void) __interrupt(1) {
    __asm
        ljmp _myTimer0Handler
    __endasm;
}

/* 
 * TODO:
 * declare global variables 
 */ 

__data __at (0x20) char BUF;
__data __at (0x21) char BUF_AVALIABLE;
__data __at (0x22) char next_produced;

__data __at (0x23) char mutex; 
__data __at (0x24) char full;
__data __at (0x25) char empty;

/* 
 * TODO: 
 * revise Producer function into preemptive ver.
 */

void Producer(void) {
	next_produced = 'A';
	while (1) {
		if(BUF_AVALIABLE == 0){
			SemaphoreWait(empty);
			SemaphoreWait(mutex);
			BUF = next_produced;
			BUF_AVALIABLE = 1;
			SemaphoreSignal(mutex);
			SemaphoreSignal(full);
            if(next_produced == 'Z') next_produced = 'A';
            else next_produced++;
        }
	}
	
}

/* TODO:  
 * revise Consumer() function into preemptive ver.
 */
void Consumer(void) {
	/* TODO: [2 pt] initialize Tx for polling */
	TMOD |= 0x20;
	TH1   = -6;
	SCON  = 0x50;
	TR1   = 1;
	while (1) {
		/* TODO: 
		 * write data to serial port Tx
		 */
	    if(BUF_AVALIABLE == 1) {
			SemaphoreWait(full);
			SemaphoreWait(mutex);
			SBUF = BUF;
			BUF_AVALIABLE = 0;
			SemaphoreSignal(mutex);
			SemaphoreSignal(empty);
    		while(!TI);
	    	TI = 0;
        }
	}
}

/* TODO:
 * main() is started by the thread bootstrapper as thread-0.
 * It can create more thread(s) as needed:
 * one thread can act as producer and another as consumer.
 */
void main(void) {
	/* 
	 * TODO: 
     * initialize globals & set up Producer and Consumer
	 */
	BUF_AVALIABLE = 0;
	BUF = 0;
	SemaphoreCreate(mutex, 1);
	SemaphoreCreate(full, 0);
	SemaphoreCreate(empty, 1);
	ThreadCreate(Producer);
	Consumer();
}

void _sdcc_gsinit_startup(void) {
	__asm
		ljmp  _Bootstrap
	__endasm;
}
