#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"

int main(){
  DistributionGenerator gen = DistributionGenerator(1);
  Driver driver = Driver(1, 1, 2);
  driver.run();
}
