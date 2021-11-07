#ifndef DRIVER_H
#define DRIVER_H

#include "DistributionGenerator.h"
#include "Event.h"
#include "Process.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#define ARR 1
#define DEP 2
#define PROCESSES 5

class Driver{
  private:
    DistributionGenerator arrivalTime;
    DistributionGenerator serviceTime;
    std::vector<Event> eventQueue;
    bool serverIdle;
    int readyQueueCount;
    int totalProcesses = 0;
    int totalArrivals = 0;
    int totalDepartures = 0;
    int eventCount = 0;
    float clock;

  public:
    Driver(DistributionGenerator, DistributionGenerator);
    void init();
    void run();
    void arriveHandler(Event e);
    void departureHander(Event e);
    void scheduleEvent(int, float);
    float getClock();
    int getReadyQueueCount();
    bool getServerIdleStatus();
    void printEvents();
    void printEvent(Event e);

};
  
#endif