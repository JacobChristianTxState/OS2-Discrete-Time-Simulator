#include "Event.h"

Event::Event(unsigned long t, int ty, Process* p) : time(t), eType(ty), process(p) {}

int Event::getType(){
  return eType;
}

unsigned long Event::getTime(){
  return time;
}

void Event::setProcess(Process* p) {
  this->process = p;
}

Process* Event::getProcess() {
  return this->process;
}



