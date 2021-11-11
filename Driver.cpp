#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st, int scheduleType, float quantum) :
  arrivalTime(at), serviceTime(st), scheduleType(scheduleType), quantum(quantum) {
    this->quantum = std::round(this->quantum * 1000);
    this->roundRobinInterruptedProcess = false;
    this->roundRobinTimeAdjustment = 0;
    this->serverIdle = true;
  }

void Driver::scheduleNextEvent(int eventType, unsigned long time, Process* process)
{
  Event event(time, eventType, process);
  this->eventCount++;
  eventQueue.push_back(event);
  std::sort(
    eventQueue.begin(),
    eventQueue.end(),
    [](Event e1, Event e2) {return e1.getTime() < e2.getTime();});
  
}

void Driver::init()
{
  this->clock = 0.0;
  this->serverIdle = true;
  this->currentlyRunningProcess = nullptr;
  this->readyQueueCount = 0;
  unsigned long initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  this->totalArrivals++;
  scheduleNextEvent(eventTypeEnums::ARR, initialArrivalTime, createNewProcess());
}

void Driver::run()
{
  unsigned long totalEvents = 0;
  this->logger.OpenFile();
  while (this->totalProcesses < PROCESSCOUNT || !eventQueue.empty())
  {
    std::cout << "\nRUNNING EVENT :" << ++totalEvents << "\n";
    std::cout << "QUANTUM: " << this->quantum << "\n";
    Event e = this->eventQueue.front();
    this->clock = e.getTime();
    stats.incrementClock(getClock());
    std::cout << "Current event schedule: ";
    printEvents();
    scheduleEvent(e);
    this->eventQueue.pop_front();
  }
  std::cerr << "\tTotal arrivals: " << this->totalArrivals << "\n";
  std::cerr << "\tTotal departures: " << this->totalDepartures << "\n";
  std::cerr << "\tTotal processes: " << this->totalProcesses << "\n";
  std::cerr << "\tTotal events: " << this->eventCount << "\n";
  std::cerr << "\tFinal Time: " << this->clock << "\n\n";
  this->logger.WriteToFile(transferDataResults());
  this->logger.CloseFile();
}

void Driver::arrivalHandlerFCFS(Event arrivingEvent){
  if (serverIdle) {
    serverIdle = false;
    scheduleNextEvent(eventTypeEnums::DEP, this->clock + arrivingEvent.getProcess()->getServiceTime(), arrivingEvent.getProcess());
  } else {
    this->processReadyQueue.push_back(arrivingEvent.getProcess());
  }
  
  if (this->totalProcesses < PROCESSCOUNT) {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleNextEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime, createNewProcess());
    this->totalArrivals++;
  }
}

void Driver::runDepartureFCFS(Event runningEvent){
  runningEvent.getProcess()->setCompletionTime(this->clock);
  stats.collectDepartureStats(*runningEvent.getProcess());
  delete runningEvent.getProcess();


  if (processReadyQueue.empty()) {
    serverIdle = true;
  } else {
    Process* nextProcess = processReadyQueue.front();
    processReadyQueue.pop_front();
    scheduleNextEvent(eventTypeEnums::DEP, this->clock + nextProcess->getRemainingServiceTime(), nextProcess);
  }
}

void Driver::arrivalHandlerRR(Event arrivingEvent) {
  if (serverIdle) {
    serverIdle = false;
    scheduleNextEvent(eventTypeEnums::DEP, this->clock + this->quantum, arrivingEvent.getProcess());
  } else {
    this->processReadyQueue.push_back(arrivingEvent.getProcess());
  }
  
  if (this->totalProcesses < PROCESSCOUNT) {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleNextEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime, createNewProcess());
    this->totalArrivals++;
  }
}

void Driver::runDepartureRR(Event runningEvent) {
  if (processReadyQueue.empty()) {
    serverIdle = true;
  }
  long check = runningEvent.getProcess()->getRemainingServiceTime() - this->quantum;
  std::cout << "Curently running process: ";
  printProcess(runningEvent.getProcess());
  std::cout << "Check's value: " << check << "\n";
  
  if (check <= 0) {
    runningEvent.getProcess()->setCompletionTime(this->clock);
    if (runningEvent.getProcess()->getRemainingServiceTime() == 0) {
      runningEvent.getProcess()->setCompletionTime(this->clock);
      stats.collectDepartureStats(*runningEvent.getProcess());
      delete runningEvent.getProcess();
    } else {
    runningEvent.getProcess()->setRemainingServiceTime(runningEvent.getProcess()->getRemainingServiceTime() - this->quantum);
    this->processReadyQueue.push_back(runningEvent.getProcess());
    Process* nextProcess = processReadyQueue.front();
    processReadyQueue.pop_front();
    scheduleNextEvent(eventTypeEnums::ARR, this->clock + this->quantum, createNewProcess());
    }
  }
}

Process* Driver::createNewProcess() {
    unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
    Process* newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
    return newProcess;
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

void Driver::printEvent(Event e){
  std::string type;
  switch (e.getType()) {
  case eventTypeEnums::ARR:
    type = "ARR";
    break;
  case eventTypeEnums::DEP:
    type = "DEP";
    break;
  case eventTypeEnums::RUN:
    type = "RUN";
    break;
  default:
    type = "ERR";
    break;
  }
  if (e.getType() == eventTypeEnums::ARR) {
    type = "ARR";
  } else if (e.getType() == eventTypeEnums::DEP) {
    type = "DEP";
  } else {
    type = "RUN";
  }
  std::cerr << "(" << type << ", " << e.getTime() << ")";
}

void Driver::printEvents()
{
  std::cerr << "{";
  if (eventQueue.empty()) {
    std::cerr << "}\n";
  }
  else {
    for (Event &event : eventQueue) {
      printEvent(event);
      if (&event == &eventQueue.back()) {
        std::cerr << ")}\n";
      } else {
        std::cerr << "), ";
      }
    }
  }
}

void Driver::printProcessReadyQueue() {
  std::cerr << "{ ";
  if (processReadyQueue.empty()) {
    std::cerr << "}\n";
  } else {
    for (Process *process : processReadyQueue) {
      if (process == processReadyQueue.back()) {
        std::cerr << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << ") }\n";
      } else {
        std::cerr << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << "), ";
      }
    }
  }
}

void Driver::printProcess(Process *process) {
  if (process != nullptr) {
    std::cerr << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << ")}\n";
  } else {
    std::cerr << "()\n";
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


void Driver::scheduleArrival(Event e) {
  switch (e.getType()) {
  case eventTypeEnums::RR:
    arrivalHandlerRR(e);
    break;
  case eventTypeEnums::FCFS:
    arrivalHandlerFCFS(e);
    break;
  case eventTypeEnums::SRTF:
    //arrivalHandlerSRTF(e);
    break;
  }
}

void Driver::scheduleDeparture(Event e) {
  switch(this->scheduleType) {
    case eventTypeEnums::FCFS:
      runDepartureFCFS(e);
    break;
  case eventTypeEnums::RR:
      runDepartureRR(e);
    break;
  case eventTypeEnums::SRTF:
   // runHandlerSRTF(e);
    break;
  }
}

void Driver::scheduleEvent(Event e) {
  switch (e.getType()) {
  case eventTypeEnums::ARR:
    scheduleArrival(e);
    break;
  case eventTypeEnums::DEP:
    scheduleDeparture(e);
    break;
  default:
    std::cerr << "ERROR WITH SCHEDULING\n";
  }
}

