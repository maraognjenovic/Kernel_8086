#include "PCB.h"
//#include "Timer.h"
#include "Schedule.h"
#include <dos.h>
#include "System.h"
class System;
PCB* PCB::idle = 0;
PCB* PCB::mainPCB=0;
PCB* PCB::running = 0;
Thread* PCB::starting = 0;
ID PCB::ukID=-2;
PCBlist PCB::PCBlst;
PCB::PCB(Thread* myThr,StackSize stackSize, Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 myThrState=BABY;
	 myThread = myThr;
	 stack=0;
	 myTime=timeSlice;
	 passTime = 1;
	 if(stackSize>defaultStackSize || stackSize<smallStackSize){
		 if(stackSize>defaultStackSize)
		 myStackSize=defaultStackSize;
		 else
		myStackSize=smallStackSize;
	 }
	 else
	 myStackSize = stackSize;

	 myThrID=++ukID;
	 if(myThrID==-1) {idle = this;}
	 if(myThrID==0) {
		 	 mainPCB=this;
			starting=this->myThread;
			}
	if(myThrID>0)PCBlst.put(this);

#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
PCB::~PCB(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 //waitToComplete();
	PCBlst.removeById(myThrID);
	 delete stack;

#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
void PCB::deletePointers(){
	delete idle;
	delete mainPCB;
	delete starting;

}
void PCB::start(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	myThread->start();
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
void PCB::decTime(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 myTime--;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
void PCB::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
//running ne ceka novu nit,samu sebe,gotovu nit,pocetnu i besposlenu nit running != idle &&

	if(myThrState!=OVER && myThrState!=BABY  && running!=this && this!=idle && this!=mainPCB){
	//if(myThrState!=OVER && myThrState!=BABY && running!=this && this!= idle && this!=mainPCB && myThread!=starting && this!=System::idle->getMyPCB()){
	 	 	 	 running->setState(BLOCKED);
				blocked.put(PCB::running);
				//syncPrintf("Thread %d: loop2 ends\n",getId());
				 //syncPrintf("	waittocomplete %d\n",this->myThrID);
				#ifndef BCC_BLOCK_IGNORE
					 unlock();
				#endif
					 dispatch();
					 return;
			}else{
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
	 return;
			}
}
ID PCB::getThrId(){return myThrID;}

Time PCB::getTime(){return myTime;}

void PCB::setTime(Time t){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 this->myTime=t;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
void PCB::createStack(){
#ifndef BCC_BLOCK_IGNORE
	 lock();

	 stack = new unsigned [myStackSize];
	 stack[myStackSize-1] =0x200;
		stack[myStackSize-2] = FP_SEG(wrapper);
		stack[myStackSize-3] = FP_OFF(wrapper);

	 sp = FP_OFF(stack+myStackSize-12);
	 ss = FP_SEG(stack+myStackSize-12);
	 //bp = FP_OFF(stack+myStackSize-12);

	 unlock();
#endif
}

Thread* PCB::getThread(){return myThread;}

void PCB::setState(state s){
	#ifndef BCC_BLOCK_IGNORE
	 lock();
	 myThrState=s;
	 unlock();
	#endif
	 }

PCB::state PCB::getState(){return myThrState;}

PCB* PCB::getIdle(){return idle;}

Thread* PCB:: getThreadById(ID idd){
	if(idd>0){
	PCB* pcb = PCB::PCBlst.getById(idd);
	if(pcb) return pcb->myThread;}
	else {
		if(idd == -1) return idle->getThread();
		if(idd == 0) return starting;
	}
	return 0;
}

Thread* PCB::getmainThread(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	if(!starting){
	starting=new Thread();
	}
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
return starting;
}

void PCB::wrapper(){
	running->myThread->run();
	#ifndef BCC_BLOCK_IGNORE
		 lock();
	#endif
		  //kada proces running zavrsi sve procese koji su bili blokirani na njemu, stavi u scheduler
		 running->myThrState=OVER;
		 running->blocked.putInSch();
	#ifndef BCC_BLOCK_IGNORE
		 unlock();
	#endif
		 dispatch();
}
