#ifndef KERNLSEM_H_
#define KERNLSEM_H_

#include "QueueS.h"
//#include "ListSem.h"
class PCBlist;
class Semlist;
//class SemQueue;

typedef unsigned int Time;

class KernelSem {
public:

	friend class System;
	friend class PCB;
	friend class Semlist;
	int wait(Time);
	int signal(int n=0);
	int val() const;
	void addToValue(int);
	void putInSch(PCB*);
	void block(PCB*,Time);
	 KernelSem(int init=1);
	static void updateSem();
	void decrease();
	static Semlist sems;

private:
	int value;
	SemQueue waiting;
};
#endif
