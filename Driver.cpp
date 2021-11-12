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
  this->serverIdle = true;
  this->currentlyRunningProcess = nullptr;
  this->readyQueueCount = 0;
  unsigned long initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  this->totalArrivals++;
  scheduleNextEvent(eventTypeEnums::ARR, initialArrivalTime, createNewProcess(initialArrivalTime));
}

void Driver::run()
{
  this->logger.OpenFile();
  while (this->totalProcesses < PROCESSCOUNT || !eventQueue.empty())
  {
    Event e = this->eventQueue.front();
    this->clock = e.getTime();
    stats.incrementClock(getClock());
    std::cout << "CURRENT QUEUE: ";
    printEvent(e);
    std::cout << "\nCURRENT PROCESS: ";
    printProcess(e.getProcess());
    std::cout << "\n\n";
    std::cout << "CURRENT PROCESS QUEUE: ";
    printProcessReadyQueue();
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
    scheduleNextEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime, createNewProcess(this->clock+nextArrivalTime));
    this->totalArrivals++;
  }
}

void Driver::runDepartureFCFS(Event runningEvent){
  if (processReadyQueue.empty()) {
    serverIdle = true;
    std::cout << "1c. NO OTHER PROCESSES RUNNING, SERVER WILL BE IDLE.\n";
  } else {
    Process* nextProcess = processReadyQueue.front();
    processReadyQueue.pop_front();
    std::cout << "1d. NEXT DEPARURE STATUS: " << this->clock + nextProcess->getRemainingServiceTime() << "\n";
    printProcess(nextProcess);
    scheduleNextEvent(eventTypeEnums::DEP, this->clock + nextProcess->getRemainingServiceTime(), nextProcess);
  }
  handleProcessExit(runningEvent.getProcess(), this->clock);
}


void Driver::arrivalHandlerRR(Event arrivingEvent) {
  unsigned long workTodo;
  long check = arrivingEvent.getProcess()->getRemainingServiceTime() - this->quantum;
  if (check <= 0) {
    workTodo = arrivingEvent.getProcess()->getRemainingServiceTime();
  } else {
    workTodo = this->quantum;
  }
  if (serverIdle) {
    serverIdle = false;
    scheduleNextEvent(eventTypeEnums::DEP, this->clock + workTodo, arrivingEvent.getProcess());
  } else {
      this->processReadyQueue.push_back(arrivingEvent.getProcess());
  }
  
  if (this->totalProcesses < PROCESSCOUNT) {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleNextEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime, createNewProcess(this->clock + nextArrivalTime));
    this->totalArrivals++;
  }
}


void Driver::runDepartureRR(Event runningEvent) {
  long check = runningEvent.getProcess()->getRemainingServiceTime() - this->quantum;
  unsigned long workRemaining = (check <= 0) ? runningEvent.getProcess()->getRemainingServiceTime() : this->quantum;
  if (check <= 0) {
    if (processReadyQueue.empty()) {
      serverIdle = true;
    } else {
      Process* nextProcess = processReadyQueue.front();
      long nextCheck = nextProcess->getRemainingServiceTime() - this->quantum;
      unsigned long nextWorkRemaining = (nextCheck <= 0) ? nextProcess->getRemainingServiceTime() : this->quantum;
      processReadyQueue.pop_front();
      scheduleNextEvent(eventTypeEnums::DEP, this->clock + nextWorkRemaining, nextProcess);
    }
    handleProcessExit(runningEvent.getProcess(), this->clock);
  } else {
    long nextCheck = runningEvent.getProcess()->getRemainingServiceTime() - this->quantum;
    unsigned long nextWorkRemaining = (nextCheck <= 0) ? runningEvent.getProcess()->getRemainingServiceTime() : this->quantum;
    runningEvent.getProcess()->setRemainingServiceTime(runningEvent.getProcess()->getRemainingServiceTime() - this->quantum);
    if(processReadyQueue.empty()) {
      scheduleNextEvent(eventTypeEnums::DEP, this->clock + nextWorkRemaining, runningEvent.getProcess());
    } else {
      this->processReadyQueue.push_back(runningEvent.getProcess());
      Process* nextProcess = processReadyQueue.front();
      processReadyQueue.pop_front();
      scheduleNextEvent(eventTypeEnums::DEP, this->clock + nextWorkRemaining, nextProcess);
    }
  }

}

void Driver::arrivalHandlerSRTF(Event arrivingEvent) {
  std::cout << "CLOCK: " << this->clock << "\n";
  std::cout << "RUNNING PROCESS: ";
  printProcess(arrivingEvent.getProcess());
  if (serverIdle) {
    std::cout << "1a. SERVER IS IDLE.\n";
    serverIdle = false;
    scheduleNextEvent(eventTypeEnums::DEP, this->clock + arrivingEvent.getProcess()->getRemainingServiceTime(), arrivingEvent.getProcess());
  } else {
    eventQueue.pop_front();
    if (!eventQueue.empty()) {
      std::cout << "1b. EVENT QUEUE IS NOT EMPTY.\n";
      Event currentEvent = eventQueue.front();
      Event nextEvent = eventQueue.front();
      if (nextEvent.getType() == eventTypeEnums::DEP) {
        Process* nextProcess = nextEvent.getProcess();
        Process* currentProcess = arrivingEvent.getProcess();
        std::cout << "IS ARRIVING EVENT < NEXT EVENT: " << nextProcess->getRemainingServiceTime() << " <? " << currentProcess->getRemainingServiceTime() << "\n";
        if (arrivingEvent.getProcess()->getRemainingServiceTime() < nextEvent.getProcess()->getRemainingServiceTime()) {
          eventQueue.pop_front();
          std::cout << "2b. NEXT EVENT IS AN ARRIVAL AND IT'LL HAPPEN BEFORE WE'RE FINISHED.\n";
          long netTime = nextEvent.getTime()-nextProcess->getRemainingServiceTime() - this->clock;
          if (netTime < 0) {
            netTime *= -1;
          }
          std::cout << "3b. NEXT PROCESS RAN FOR: " << netTime << "\n";
          nextProcess->setRemainingServiceTime(nextProcess->getRemainingServiceTime() - netTime);
          std::cout << "4b. UPDATED NEXT PROCESS: ";
          printProcess(nextProcess);
          std::cout << "5b. SCHEDULING FOR THE FOLLOWING PROCESS: ";
          printProcess(currentProcess);
          scheduleNextEvent(eventTypeEnums::DEP, this->clock + currentProcess->getRemainingServiceTime(), currentProcess);
          processReadyQueue.push_back(nextProcess);
        } else {
           this->processReadyQueue.push_back(arrivingEvent.getProcess());
          eventQueue.push_front(currentEvent);

        }
      }

    }
  }
  
  if (this->totalProcesses < PROCESSCOUNT) {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleNextEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime, createNewProcess(this->clock+nextArrivalTime));
    this->totalArrivals++;
  }
  std::sort(
    processReadyQueue.begin(),
    processReadyQueue.end(),
            [](Process* p1, Process* p2) {return p1->getRemainingServiceTime() < p2->getRemainingServiceTime();});
  std::cout << "Process ready queue sorted: ";
  printProcessReadyQueue();
}

void Driver::runDepartureSRTF(Event departingEvent) {
  runDepartureFCFS(departingEvent);
}

Process* Driver::createNewProcess(unsigned long time) {
    unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
    Process* newProcess = new Process(++this->totalProcesses, time, nextServiceTime);
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
  default:
    type = "ERR";
    break;
  }
  if (e.getType() == eventTypeEnums::ARR) {
    type = "ARR";
  } else if (e.getType() == eventTypeEnums::DEP) {
    type = "DEP";
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
  switch (this->scheduleType) {
  case eventTypeEnums::RR:
      arrivalHandlerRR(e);
    break;
  case eventTypeEnums::FCFS:
    arrivalHandlerFCFS(e);
    break;
  case eventTypeEnums::SRTF:
      arrivalHandlerSRTF(e);
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
    runDepartureSRTF(e);
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

void Driver::handleProcessExit(Process* process, unsigned long finalTime) {
  process->setCompletionTime(finalTime);
  process->setRemainingServiceTime(0);
  stats.collectDepartureStats(*process);
  this->totalDepartures++;
  process->printProcessInformation();
  delete process;
}

