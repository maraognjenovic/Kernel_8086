#include "System.h"
#include "Event.h"
#include "KernelE.h"
#include "IvtEntry.h"

Event::Event(IVTnum i){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl = new KernelEv(i);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
void Event::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
void Event::wait(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
Event::~Event(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	delete myImpl;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
