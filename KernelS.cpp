#include "KernelS.h"
#include "System.h"
#include "PCB.h"
#include "Queue.h"
#include "QueueS.h"
#include "ListSem.h"
#include "Thread.h"
#include "SCHEDULE.h"

Semlist KernelSem::sems;

KernelSem:: KernelSem(int init){
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
 value=init;
 sems.put(this);
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
}

int KernelSem::val() const{
int retVal=0;
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
 retVal=value;
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
 return retVal;}


void KernelSem::block(PCB *p,Time t){
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
	p->setState(PCB::BLOCKED);
	waiting.put(p,t);
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
}
int KernelSem::wait(Time maxTimeToWait){
	int pom = 0;
#ifndef BCC_BLOCK_IGNORE
 lock();
#endif
	//syncPrintf("POZVAO SE WAIT\n");
 if (--value < 0){
	if (maxTimeToWait == 0){
		PCB::running->passTime = 1;
		pom = 1;
		block(PCB::running,maxTimeToWait);
		#ifndef BCC_BLOCK_IGNORE
		 unlock();
		#endif
		dispatch();
	}
	else{
		PCB::running->passTime = 0;
		pom = 0;
		block(PCB::running,maxTimeToWait);
		PCB::running->setTime(maxTimeToWait);
		#ifndef BCC_BLOCK_IGNORE
		 unlock();
		#endif
		dispatch();
		if(PCB::running->passTime == 1)
			pom = 1;
	}
	return pom;
}
#ifndef BCC_BLOCK_IGNORE
 unlock();
#endif
 	return 1;
}
void KernelSem::putInSch(PCB* p){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	p->passTime = 1;
	p->setState(PCB::READY);
	Scheduler::put(p);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
int KernelSem::signal(int n){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
if( n >=0 ){
	if(n==0){
		if(value++<0){
			PCB* take = waiting.get();
			//if(take->passTime == 0)
				putInSch(take);
			}
		}
else{
	int deblock = 0;
	value += n;
	if(waiting.size()> n)
		deblock = n;
	else
		deblock = waiting.size();

	for(int i = 0; i < deblock; i++){
		PCB* take = waiting.get();
		//if(take->passTime == 0)
			putInSch(take);
	}
	#ifndef BCC_BLOCK_IGNORE
		unlock();
	#endif
	return deblock;
}
}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return n;
}
void KernelSem::addToValue(int k){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	value+=k;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
void KernelSem::updateSem(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	for(Semlist::Elem* curr=sems.getFirst();curr;curr=curr->next) {
	  	int k=curr->sem->waiting.decTime();
	  	curr->sem->addToValue(k);
	}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
