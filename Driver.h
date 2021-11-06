#ifndef DRIVER_H
#define DRIVER_H
#include "DistributionGenerator.h"
#include "Event.h"
#include "Process.h"
#include <vector>
#include <algorithm>
#include <iostream>

class Driver{
  private:
    bool serveridle;
    int algorithm_id;
    int process_count;
    int pid;
    float quantum;
    float lambda;
    float clock;
    std::vector<Process> processReadyQueue;
    std::vector<Event> eventList;
    DistributionGenerator arrivalTime;
    DistributionGenerator serviceTime;
    void scheduleProcess(Process);
    void determineLocation(Process);
    void handleEvent();
    float calcProcessTime();
    void handleArr();
    void handleDep();
    void scheduleEvent(Event);
    void printList();
  public:
    Driver(float, float, int, float);
    void run();
};

#endif
