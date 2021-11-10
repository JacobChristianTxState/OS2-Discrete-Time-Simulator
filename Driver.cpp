#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st, int scheduleType, float quantum) : arrivalTime(at), serviceTime(st), scheduleType(scheduleType), quantum(quantum) {}

void Driver::scheduleEvent(int eventType, unsigned long time)
{
  Event event(time, eventType);
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
    printEvent(e);
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
  unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  Process *newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
  if (currentlyRunningProcess == nullptr)
  {
    currentlyRunningProcess = newProcess;
    scheduleEvent(eventTypeEnums::RUN, this->clock);
  }
  else
  {
    if(newProcess->getServiceTime() < currentlyRunningProcess->getRemainingServiceTime()){
    
      currentlyRunningProcess->setRemainingServiceTime(newProcess->getArrivalTime() - currentlyRunningProcess->getArrivalTime());
      processReadyQueue.push_back(currentlyRunningProcess);
      if(processReadyQueue.size() >= 2){
        std::sort(
        processReadyQueue.begin(),
        processReadyQueue.end(),
        [](Process *p1, Process *p2) {return p1->getRemainingServiceTime() < p2->getRemainingServiceTime();});
      }
      currentlyRunningProcess = newProcess; 
      
      scheduleEvent(eventTypeEnums::RUN, this->clock);
   
    }
    else { 
      processReadyQueue.push_back(newProcess);
      if(processReadyQueue.size() >= 2){
        std::sort(
        processReadyQueue.begin(),
        processReadyQueue.end(),
        [](Process *p1, Process *p2) {return p1->getRemainingServiceTime() < p2->getRemainingServiceTime();});
      }
      }
    }

  if(this->totalProcesses < PROCESSCOUNT)
  {
    unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalArrivals++;
  }

  /*
  unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  scheduleEvent(eventTypeEnums::RUN, this->clock + nextArrivalTime);
  this->totalProcesses++;
  */
}

void Driver::arrivalHandlerRR(Event e)
{
  unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  Process *newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
  if (currentlyRunningProcess == nullptr)
  {
    if(processReadyQueue.empty()){
      currentlyRunningProcess = newProcess;
      scheduleEvent(eventTypeEnums::RUN, this->clock + currentlyRunningProcess->getArrivalTime());
    }else{
      currentlyRunningProcess = processReadyQueue.front();
      processReadyQueue.pop_front();
      scheduleEvent(eventTypeEnums::RUN, this->clock + currentlyRunningProcess->getArrivalTime());
    }
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

void Driver::runHandlerFCFS(Event e)
{
  // get front of Event Queue
  Event eventQueueFrontElement = this->eventQueue.front();
  // pop it from Event Queue
  eventQueue.pop_front();
  // get time of event
  unsigned long nextEventTimeStart = this->eventQueue.front().getTime();
  // get time current process will end
  float elapsedCompletionTime = this->clock + currentlyRunningProcess->getRemainingServiceTime();
  // get time difference of current process end to incoming event start
  long netServiceTime = elapsedCompletionTime - nextEventTimeStart;
  // if time difference is <= 0, a process will arrive before current one departs
  // so schedule departure of current process
  // if a process will not arrive before current one departs,
  // update current process service time and schedule a run with updated time
  // no matter what, put the front of event queue back where it was
  if(netServiceTime <= 0 || eventQueue.empty()) {
    //a process is
    scheduleEvent(eventTypeEnums::DEP, elapsedCompletionTime);
  } else {
    
    this->currentlyRunningProcess->setRemainingServiceTime(elapsedCompletionTime - nextEventTimeStart);
    scheduleEvent(eventTypeEnums::RUN, nextEventTimeStart);
  }
  eventQueue.push_front(eventQueueFrontElement);
}

void Driver::runHandlerSRTF(Event e)
{
  //take event at front of queue
  //get event after that's time
  // check if current process time is 

  // put event at front of queue

  // get front of Event Queue
  Event eventQueueFrontElement = this->eventQueue.front();
  // pop it from Event Queue
  eventQueue.pop_front();
  // get time of event
  unsigned long nextEventTimeStart = this->eventQueue.front().getTime();
  // get time current process will end
  float elapsedCompletionTime = this->clock + currentlyRunningProcess->getRemainingServiceTime();
  // get time difference of current process end to incoming event start
  long netServiceTime = elapsedCompletionTime - nextEventTimeStart;
  // if time difference is <= 0, a process will arrive before current one departs
  // so schedule departure of current process
  // if a process will not arrive before current one departs,
  // update current process service time and schedule a run with updated time
  // no matter what, put the front of event queue back where it was
  if(netServiceTime <= 0 || eventQueue.empty()) {
    //a process is
    scheduleEvent(eventTypeEnums::DEP, elapsedCompletionTime);
  } else {
    this->currentlyRunningProcess->setRemainingServiceTime(elapsedCompletionTime - nextEventTimeStart);
    scheduleEvent(eventTypeEnums::RUN, nextEventTimeStart);
  }
  eventQueue.push_front(eventQueueFrontElement);
}

void Driver::runHandlerRR(Event e)
{
  //printProcessReadyQueue();
    float check = currentlyRunningProcess->getRemainingServiceTime() - quantum;
    if(check <= 0){
      //Event dep = Event(eventTypeEnums::DEP, clock + currentlyRunningProcess->getRemainingServiceTime());
      scheduleEvent(eventTypeEnums::DEP, clock + currentlyRunningProcess->getRemainingServiceTime());
    }else{
      currentlyRunningProcess->setRemainingServiceTime(check);
      processReadyQueue.push_back(currentlyRunningProcess);
      currentlyRunningProcess = processReadyQueue.front();
      processReadyQueue.pop_front();
      scheduleEvent(eventTypeEnums::RUN, clock + quantum);
    }
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
