#include <dos.h>
#include <iostream.h>
#include <stdio.h>

#include "system.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "thread.h"
#include "queue.h"
#include "idle.h"
#include "semList.h"
#include "kernSem.h"
#include "kernEv.h"
#include "ivtentry.h"

PCB* mainPCB;
Thread* mainThread;
PCB* running;
Idle* idleThread;


pInterrupt System::oldISR;
int System::isTimer = 1;

unsigned tsp;
unsigned tss;
unsigned tbp;

System::System() {

}


System::~System() {

}


void System::inic() {
	idleThread = new Idle();
	mainThread = new Thread(4096, 1);

	mainPCB = mainThread->myPCB;
	mainPCB->state = PCB::READY;
	running = mainPCB;
	idleThread->start();

	//allPCB = new Queue();
	allPCB.insert((PCB*)mainPCB); //ne znam da li ovo treba??
	allPCB.insert((PCB*)idleThread->getPCB()); //ne znam da li ovo treba??

	//allSem = new SemList();

	lock
	oldISR = getvect(0x08);
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, timer);
	setvect(0x60, oldISR);
#endif
	unlock
}


void System::restore() {
	lock
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldISR);
#endif

	delete mainThread;
	delete idleThread;
	delete mainPCB;

	//delete allPCB;

	//delete allSem;
	unlock
}


void System::dispatchS() {
	lock
	System::isTimer = 0;
	timer();
	unlock
}


void interrupt timer(...) {
	if(System::isTimer == 1) {
		tick();
		lock
		running->timeCount--;
		allSem.update();
		unlock
	}

	if(System::isTimer == 0 || running->timeCount == 0 &&  running->timeSlice != 0) {
		lock
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		if(running->state == PCB::READY && running->id != idleThread->getId()) {
			Scheduler::put((PCB*)running);
		}

		running = Scheduler::get();

		if(running == 0) {
			running = idleThread->getPCB();
		}

		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}

		running->timeCount = running->timeSlice;
		System::isTimer = 1;
		unlock
	}

	if(System::isTimer == 1) {
		asm int 60h;
	}
}




