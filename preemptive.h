/*
 * file: preemptive.h
 *
 * this is the include file for the preemptive multithreading
 * package.  It is to be compiled by SDCC and targets the EdSim51 as
 * the target architecture.
 *
 * CS 3423 Fall 2018
 */

#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define MAXTHREADS 4  /* not including the scheduler */
/* the scheduler does not take up a thread of its own */

__data __at (0x35) char threadID_current;
__data __at (0x3A) unsigned char time;
__data __at (0x3B) unsigned char thread_time_target[4];

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)
#define label CONCAT(__COUNTER__, $)

#define CNAME(s) _ ## s

#define SemaphoreCreate(s, n) \
{   s = n;\ }

#define SemaphoreSignal(s) \
{	__asm \
		INC CNAME(s) \
	__endasm; \
}

#define SemaphoreWait(s, label) \
{   __asm \
        label: \
            MOV A, CNAME(s) \
            JB ACC.7, label ;; negative \
            JZ label ;; zero \
            dec CNAME(s) \
    __endasm; \
}

typedef char ThreadID;
typedef void (*FunctionPtr)(void);

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);
void myTimer0Handler(void);

void delay(unsigned char n);
unsigned char now(void);

#endif // __PREEMPTIVE_H__

