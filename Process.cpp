#include "Process.h"

Process::Process(int processID, unsigned long arrivalTime, unsigned long serviceTime) {
  this->processID = processID;
  this->arrivalTime = arrivalTime;
  this->serviceTime = serviceTime;
  this->remainingServiceTime = this->serviceTime;
}

int Process::getID(){
  return this->processID;
}

unsigned long Process::getArrivalTime(){
  return this->arrivalTime;
}

unsigned long Process::getServiceTime(){
  return this->serviceTime;
}

unsigned long Process::getRemainingServiceTime(){
  return this->remainingServiceTime;
}

void Process::setRemainingServiceTime(float timeSlice){
    this->remainingServiceTime = timeSlice;
}

unsigned long Process::getCompletionTime() {
  return this->completionTime;
}
void Process::setCompletionTime(float completionTime) {
  this->completionTime = completionTime;
}

void Process::setArrivalTime(float arrivalTime) {
  this->arrivalTime = arrivalTime;
}

void Process::printProcessInformation() {
  std::cout << "Process information: \n"
  << "\tProcess ID: " << this->getID()
  << "\n\tProcess arrival time: " << this->getArrivalTime()
  << "\n\tProcess service time: " << this->getServiceTime()
  << "\n\tProcess remaining service time: " << this->getRemainingServiceTime()
  << "\n\tProcess completion time: " << this->getCompletionTime() << "\n";
}
