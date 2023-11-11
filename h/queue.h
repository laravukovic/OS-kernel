#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "pcb.h"

class PCB;

class Elem {
public:

	PCB* pcb;
	Elem* next;
};

class Queue {
public:
	void insert(PCB* pcb);
	PCB* remove();

	Queue();
	~Queue();

	int getLength() const;
	int empty();
	PCB* getbyId(int id);
	Elem* getFirst();

	Elem* first;
	Elem* last;
	int length;


};

extern Queue allPCB;

#endif
