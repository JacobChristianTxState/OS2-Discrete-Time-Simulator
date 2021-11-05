#ifndef PROCESS
#define PROCESS

class Process{
  public:
    int id;
    float arrivalTime;
    float serviceTime;
    float remainingServiceTime;
    int priority;
    //Constructor takes an id, an arrivalTime, a serviceTime, and a priority
    Process(int, float, float, int);
    Process(const Process &);
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
};

#endif
