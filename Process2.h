#ifndef PROCESS2
#define PROCESS2

class Process2 {
    private:
        int totalNumOfProcesses;
        unsigned long arrivalTime = 0;
        unsigned long serviceTime = 0;
        unsigned long startTime = 0;
        unsigned long endTime = 0;
    
    public:
        Process2(int at, int st) : arrivalTime(at), serviceTime(st) {}
        unsigned long getArrivalTime();
        unsigned long getServiceTime();
        unsigned long getStartTime();
        unsigned long getEndTime();
        void setStartTime(unsigned long startTime);
        void setEndTime(unsigned long endTime);
};

#endif //process 2