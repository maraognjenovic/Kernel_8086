#include "ListSem.h"
#include "KernelS.h"
#include "System.h"
#include "SCHEDULE.h"

Semlist::Semlist() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
 first = last= 0;
 len = 0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
Semlist::~Semlist() {
	 Semlist::deletelist();
}
Semlist::Elem* Semlist::getFirst(){
	return first;
}
void Semlist::deletelist(){
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 while(first){
		 Elem* old=first;
		 first=first->next;
		 old->next=0;
		 delete old;
		 len--;
	 }
	first=last=0;
	len=0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
int Semlist::size() const {return len;}

void Semlist::put(KernelSem* s) {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	Elem *newone=new Elem(s);
	last = (first == 0 ? first : last->next) = newone;
	len++;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}

