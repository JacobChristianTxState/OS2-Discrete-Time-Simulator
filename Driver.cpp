#include "Driver.h"
#include <iostream>

Driver::Driver(float lambda, int a_id, float quantum):serviceTime(lambda), arrivalTime(lambda){
  algorithm_id = a_id;
  serveridle = true;
  this->quantum = quantum;
  this -> lambda = lambda;
  process_count = 0;
  float aT = arrivalTime.generateExponentialDist();
  Process p = Process(0, aT, serviceTime.generateExponentialDist(), 0);
  processReadyQueue.push_back(p);
  Event e = Event(aT, true);
  eventList.push_back(e);
  clock = 0;
}

void Driver::run(){
  //for(int i = 0; i != 30; i++){
    process_count = 0;
    while(process_count <= 10){
      clock = eventList[0].getTime();
      handleEvent();
    }
  //}
}

void Driver::handleEvent(){
  //handling arrival
  clock = eventList[0].getTime();
  if(eventList[0].getType()){
    std::cout << "Arrival at Time: " << clock << "\n-----------\n";
    handleArr();
  }else{
    std::cout << "Departure at Time: " << clock << "\n-----------\n";
    handleDep();
  }
  this->eventList.erase(eventList.begin());
}

void Driver::handleArr(){
  float arrTime = arrivalTime.generateExponentialDist();
  if(serveridle == true){
    serveridle = false;
    float duh = clock + calcProcessTime();
    Event dep = Event(duh, false);
    eventList.push_back(dep);
  }else{
    Process p = Process(0, clock + arrTime, serviceTime.generateExponentialDist(), 0);
    //scheduleProcess
    scheduleProcess(p);
  }
  Event arr = Event(clock + arrTime, true);
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
