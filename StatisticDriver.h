#ifndef STATISTICDRIVER_
#define STATISTICDRIVER_

#include <iostream>

//from main.cpp
extern const int PROCESSCOUNT;
extern float arrivalLambda; 

class StatisticDriver {
    private:
        float lambda;
        float accumulatingWorkTime;
        float simulatorClock;
        float accumulatingTurnaroundTime;
        float accumulatingWaitingTime;
    public:
        StatisticDriver();
        void incrementWorkTime(float work);
        void incrementClock(float clock);
        void incrementTurnaroundTime(float turnAroundTime);
        void incrementWaitingTime(float waitingTime);
        float getAverageWaitingTime();
        float getCpuUitlization();
        float getAverageTurnaroundTime();
        float getAverageQueueLength();
        float getThroughput();
};

#endif //STATISTICDRIVER_

// Waiting time = completion time - arrival time - service time

// CPU utilization = total work / final clock
// Throughput = total number of processes / final clock
// Turnaround Time = Service time - arrival time
// Average queue length = (average arrival time (lambda))(average waiting time)