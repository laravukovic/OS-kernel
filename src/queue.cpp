#include <stdlib.h>
#include "queue.h"
#include "system.h"

Queue allPCB;

Queue::Queue() {
	lock
	first = 0;
	unlock
}

Queue::~Queue() {
	lock
	Elem* temp;
	while( first != 0 )	{
		temp = first;
		first = first->next;
		delete temp;
	}
	unlock
}


void Queue::insert(PCB* pcb) {
	lock
	Elem **temp = &first;
	while( (*temp) != 0 )
	{
		if( (*temp)->pcb == pcb ) { unlock return; }
		temp = &((*temp)->next);
	}
	(*temp) = new Elem();
	(*temp)->next = 0;
	(*temp)->pcb = pcb;
	unlock
}


PCB* Queue::remove() {
	lock
	if( first == 0 ) { unlock return 0; }

	Elem* temp = first;
	PCB *ret = temp->pcb;

	first = first->next;

	delete temp;
	unlock
	return ret;
}


int Queue::empty() {
	if (getLength() == 0) return 1;
	else return 0;
}


int Queue::getLength() const {
	return length;
}


PCB* Queue::getbyId(int id) {
	lock

	Elem *temp = first;
	while(temp != 0){
		if (temp->pcb->myThread->getId() == id){
			unlock
			return temp->pcb;
		}
		temp = temp->next;
	}

	unlock
	return 0;
}
