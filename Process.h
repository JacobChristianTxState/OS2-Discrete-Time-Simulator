#ifndef PROCESS
#define PROCESS

class Process{
    private:
      int processID;
      float arrivalTime;
      float serviceTime;
      float remainingServiceTime;
    public:
      Process(int processID, float arrivalTime, float serviceTime);
      int getID();
      float getArrivalTime();
      float getServiceTime();
      float getRemainingServiceTime();
      void setRemainingServiceTime(float timeSlice);
  // public:
  //   int id;
  //   float arrivalTime;
  //   float serviceTime;
  //   float remainingServiceTime;
  //   int priority;
  //   //Constructor takes an id, an arrivalTime, a serviceTime, and a priority
  //   Process(int, float, float, int);
  //   Process(const Process &);
  //   //Getters
  //   int getID();
  //   float getArrivalTime();
  //   float getServiceTime();
  //   float getRemainingServiceTime();
  //   int getPriority();
  //   //Setters
  //   void setRemainingServiceTime(float);
  //   /* Sets the arrival time of the process*/
  //   void setArrivalTime(float);
  //   void setPriority(int);
};

#endif
