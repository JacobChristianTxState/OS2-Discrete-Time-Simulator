#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"
#include "EventTypeEnum.h"

float arrivalLambda;
float serviceLambda;
const int PROCESSCOUNT = 10000;

int main(int argc, char *argv[]) {
    int scheduleType = std::stoi(argv[1]);
    arrivalLambda = std::stof(argv[2]);
    serviceLambda = std::stof(argv[3]);
    float quantum = std::stof(argv[4]);

    DistributionGenerator arrivalTime(arrivalLambda, eventTypeEnums::ARR);
    DistributionGenerator serviceTime(serviceLambda, eventTypeEnums::DEP);
    Driver driver(arrivalTime, serviceTime, scheduleType, quantum);
    driver.init();
    driver.run();
    return 0;
}
