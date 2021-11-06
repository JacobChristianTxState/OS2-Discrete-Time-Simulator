#ifndef PROCESS2
#define PROCESS2

class Process2 {
    private:
        int totalNumOfProcesses;
        float arrivalTime = 0;
        float serviceTime = 0;
        float startTime = 0;
        float endTime = 0;
    
    public:
        Process2(int at, int st) : arrivalTime(at), serviceTime(st) {}
        float getArrivalTime();
        float getServiceTime();
        float getStartTime();
        float getEndTime();
        void setStartTime(float startTime);
        void setEndTime(float endTime);
};

#endif //process 2