#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "system.h"

void Thread::start() {
	//lock
	if(myPCB->state != PCB::NEW)
		//unlock
		return;

	myPCB->createStack();

	if(this->getId() != idleThread->getId()) {
		myPCB->state = PCB::READY;
		Scheduler::put(myPCB);
	}
	//unlock
}


Thread::Thread(StackSize stackSize, Time timeSlice) {
	//lock
	myPCB = new PCB(stackSize, timeSlice, this);
	//unlock
}


Thread::~Thread() {
	//lock
	waitToComplete();
	delete myPCB;
	//unlock
}


ID Thread::getId() {
	//lock
	return myPCB->id;
	//unlock
}


ID Thread::getRunningId() {
	//lock
	return running->id;
	//unlock
}


Thread* Thread::getThreadById(ID id) {
	//lock
	PCB* temp = allPCB.getbyId(id);
	return temp->myThread;
	//unlock
}


void Thread::waitToComplete() {
	//lock
	if (myPCB != running &&  this->getId() != idleThread->getId() && myPCB->state != PCB::FINISHED) {
		myPCB->waiting->insert((PCB*)running);
		running->state = PCB::BLOCKED;
		dispatch();
	}
	//unlock
}


void dispatch() {
	//lock
    System::dispatchS();
	//unlock
}
