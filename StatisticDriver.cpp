#include "StatisticDriver.h"

StatisticDriver::StatisticDriver() {
    this->lambda = arrivalLambda;
    this->accumulatingTurnaroundTime = 0;
    this->accumulatingWaitingTime = 0;
    this->accumulatingWorkTime = 0;
    this->simulatorClock = 0;
}

void StatisticDriver::incrementWorkTime(unsigned long work) {
    this->accumulatingWorkTime += work;
}

void StatisticDriver::incrementClock(unsigned long clock) {
    this->simulatorClock = clock;
}

void StatisticDriver::incrementTurnaroundTime(unsigned long turnAroundTime) {
    this->accumulatingTurnaroundTime += turnAroundTime;
}

void StatisticDriver::incrementWaitingTime(unsigned long waitingTime) {
    this->accumulatingWaitingTime += waitingTime;
}

float StatisticDriver:: getAverageWaitingTime() {
    return this->accumulatingWaitingTime / (1.0*PROCESSCOUNT);
}

float StatisticDriver::getCpuUitlization() {
    return 100*(this->accumulatingWorkTime / (1.0*this->simulatorClock));   //multiply by 100 to get % value
}

float StatisticDriver::getAverageTurnaroundTime() {
    return (this->accumulatingTurnaroundTime / (1000.0*PROCESSCOUNT)); //convert ms to sec
}

float StatisticDriver::getAverageQueueLength() {
    return this->lambda * getAverageWaitingTime();
}

float StatisticDriver::getThroughput() {
    return (float)((PROCESSCOUNT*1000) / (1.0*this->simulatorClock));   //convert ms to sec
}

unsigned long StatisticDriver::getFinalTime() {
    return this->simulatorClock;
}

void StatisticDriver::collectDepartureStats(Process departingProcess) {
    incrementWorkTime(departingProcess.getServiceTime());
    incrementTurnaroundTime(departingProcess.getCompletionTime() - departingProcess.getArrivalTime());
    incrementWaitingTime(
        departingProcess.getCompletionTime() - 
        departingProcess.getServiceTime() - 
        departingProcess.getArrivalTime()
    );
}