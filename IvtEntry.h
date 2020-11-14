#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef unsigned char IVTnum;
class KernelEv;

class IVTEntry {

public:
	IVTEntry(int, void interrupt (*newR)(...));
	virtual ~IVTEntry();
	void callOldRoutine();
	void signal();
	void inicVect();
	void restoreVect();
	void setEvent(KernelEv*);
	KernelEv* getEvent();
	IVTEntry *&operator[](int i);
	static IVTEntry* table[256];

private:
	////	IVTEntry::IVTable[##numEntry]->callOldRoutine();
	friend class KernelEv;
	void interrupt (*oldRoutine)(...);
	void interrupt(*newRoutine)(...);
	int entryId;
	KernelEv *kernelEvent;


};
#endif
