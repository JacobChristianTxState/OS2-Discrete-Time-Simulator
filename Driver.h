#ifndef DRIVER_H
#define DRIVER_H

#include "DistributionGenerator.h"
#include "Event.h"
#include "Process.h"
#include "SimulatorLogger.h"
#include "StatisticDriver.h"
#include "EventTypeEnum.h"
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

//from main.cpp
extern const int PROCESSCOUNT;
extern float arrivalLambda; 

class Driver{
  private:
    DistributionGenerator arrivalTime;
    DistributionGenerator serviceTime;
    SimulatorLogger logger;
    StatisticDriver stats;
    std::deque<Event> eventQueue;
    std::deque<Process*> processReadyQueue;
    Process* currentlyRunningProcess;
    bool roundRobinInterruptedProcess;
    unsigned long roundRobinTimeAdjustment;
    bool serverIdle;
    int readyQueueCount;
    int totalProcesses = 0;
    int totalArrivals = 0;
    int totalDepartures = 0;
    int eventCount = 0;
    unsigned long clock;
    int scheduleType;
    float quantum;

  public:
    Driver(DistributionGenerator, DistributionGenerator, int scheduleType, float quantum);
    void init();
    void run();
    void arrivalHandlerFCFS(Event e);
    void arrivalHandlerSRTF(Event e);
    void arrivalHandlerRR(Event e);
    void runHandlerFCFS(Event e);
    void runHandlerRR(Event e);
    void runHandlerSRTF(Event e);
    void departureHandler(Event e);
    void scheduleEvent(int, unsigned long);
    unsigned long getClock();
    int getReadyQueueCount();
    bool getServerIdleStatus();
    void printEvents();
    void printProcessReadyQueue();
    void printEvent(Event e);
    void printProcess(Process* process);
    std::string transferDataResults();
    void scheduleArrival(Event e);
    void scheduleRun(Event e);
    void scheduleEvent(Event e);

};
  
#endif
