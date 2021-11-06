#include <iostream>
#include "DistributionGenerator.h"
#include "Driver.h"
#define ARR 1
#define DEP 2

int main(int argc, char *argv[]) {

    std::cout << "\n\n\nCURRENTLY RUNNING LAMBDA = " << argv[2] << "\n";
    DistributionGenerator arrivalTime(atoi(argv[2]), ARR);
    DistributionGenerator serviceTime(1/atof(argv[3]), DEP);

    Driver driver(arrivalTime, serviceTime);
    driver.init();
    //driver.displayList();

    driver.run();
    driver.runSimulationStatistics();
    std::cout << "\n\nAverage CPU Utilization : " << driver.generateCpuUitilization() << " percent.\n";
    std::cout << "     Average Throughput : " << driver.generateThroughput() << " process/sec. \n";
    std::cout << "Average Turnaround Time : " << driver.generateAverageTurnaroundTime() << " ms.\n";
    std::cout << "       Final Clock Time : " << driver.returnClock() << " ms.\n";
    std::cout << "   Average Arrival Time : " << driver.generateAverageArrivalTime() << " ms.\n";

    return 0;
}