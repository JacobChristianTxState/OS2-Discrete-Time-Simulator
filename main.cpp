#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"

#define ARR 1
#define DEP 2

float arrivalLambda;
float serviceLambda;

int main(int argc, char *argv[]) {
    arrivalLambda = atof(argv[2]);
    serviceLambda = atof(argv[3]);
    DistributionGenerator arrivalTime(arrivalLambda, ARR);
    DistributionGenerator serviceTime(serviceLambda, DEP);
    Driver driver(arrivalTime, serviceTime);
    driver.init();
    driver.run();
    std::cout << "\t\t\tFinal time: " << driver.getClock() << "\n\n";

    return 0;
}
