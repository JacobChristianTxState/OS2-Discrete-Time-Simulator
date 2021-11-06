#include "Driver.h"
#include <iostream>
#include <algorithm>

void Driver::init() {
  this->clock = 0;
  generateProcesses();
  std::sort(process2Queue.begin(), process2Queue.end(), 
    [](Process2 p1, Process2 p2)  {
        return p1.getArrivalTime() < p2.getArrivalTime();
    });
  process2Queue.front().setStartTime(
    process2Queue.front().getArrivalTime()
  );
  process2Queue.front().setEndTime(
    process2Queue.front().getArrivalTime() + process2Queue.front().getServiceTime()
  );
  
}

void Driver::generateProcesses() {
  for (int i = 0; i < this->numOfProcesses; i++) {
    process2Queue.push_back(
      Process2(
        std::round(arrivalTime.generateExponentialDist()),
        std::round(serviceTime.generateExponentialDist())
      )
    );
  }
}

void Driver::run() {
  for(size_t i = 1; i < process2Queue.size(); i++) {
    Process2* currentProcess = &process2Queue[i];
    Process2* previousProcess = &process2Queue[i-1];
    this->clock = previousProcess->getEndTime();
    if (this->clock < currentProcess->getArrivalTime()) {
      this->clock = currentProcess->getArrivalTime();
    }
    currentProcess->setStartTime(this->clock);
    currentProcess->setEndTime(this->clock + currentProcess->getServiceTime());
  }
  //get last process
  this->clock = process2Queue.back().getEndTime();
}

void Driver::runSimulationStatistics() {
  for (Process2& process : process2Queue) {
    float processTurnAroundTime = process.getEndTime() - process.getArrivalTime();
    this->totalTurnAroundTime += processTurnAroundTime;
    this->totalWorkDone += process.getServiceTime();
  }
}

int Driver::returnClock() {
  return this->clock;
}

void Driver::displayList() {
  for (size_t i = 0; i < process2Queue.size(); i++) {
    std::cout << "Process " << i << ":\n";
    std::cout << "\n\tArrival Time: " << process2Queue[i].getArrivalTime();
    std::cout << "\n\tService Time: " << process2Queue[i].getServiceTime();
    std::cout << "\n\t  Start Time: " << process2Queue[i].getStartTime();
    std::cout << "\n\t    End Time: " << process2Queue[i].getEndTime();
    std::cout << "\n\n";
  }
}

float Driver::generateAverageTurnaroundTime() {
  return this->totalTurnAroundTime / this->numOfProcesses;
}

float Driver::generateCpuUitilization() {
  return (this->totalWorkDone / this->clock)*100;
}

float Driver::generateThroughput() {
  return ((float)this->numOfProcesses / (float)this->clock) * 1000;
}
Driver::Driver(DistributionGenerator arrivalTime, DistributionGenerator serviceTime) : 
arrivalTime(arrivalTime), serviceTime(serviceTime) {
  this->averageArrivalTime = 0.0;
}

// void Driver::init() {
//   this->clock = 0;
//   this->readyQueueCount = 0;
//   this->serverIdle = true;
//   float initialArrivalTime = arrivalTime.generateExponentialDist();
//   //std::cout << "\tFIRST INITIAL ARRIVAL TIME: " << initialArrivalTime << " ms.\n\n";
//   averageArrivalTime += initialArrivalTime;
//   scheduleEvent(ARR, initialArrivalTime);
//   // this->clock = 0;
//   // this->activeProcess = nullptr;
//   // Process initialProcess = createProcess();
//   // this->processQueue.push_back(initialProcess);
//   // float arrivalTime = this->arrivalTime.generateExponentialDist();
//   // initialProcess.setArrivalTime(arrivalTime);
//   // scheduleEvent(ARR, arrivalTime, nullptr);
// }

// void Driver::run() {
//   while (processes > 0) {
//     Event e = eventList[0];
//     this->clock = e.getTime();
//     this->finalClock = this->clock;
//    // std::cout << "\n** Current clock: " << this->clock << " ms **\n";

//     switch(e.getType()) {
//       case ARR:
//         arriverHandler(e);
//         break;
//       case DEP:
//         departureHander(e);
//         break;
//     }
//     eventList.erase(eventList.begin());
//   }
// }

// void Driver::scheduleEvent(int type, int time, Process* process) {
//   this->eventList.push_back(Event(time, type, process));
// }

// void Driver::scheduleEvent(int type, int time) {
//   this->eventList.push_back(Event(time, type));
// }

// void Driver::arriverHandler(Event e) {
//   if (this->serverIdle) {
//     //std::cout << "\ta) Server is currently empty, scheduling a departure.\n";
//     this->serverIdle = false;
//     float nextServiceTime = serviceTime.generateExponentialDist();
//     this->totalProcessTime += nextServiceTime;
//     //std::cout << "\t\t The upcoming process' service time is: " << nextServiceTime << " ms.\n";
//     scheduleEvent(DEP, this->clock + nextServiceTime);
//   } else {
//     std::cout << "\tb) Server is not empty, adding to readyQueueCount.\n";
//     this->readyQueueCount++;
//   }
//   //std::cout << "\tc) Scheduling a new arrival.\n";
//   float nextArrivalTime = arrivalTime.generateExponentialDist();
//   this->averageArrivalTime += nextArrivalTime;
//  // std::cout << "\t\t The upcoming process' arrival time is: " << nextArrivalTime << " ms.\n";
//   scheduleEvent(ARR, this->clock + arrivalTime.generateExponentialDist());
//   processes--;
  
// }

// // void Driver::arriverHandler(Event e) {
// //   if (activeProcess == nullptr) {
// //     Process headProcess = processQueue[0];
// //     processQueue.erase(processQueue.begin());
// //     scheduleEvent(DEP, this->clock + headProcess.getArrivalTime(), &headProcess);
// //   } else {
// //     processQueue.push_back(createProcess());
// //   }
// //   scheduleEvent(ARR, )
// // }

// void Driver::departureHander(Event e) {
//   if (this->readyQueueCount == 0) {
//     //std::cout << "\ta) readyQueueCount is empty, setting serverIdle to true.\n";
//     this->serverIdle = true;
//   } else {
//     //std::cout << "\tb) readyQueueCount is not empty, decrementing readyQueueCount and scheduling a departure.\n";
//     this->readyQueueCount--;
//     float nextServiceTime = this->clock + serviceTime.generateExponentialDist();
//     this->totalProcessTime += nextServiceTime;
//    // std::cout << "\t\t The upcoming process' service time is: " << nextServiceTime << " ms.\n";
//     scheduleEvent(DEP, nextServiceTime);
//   }
// }

// Process Driver::createProcess() {
//     return Process(arrivalTime.generateExponentialDist(), serviceTime.generateExponentialDist());

// }

// // #include "Driver.h"

// // Driver::Driver(float lambda, int a_id, float quantum): serviceTime(lambda), arrivalTime(lambda){
// //   algorithm_id = a_id;
// //   serveridle = true;
// //   this->quantum = quantum;
// //   lambda = lambda;
// //   clock = 0;
// // }

// // void Driver::run(){
// //   for(int i = 0; i != 30; i++){
// //     int process_count = 0;
// //     Event e = Event(arrivalTime.generateExponentialDist(), true);
// //     eventList.push_back(e);
// //     while(process_count <= 10000){
// //       handleEvent();
// //       eventList.erase(eventList.begin());
// //     }
// //   }
// // }

// // void Driver::handleEvent(){
// //   //handling arrival
// //   if(eventList[0].getType()){
// //     handleArr();
// //   }else{
// //     handleDep();
// //   }
// //   eventList.erase(eventList.begin());
// // }

// // void Driver::handleArr(){
// //   float arrTime = arrivalTime.generateExponentialDist();
// //   if(serveridle == true){
// //     serveridle = false;
// //     float duh = calcProcessTime();
// //     Event dep = Event(duh, false);
// //     eventList.push_back(dep);
// //   }else{
// //     Process p = Process(0, arrTime, serviceTime.generateExponentialDist(), 0);
// //     //scheduleProcess
// //     scheduleProcess(p);
// //   }
// //   Event arr = Event(clock + arrivalTime.generateExponentialDist(), true);
// //   eventList.push_back(arr);
// // }

// // void Driver::handleDep(){
// //   if(processReadyQueue.empty()){
// //     serveridle = true;
// //   }else{
// //     Event dep = Event(clock + calcProcessTime(), false);
// //     eventList.push_back(dep);
// //     processReadyQueue.erase(processReadyQueue.begin());
// //   }
// // }

// // float Driver::calcProcessTime(){
// //   switch(algorithm_id){
// //     case 3:
// //       return 0.0;
// //       break;
// //     case 4:
// //       return 0.0;
// //       break;
// //     default:
// //       process_count++;
// //       return processReadyQueue[0].getRemainingServiceTime();
// //       break;
// //   }
// // }

// // void Driver::scheduleProcess(Process p){
// //   switch(algorithm_id){
// //     case 2:
// //       break;
// //     default:
// //       processReadyQueue.push_back(p);
// //       break;
// //   }
// // }
