#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "PCB.h"
#include "IvtEntry.h"
#include "Event.h"

class KernelEv {

public:
	KernelEv(IVTnum);
	virtual ~KernelEv();
	void setHandler(PCB*);
	void setVal(int);
	int getVal();
	PCB* getHandler();
	void signal();
	void wait();
	void putInSh(PCB*);
	void block(PCB*);
	static void setEntryInTable(int, IVTEntry*);

private:
	IVTEntry *entry;
	PCB *handler;
	int val;
};

#endif
