#ifndef EVENT_H_
#define EVENT_H_
#include "IvtEntry.h"
typedef unsigned char IVTnum;

class KernelEv;

class Event {

public:
	Event (IVTnum i);
	~Event();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};

#include "PreEntry.h"
#endif
