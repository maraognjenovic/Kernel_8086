#ifndef LISTPCB_H_
#define LISTPCB_H_

typedef int ID;
class PCB;

class PCBlist{

	struct Elem {
	 PCB* pcb;
	 Elem *next;
	 Elem(PCB* p) :pcb(p), next(0) {}
	};

	 Elem* first, *last;
	 int len;
	 void deletelist();
public:
	int size() const;
	void put(PCB* pcb);
	PCB* removeById(ID id); //bice static ako je isti za sve objekte klase
	PCB* getById(ID id);
	PCBlist();
	virtual ~PCBlist();

};

#endif
