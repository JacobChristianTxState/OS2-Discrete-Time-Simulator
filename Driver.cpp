#include "Driver.h"

Driver::Driver(float lambda1, float lambda2, int a_id, float quantum):serviceTime(1/lambda1, DEP), arrivalTime(lambda2, ARR){
  algorithm_id = a_id;
  serveridle = true;
  this->quantum = quantum;
  this -> lambda = lambda1;
  int pid = 0;
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
  eventList.erase(eventList.begin());
}

void Driver::handleArr(){
  float arrTime = arrivalTime.generateExponentialDist();
  if(serveridle == true){
    serveridle = false;
    float duh = clock + calcProcessTime();
    Event dep = Event(duh, false);
    scheduleEvent(dep);
  }else{
    Process p = Process(pid, clock + arrTime, serviceTime.generateExponentialDist(), 0);
    pid++;
    //scheduleProcess
    scheduleProcess(p);
  }
  Event arr = Event(clock + arrTime, true);
  scheduleEvent(arr);
}

void Driver::handleDep(){
  if(processReadyQueue.empty()){
    serveridle = true;
  }else{
    Event dep = Event(clock + calcProcessTime(), false);
    scheduleEvent(dep);
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

void Driver::scheduleEvent(Event e){
  eventList.push_back(e);
  if(eventList.size() > 1){
    std::sort(eventList.begin(), eventList.end(), [](Event &E1, Event &E2){return E1.getTime()<E2.getTime();});
  }
  printList();
}

void Driver::printList(){
  std::cout << "Print Events\n";
  for (auto i : eventList){
    std::cout << i.getType() << " time: " << i.getTime() << "\n";
  }
  std::cout << "Events done printing\n";
}
