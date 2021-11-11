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
  bool preemptiveSwap = false;;
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
    void arrivalHandlerFCFS(Event);
    void arrivalHandlerSRTF(Event);
    void arrivalHandlerRR(Event);
    void runDepartureFCFS(Event);
  void runDepartureRR(Event);
  void runDepartureSRTF(Event);
    //void runHandlerRR(Event);
    //void runHandlerSRTF(Event);
    //void departureHandler(Event);
    void scheduleNextEvent(int, unsigned long, Process*);
    unsigned long getClock();
    int getReadyQueueCount();
    bool getServerIdleStatus();
    void printEvents();
    void printProcessReadyQueue();
    void printEvent(Event );
    void printProcess(Process*);
    std::string transferDataResults();
    void scheduleArrival(Event);
    void scheduleRun(Event);
    void scheduleEvent(Event);
  Process* createNewProcess(unsigned long time);
  void preemptProcessReadyQueue();
  void scheduleDeparture(Event);
  

};
  
#endif
