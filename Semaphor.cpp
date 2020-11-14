#include "Semaphor.h"
#include "KernelS.h"
#include "System.h"

Semaphore::Semaphore(int init) {
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
 myImpl = new KernelSem(init);
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
}
Semaphore::~Semaphore() {
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
 delete myImpl;
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
}

int Semaphore::wait(Time t) {
	int retVal=0;
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
retVal= myImpl->wait(t);
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
return retVal;
}

int Semaphore::signal(int n) {
	int retVal=0;
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
retVal= myImpl->signal(n);
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
return retVal;
}

int Semaphore::val() const {return myImpl->val();}
