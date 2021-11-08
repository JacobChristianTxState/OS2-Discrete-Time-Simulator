#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st) : arrivalTime(at), serviceTime(st) {}

void Driver::scheduleEvent(int eventType, float time) {
  if (eventType == eventTypeEnums::DEP) {
    // std::cout << "Departure time: " << time << "\n";
    // std::cout << "Currently Running Process arrival time: " << currentlyRunningProcess->getArrivalTime() << "\n";
    //stats.incrementTurnaroundTime(time - currentlyRunningProcess->getArrivalTime());
  }
  Event event(time, eventType);
  this->eventCount++;
  if (arrivalLambda == 3) {
    std::cout << "Creating the following event: ";
    printEvent(event);
  }
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
  // std::cout << "Initial arrival time: " << initialArrivalTime << "\n";
  // std::cout << "\n";
  this->totalArrivals++;
  scheduleEvent(eventTypeEnums::ARR, initialArrivalTime);
  //stats.incrementWorkTime(initialArrivalTime);
}

void Driver::run() {
  this->logger.OpenFile();
  while(this->totalProcesses < PROCESSCOUNT || eventQueue.size() > 0){
    Event e = eventQueue[0];
    this->clock = e.getTime();
    if (arrivalLambda == 3) {
      std::cout << "Current clock: " << getClock() << "\n";
      std::cout << "Current event: ";
      printEvent(e);
      std::cout << "Current process in running state:\n";
      printProcess(this->currentlyRunningProcess);
    }
    stats.incrementClock(getClock());
    // std::cout << "(run) a2. Current process count: " << this->totalProcesses << "\n";
    // std::cout << "(run) a3. Current event in run: ";
    // printEvent(e);
    //std::cout << "(run) a4. Currently running process: ";
    // printProcess(this->currentlyRunningProcess);
    switch(e.getType()) {
      case eventTypeEnums::ARR:
        // std::cout << "\t(run, arr) a5_1. Handling arrival...\n\n";
        arriveHandler(e);
        break;
      case eventTypeEnums::DEP: 
        // std::cout << "\t(run, dep) a5_2. Handling departure...\n\n";
        departureHander(e);
        break;
    }
    // std::cout << "(run) a6. Erasing head from queue.\n\n";
    eventQueue.erase(eventQueue.begin());
    // std::cout << "(run) a7. Current event queue: ";
    // printEvents();
  }
  // std::cout << "\tTotal arrivals: " << this->totalArrivals << "\n";
  // std::cout << "\tTotal departures: " << this->totalDepartures << "\n";
  // std::cout << "\tTotal processes: " << this->totalProcesses << "\n";
  // std::cout << "\tTotal events: " << this->eventCount << "\n";
  std::stringstream ss;
  ss << std::to_string(arrivalLambda) << "," 
     << std::to_string(stats.getThroughput()) << "," 
     << std::to_string(stats.getAverageTurnaroundTime()) << ","
     << std::to_string(stats.getCpuUitlization()) << ","
     << std::to_string(stats.getAverageWaitingTime()) << ","
     << std::to_string(stats.getAverageQueueLength()) << ","
     << std::to_string(stats.getFinalTime());
  this->logger.WriteToFile(ss.str());
  this->logger.CloseFile();
  // std::cout << "\tProcess completion list: " << "\n";
  //printCompletionList();
}

void Driver::arriveHandler(Event e) {
  unsigned long nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  unsigned long nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
  // std::cout << "Next arrival will happen at time: " << this->clock + nextArrivalTime << "\n\n";
  // std::cout << "Next arrival takes a total of   : " << nextArrivalTime << "\n\n";
  // std::cout << "Next arrival's service time: " << nextServiceTime << "\n\n";
  Process* newProcess = new Process(++this->totalProcesses, this->clock, nextServiceTime);
  if (arrivalLambda == 3) {
      std::cout << "New process created:\n";
      printProcess(newProcess);
  }
  if(!currentlyRunningProcess){ //&& serverIdle) {
    //serverIdle = false;
    currentlyRunningProcess = newProcess;
    //currentlyRunningProcess->setArrivalTime(this->clock);
    unsigned long currentServiceTime = currentlyRunningProcess->getServiceTime();
    if (currentlyRunningProcess->getID() == 54797) {
      std::cout << "current clock: " << this->clock << "\n";
      std::cout << "process run time: " << currentlyRunningProcess->getServiceTime() << "\n";
      std::cout << "departure time (should be 16778401): " << this->clock + currentlyRunningProcess->getServiceTime() << "\n";
    }
    // std::cout << "\t\t(arr, serverIdle) b4_1. Server is idle, creating a departure.\n";
    // std::cout << "\t\t(arr, serverIdle) b4_2. Next departure will happen at time: " << this->clock + nextServiceTime << "\n\n";
    // std::cout << "\t\t(arr, serverIdle) b4_3. Next departure takes a total of   : " << nextServiceTime << "\n\n";
    scheduleEvent(eventTypeEnums::DEP, this->clock + currentServiceTime);
    // stats.incrementWorkTime(currentServiceTime);
    //stats.incrementTurnaroundTime(this->clock + currentServiceTime - currentlyRunningProcess->getArrivalTime());

    this->totalDepartures++;
    // std::cout << "\t\t(arr, serverIdle) Current event queue: ";
    // printEvents();
    
  } else {
    // std::cout << "Server is currently busy, adding to back of readyQueueCount\n\n";
    //readyQueueCount++;
    processReadyQueue.push_back(newProcess);
    // std::cout << "\t\t(arr, active) b5_2. Process Ready Queue: ";
    // printProcessReadyQueue();

  }

  //float nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  if(this->totalProcesses < PROCESSCOUNT) {
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    // std::cout << "\t\t(arr, create) b6_1. Next arrival will happen at time: " << this->clock + nextArrivalTime << "\n\n";
    // std::cout << "\t\t(arr, create) b6_2. Next arrival takes a total of   : " << nextArrivalTime << "\n\n";
    this->totalArrivals++;
    // std::cout << "\t\t(arr) b6_3. Current event queue: ";
    // printEvents();
  } else {
    //delete newProcess;
    // std::cout << "\t\t(arr, complete) b7_1. All processes have been created.\n\n";
  }
  // if (this->totalProcesses < PROCESSCOUNT) {
  //   float nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  //   // std::cout << "\t\t(arr) Next arrival will happen at time: " << this->clock + nextArrivalTime << "\n\n";
  //   // std::cout << "\t\t(arr) Next arrival takes a total of   : " << nextArrivalTime << "\n\n";
  //   scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
  //   this->totalProcesses++;
  //   this->totalArrivals++;
  //   // std::cout << "\t\t(arr) Current event queue: ";
  //   printEvents();
  // }
}

void Driver::departureHander(Event e) {
  if (processReadyQueue.empty()){//&& readyQueueCount == 0) {//readyQueueCount == 0) {
    // std::cout << "\t\t(dep, empty) c1_1. No processes are ready to run, server is idle.\n\n";
    Process* completedProcess(this->currentlyRunningProcess);
    this->currentlyRunningProcess = nullptr;
    //serverIdle = true;
    // std::cout << "\t\t(dep, empty) c1_2. Adding the following process to completion queue...\n\n";
    completedProcess->setCompletionTime(this->clock);
    if(arrivalLambda==3) {printProcess(completedProcess);}
    stats.incrementWorkTime(completedProcess->getServiceTime());
    stats.incrementTurnaroundTime(completedProcess->getCompletionTime() - completedProcess->getArrivalTime());
    stats.incrementWaitingTime(
      completedProcess->getCompletionTime() - completedProcess->getServiceTime() - completedProcess->getArrivalTime()
    );
    // std::cout << "\t\t(dep, empty) c1_3. completed process completion time == clock time: " << (completedProcess->getCompletionTime() == getClock()) << "\n";
    this->completionList.push_back(completedProcess);
    if (arrivalLambda == 3) {
      printProcess(completedProcess);
    }
    //delete completedProcess;

  } else {
    //readyQueueCount--;
    currentlyRunningProcess = processReadyQueue[0];
    processReadyQueue.erase(processReadyQueue.begin());
    // std::cout << "\t\t (dep, occupied) c2_1. Moving next process from ready queue to run.\n\n";
    float nextServiceTime = std::round(currentlyRunningProcess->getServiceTime());
    // std::cout << "\t\t (dep, occupied) c2_2. Next departure will happen at time: " << this->clock + nextServiceTime << "\n";
    // std::cout << "\t\t (dep, occupied) c2_3. Next departure takes a total of   : " << nextServiceTime << "\n\n";
    scheduleEvent(eventTypeEnums::DEP, this->clock + nextServiceTime);
    // std::cout << "\t\t(dep), occupied c2_4. Process Ready Queue: ";
    //printProcessReadyQueue();
    //MARKstats.incrementWorkTime(nextServiceTime);
    //stats.incrementTurnaroundTime(this->clock + nextServiceTime - currentlyRunningProcess->getArrivalTime());
    this->totalDepartures++;
    // std::cout << "\t\t(dep, occupied) c2_5. Current event queue: ";
    // printEvents();
  }
}

float Driver::getClock() {
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
        std::cout << "(" << process->getArrivalTime() << ", " << process->getServiceTime() << ")}\n\n";
      } else {
        std::cout << "(" << process->getArrivalTime() << ", " << process->getServiceTime() << "), ";
      }
    }
  }
}

void Driver::printCompletionList() {
  for(Process* process: completionList) {
      std::cout << "\t\tProcess id: " << process->getID();
      std::cout << "\n\t\tProcess arrival time: " << process->getArrivalTime();
      std::cout << "\n\t\tProcess service time: " << process->getServiceTime();
      std::cout << "\n\t\tProcess completion time: " << process->getCompletionTime();
      std::cout << "\n\t\tProcess Turnaround Time: " << process->getCompletionTime() - process->getArrivalTime();
      std::cout << "\n\t\tProcess Waiting Time: " << process->getCompletionTime() - process->getServiceTime() - process->getArrivalTime();
      std::cout << "\n\n";
  }
}

void Driver::printProcess(Process* process) {
  if (process != nullptr) {
    std::cout << "\t\tProcess id: " << process->getID();
    std::cout << "\n\t\tProcess arrival time: " << (unsigned long long)process->getArrivalTime();
    std::cout << "\n\t\tProcess service time: " << (unsigned long long)process->getServiceTime();
    std::cout << "\n\t\tProcess completion time: " << (unsigned long long)process->getCompletionTime();
    std::cout << "\n\t\tProcess Turnaround Time: " << (unsigned long long)process->getCompletionTime() - process->getArrivalTime();
    std::cout << "\n\t\tProcess Waiting Time: " << (unsigned long long)process->getCompletionTime() - process->getServiceTime() - process->getArrivalTime();
    std::cout << "\n\n";
  } else {
    std::cout << "\t\tNo process currently running.\n\n";
  }
}


