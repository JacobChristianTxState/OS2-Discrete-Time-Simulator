#ifndef EVENT_H
#define EVENT_H

#include "Process.h"

class Event{
  private:
    unsigned long time;
    int eType;
    Process* process;
  public:
    Event(unsigned long, int, Process* p);
    int getType();
    unsigned long getTime();
    void setProcess(Process* p);
    Process* getProcess();
};

#endif
