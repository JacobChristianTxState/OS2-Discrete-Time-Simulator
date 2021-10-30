#include "Event.h"


Event::Event(int type, float time){
    this->type = type;
    this->time = time;
}

int Event::getType(){
    return type;
}

float Event::getTime(){
    return time;
}
void Event::setProcess(Process p){
    this->p = p;
}
Process Event::getProcess(){
    return this->p;
}