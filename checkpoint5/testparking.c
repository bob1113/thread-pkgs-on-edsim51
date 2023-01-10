/* 
 * file: testparking.c 
 */
#include <8051.h>
#include "preemptive.h"


/*
 * declare ISR
 */
void _mcs51_genRAMCLEAR(void) {}
void _mcs51_genXINIT(void) {}
void _mcs51_genXRAMCLEAR(void) {}

void timer0_ISR(void) __interrupt(1) {
    __asm
        ljmp _myTimer0Handler
    __endasm;
}


/* 
 * REVIEW:
 * declare global variables 
 */ 


__data __at (0x38) char SBUF_MUTEX;
__data __at (0x34) char sem;
__data __at (0x21) char spots_empty;
__data __at (0x22) char make_car;
__data __at (0x23) char spot0;
__data __at (0x24) char spot1;
__data __at (0x25) char car_name[4];
__data __at (0x29) char car;
__data __at (0x2A) char car_id;

__data __at (0x2B) char time_temp0;
__data __at (0x2C) char time_temp1;
__data __at (0x2D) char time_temp2;
__data __at (0x2E) char time_temp3;
__data __at (0x2F) char index;

void print_log(char io, char spot) {
	SemaphoreWait(SBUF_MUTEX, label);
	TMOD |= 0x20;
	TH1   = -6;
	SCON  = 0x50;
	TR1   = 1;

	SBUF = car_name[ threadID_current];
	while(!TI);
	TI = 0;

	SBUF = io;
	while(!TI);
	TI = 0;

	SBUF = spot;
	while(!TI);
	TI = 0;

	SBUF = '-';
	while(!TI);
	TI = 0;

	SemaphoreSignal(SBUF_MUTEX);
}

void print_time(void) {
	SemaphoreWait(SBUF_MUTEX, label);
	TMOD |= 0x20;
	TH1   = -6;
	SCON  = 0x50;
	TR1   = 1;

	time_temp0 = now();

	SBUF = ((time_temp0 >> 6) & 7) + '0';
	while(!TI);
	TI = 0;

	SBUF = ((time_temp0 >> 3) & 7) + '0';
	while(!TI);
	TI = 0;

	SBUF = (time_temp0 & 7) + '0';
	while(!TI);
	TI = 0;
	
	SBUF = '\n';
	while(!TI);
	TI = 0;
	SemaphoreSignal(SBUF_MUTEX);
}


void MakeCar(void) {
	
	// SemaphoreWait(make_car, label);
	SemaphoreWait(spots_empty, label);
	
	EA = 0;
	SemaphoreWait(sem, label);
	if ( spot0 == '_' ) {
		// park the current car into spot0
		spot0 = car_name[ threadID_current ];
		print_log ('i', '0');
		print_time();

	} else if ( spot1 == '_') {
		// park the current car into spot1
		spot1 = car_name[ threadID_current ];
		print_log ('i', '1');
		print_time();
	}
	SemaphoreSignal(sem);
	EA = 1;

	delay(10);

	EA = 0;
	SemaphoreWait(sem, label);
	if ( spot0 == car_name[ threadID_current ] ) {
		// remove the current car from spot0
		spot0 = '_';
		print_log ('o', '0');
		print_time();

	} else if ( spot1 == car_name[ threadID_current ] ) {
		// remove the current car from spot1
		spot1 = '_';
		print_log ('o', '1');
		print_time();
	}
	SemaphoreSignal(sem);
	EA = 1;

	SemaphoreSignal ( spots_empty );
	SemaphoreSignal ( make_car );
	
	ThreadExit();
}


void main(void) {
	
	// print_now();
	SemaphoreCreate ( sem, #1 );
	SemaphoreCreate ( spots_empty, #2 );
	SemaphoreCreate ( make_car, #3 );
	SemaphoreCreate ( SBUF_MUTEX, #1 );

	spot0 = '_';
	spot1 = '_';
	car = '1';

	EA = 1;        

	for(index = 0; index < 5; index++) {
		SemaphoreWait ( make_car, label );
		car_id = ThreadCreate( MakeCar );
		car_name[ car_id ] = car;
		car += 1;
	}
	
	ThreadExit();
}

void _sdcc_gsinit_startup(void) {
	__asm
		ljmp  _Bootstrap
	__endasm;
}
