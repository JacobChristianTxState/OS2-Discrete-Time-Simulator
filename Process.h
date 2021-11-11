#ifndef PROCESS
#define PROCESS

#include <iostream>

class Process{
    private:
      int processID = -1;
      unsigned long arrivalTime;
      unsigned long serviceTime;
      unsigned long remainingServiceTime = 0;
      unsigned long completionTime;
    public:
      Process(int processID, unsigned long arrivalTime, unsigned long serviceTime);
      int getID();
      unsigned long getArrivalTime();
      unsigned long getServiceTime();
      unsigned long getRemainingServiceTime();
      void setRemainingServiceTime(float timeSlice);
      unsigned long getCompletionTime();
      void setCompletionTime(float completionTime);
      void setArrivalTime(float arrivalTime);
  void printProcessInformation();
};

#endif
