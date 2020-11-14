#ifndef LISTSEM_H_
#define LISTSEM_H_
class KernelSem;
class Semlist{
	struct Elem {
	KernelSem* sem;
	 Elem *next;
	 Elem(KernelSem* s) :sem(s), next(0) {}
	};

public:

Semlist();
~Semlist();
int size() const;
void put(KernelSem* s);
Elem* getFirst();
private:
 Elem* first, *last;
 int len;
 void deletelist();

};

#endif

