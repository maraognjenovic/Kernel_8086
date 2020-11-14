#ifndef IDLE_H_
#define IDLE_H_
#include "Thread.h"
class Thread;
class IdleThread : public Thread {
public:
 IdleThread();
 virtual void run();
void start();
};

#endif
