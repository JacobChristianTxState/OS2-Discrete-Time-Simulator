#ifndef EVENT_H
#define EVENT_H

#include <list>
#include "Process.h"

class Event{
  private:
    float time;
    int eType;
    std::list<Event> listOfEvents;
    Process* currentProcess;
  public:
    Event(float, int, Process*);
    Event(float, int);
    int getType();
    float getTime();
    Process* getProcess();
    void setProcess(Process*);
};

#endif
