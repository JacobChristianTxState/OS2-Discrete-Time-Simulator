#include "Driver.h"

//my notes
//serveridle is now Process curr_process;
//readyQueueCount is now ready_queue

Driver::Driver(DistributionGenerator *arrivalTime, DistributionGenerator *serviceTime){
    
    this->arrivalTime = *arrivalTime;
    this->serviceTime = *serviceTime;

    init();
}
void Driver::init(){
    this->clock = 0;
    this->process_count = 0;
    current_process = nullptr;
}
void Driver::schedule_event(int type, float time){

    Event new_event = Event(type, time);
    events.push_back(new_event);
}
void Driver::run(int algorithm_id, int quantum){

    // switch statement here
    switch(algorithm_id){
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
    }
}

void Driver::arr_handler(Event e, Process *p){

    if(current_process == nullptr){
        current_process = p;
        schedule_event(DEP, clock + /*s*/);
    } else {
        ready_queue.push_back(current_process);

    }
    schedule_event(ARR, clock + /* a*/);
}

void Driver::dep_handler(Event e, Process *p){

    if(ready_queue.empty()){
        current_process = nullptr;

    } else {
        remove_process();
        schedule_event(DEP, clock + /* s*/);

        if(current_process->getRemainingServiceTime() == 0){
            process_count++;
        }
    }
}
void Driver::remove_event(){
    events.erase(events.begin());
}

void Driver::remove_process(){
    ready_queue.erase(ready_queue.begin());
}

}
void Driver::setCurrentProcess(Process current_process){
    this->current_process = &current_process;
}
Process Driver::getCurrentProcess(){
    return *current_process;
}


/*

 int id = 1;
    while(process_count < 10001 && !ready_queue.empty()){
        
        float local_arrival_time = arrivalTime.generateExponentialDist();
        float local_service_time = serviceTime.generateExponentialDist();
        Process temp_process(id, local_arrival_time, local_service_time, 1);
        schedule_event(ARR, local_arrival_time);
        events.back().p = temp_process;
        

        Event current_event = events[0];
        clock = current_event.time;
        if(current_event.type == ARR){
            arr_handler(current_event, &current_event.getProcess()); 
        } else if(current_event.type == DEP){
            dep_handler(current_event, current_process);
        }
        remove_event();
        id++;
    }
    */