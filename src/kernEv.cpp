#include "kernEv.h"
#include "event.h"
#include "ivtentry.h"
#include "system.h"
#include "pcb.h"
#include "SCHEDULE.H"

int KernelEv::count = 0;

KernelEv::KernelEv(unsigned ivtNo) {
	value = 1;
	ivtno = ivtNo;

	owner = (PCB*)running;
	blocked = 0;

	IVTEntry::IVTable[ivtno]->kernelEv = this;
	count++;
}


KernelEv::~KernelEv() {
	lock

	IVTEntry::IVTable[ivtno]->kernelEv = 0;
	count--;

	unlock
}


void KernelEv::wait() {
	lock

	if(owner == running) {
		owner->state = PCB::BLOCKED;
		unlock
		dispatch();
	} else {
		unlock
	}
	return;
}


void KernelEv::signal() {
	lock

	if(owner->state == PCB::BLOCKED) {
		owner->state = PCB::READY;
		Scheduler::put(owner);
	}

	unlock
	return;
}
