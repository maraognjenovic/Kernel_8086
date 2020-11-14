#include "ListPCB.h"
#include "System.h"
#include "PCB.h"
#include "SCHEDULE.h"
PCBlist::PCBlist() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
 first = last= 0;
 len = 0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
PCBlist::~PCBlist() {
 PCBlist::deletelist();
}
void PCBlist::deletelist(){
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
int PCBlist::size() const {return len;}

PCB* PCBlist::getById(ID id) {
	if(first==0 ) return 0;
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
		Elem* temp=first;
		while(temp!=0){
			if(temp->pcb->getThrId()==id){
				#ifndef BCC_BLOCK_IGNORE
					 unlock();
				#endif
				return temp->pcb;
			}
			temp=temp->next;
		}
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
	 return 0;
}
PCB* PCBlist::removeById(ID id){
	if( first == 0 ) return 0;
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	Elem* temp=first,*prev=0,*old;
	PCB* value;

		while(temp){
		if(temp->pcb->getThrId()==id){
			old=temp;
			value=temp->pcb;

			if(prev==0){//znaci da je trazeni element prvi u listi
				first=temp->next;
			}
			else{
				prev->next=temp->next;
				if(last==old)last=prev;
			}
			temp=temp->next;
			old->next=0;
			delete old;
			len--;
			#ifndef BCC_BLOCK_IGNORE
				 unlock();
			#endif
			return value;
		}else{
		prev=temp;
		temp=temp->next;
		}
	}
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
	 return 0;
}
void PCBlist::put(PCB* pcb) {
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
