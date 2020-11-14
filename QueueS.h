#ifndef QUEUES_H_
#define QUEUES_H_

class PCB;
typedef unsigned int Time;

class SemQueue {
	struct Elem{
		PCB *pcb;
		Time myTimeToWait;
		Elem *next;
		Elem(PCB* p,Time time):pcb(p),myTimeToWait(time),next(0){}
	};
	Elem *first, *last;
	int n;
public:
	SemQueue();

	void put(PCB*,Time);
	PCB* get();
	void putInSch(Elem*);
	Elem* getFirst();
	int size() const;
	int decTime();
	~SemQueue();
};




#endif
