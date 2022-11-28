/*
 * file: preemptive.h
 *
 * this is the include file for the cooperative multithreading
 * package.  It is to be compiled by SDCC and targets the EdSim51 as
 * the target architecture.
 *
 * CS 3423 Fall 2018
 */

#ifndef __PREEMPTIVE_H__
#define __PREEMPTIVE_H__

#define MAXTHREADS 4  /* not including the scheduler */
/* the scheduler does not take up a thread of its own */

#define SemaphoreCreate(s, n) \     // create a counting semaphore s that is initialized to n
{	__asm \
		MOV s, #n \
	__endasm;
}

#define CNAME(s) _ ## s

#define SemaphoreSignal(s) \   // signal() semaphore s
{	__asm \
		INC CNAME(s) \
	__endasm;
}

#define SemaphoreWait(S,  __COUNTER__ ) \
{   __asm \
    __COUNTER__ : \
        MOV A, s \
        JB ACC.7 label ;; negative \
        JZ label ;; zero \
        dec  CNAME(S) \
    __endasm;
}

typedef char ThreadID;
typedef void (*FunctionPtr)(void);

ThreadID ThreadCreate(FunctionPtr);
void ThreadYield(void);
void ThreadExit(void);
void myTimer0Handler(void);

#endif // __PREEMPTIVE_H__
