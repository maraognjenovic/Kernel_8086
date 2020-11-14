#include "Kernel.h"

 int main(int argc, char *args[]){
 System::inic();

 System::createAll();

 	 //IdleThread* idle=IdleThread::getIdle();
 	// idle->start();
 	//Thread *mainThread = PCB::getmainThread();

 int returnValue = userMain(argc, args);

 //syncPrintf("after usermain\n");

 	//delete idle;
 	//delete mainThread;

 System::deleteAll();
// syncPrintf("after usermain2\n");
 System::restore();
 //syncPrintf("after usermain3\n");

 return returnValue;
//return 0;

 }
