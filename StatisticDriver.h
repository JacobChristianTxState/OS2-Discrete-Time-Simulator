#ifndef STATISTICDRIVER_
#define STATISTICDRIVER_

#include "Process.h"

//from main.cpp
extern const int PROCESSCOUNT;
extern float arrivalLambda; 

class StatisticDriver {
    private:
        float lambda;
        unsigned long accumulatingWorkTime;
        unsigned long simulatorClock;
        unsigned long accumulatingTurnaroundTime;
        unsigned long accumulatingWaitingTime;
    public:
        StatisticDriver();
        void incrementWorkTime(unsigned long work);
        void incrementClock(unsigned long clock);
        void incrementTurnaroundTime(unsigned long turnAroundTime);
        void incrementWaitingTime(unsigned long waitingTime);
        float getAverageWaitingTime();
        float getCpuUitlization();
        float getAverageTurnaroundTime();
        float getAverageQueueLength();
        float getThroughput();
        unsigned long getFinalTime();
        unsigned long getFinalWorkTime();
        void collectDepartureStats(Process departingProcess);
};

#endif //STATISTICDRIVER_

/* NOTES FOR STATISTICAL DATA */
// Waiting time = completion time - arrival time - service time
// CPU utilization = total work / final clock
// Throughput = total number of processes / final clock
// Turnaround Time = Service time - arrival time
// Average queue length = (average arrival time (lambda))(average waiting time)