#include<stdio.h>
#include "SCHEDULE.h"
#include "System.h"
//#include "Thread.h"
#include "KernelE.h"
//IVTEntry* IVTEntry::table;

KernelEv::KernelEv(IVTnum i){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	val = 0;
	IVTEntry::table[i]->setEvent(this);
	setHandler(PCB::running);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
KernelEv::~KernelEv(){
	entry->kernelEvent=0;
	handler=0;
}
void KernelEv::setEntryInTable(int i,IVTEntry* e){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	IVTEntry::table[i]=e;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
/*void KernelEv::signal(){
	if(val==0){val++; return;}
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(val==1){
	val = 0;
	putInSh(handler);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	}

}*/
void KernelEv::signal(){
	if(val==0)return;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	val = 0;
	putInSh(handler);
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
void KernelEv::putInSh(PCB* pcb){
	pcb->setState(PCB::READY);
	Scheduler::put(pcb);
}
void KernelEv::block(PCB* pcb){
	pcb->setState(PCB::BLOCKED);
}
int KernelEv::getVal(){return val;}

PCB* KernelEv::getHandler(){return handler;}

void KernelEv::setVal(int v){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
		val = v;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

/*void KernelEv::wait(){
if(handler!=PCB::running)return;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	if(val==1){
		val--;
		return;
	}
	if(val==0){
	val--;
	block(handler);
	#ifndef BCC_BLOCK_IGNORE
	unlock();
	#endif
	dispatch();
	}
}*/
void KernelEv::wait(){
if(handler!=PCB::running)return;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
		val = 1;
		block(handler);
		#ifndef BCC_BLOCK_IGNORE
		unlock();
		#endif
		dispatch();
}
void KernelEv::setHandler(PCB* p){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	handler = p;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
