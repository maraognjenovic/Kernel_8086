#include "Idle.h"
#include "PCB.h"
#include "System.h"
IdleThread::IdleThread(): Thread( defaultStackSize, 1) {}
void IdleThread::run() {
	//syncPrintf("IDLE.\n");
	while(1);}

void IdleThread::start(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 myPCB->setState(PCB::READY);
	 myPCB->createStack();
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}

