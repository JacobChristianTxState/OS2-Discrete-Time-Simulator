#ifndef EVENT_H
#define EVENT_H

#include "Process.h"

class Event{
  private:
    unsigned long time;
    int eType;
    Process* eventProcess;
  public:
    Event(unsigned long, int, Process*);
    int getType();
    unsigned long getTime();
    void setProcess(Process* process);
    Process* getProcess();
};

#endif
