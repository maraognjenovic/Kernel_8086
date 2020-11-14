#include "QueueS.h"

#include "ListPCB.h"
#include "System.h"
#include "PCB.h"
#include "schedule.h"

SemQueue::SemQueue() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
 first = 0;
 last = 0;
 n = 0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
void SemQueue::put(PCB *p,Time time){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	Elem *newone=new Elem(p,time);
	last = (first == 0 ? first : last->next) = newone;
	n++;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
SemQueue::Elem* SemQueue::getFirst(){
	return first;
}
PCB* SemQueue::get(){
	if( first==0) return 0;
	#ifndef BCC_BLOCK_IGNORE
		 lock();
	#endif
	Elem* temp;
	PCB* tempPCB=0;

	if(first!=0){
		 n--;
		 temp= first;
		 tempPCB=temp->pcb;
		 first = first->next;

		 if(!first) last = 0;


		 temp->next=0;
		 delete temp;
	}
	#ifndef BCC_BLOCK_IGNORE
		 unlock();
	#endif
	 return tempPCB;
}

void SemQueue::putInSch(Elem* e){
	e->pcb->passTime = 0; //ako jeste passTime stavi na 0
	e->pcb->setState(PCB::READY); //oznaci da je nit spremna
	Scheduler::put(e->pcb);// i stavi je u skedzuler
}

int SemQueue::size() const {return n;}

int SemQueue::decTime(){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	Elem *curr = first, *prev = 0, *old = 0;
	int cnt = 0;
	while(curr){
		if(--curr->myTimeToWait==0){
			putInSch(curr);
			old = curr;
			if(!prev)
				first = curr->next;
			else
				prev->next=curr->next;

			if(last==old) last = prev;
			n--;
			curr=curr->next;
			old->next=0;
			delete old;
			cnt++;
			break;
		}else{
			prev=curr;
			curr=curr->next;
			prev->next = curr;
			}

		}
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
	return cnt;
}

SemQueue::~SemQueue() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
 while(first){
	 Elem* old=first;
	 first=first->next;
	 old->next=0;
	 n--;
	 delete old;
 }
 first=last=0;
 n=0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif

}


