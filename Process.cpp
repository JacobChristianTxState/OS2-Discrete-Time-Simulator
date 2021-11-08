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

float Process::getCompletionTime() {
  return this->completionTime;
}
void Process::setCompletionTime(float completionTime) {
  this->completionTime = completionTime;
}

void Process::setArrivalTime(float arrivalTime) {
  this->arrivalTime = arrivalTime;
}