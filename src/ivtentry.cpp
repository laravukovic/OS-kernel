#include "event.h"
#include "kernEv.h"
#include "system.h"

IVTEntry * IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(unsigned numEntry, pInterrupt newRoutine) {
	lock

	this->numEntry = numEntry;
	this->kernelEv = 0;

	asm pushf;
	asm cli;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(numEntry);
	setvect(numEntry,newRoutine);
#endif
	asm popf;

	IVTable[this->numEntry] = this;

	unlock
}


IVTEntry::~IVTEntry() {
	asm pushf;
	asm cli;
	oldRoutine();
#ifndef BCC_BLOCK_IGNORE
	setvect(numEntry,oldRoutine);
	IVTable[this->numEntry] = 0;
#endif
	asm popf;
}


void IVTEntry::signal() {
	if(this->kernelEv != 0) {
		kernelEv->signal();
	}
}


void IVTEntry::callOldRoutine() {
	this->oldRoutine();
}
