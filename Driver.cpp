#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st, int scheduleType, float quantum) : arrivalTime(at), serviceTime(st), scheduleType(scheduleType), quantum(quantum) {}

void Driver::scheduleEvent(int eventType, unsigned long time)
{
  Event event(time, eventType);
  this->eventCount++;
  eventQueue.push_back(event);
  finalScheduleSnaphot.push_back(event);
  std::sort(
    eventQueue.begin(),
    eventQueue.end(),
    [](Event& e1, Event& e2) -> bool {return e1.getTime() < e2.getTime();});
  
  std::sort(
    finalScheduleSnaphot.begin(),
    finalScheduleSnaphot.end(),
    [](Event& e1, Event& e2) -> bool {return e1.getTime() < e2.getTime();});
}

void Driver::init()
{
  this->clock = 0.0;
  this->serverIdle = true;
  this->currentlyRunningProcess = nullptr;
  this->readyQueueCount = 0;
  unsigned long initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  this->totalArrivals++;
  scheduleEvent(eventTypeEnums::ARR, initialArrivalTime);
}

void Driver::run()
{
  this->logger.OpenFile();
  while (this->totalProcesses < PROCESSCOUNT || !eventQueue.empty())
  {
    Event e = this->eventQueue.front();
    this->clock = e.getTime();
    stats.incrementClock(getClock());
    scheduleEvent(e);
    this->eventQueue.pop_front();
  }
  std::cout << "\tTotal arrivals: " << this->totalArrivals << "\n";
  std::cout << "\tTotal departures: " << this->totalDepartures << "\n";
  std::cout << "\tTotal processes: " << this->totalProcesses << "\n";
  std::cout << "\tTotal events: " << this->eventCount << "\n";
  this->logger.WriteToFile(transferDataResults());
  this->logger.CloseFile();
}

void Driver::arrivalHandlerFCFS(Event e)
{
  unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  Process *newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
  if (currentlyRunningProcess == nullptr)
  {
    currentlyRunningProcess = newProcess;
    scheduleEvent(eventTypeEnums::RUN, this->clock);
    // scheduleEvent(eventTypeEnums::DEP, this->clock + currentlyRunningProcess->getServiceTime());
  }
  else
  {
    processReadyQueue.push_back(newProcess);
  }

  if (this->totalProcesses < PROCESSCOUNT)
  {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalArrivals++;
  }
}

void Driver::arrivalHandlerSRTF(Event e)
{
  unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  scheduleEvent(eventTypeEnums::RUN, this->clock + nextArrivalTime);
  this->totalProcesses++;
  if (this->totalProcesses < PROCESSCOUNT)
  {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalArrivals++;
  }
}

void Driver::arrivalHandlerRR(Event e)
{
  unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  scheduleEvent(eventTypeEnums::RUN, this->clock + nextArrivalTime);
  this->totalProcesses++;
  if (this->totalProcesses < PROCESSCOUNT)
  {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalArrivals++;
  }
}

void Driver::runHandlerFCFS(Event e)
{
  Event eventQueueFrontElement = this->eventQueue.front();
  eventQueue.pop_front();
  unsigned long nextEventTimeStart = this->eventQueue.front().getTime();
  unsigned long elapsedCompletionTime = this->clock + currentlyRunningProcess->getRemainingServiceTime();
  unsigned long currentRemainingServiceTime = this->currentlyRunningProcess->getRemainingServiceTime();
  long netServiceTime = elapsedCompletionTime - nextEventTimeStart;
  
  if (netServiceTime <= 0 || eventQueue.empty()) {
    scheduleEvent(eventTypeEnums::DEP, elapsedCompletionTime);
  } else {
    this->currentlyRunningProcess->setRemainingServiceTime(elapsedCompletionTime - nextEventTimeStart);
    scheduleEvent(eventTypeEnums::RUN, nextEventTimeStart);
  }
  eventQueue.push_front(eventQueueFrontElement);
}

void Driver::runHandlerSRTF(Event e)
{
}

void Driver::runHandlerRR(Event e)
{
}

void Driver::departureHandler(Event e)
{
  Process *runningProcess = this->currentlyRunningProcess;
  runningProcess->setCompletionTime(this->clock);
  stats.collectDepartureStats(*runningProcess);
  this->totalDepartures++;
  if (processReadyQueue.empty())
  {
    this->currentlyRunningProcess = nullptr;
  }
  else
  {
    Process *nextProcess = processReadyQueue.front();
    processReadyQueue.pop_front();
    this->currentlyRunningProcess = nextProcess;
    scheduleEvent(eventTypeEnums::RUN, this->clock);
  }
  delete runningProcess; //deallocate dynamic memory
}

unsigned long Driver::getClock()
{
  return this->clock;
}

int Driver::getReadyQueueCount()
{
  return this->readyQueueCount;
}

bool Driver::getServerIdleStatus()
{
  return this->serverIdle;
}

void Driver::printEvent(Event e)
{
  std::string type;
  switch (e.getType())
  {
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
  if (e.getType() == eventTypeEnums::ARR)
  {
    type = "ARR";
  }
  else if (e.getType() == eventTypeEnums::DEP)
  {
    type = "DEP";
  }
  else
  {
    type = "RUN";
  }
  std::cout << "(" << type << ", " << e.getTime() << ")";
}

void Driver::printEvents()
{
  std::cout << "{";
  if (eventQueue.empty())
  {
    std::cout << "}\n";
  }
  else
  {
    for (Event &event : eventQueue)
    {
      printEvent(event);
      if (&event == &eventQueue.back())
      {
        std::cout << ")}\n";
      }
      else
      {
        std::cout << "), ";
      }
    }
  }
}

void Driver::printProcessReadyQueue()
{
  std::cout << "{";
  if (processReadyQueue.empty())
  {
    std::cout << "}\n";
  }
  else
  {
    for (Process *process : processReadyQueue)
    {
      if (process == processReadyQueue.back())
      {
        std::cout << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << ")}\n";
      }
      else
      {
        std::cout << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << "), ";
      }
    }
  }
}

void Driver::printProcess(Process *process)
{
  if (process != nullptr)
  {
    std::cout << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << ")}\n";
  }
  else
  {
    std::cout << "()\n";
  }
}

std::string Driver::transferDataResults()
{
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

void Driver::scheduleArrival(Event e)
{
  switch (this->scheduleType)
  {
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

void Driver::scheduleRun(Event e)
{
  switch (this->scheduleType)
  {
  case eventTypeEnums::FCFS:
    runHandlerFCFS(e);
    break;
  case eventTypeEnums::RR:
    runHandlerRR(e);
    break;
  case eventTypeEnums::SRTF:
    runHandlerSRTF(e);
    break;
  }
}

void Driver::scheduleEvent(Event e)
{
  switch (e.getType())
  {
  case eventTypeEnums::ARR:
    scheduleArrival(e);
    break;
  case eventTypeEnums::RUN:
    scheduleRun(e);
    break;
  case eventTypeEnums::DEP:
    departureHandler(e);
    break;
  default:
    std::cout << "ERROR WITH SCHEDULING\n";
  }
}