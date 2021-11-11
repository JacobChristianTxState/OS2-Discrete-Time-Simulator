#include "Event.h"

Event::Event(unsigned long t, int ty, Process* process){
  time = t;
  eType = ty;
    this->eventProcess = process;
}

int Event::getType(){
  return eType;
}

unsigned long Event::getTime(){
  return time;
}

void Event::setProcess(Process* process) {
    this->eventProcess = process;
}

Process* Event::getProcess() {
    return this->eventProcess;
}


