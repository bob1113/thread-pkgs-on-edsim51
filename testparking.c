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
void _mcs51_genXRAMCLEAR(void){}

void timer0_ISR(void) __interrupt(1) {
    __asm
        ljmp _myTimer0Handler
    __endasm;
}


/* 
 * REVIEW:
 * declare global variables 
 */ 

__data __at (0x20) char sem;
__data __at (0x21) char spotsEmpty;
__data __at (0x22) char make_car;
__data __at (0x23) char spot0;
__data __at (0x24) char spot1;
__data __at (0x25) char car_name[4];
__data __at (0x29) char car;
__data __at (0x2A) char car_id;

void print_now(void) {
	TMOD |= 0x20;
	TH1   = -6;
	SCON  = 0x50;
	TR1   = 1;
	
	SBUF = time;
	while(!TI);
	TI = 0;
}

void print_log(char io, char spot) {
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

	SBUF = ' ';
	while(!TI);
	TI = 0;
}


void MakeCar(void) {
	
	// SemaphoreWait(make_car, label);
	SemaphoreWait(spotsEmpty, label);
	
	EA = 0;
	if ( spot0 == '_' ) {
		// park the current car into spot0
		SemaphoreWait(sem, label);
		spot0 = car_name[ threadID_current ];
		SemaphoreSignal(sem);
		print_log ('i', '0');

	} else if ( spot1 == '_') {
		// park the current car into spot1
		SemaphoreWait(sem, label);
		spot1 = car_name[ threadID_current ];
		SemaphoreSignal(sem);
		print_log ('i', '1');
	}
	EA = 1;

	delay(10);

	EA = 0;
	if ( spot0 == car_name[ threadID_current ] ) {
		// remove the current car from spot0
		SemaphoreWait(sem, label);
		spot0 = '_';
		SemaphoreSignal(sem);
		print_log ('o', '0');

	} else if ( spot1 == car_name[ threadID_current ] ) {
		// remove the current car from spot1
		SemaphoreWait(sem, label);
		spot1 = '_';
		SemaphoreSignal(sem);
		print_log ('o', '1');

	}
	EA = 1;

	SemaphoreSignal ( spotsEmpty );
	SemaphoreSignal ( make_car );
	
	ThreadExit();
}


void main(void) {
	
	// print_now();
	SemaphoreCreate ( sem, #1 );
	SemaphoreCreate ( spotsEmpty, #2 );
	SemaphoreCreate ( make_car, #3 );

	spot0 = '_';
	spot1 = '_';
	car = '1';

	EA = 1;        

	SemaphoreWait ( make_car, label );
	car_id = ThreadCreate( MakeCar );
	car_name[ car_id ] = car;
	car += 1;

	SemaphoreWait ( make_car, label );
	car_id = ThreadCreate( MakeCar );
	car_name[ car_id ] = car;
	car += 1;

	SemaphoreWait ( make_car, label );
	car_id = ThreadCreate( MakeCar );
	car_name[ car_id ] = car;
	car += 1;

	// when to create other cars(threads)?
	
	SemaphoreWait ( make_car, label );
	car_id = ThreadCreate( MakeCar );
	car_name[ car_id ] = car;
	car += 1;

	SemaphoreWait ( make_car, label );
	car_id = ThreadCreate( MakeCar );
	car_name[ car_id ] = car;
	car += 1;

	ThreadExit();
}

void _sdcc_gsinit_startup(void) {
	__asm
		ljmp  _Bootstrap
	__endasm;
}
