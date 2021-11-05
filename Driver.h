#ifndef DRIVER_H
#define DRIVER_H
#include "DistributionGenerator.h"
#include "Event.h"
#include "Process.h"
#include <vector>

class Driver{
  private:
    bool serveridle;
    int algorithm_id;
    int process_count;
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
  public:
    Driver(float, int, float);
    void run();
};

#endif
