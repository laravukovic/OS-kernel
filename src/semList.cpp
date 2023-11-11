#include "semList.h"
#include "kernSem.h"
#include "system.h"
#include "queue.h"
#include "SCHEDULE.H"

SemList::SemNode* SemList::first = 0;
SemList::SemNode* SemList::last = 0;

SemList allSem;

int SemList::insert(KernelSem* kernelSem) {
	lock
	SemNode* newNode = new SemNode (kernelSem);

	if(SemList::first == 0){
		SemList::first = newNode;
		SemList::last=SemList::first;
	} else {
		SemList::last->next = newNode;
		SemList::last = SemList::last->next;
	}
	unlock
	return 1;
}


int SemList::remove(KernelSem* kernelSem) {
	lock
	SemNode* temp = SemList::first;
	SemNode* prev = 0;


	while(temp != 0 && (temp->kernelSem != kernelSem)) {
		prev = temp;
		temp = temp->next;
	}

	if(temp == 0) {
		unlock
		return 0;
	}

	if(prev == 0){
		SemList::first = temp->next;
	}
	else{
		prev->next = temp->next;
	}

	if(temp->next == 0){
		SemList::last = prev;
	}

	delete temp;
	unlock
	return 1;
}


KernelSem* SemList::getFirst() {
	lock
	if(SemList::first == 0){
		unlock
		return 0;
	}

	SemNode* temp = SemList::first;
	SemList::first = SemList::first->next;
	if(SemList::first == 0) SemList::last = 0;
	KernelSem* kernelSem = temp->kernelSem;

	delete temp;
	unlock
	return kernelSem;
}



SemList::~SemList() {

	while(SemList::first) {
		KernelSem* temp = this->getFirst();
		if(temp == 0) break;
	}

	SemList::first=SemList::last = 0;

}


void SemList::update() {
	SemNode* temp = SemList::first;

	while(temp) {
		temp->kernelSem->updateHelp();
		temp = temp->next;
	}
}

