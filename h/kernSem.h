#ifndef KERNSEM_H_
#define KERNSEM_H_

#include "queue.h"
#include "pcb.h"
#include "semList.h"

typedef unsigned int Time;


class KernelSem {
public:

	KernelSem(int init = 1);
	virtual ~KernelSem();

	int wait (Time time);
	int signal(int n = 0);

	int value () const;
	void incVal();

	void updateHelp();

private:

	Queue* blocked;
	Queue* waitBlocked;
	int val;

};




#endif
