#ifndef PROCESS
#define PROCESS

class Process{
    private:
      int processID = -1;
      unsigned long arrivalTime;
      unsigned long serviceTime;
      unsigned long remainingServiceTime = 0;
      unsigned long startRunTime=0;
      unsigned long completionTime;
    public:
      Process(int processID, unsigned long arrivalTime, unsigned long serviceTime);
      int getID();
      unsigned long getArrivalTime();
      unsigned long getServiceTime();
      unsigned long getRemainingServiceTime();
      unsigned long getCompletionTime();
      unsigned long getStartRunTime();
      void setRemainingServiceTime(float timeSlice);
      void setCompletionTime(float completionTime);
      void setArrivalTime(float arrivalTime);
      void setStartRunTime(float startTime);
};

#endif
