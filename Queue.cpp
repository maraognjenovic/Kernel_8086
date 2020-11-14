#include "Queue.h"
#include "System.h"
#include "PCB.h"
#include "SCHEDULE.H"
Queue::Queue() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
 first = 0;
 last = 0;
 len = 0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
Queue::~Queue() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 while(first){
		 Elem* old=first;
		 first=first->next;
		 old->next=0;
		 len--;
		 delete old;
	 }
	 first=last=0;
	 len=0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
int Queue::size() const {
return len;
}
PCB* Queue::get() {
if( first==0) return 0;
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 Elem* temp;
	 PCB* tempPCB;
if(first!=0){
 len--;
 temp= first;
 tempPCB=temp->pcb;
 first = first->next;
 temp->next=0;
 delete temp;
}
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
 return tempPCB;


}
void Queue::put(PCB* pcb){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	Elem *newone=new Elem(pcb);
	last = (first == 0 ? first : last->next) = newone;
	len++;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}

void Queue::putInSch(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
for(Elem* tmp=first;first!=0;){
	first->pcb->setState(PCB::READY);
	Scheduler::put(first->pcb);
	first=first->next;
	tmp->next=0;
	len--;
	delete tmp;
}
	first=last=0;
	len=0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
