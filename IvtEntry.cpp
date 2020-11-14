#include <dos.h>
#include "System.h"
#include "KernelE.h"
#include "IvtEntry.h"
IVTEntry* IVTEntry::table[256];
IVTEntry::IVTEntry(int i, void interrupt (*newR)(...)){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	kernelEvent=0;
	oldRoutine=0;
	int k=0;
	while(k<256){
		IVTEntry::table[k] = 0;
		k++;
	}
	entryId = i;
	 KernelEv::setEntryInTable(entryId,this);
	 newRoutine=newR;
	 inicVect();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
IVTEntry*& IVTEntry::operator [](int i){return IVTEntry::table[i];}

void IVTEntry::inicVect(){
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(entryId);
	setvect(entryId,newRoutine);
#endif
}
void IVTEntry::restoreVect(){
#ifndef BCC_BLOCK_IGNORE
	 KernelEv::setEntryInTable(entryId,0);
	setvect(entryId,oldRoutine);

#endif
}
void IVTEntry::setEvent(KernelEv* ev){
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	kernelEvent = ev;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}
KernelEv* IVTEntry::getEvent(){return kernelEvent;}

void IVTEntry::signal(){
	if(kernelEvent==0)return;
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	kernelEvent->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

void IVTEntry::callOldRoutine(){if(oldRoutine) (*oldRoutine)();}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock();
#endif
	(*oldRoutine)();
	restoreVect();
	delete IVTEntry::table;
#ifndef BCC_BLOCK_IGNORE
	unlock();
#endif
}

