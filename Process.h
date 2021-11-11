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
      unsigned long returnTime = 0;
    public:
      Process(int processID, unsigned long arrivalTime, unsigned long serviceTime);
      int getID();
      unsigned long getArrivalTime();
      unsigned long getServiceTime();
      unsigned long getRemainingServiceTime();
      void setRemainingServiceTime(float timeSlice);
      unsigned long getCompletionTime();
      unsigned long getStartRunTime();
      void setRemainingServiceTime(float timeSlice);
      void setCompletionTime(float completionTime);
      void setArrivalTime(float arrivalTime);
<<<<<<< HEAD
      void setReturnTime(unsigned long);
      unsigned long getReturnTime();
=======
      void setStartRunTime(float startTime);
>>>>>>> 4cb93ac84190b9f3732aee6cb138df1031f54908
};

#endif
