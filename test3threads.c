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
__data __at (0x22) char next_produced1;
__data __at (0x23) char next_produced2;

__data __at (0x24) char mutex; 
__data __at (0x25) char full;
__data __at (0x26) char empty;
__data __at (0x27) char BUF_SIZE;
__data __at (0x28) char BUF[3];
__data __at (0x2B) char producer_wait_1;
__data __at (0x2C) char producer_wait_2;



/* 
 * TODO: 
 * revise Producer function into preemptive ver.
 */

void Producer1(void) {
	next_produced1 = 'A';
	while (1) {
		SemaphoreWait(producer_wait_2, label);
		SemaphoreWait(empty, label);
		SemaphoreWait(mutex, label);
		BUF[BUF_HEAD] = next_produced1;
		BUF_HEAD = (BUF_HEAD + 1) % BUF_SIZE;
		SemaphoreSignal(mutex);
		SemaphoreSignal(full);
		SemaphoreSignal(producer_wait_1);
		if(next_produced1 == 'Z') next_produced1 = 'A';
		else next_produced1++;
	}
}

void Producer2(void) {
	next_produced2 = '0';
	while (1) {
		SemaphoreWait(producer_wait_1, label);
		SemaphoreWait(empty, label);
		SemaphoreWait(mutex, label);
		BUF[BUF_HEAD] = next_produced2;
		BUF_HEAD = (BUF_HEAD + 1) % BUF_SIZE;
		SemaphoreSignal(mutex);
		SemaphoreSignal(full);
		SemaphoreSignal(producer_wait_2);
		if(next_produced2 == '9') next_produced2 = '0';
		else next_produced2++;
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
	// mutex = 1;	// SemaphoreCreate(mutex, 1);
	// full = 0;	// SemaphoreCreate(full, 0);
	// empty = 3;	// SemaphoreCreate(empty, 3);

	SemaphoreCreate(mutex, #1);
	SemaphoreCreate(full, #0);
	SemaphoreCreate(empty, #3);
	SemaphoreCreate(producer_wait_1, #1);
	SemaphoreCreate(producer_wait_2, #1);
	BUF_SIZE = 3;
	BUF_HEAD = 0;
	BUF_TAIL = 0;
	ThreadCreate(Consumer);
	ThreadCreate(Producer2);
	Producer1();
}

void _sdcc_gsinit_startup(void) {
	__asm
		ljmp  _Bootstrap
	__endasm;
}
