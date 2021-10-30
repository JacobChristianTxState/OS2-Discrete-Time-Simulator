#ifndef DRIVER_H
#define DRIVER_H

#define ARR 0
#define DEP 1

#include "DistributionGenerator.h"
#include "Process.h"
#include "Event.h"
#include <vector>

class Driver
{
    public:

        std::vector <Event> events;
        std::vector <Process *> ready_queue;
        int clock;
        int process_count;
        float service_time_constant;
        Process *current_process;
        DistributionGenerator arrivalTime;
        DistributionGenerator serviceTime;

        Driver(DistributionGenerator *, DistributionGenerator * );
        void init();
        void schedule_event(int, float);
        void run(int, int);
        void arr_handler(Event, Process *);
        void dep_handler(Event, Process *);
        void remove_event();
        void remove_process();
        void setArrivalTime(DistributionGenerator);
        void setServiceTime(DistributionGenerator);
        void setCurrentProcess(Process);
        Process getCurrentProcess();
        


    protected:

    private:
};

#endif // DRIVER_H
