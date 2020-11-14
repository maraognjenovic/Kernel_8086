#include "Thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
//#include "Timer.h"
#include "System.h"

Thread:: Thread(StackSize stackSize, Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	myPCB=new PCB(this,stackSize/2,timeSlice);

#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}

void Thread::start(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 if(myPCB->getState()==PCB::BABY){
	 myPCB->setState(PCB::READY);
	 myPCB->createStack();
	//if(myPCB!=myPCB->getIdle())
	 Scheduler::put(myPCB);
	 }
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
PCB* Thread::getIdle(){
	return myPCB->getIdle();
}
void Thread::waitToComplete(){myPCB->waitToComplete();}

Thread:: ~Thread(){	//mora biti pozvan waitToComplete
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	//waitToComplete();
	delete myPCB;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}

ID Thread::getId(){return myPCB->getThrId();}

ID Thread::getRunningId(){return PCB::running->getThrId(); }

Thread* Thread::getThreadById(ID id){return PCB::getThreadById(id);}

PCB* Thread::getMyPCB(){return myPCB;}

void dispatch() {System::dispatch();}
