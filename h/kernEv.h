#ifndef KERNEV_H_
#define KERNEV_H_

#include "event.h"

class PCB;

class KernelEv {
public:

	static int count;

	KernelEv(unsigned ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:

	int value;
	unsigned ivtno;

	PCB* owner;
	PCB* blocked;

};

#endif
