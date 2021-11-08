#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"
#include "EventTypeEnum.h"

float arrivalLambda;
float serviceLambda;
const int PROCESSCOUNT = 10000;

int main(int argc, char *argv[]) {
    arrivalLambda = atof(argv[2]);
    serviceLambda = atof(argv[3]);
    DistributionGenerator arrivalTime(arrivalLambda, eventTypeEnums::ARR);
    DistributionGenerator serviceTime(serviceLambda, eventTypeEnums::DEP);
    Driver driver(arrivalTime, serviceTime);
    driver.init();
    driver.run();
    std::cout << "\t\t\tFinal time: " << driver.getClock() << "\n\n";
    return 0;
}
