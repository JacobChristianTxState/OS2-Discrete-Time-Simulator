#ifndef PROCESS
#define PROCESS

class Process{
  private:
    static int processCount;
    int id;
    float arrivalTime;
    float serviceTime;
    float remainingServiceTime;
    int priority;
  public:
    //Constructor takes an id, an arrivalTime, a serviceTime, and a priority
    Process(int, float, float, int);
    Process(const Process &);
    Process(float, float);
    //Getters
    int getID();
    float getArrivalTime();
    float getServiceTime();
    float getRemainingServiceTime();
    int getPriority();
    //Setters
    void setRemainingServiceTime(float);
    /* Sets the arrival time of the process*/
    void setArrivalTime(float);
    void setPriority(int);
    void updateProcessServiceCount();
    int getProcessCount();
};

#endif
