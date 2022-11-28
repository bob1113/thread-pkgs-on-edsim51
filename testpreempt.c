/* 
 * file: testpreempt.c 
 */
#include <8051.h>
#include "preemptive.h"

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)
#define label CONCAT(__COUNTER__, $)
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

__data __at (0x20) char BUF_HEAD;
__data __at (0x21) char BUF_TAIL;
__data __at (0x22) char next_produced;

__data __at (0x23) char mutex; 
__data __at (0x24) char full;
__data __at (0x25) char empty;
__data __at (0x26) char BUF_SIZE;
__data __at (0x27) char BUF[3];

/* 
 * TODO: 
 * revise Producer function into preemptive ver.
 */

void Producer(void) {
	next_produced = 'A';
	while (1) {
		SemaphoreWait(empty, label);
		SemaphoreWait(mutex, label);
		BUF[BUF_HEAD] = next_produced;
		BUF_HEAD = (BUF_HEAD + 1) % BUF_SIZE;
		SemaphoreSignal(mutex);
		SemaphoreSignal(full);
		if(next_produced == 'Z') next_produced = 'A';
		else next_produced++;

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
		SemaphoreWait(full, label);
		SemaphoreWait(mutex, label);
		SBUF = BUF[BUF_TAIL];
		BUF_TAIL = (BUF_TAIL + 1) % BUF_SIZE;
		SemaphoreSignal(mutex);
		SemaphoreSignal(empty);
		while(!TI);
		TI = 0;
	
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
	BUF_SIZE = 3;
	BUF_HEAD = 0;
	BUF_TAIL = 0;
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
