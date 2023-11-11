#ifndef IDLE_H_
#define IDLE_H_

#include <iostream.h>
#include "thread.h"
#include "system.h"
#include "pcb.h"

class Idle : public Thread {
	friend class PCB;
public:
	Idle();

	PCB* getPCB() const;
	virtual void run();
	void start();
};

#endif
