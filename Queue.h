#ifndef QUEUE_H_
#define QUEUE_H_

class PCB;

class Queue {
	struct Elem {
	 PCB* pcb;
	 Elem* next;
	 Elem(PCB* p) :pcb(p), next(0) {}
	};
	Queue(const Queue&){first = 0; last = 0; len = 0;}
	void operator = (const Queue &){}
public:
Queue();
~Queue();
int size() const;
void put(PCB* pcb);
PCB* get();
void putInSch();//sve procese u redu cekanja proglasi da su spremni i stavi ih u schedule4r
private:
int len;
Elem* first,*last;

};

#endif
