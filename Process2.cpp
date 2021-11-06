#include "Process2.h"

void Process2::setStartTime(float startTime) {
    this->startTime = startTime;
}

void Process2::setEndTime(float endTime) {
    this->endTime = endTime;
}

float Process2::getStartTime() {
    return this->startTime;
}

float Process2::getEndTime() {
    return this->endTime;
}

float Process2::getArrivalTime() {
    return this->arrivalTime;
}

float Process2::getServiceTime() {
    return this->serviceTime;
}
