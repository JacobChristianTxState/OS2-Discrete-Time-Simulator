#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st, int scheduleType, float quantum) : arrivalTime(at), serviceTime(st), scheduleType(scheduleType), quantum(quantum)
{
  this->quantum = std::round(this->quantum * 1000);
  this->roundRobinInterruptedProcess = false;
  this->roundRobinTimeAdjustment = 0;
}

void Driver::scheduleEvent(int eventType, unsigned long time)
{
  Event event(time, eventType);
  this->eventCount++;
  eventQueue.push_back(event);
  std::cerr << "\t\tCreated Event: ";
  printEvent(event);
  std::sort(
      eventQueue.begin(),
      eventQueue.end(),
      [](Event e1, Event e2)
      { return e1.getTime() < e2.getTime(); });
}

void Driver::init()
{
  this->clock = 0.0;
  this->serverIdle = true;
  this->currentlyRunningProcess = nullptr;
  this->readyQueueCount = 0;
  unsigned long initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  this->totalArrivals++;
  std::cerr << "EVENT " << this->eventCount << "\n";
  scheduleEvent(eventTypeEnums::ARR, initialArrivalTime);
}

void Driver::run()
{
  unsigned long totalEvents = 0;
  this->logger.OpenFile();
  while (this->totalProcesses < PROCESSCOUNT || !eventQueue.empty())
  {
    Event e = this->eventQueue.front();
    this->clock = e.getTime();
    stats.incrementClock(getClock());
    std::cerr << "\n\nEVENT :" << ++totalEvents << "\n";
    std::cerr << "TIME QUANTUM: " << this->quantum << "\n";
    std::cerr << "EVENT TIME: " << this->clock << "\n";
    std::cerr << "Event queue: ";
    printEvents();
    std::cerr << "\tCurrent Event: ";
    printEvent(e);
    std::cerr << "\tCurrent Process: ";
    printProcess(this->currentlyRunningProcess);
    std::cerr << "\n\tProcess Ready Queue: ";
    printProcessReadyQueue();
    scheduleEvent(e);
    this->eventQueue.pop_front();
    std::cerr << "\n\n";
  }
  std::cerr << "\tTotal arrivals: " << this->totalArrivals << "\n";
  std::cerr << "\tTotal departures: " << this->totalDepartures << "\n";
  std::cerr << "\tTotal processes: " << this->totalProcesses << "\n";
  std::cerr << "\tTotal events: " << this->eventCount << "\n";
  std::cerr << "\tFinal Time: " << this->clock << "\n\n";
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
  unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  Process *newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
  std::cerr << "\t\tCreating new process: ";
  printProcess(newProcess);
  if (currentlyRunningProcess == nullptr)
  {
    currentlyRunningProcess = newProcess;
  }
  else
  {
    std::cerr << "\n\t\tPushing current process to back of queue: ";
    printProcess(newProcess);
    processReadyQueue.push_back(newProcess);
    if (this->roundRobinInterruptedProcess)
    {
      std::cerr << "Process had an arrival mid run. Pushing back current process.\n";
      processReadyQueue.push_back(this->currentlyRunningProcess);
      printProcessReadyQueue();
      this->currentlyRunningProcess = this->processReadyQueue.front();
      this->processReadyQueue.pop_front();
      printProcessReadyQueue();
      this->roundRobinInterruptedProcess = false;
    }
  }
  scheduleEvent(eventTypeEnums::RUN, this->clock + roundRobinTimeAdjustment);
  this->roundRobinTimeAdjustment = 0;

  if (this->totalProcesses < PROCESSCOUNT)
  {
    std::cerr << "\n\t\tScheduling a new arrival.\n";
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
  long netServiceTime = elapsedCompletionTime - nextEventTimeStart;

  if (netServiceTime <= 0 || eventQueue.empty())
  {
    scheduleEvent(eventTypeEnums::DEP, elapsedCompletionTime);
  }
  else
  {
    this->currentlyRunningProcess->setRemainingServiceTime(elapsedCompletionTime - nextEventTimeStart);
    scheduleEvent(eventTypeEnums::RUN, nextEventTimeStart);
  }
  eventQueue.push_front(eventQueueFrontElement);
}

void Driver::runHandlerSRTF(Event e)
{

  // eventQueue.push_front(eventQueueFrontElement);
}

void Driver::runHandlerRR(Event e)
{
  long remainingWorkingTime =
      this->currentlyRunningProcess->getRemainingServiceTime() - this->quantum;
  long runFinishTime;
  std::cerr << "\t\tCurrent process running: ";
  printProcess(this->currentlyRunningProcess);
  std::cerr << "Current Process remaining work after run: " << remainingWorkingTime << "\n";
  if (remainingWorkingTime < 0)
  {
    remainingWorkingTime += this->quantum;
    runFinishTime = this->clock + this->currentlyRunningProcess->getRemainingServiceTime();
  }
  else
  {
    runFinishTime = this->clock + this->quantum;
  }
  std::cerr << "Current remaining work time: " << remainingWorkingTime << "\n";
  Event eventQueueFrontElement = this->eventQueue.front();
  eventQueue.pop_front();
  unsigned long nextEventTimeStart = ULLONG_MAX;
  if (!eventQueue.empty())
  {
    nextEventTimeStart = this->eventQueue.front().getTime();
  }
  else
  {
    std::cerr << "Event queue is empty.\n";
  }
  std::cerr << "\t\tNext event start time: " << nextEventTimeStart << "\n";
  std::cerr << "\t\tCurrent event finish time: " << runFinishTime << "\n";
  if (nextEventTimeStart > runFinishTime)
  {
    std::cerr << "Current Process remaining work after run: " << remainingWorkingTime << "\n";
    if (remainingWorkingTime == 0)
    {
      std::cerr << "\t\tProcess complete exactly at quantum.\n";
      if (this->eventQueue.empty())
      {
      }
      scheduleEvent(eventTypeEnums::DEP, runFinishTime);
    }
    else if (remainingWorkingTime < 0)
    {
      runFinishTime = this->clock + this->currentlyRunningProcess->getRemainingServiceTime();
      std::cerr << "\t\tProcess finished before quantum, scheduling departure for: " << this->currentlyRunningProcess->getRemainingServiceTime() << "\n";
      scheduleEvent(eventTypeEnums::DEP, runFinishTime);
    }
    else
    {
      std::cerr << "\t\tProcess remaining time greater than quantum, putting back into ready queue.\n";
      this->currentlyRunningProcess->setRemainingServiceTime(
          this->currentlyRunningProcess->getRemainingServiceTime() - this->quantum);
      std::cerr << "Current process' next run stats: ";
      printProcess(this->currentlyRunningProcess);
      scheduleEvent(eventTypeEnums::RUN, runFinishTime);
      if (!this->processReadyQueue.empty())
      {
        Process *nextProcess = this->processReadyQueue.front();
        this->processReadyQueue.pop_front();
        this->processReadyQueue.push_back(this->currentlyRunningProcess);
        this->currentlyRunningProcess = nextProcess;
        std::cerr << "\n\t\tNext Process running is: ";
        printProcess(this->currentlyRunningProcess);
      }
      else
      {
        std::cerr << "\n\t\tNo processes in ready queue, will continue to run process: ";
        printProcess(this->currentlyRunningProcess);
        printEvents();
      }
    }
  }
  else
  {
    this->roundRobinTimeAdjustment = runFinishTime - nextEventTimeStart;
    std::cerr << "Going to decrement current process by : " << this->currentlyRunningProcess->getRemainingServiceTime() - remainingWorkingTime << "\n";
    this->currentlyRunningProcess->setRemainingServiceTime(remainingWorkingTime);
    std::cerr << "We will handle an arrival next.\n";
    std::cerr << "Process after finishing quantum: ";
    printProcess(this->currentlyRunningProcess);
    this->roundRobinInterruptedProcess = true;
  }
  this->eventQueue.push_front(eventQueueFrontElement);
}

void Driver::departureHandler(Event e)
{
  Process *runningProcess = this->currentlyRunningProcess;
  runningProcess->setCompletionTime(this->clock);
  std::cerr << "Process " << runningProcess->getID() << "'s turnaround time: " << runningProcess->getCompletionTime() - runningProcess->getArrivalTime() << "\n";

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
  std::cerr << "(" << type << ", " << e.getTime() << ")";
}

void Driver::printEvents()
{
  std::cerr << "{";
  if (eventQueue.empty())
  {
    std::cerr << "}\n";
  }
  else
  {
    for (Event &event : eventQueue)
    {
      printEvent(event);
      if (&event == &eventQueue.back())
      {
        std::cerr << ")}\n";
      }
      else
      {
        std::cerr << "), ";
      }
    }
  }
}

void Driver::printProcessReadyQueue()
{
  std::cerr << "{ ";
  if (processReadyQueue.empty())
  {
    std::cerr << "}\n";
  }
  else
  {
    for (Process *process : processReadyQueue)
    {
      if (process == processReadyQueue.back())
      {
        std::cerr << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << ") }\n";
      }
      else
      {
        std::cerr << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << "), ";
      }
    }
  }
}

void Driver::printProcess(Process *process)
{
  if (process != nullptr)
  {
    std::cerr << "(" << process->getID() << ", " << process->getArrivalTime() << ", " << process->getRemainingServiceTime() << ")}\n";
  }
  else
  {
    std::cerr << "()\n";
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
    std::cerr << "ERROR WITH SCHEDULING\n";
  }
}
