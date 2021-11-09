#include "Driver.h"

Driver::Driver(float lambda1, float lambda2, int a_id, float quantum):serviceTime(1/lambda1, 2), arrivalTime(lambda2, 1){
  algorithm_id = a_id;
  serveridle = true;
  this->quantum = quantum;
  this -> lambda = lambda1;
  int pid = 0;
  process_count = 0;
  float aT = arrivalTime.generateExponentialDist();
  running_process = nullptr;
  Event e = Event(aT, true);
  eventList.push_back(e);
  clock = 0;
}

void Driver::run(){
  //for(int i = 0; i != 30; i++){
    process_count = 0;
    while(process_count <= 10){
      handleEvent();
  }
//}
}

void Driver::handleEvent(){
//handling arrival
  clock = eventList[0].getTime();
  if(eventList[0].getType()){
    std::cout << "Handling Arrival Clock: " << clock << "\n------\n";
    handleArr();
  }else{
    std::cout << "Handling Departure Clock: " << clock << "\n------\n";
    handleDep();
  }
  eventList.erase(eventList.begin());
}

void Driver::handleArr(){
  float arrTime = arrivalTime.generateExponentialDist();
  Process *p = new Process(pid, clock + arrTime, this->serviceTime.generateExponentialDist(), 0);
  if(running_process == nullptr){
    running_process = p;
    float duh = clock + calcProcessTime();
    Event dep = Event(duh, false);
    scheduleEvent(dep);
  }else{
    pid++;
    scheduleProcess(p);
    //scheduleProcess
  }
  Event arr = Event(clock + arrTime, true);
  scheduleEvent(arr);
}

void Driver::handleDep(){
  if(processReadyQueue.empty()){
    Process *completed_process = running_process;
    delete completed_process;
    running_process = nullptr;
  }else{
    running_process = processReadyQueue[0];
    processReadyQueue.erase(processReadyQueue.begin());
    Event dep = Event(clock + calcProcessTime(), false);
    scheduleEvent(dep);
    //check if we're incremending process count or not
  }
}

float Driver::calcProcessTime(){
  float t = 0.0;
  switch(algorithm_id){
    case 3:
      return 0.0;
      break;
    case 4:
      return 0.0;
      break;
    default:
      this -> process_count++;
      t = running_process->getServiceTime();
      return t;
      break;
  }
  return 0;
}

void Driver::determineProcessComp(){
  if(running_process){
    switch(algorithm_id){
      case 1:
        break;
      case 2:
        break;
      case 3:
        if(running_process->getRemainingServiceTime() - quantum <= 0){
        }else{
          Process *returning_process = running_process;
          returning_process->setRemainingServiceTime(quantum);
          processReadyQueue.push_back(returning_process);
        }
        break;
    }
  }
}

void Driver::scheduleProcess(Process *p){
  switch(algorithm_id){
    case 2:
      break;
    default:
      processReadyQueue.push_back(p);
      break;
  }
}

void Driver::scheduleEvent(Event e){
  eventList.push_back(e);
  if(eventList.size() > 1){
    std::sort(eventList.begin(), eventList.end(), [](Event &E1, Event &E2){return E1.getTime()<E2.getTime();});
  }
  //printList();
}

void Driver::printList(){
  std::cout << "Print Events\n";
  for (auto i : eventList){
    std::cout << i.getType() << " time: " << i.getTime() << "\n";
  }
  std::cout << "Events done printing\n";
}
