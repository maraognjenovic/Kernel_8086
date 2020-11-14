#ifndef PREENTRY_H_
#define PREENTRY_H_
#include "IvtEntry.h"


#define PREPAREENTRY(num, flag)\
void interrupt intr##num(...);\
IVTEntry newEntry##num(num, intr##num);\
void interrupt intr##num(...){\
	(IVTEntry::table[##num])->signal();\
	if (flag)\
		newEntry##num.callOldRoutine();\
	dispatch();\
}

/*
#define PREPAREENTRY(num, flag)\
void interrupt intr##num(...);\
IVTEntry entry##num(num, intr##num);\
void interrupt intr##num(...) {\
entry##num.signal();\
	if (flag) entry##num.callOldRoutine();\
}\
*/
#endif
