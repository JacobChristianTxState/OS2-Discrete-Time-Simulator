#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st) : arrivalTime(at), serviceTime(st) {}

void Driver::scheduleEvent(int eventType, float time) {
  if (eventType == eventTypeEnums::DEP) {
    stats.incrementTurnaroundTime(time - currentlyRunningProcess->getArrivalTime());
  }
  Event event(time, eventType);
  eventQueue.push_back(event);
  std::sort(
    eventQueue.begin(), 
    eventQueue.end(),
    [](Event& e1, Event& e2) -> bool {return e1.getTime() < e2.getTime();});
}

void Driver::init() {
  this->clock = 0.0;
  this->currentlyRunningProcess = nullptr;
  float initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  this->totalArrivals++;
  scheduleEvent(eventTypeEnums::ARR, initialArrivalTime);
  stats.incrementWorkTime(initialArrivalTime);
}

void Driver::run() {
  this->logger.OpenFile();
  while(this->totalProcesses < PROCESSCOUNT || eventQueue.size() > 0){
    Event e = eventQueue[0];
    this->clock = e.getTime();
    stats.incrementClock(getClock());
    switch(e.getType()) {
      case eventTypeEnums::ARR:
        arriveHandler(e);
        break;
      case eventTypeEnums::DEP: 
        departureHander(e);
        break;
    }
    eventQueue.erase(eventQueue.begin());
  }
  std::stringstream ss;
  ss << std::to_string((int)arrivalLambda) << "," 
     << std::to_string(stats.getThroughput()) << "," 
     << std::to_string(stats.getAverageTurnaroundTime()) << ","
     << std::to_string(stats.getCpuUitlization());
  this->logger.WriteToFile(ss.str());
  this->logger.CloseFile();
}

void Driver::arriveHandler(Event e) {
  float nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  float nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  Process* newProcess = new Process(++this->totalProcesses, this->clock + nextArrivalTime, nextServiceTime);
  if(!currentlyRunningProcess) {
    currentlyRunningProcess = newProcess;
    currentlyRunningProcess->setArrivalTime(this->clock);
    float currentServiceTime = currentlyRunningProcess->getServiceTime();
    scheduleEvent(eventTypeEnums::DEP, this->clock + currentServiceTime);
    this->totalDepartures++;
  } else {
    processReadyQueue.push_back(newProcess);
  }

  if(this->totalProcesses < PROCESSCOUNT) {
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalArrivals++;
  } 
}

void Driver::departureHander(Event e) {
  if (processReadyQueue.empty()) {
    Process* completedProcess(this->currentlyRunningProcess);
    this->currentlyRunningProcess = nullptr;
    completedProcess->setCompletionTime(this->clock);
    stats.incrementWorkTime(completedProcess->getServiceTime());
    stats.incrementTurnaroundTime(completedProcess->getCompletionTime() - completedProcess->getArrivalTime());
    //this->completionList.push_back(completedProcess);
    delete completedProcess;

  } else {
    currentlyRunningProcess = processReadyQueue[0];
    processReadyQueue.erase(processReadyQueue.begin());
    float nextServiceTime = std::round(currentlyRunningProcess->getServiceTime());
    scheduleEvent(eventTypeEnums::DEP, this->clock + nextServiceTime);
    stats.incrementWorkTime(nextServiceTime);
    this->totalDepartures++;
  }
}

float Driver::getClock() {
  return this->clock;
}

