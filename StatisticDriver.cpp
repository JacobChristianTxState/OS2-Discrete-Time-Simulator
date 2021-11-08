#include "StatisticDriver.h"

StatisticDriver::StatisticDriver() {
    this->lambda = arrivalLambda;
    std::cout << "Arrival Lambda: " << this->lambda << "\n";
    this->accumulatingTurnaroundTime = 0.0;
    this->accumulatingWaitingTime = 0.0;
    this->accumulatingWorkTime = 0.0;
    this->simulatorClock = 0.0;
}

void StatisticDriver::incrementWorkTime(float work) {
    this->accumulatingWorkTime += work;
}

void StatisticDriver::incrementClock(float clock) {
    this->simulatorClock = clock;
}

void StatisticDriver::incrementTurnaroundTime(float turnAroundTime) {
    this->accumulatingTurnaroundTime += turnAroundTime;
}

void StatisticDriver::incrementWaitingTime(float waitingTime) {
    this->accumulatingWaitingTime += waitingTime;
}

float StatisticDriver:: getAverageWaitingTime() {
    return this->accumulatingWaitingTime / PROCESSCOUNT;
}

float StatisticDriver::getCpuUitlization() {
    return (this->accumulatingWorkTime / this->simulatorClock) * 100;   //multiply by 100 to get % value
}

float StatisticDriver::getAverageTurnaroundTime() {
    return (this->accumulatingTurnaroundTime / (PROCESSCOUNT*1000)); //convert ms to sec
}

float StatisticDriver::getAverageQueueLength() {
    return this->lambda * getAverageWaitingTime();
}

float StatisticDriver::getThroughput() {
    return (PROCESSCOUNT / this->simulatorClock) * 1000;   //convert ms to sec
}