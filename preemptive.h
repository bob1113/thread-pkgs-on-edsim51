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

#define CNAME(s) _ ## s

// #define SemaphoreCreate(s, n) \
// {   __asm \
//         MOV CNAME(s), n \
//     __endasm; \
// }

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

#endif // __PREEMPTIVE_H__
