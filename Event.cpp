#include "Event.h"

Event::Event(float t, bool ty){
  time = t;
  eType = ty;
  this->currentProcess = nullptr;
}

int Event::getType(){
  return eType;
}

float Event::getTime(){
  return time;
}

Process* Event::getProcess() {
  return this->currentProcess;
}

void Event::setProcess(Process* process) {
  this->currentProcess = process;
}

