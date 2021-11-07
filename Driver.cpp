#include "Driver.h"

Driver::Driver(DistributionGenerator at, DistributionGenerator st) : arrivalTime(at), serviceTime(st) {}

void Driver::scheduleEvent(int eventType, float time) {
  Event event(time, eventType);
  this->eventCount++;
  //std::cout << "\t\t\t(schedule) Creating the following event: ";
  printEvent(event);
  eventQueue.push_back(event);
  std::sort(
    eventQueue.begin(), 
    eventQueue.end(),
    [](Event& e1, Event& e2) -> bool {return e1.getTime() < e2.getTime();});
}

void Driver::init() {
  this->clock = 0.0;
  this->serverIdle = true;
  this->readyQueueCount = 0;
  float initialArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
  //std::cout << "Initial arrival time: " << initialArrivalTime << "\n";
  //std::cout << "\n";
  this->totalProcesses++;
  this->totalArrivals++;
  scheduleEvent(eventTypeEnums::ARR, initialArrivalTime);
}

void Driver::run() {
  this->logger.OpenFile();
  while(totalProcesses < PROCESSES || eventQueue.size() > 0){//} || !this->eventQueue.empty()) {  //run for 10 processes
    Event e = eventQueue[0];
    this->clock = e.getTime();
    stats.incrementClock(getClock());
    //std::cout << "(run) Current event in run: ";
    printEvent(e);
    switch(e.getType()) {
      case eventTypeEnums::ARR:
        //std::cout << "\t(run, arr) Handling arrival...\n\n";
        arriveHandler(e);
        break;
      case eventTypeEnums::DEP: 
        //std::cout << "\t(run, dep) Handling departure...\n\n";
        departureHander(e);
        break;
    }
    //std::cout << "(run) Erasing head from queue.\n\n";
    eventQueue.erase(eventQueue.begin());
    //std::cout << "(run) Current event queue: ";
    printEvents();
  }
  std::cout << "\tTotal arrivals: " << this->totalArrivals << "\n";
  std::cout << "\tTotal departures: " << this->totalDepartures << "\n";
  std::cout << "\tTotal processes: " << this->totalProcesses << "\n";
  std::cout << "\tTotal events: " << this->eventCount << "\n";
  std::stringstream ss;
  ss << std::to_string(arrivalLambda) << "," << std::to_string(stats.getThroughput()) << "," << std::to_string(stats.getCpuUitlization());
  this->logger.WriteToFile(ss.str());
  this->logger.CloseFile();

}

void Driver::arriveHandler(Event e) {
  if(serverIdle) {
    serverIdle = false;
    float nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
    // std::cout << "\t\t(arr, serverIdle) Next departure will happen at time: " << this->clock + nextServiceTime << "\n\n";
    // std::cout << "\t\t(arr, serverIdle) Next departure takes a total of   : " << nextServiceTime << "\n\n";

    scheduleEvent(eventTypeEnums::DEP, this->clock + nextServiceTime);
    stats.incrementWorkTime(nextServiceTime);
    this->totalDepartures++;
    // std::cout << "\t\t(arr, serverIdle) Current event queue: ";
    printEvents();
    
  } else {
    // std::cout << "\t\t(arr) Server is currently busy, adding to back of readyQueueCount\n\n";
    readyQueueCount++;
  }
  if (this->totalProcesses < PROCESSES) {
    float nextArrivalTime = std::round(this->arrivalTime.generateExponentialDist());
    // std::cout << "\t\t(arr) Next arrival will happen at time: " << this->clock + nextArrivalTime << "\n\n";
    // std::cout << "\t\t(arr) Next departure takes a total of   : " << nextArrivalTime << "\n\n";
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextArrivalTime);
    this->totalProcesses++;
    this->totalArrivals++;
    // std::cout << "\t\t(arr) Current event queue: ";
    printEvents();
  }
  
}

void Driver::departureHander(Event e) {
  if (readyQueueCount == 0) {
    // std::cout << "\t\t(dep) No processes are ready to run, server is idle.\n\n";
    serverIdle = true;
  } else {
    readyQueueCount--;
    // std::cout << "\t\t (dep) Moving next process from ready queue to run.\n\n";
    float nextServiceTime = std::round(this->serviceTime.generateExponentialDist());
    // std::cout << "\t\t (dep) Next departure will happen at time: " << this->clock + nextServiceTime << "\n";
    // std::cout << "\t\t (dep) Next departure takes a total of   : " << nextServiceTime << "\n\n";
    scheduleEvent(eventTypeEnums::ARR, this->clock + nextServiceTime);
    stats.incrementWorkTime(nextServiceTime);
    this->totalDepartures++;
    // std::cout << "\t\t(dep) Current event queue: ";
    printEvents();
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
  // std::string type;
  //   if ( e.getType() == 1) {
  //     type = "ARR";
  //   } else {
  //     type = "DEP";
  //   }
  // std::cout << "Event: (" << type << ", " << e.getTime() << ")\n\n";
}

void Driver::printEvents() {
  // std::cout << "{";
  // if (eventQueue.empty()) {
  //   std::cout << "}\n";
  // } else {
  //   for(Event& event: eventQueue) {
  //     std::string type;
  //     if (event.getType() == 1) {
  //       type = "ARR";
  //     } else {
  //       type = "DEP";
  //     }
  //     if (&event == &eventQueue.back()) {
  //       std::cout << "(" << type << ", " << event.getTime() << ")}\n\n";
  //     } else {
  //       std::cout << "(" << type << ", " << event.getTime() << "), ";
  //     }
  //   }
  // }
}

