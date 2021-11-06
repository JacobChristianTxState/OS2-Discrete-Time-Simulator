#include "Process2.h"

void Process2::setStartTime(unsigned long startTime) {
    this->startTime = startTime;
}

void Process2::setEndTime(unsigned long endTime) {
    this->endTime = endTime;
}

unsigned long Process2::getStartTime() {
    return this->startTime;
}

unsigned long Process2::getEndTime() {
    return this->endTime;
}

unsigned long Process2::getArrivalTime() {
    return this->arrivalTime;
}

unsigned long Process2::getServiceTime() {
    return this->serviceTime;
}
