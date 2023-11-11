#include "pcb.h"
#include <dos.h>
#include <iostream.h>
#include "SCHEDULE.H"
#include "queue.h"


const int PCB::NEW = 0;
const int PCB::READY = 1;
const int PCB::BLOCKED = 3;
const int PCB::FINISHED = 4;

int PCB::ID = 0;

PCB::PCB(StackSize stack, Time time, Thread *thread) {
	lock
	id = ID++;
	stackSize = stack;
	timeSlice = time;
	timeCount = time;
	myThread = thread;

	state = NEW;
	stack = 0;

	waiting = new Queue();
	allPCB.insert(this);
	unlock
}


void PCB::createStack() {
	lock
	unsigned n = stackSize / 2;
	stack = new unsigned[n];

	stack[n - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[n - 2] = FP_SEG(PCB::wrapper);
	stack[n - 3] = FP_OFF(PCB::wrapper);

	sp = FP_OFF(stack + n - 12);
	ss = FP_SEG(stack + n - 12);
	bp = FP_OFF(stack + n - 12);
#endif
	unlock
}


PCB::~PCB() {
	lock
	delete stack;
	delete waiting;
	allPCB.getbyId(this->id);
	unlock
}


void PCB::wrapper() {
	running->myThread->run();
	lock
	running->state = PCB::FINISHED;

	PCB* temp;
	while(running->waiting->first != 0) {
		temp = running->waiting->remove();
		temp->state = PCB::READY;
		Scheduler::put(temp);
	}
	unlock

	dispatch();
}
