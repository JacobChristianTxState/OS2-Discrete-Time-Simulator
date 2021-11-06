#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"

int main(){
  DistributionGenerator gen = DistributionGenerator(.06);
  Driver driver = Driver(.06, 1, 2);
  driver.run();
}
