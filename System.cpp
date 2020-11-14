#include "System.h"
#include "PCB.h"
#include "Schedule.h"
#include "KernelS.h"
#include <stdio.h>
#include <stdarg.h>
#include <dos.h>
static unsigned tsp= 0;
static unsigned tss= 0;
volatile int System::contextOnDemand=0;
volatile int System::softLock=1;
InterruptRoutine System::old = 0;
static int counter = defaultTimeSlice;
IdleThread* System::idle=0;
Thread* System::starting=0;
unsigned oldTimerOFF, oldTimerSEG;
int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
		return res;
}
void interrupt timer(){
	if(!System::contextOnDemand){
		tick();
		KernelSem::updateSem();
		#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
		#endif
		if(PCB::running->getTime()){ counter--;}
	}
	if(!System::contextOnDemand && (counter || !PCB::running->getTime())){
		return;
	}
	else {	if(System::softLock){
		System::contextOnDemand=0;
		#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tss,ss
			mov tsp,sp

			}
		#endif
		PCB::running->sp=tsp;
		PCB::running->ss=tss;

		if(!PCB::running) syncPrintf("Error running=0...\n");
		if(PCB::running->getState()==PCB::RUNNING && PCB::running!=PCB::getIdle() && PCB::running->getState()!=PCB::BLOCKED){
			PCB::running->setState(PCB::READY);
			Scheduler::put(PCB::running);
		}

		PCB::running=Scheduler::get();
		if(!PCB::running ) PCB::running=PCB::getIdle();

		tsp=PCB::running->sp;
		tss=PCB::running->ss;
		PCB::running->setState(PCB::RUNNING);
		counter=PCB::running->getTime();

		#ifndef BCC_BLOCK_IGNORE
			asm {
				mov ss, tss
				mov sp, tsp
			}
		#endif
	}
	else System::contextOnDemand = 1;
	}
}

void System::inic(){
#ifndef BCC_BLOCK_IGNORE
	//lock();
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}

	/*old = getvect(0x8);
	setvect(0x8,&timer);
	setvect(0x60, old);*/

//unlock();
#endif

}
void System::createAll(){
	 //idle=IdleThread::getIdle();
	idle=new IdleThread();
	idle->start();
	starting = PCB::getmainThread();
	starting->getMyPCB()->setState(PCB::RUNNING);
	PCB::running = starting->getMyPCB();
}
void System::restore(){
#ifndef BCC_BLOCK_IGNORE

//lock();
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
//setvect(0x8, old);

//unlock();
#endif
}
void System::dispatch() {
#ifndef BCC_BLOCK_IGNORE
	 lock();
#endif
	 System::contextOnDemand=1;
	timer();
	// System::timer();
	//System::contextOnDemand=0;
#ifndef BCC_BLOCK_IGNORE
	 unlock();
#endif
}
void System::deleteAll(){
	delete idle;
	delete starting;
	System::softLock = 0;
	//PCB::deletePointers();
}
IdleThread* System::getIdle(){
	if(idle==0) idle=new IdleThread();
	return idle;
}
