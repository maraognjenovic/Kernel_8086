#ifndef SYSTEM_H_
#define SYSTEM_H_
#include "Thread.h"
#include "PCB.h"
#include "Idle.h"
class Thread;
class PCB;
void interrupt timer();
extern void tick();
typedef void interrupt (*InterruptRoutine)(...);
#define lock() asm{\
 		pushf;\
 		cli;\
}
#define unlock() asm popf

class System{

public:
	System(){}
	static void _lock();
	static void _unlock();
	static void resetLock();
	static int retsoftLock();
	static void inic();
	static void restore();
	static void createAll();
	static void deleteAll();
	static void dispatch();
	static IdleThread* getIdle();
	//static void interrupt timer(...);
	static InterruptRoutine old;
	static volatile int contextOnDemand;
	static volatile int softLock;

	static Thread* starting;
	static IdleThread* idle;
};

int syncPrintf(const char* fmt, ...);
#endif /* SYSTEM_H_ */
