#include "Process.h"

Process::Process(int id, float arrivalTime, float serviceTime, int priority){
  this->id = id;
  this->arrivalTime = arrivalTime;
  this->serviceTime = serviceTime;
  this->remainingServiceTime = serviceTime;
  this->priority = priority;
}

int Process::getID(){
  return id;
}

float Process::getArrivalTime(){
  return arrivalTime;
}

float Process::getServiceTime(){
  return serviceTime;
}

float Process::getRemainingServiceTime(){
  return remainingServiceTime;
}

int Process::getPriority(){
  return priority;
}

void Process::setRemainingServiceTime(float quantum){
  remainingServiceTime = getServiceTime() - quantum;
}

void Process::setArrivalTime(float time){
  arrivalTime = time;
}

void Process::setPriority(int priority){
  this->priority = priority;
}
