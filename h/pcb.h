#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "queue.h"
#include "system.h"
#include "idle.h"

#define lock asm pushf; asm cli;
#define unlock asm popf;

class Queue;

class PCB {
public:

	friend class System;
	friend class Idle;

	static int ID;
	int id;

	static const int NEW, READY, BLOCKED, FINISHED;
	volatile int state;

	Thread* myThread;

	unsigned int ss, sp, bp;
	StackSize stackSize;
	unsigned* stack;

	Time timeSlice;
	Time timeCount;

	Queue* waiting;

	int semCheck;

	PCB(StackSize stack, Time time, Thread *thread);
	void createStack();
	static void wrapper();
	~PCB();
};


extern void tick();


#endif
