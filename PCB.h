#ifndef PCB_H_
#define PCB_H_
#include "ListPCB.h"
#include "Thread.h"
#include "Queue.h"
class Thread;
class PCBlist;
class Queue;
typedef unsigned int Time; // time, x 55ms
typedef unsigned long StackSize;
const StackSize smallStackSize=64;
class PCB{

public:
	enum state{BABY,READY,BLOCKED,RUNNING,OVER};
	PCB(Thread* myThr,StackSize stackSize, Time timeSlice);
	~PCB();
	volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;

	volatile int passTime;
	void start(); //treba da uzme pcb niti i da stavi u scheduler i menja stanje niti u ready
	void waitToComplete();
	ID getThrId();
	Time getTime();
	void setTime(Time);
	void decTime();
	void createStack();
	static void deletePointers();
	Thread* getThread();
	void setState(state s);
	state getState();
	static PCB* getIdle();
	static Thread* getThreadById(ID idd);
	static Thread* getmainThread();
	static PCB* running;

	static void wrapper();
protected: friend class System;
private:
	//static Thread* mainThread;//pocetna nit koja se zavrsava kada se zavrsi sistem
	//static IdleThread* idle;
	static PCB* idle;
	static ID ukID;
	ID myThrID;
	Thread* myThread;
	state myThrState;
	Time myTime; //koje je vreme proteklo od stvaranja niti
	StackSize myStackSize;
	Queue blocked; //red blokiranih pcb-ova koji cekaju da se zavrsi
	static PCBlist PCBlst; //lista svih PCB-ova
	static Thread* starting;
	static PCB* mainPCB;
	unsigned int*stack;

};




#endif /* PCB_H_ */
