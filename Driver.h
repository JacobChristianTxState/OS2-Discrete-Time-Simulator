#ifndef DRIVER_H
#define DRIVER_H

#include "DistributionGenerator.h"
#include "Event.h"
#include "Process.h"
#include "SimulatorLogger.h"
#include "StatisticDriver.h"
#include "EventTypeEnum.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

extern const int PROCESSCOUNT;
extern float arrivalLambda; //from main.cpp

class Driver{
  private:
    DistributionGenerator arrivalTime;
    DistributionGenerator serviceTime;
    SimulatorLogger logger;
    StatisticDriver stats;
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