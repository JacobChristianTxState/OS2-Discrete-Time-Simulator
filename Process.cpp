#include "Process.h"

Process::Process(int processID, float arrivalTime, float serviceTime) {
  this->processID = processID;
  this->arrivalTime = arrivalTime;
  this->serviceTime = serviceTime;
}

int Process::getID(){
  return this->processID;
}

float Process::getArrivalTime(){
  return this->arrivalTime;
}

float Process::getServiceTime(){
  return this->serviceTime;
}

float Process::getRemainingServiceTime(){
  return this->remainingServiceTime;
}

void Process::setRemainingServiceTime(float timeSlice){
    this->remainingServiceTime -= timeSlice;
}

// //Constructor takes an id, an arrivalTime, a serviceTime, and a priority
// Process::Process(int id, float arrivalTime, float serviceTime, int priority){
//   this->id = id;
//   this->serviceTime = serviceTime;
//   this->remainingServiceTime = serviceTime;
//   this->arrivalTime = arrivalTime;
//   this->priority = priority;
// }

// Process::Process(const Process &p){
//   this->id = p.id;
//   this->arrivalTime = p.arrivalTime;
//   this->serviceTime = p.serviceTime;
//   this->remainingServiceTime = p.remainingServiceTime;
//   this->priority = p.priority;
// }

// int Process::getID(){
//   return id;
// }

// float Process::getArrivalTime(){
//   return arrivalTime;
// }

// float Process::getServiceTime(){
//   return serviceTime;
// }

// float Process::getRemainingServiceTime(){
//   return remainingServiceTime;
// }

// int Process::getPriority(){
//   return priority;
// }

// void Process::setRemainingServiceTime(float quantum){
//   remainingServiceTime = getRemainingServiceTime() - quantum;
// }

// void Process::setArrivalTime(float time){
//   arrivalTime = time;
// }

// void Process::setPriority(int priority){
//   this->priority = priority;
// }
