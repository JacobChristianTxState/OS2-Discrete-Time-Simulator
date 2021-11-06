#ifndef DRIVER_H
#define DRIVER_H

#include "DistributionGenerator.h"
#include "Process.h"
#include "Process2.h"
#include "Event.h"
#include <vector>
#include <cmath>

#define ARR 1
#define DEP 2

class Driver {
    private:
      DistributionGenerator arrivalTime;
      DistributionGenerator serviceTime;
      std::vector<Process> processQueue;
      std::vector<Event> eventList;
      unsigned long clock;
      Process* activeProcess;
      Process createProcess();

    public:
      double averageArrivalTime;
      Driver(DistributionGenerator, DistributionGenerator);
      void run();
      void init();
      void scheduleEvent(int, int, Process*);
      void scheduleEvent(int , int);
      void arriverHandler(Event e);
      void departureHander(Event e);

      //test
      int numOfProcesses = 10000;
      float totalWorkDone;
      float totalTurnAroundTime;

      std::vector<Process2> process2Queue;
      void generateProcesses();
      void runSimulationStatistics();
      float generateAverageTurnaroundTime();
      float generateCpuUitilization();
      float generateThroughput();
      int returnClock();
      void displayList();
      bool compareTimes();


};
// #include "DistributionGenerator.h"
// #include "Event.h"
// #include "Process.h"
// #include <vector>

// class Driver{
//   private:
//     bool serveridle;
//     int algorithm_id;
//     int process_count;
//     float quantum;
//     float lambda;
//     float clock;
//     std::vector<Process> processReadyQueue;
//     std::vector<Event> eventList;
//     DistributionGenerator arrivalTime;
//     DistributionGenerator serviceTime;
//     void scheduleProcess(Process);
//     void determineLocation(Process);
//     void handleEvent();
//     float calcProcessTime();
//     void handleArr();
//     void handleDep();
//   public:
//     Driver(float, int, float);
//     void run();
// };

#endif
