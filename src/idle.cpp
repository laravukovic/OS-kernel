#include "idle.h"

PCB* Idle::getPCB() const {
	return myPCB;
}


void Idle::run() {
	while(1) {

	}
}

Idle::Idle(): Thread(defaultStackSize, 1) {}

void Idle::start() {
	lock
	getPCB()->state = PCB::READY;
	getPCB()->createStack();
	unlock
}
