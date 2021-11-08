#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st) : arrivalTime(at), serviceTime(st) {}

void Driver::scheduleEvent(int eventType, unsigned long time) {
  Event event(time, eventType);
  this->eventCount++;
  eventQueue.push_back(event);
  std::sort(
    eventQueue.begin(), 
    eventQueue.end(),
    [](Event& e1, Event& e2) -> bool {return e1.getTime() < e2.getTime();});
}

void Driver::init() {
  this->clock = 0.0;
  this->serverIdle = true;
  this->currentlyRunningProcess = nullptr;
  this->readyQueueCount = 0;
  unsigned long initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  this->totalArrivals++;
  scheduleEvent(eventTypeEnums::ARR, initialArrivalTime);
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
  std::cout << "\tTotal arrivals: " << this->totalArrivals << "\n";
  std::cout << "\tTotal departures: " << this->totalDepartures << "\n";
  std::cout << "\tTotal processes: " << this->totalProcesses << "\n";
  std::cout << "\tTotal events: " << this->eventCount << "\n";
  this->logger.WriteToFile(transferDataResults());
  this->logger.CloseFile();
}

void Driver::arriveHandler(Event e) {
  unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  Process* newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
  if (currentlyRunningProcess == nullptr) {
    currentlyRunningProcess = newProcess;
    scheduleEvent(eventTypeEnums::DEP, this->clock + currentlyRunningProcess->getServiceTime());

  } else {
    processReadyQueue.push_back(newProcess);
  }
    
  if(this->totalProcesses < PROCESSCOUNT) {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalArrivals++;
  } 
}

void Driver::departureHander(Event e) {
  Process* runningProcess = this->currentlyRunningProcess;
  runningProcess->setCompletionTime(this->clock);
  stats.collectDepartureStats(*runningProcess);
  this->totalDepartures++;
  if(processReadyQueue.empty()) {
    this->currentlyRunningProcess = nullptr;
  } else {
    Process* nextProcess = processReadyQueue.front();
    processReadyQueue.erase(processReadyQueue.begin());
    this->currentlyRunningProcess = nextProcess;
    scheduleEvent(eventTypeEnums::DEP, this->clock + nextProcess->getServiceTime());
  }
  delete runningProcess; //deallocate dynamic memory
}

unsigned long Driver::getClock() {
  return this->clock;
}

int Driver::getReadyQueueCount() {
  return this->readyQueueCount;
}

bool Driver::getServerIdleStatus() {
  return this->serverIdle;
}

void Driver::printEvent(Event e) {
  std::string type;
    if ( e.getType() == 1) {
      type = "ARR";
    } else {
      type = "DEP";
    }
  std::cout << std::fixed << std::setprecision(15)<< "Event: (" << type << ", " << e.getTime() << ")\n\n";
}

void Driver::printEvents() {
  std::cout << "{";
  if (eventQueue.empty()) {
    std::cout << "}\n";
  } else {
    for(Event& event: eventQueue) {
      std::string type;
      if (event.getType() == 1) {
        type = "ARR";
      } else {
        type = "DEP";
      }
      if (&event == &eventQueue.back()) {
        std::cout << "(" << type << ", " << event.getTime() << ")}\n\n";
      } else {
        std::cout << "(" << type << ", " << event.getTime() << "), ";
      }
    }
  }
}

void Driver::printProcessReadyQueue() {
  std::cout << "{";
  if (processReadyQueue.empty()) {
    std::cout << "}\n";
  } else {
    for(Process* process: processReadyQueue) {
      if (process == processReadyQueue.back()) {
        std::cout << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getServiceTime() << ")}\n\n";
      } else {
        std::cout << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getServiceTime() << "), ";
      }
    }
  }
}

void Driver::printProcess(Process* process) {
  if (process != nullptr) {
    std::cout << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getServiceTime() << ")}\n\n";
  } else {
    std::cout << "()\n";
    std::cout << "\t\tNo process currently running.\n\n";
  }
}

std::string Driver::transferDataResults() {
  std::stringstream ss;
  ss << std::to_string(arrivalLambda) << "," 
     << std::to_string(stats.getThroughput()) << "," 
     << std::to_string(stats.getAverageTurnaroundTime()) << ","
     << std::to_string(stats.getCpuUitlization()) << ","
     << std::to_string(stats.getAverageWaitingTime()) << ","
     << std::to_string(stats.getAverageQueueLength()) << ","
     << std::to_string(stats.getFinalTime());
  return ss.str();
}



