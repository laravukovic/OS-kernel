#include "semaphor.h"
#include "kernSem.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);
}


int Semaphore::wait(Time maxTimeToWait) {
	int ret = myImpl->wait(maxTimeToWait);
	return ret;
}


int Semaphore::signal(int n) {
	int ret = myImpl->signal(n);
	return ret;
}


int Semaphore::val() const {
	return myImpl->value();
}


Semaphore::~Semaphore() {
	delete myImpl;
}

