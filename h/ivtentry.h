#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "kernEv.h"

typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry {
public:

	IVTEntry(unsigned numEntry, pInterrupt newRoutine);
	~IVTEntry();

	void signal();
	void callOldRoutine();

	static IVTEntry *IVTable[];

private:

	friend class KernelEv;
	KernelEv* kernelEv;

	unsigned numEntry;
	pInterrupt oldRoutine;

};

#endif
