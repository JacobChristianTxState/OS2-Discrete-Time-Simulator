#ifndef PROCESS
#define PROCESS

class Process{
    private:
      int processID = -1;
      float arrivalTime = -1;
      float serviceTime = -1;
      float remainingServiceTime = -1;
      float completionTime = -1;
    public:
      Process(int processID, float arrivalTime, float serviceTime);
      int getID();
      float getArrivalTime();
      float getServiceTime();
      float getRemainingServiceTime();
      void setRemainingServiceTime(float timeSlice);
      float getCompletionTime();
      void setCompletionTime(float completionTime);
      void setArrivalTime(float arrivalTime);
};

#endif
