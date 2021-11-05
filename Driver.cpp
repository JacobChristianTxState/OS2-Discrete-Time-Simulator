#include "Driver.h"
#include <iostream>

Driver::Driver(float lambda, int a_id, float quantum):serviceTime(lambda), arrivalTime(lambda){
  algorithm_id = a_id;
  serveridle = true;
  this->quantum = quantum;
  this -> lambda = lambda;
  process_count = 0;
  Process p = Process(0, 0, serviceTime.generateExponentialDist(), 0);
  processReadyQueue.push_back(p);
  clock = 0;
}

void Driver::run(){
  //for(int i = 0; i != 30; i++){
    process_count = 0;
    Event e = Event(arrivalTime.generateExponentialDist(), true);
    eventList.push_back(e);
    while(process_count <= 50){
      handleEvent();
    }
    std::cout << "Process Count: " << process_count << "\n";
  //}
}

void Driver::handleEvent(){
  //handling arrival
  if(eventList[0].getType()){
    std::cout << "Handeling arrival\n";
    handleArr();
  }else{
    std::cout << "Handeling departure\n";
    handleDep();
  }
  this->eventList.erase(eventList.begin());
}

void Driver::handleArr(){
  float arrTime = arrivalTime.generateExponentialDist();
  if(serveridle == true){
    serveridle = false;
    float duh = calcProcessTime();
    Event dep = Event(duh, false);
    eventList.push_back(dep);
  }else{
    Process p = Process(0, arrTime, serviceTime.generateExponentialDist(), 0);
    //scheduleProcess
    scheduleProcess(p);
  }
  Event arr = Event(clock + arrivalTime.generateExponentialDist(), true);
  eventList.push_back(arr);
}

void Driver::handleDep(){
  if(processReadyQueue.empty()){
    serveridle = true;
  }else{
    Event dep = Event(clock + calcProcessTime(), false);
    eventList.push_back(dep);
    processReadyQueue.erase(processReadyQueue.begin());
  }
}

float Driver::calcProcessTime(){
  switch(algorithm_id){
    case 3:
      return 0.0;
      break;
    case 4:
      return 0.0;
      break;
    default:
      this -> process_count++;
      float t = processReadyQueue[0].getRemainingServiceTime();
      processReadyQueue.erase(processReadyQueue.begin());
      return t;
      break;
  }
}

void Driver::scheduleProcess(Process p){
  switch(algorithm_id){
    case 2:
      break;
    default:
      processReadyQueue.push_back(p);
      break;
  }
}
