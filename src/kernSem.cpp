#include <iostream.h>
#include "kernSem.h"
#include "system.h"
#include "pcb.h"
#include "semList.h"
#include "queue.h"
#include "SCHEDULE.H"


KernelSem::KernelSem(int init) {
	lock
	val = init;

	if(val < 0) {
		val = 0;
	}

	blocked = new Queue();
	waitBlocked = new Queue();

	allSem.insert(this);
	unlock
}


KernelSem::~KernelSem() {
	lock
	delete blocked;
	delete waitBlocked;
	allSem.remove(this);
	unlock
}


int KernelSem::wait(Time time) {
	lock
	running->semCheck = 0;

	val--;

	if(val < 0) {

		if(time != 0) {
			running->state = PCB::BLOCKED;
			waitBlocked->insert((PCB*)running);
			running->timeCount = time;
		} else {
			running->state = PCB::BLOCKED;
			blocked->insert((PCB*)running);
		}

		dispatch();

		if(running->semCheck == 1) {
			running->semCheck = 0;
			unlock
			return 0;
		}
	}

	unlock
	return 1;
}


int KernelSem::signal(int n) {
	lock

	if(n >= 0) {
		int count = 0;
		int num;
		if(n != 0) {
			num = n;
		} else {
			num = 1;
		}

		if(val < 0) {
			val = val + num;

			for(int i = 0; i < num && !blocked->empty(); i++) {
				PCB* temp = blocked->remove();
				temp->state = PCB::READY;
				Scheduler::put(temp);
				count++;
			}

			for(int j = count; j < num && !waitBlocked->empty(); j++) {
				PCB* temp = waitBlocked->remove();
				temp->state = PCB::READY;
				Scheduler::put(temp);
				count++;
			}

			unlock
			return count;
		}

		val += num;
		unlock
		return 0;
	}

	unlock
	return n;
}


int KernelSem::value() const {
	return val;
}


void KernelSem::incVal(){
	val = val + 1;
}


void KernelSem::updateHelp() {
	Elem *temp = waitBlocked->first;
	Elem *temp1 = 0, *temp2 = 0;

	while (temp) {
		temp->pcb->timeCount--;

		if (temp->pcb->timeCount > 0) {

			temp1 = temp;
			temp = temp->next;

		} else {

			temp->pcb->state = PCB::READY;
			temp->pcb->semCheck = 1;
			Scheduler::put(temp->pcb);

			temp2 = temp;
			temp = temp->next;

			if (!temp1)
				KernelSem::waitBlocked->first = temp;
			else
				temp1->next = temp;
			delete temp2;
			incVal();
		}
	}
}

