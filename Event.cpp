#include "Event.h"

Event::Event(float t, int ty, Process* currentProcess){
  time = t;
  eType = ty;
  this->currentProcess = currentProcess;
}

Event::Event(float t, int ty){
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

