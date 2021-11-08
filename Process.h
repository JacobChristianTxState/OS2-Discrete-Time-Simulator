#ifndef PROCESS
#define PROCESS

class Process{
    private:
      int processID = -1;
      unsigned long arrivalTime = -1;
      unsigned long serviceTime = -1;
      unsigned long remainingServiceTime = -1;
      unsigned long completionTime = -1;
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
};

#endif
