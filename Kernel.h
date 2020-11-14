#ifndef KERNEL_H_
#define KERNEL_H_

#include<dos.h>
#include<iostream.h>

#include "Event.h"
#include "Idle.h"
#include "IvtEntry.h"
#include "KernelE.h"
#include "KernelS.h"
#include "ListPCB.h"
#include "PCB.h"
#include "Queue.h"
#include "SCHEDULE.h"
#include "semaphor.h"
#include "ListSem.h"
#include "System.h"
#include "Thread.h"

extern int userMain(int argc, char **args);

#endif

