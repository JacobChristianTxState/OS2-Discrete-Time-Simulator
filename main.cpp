#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"

#define ARR 1
#define DEP 2

int main(){

    for (int i = 1; i <= 1; i++) {
        DistributionGenerator arrivalTime(30, ARR);
        DistributionGenerator serviceTIme(1/0.6, DEP);
        Driver driver(arrivalTime, serviceTIme);
        driver.init();
        driver.run();
        std::cout << "\n\n\t\t\tFinal time: " << driver.getClock() << "\n";
    }
  
  return 0;
}
