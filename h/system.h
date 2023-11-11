#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <iostream.h>
#include <DOS.H>

#include "pcb.h"
#include "idle.h"
#include "queue.h"
#include "semList.h"

#define lock asm cli
#define unlock  asm sti

typedef void interrupt (*pInterrupt)(...);

class Idle;

class System {
public:
	static int isTimer;

	static pInterrupt oldISR;

	static void inic();
	static void restore();
	static void dispatchS();

	System();
	~System();
};


extern PCB* mainPCB;
extern Thread* mainThread;
extern PCB* running;
extern Idle* idleThread;


void interrupt timer(...);

#endif
