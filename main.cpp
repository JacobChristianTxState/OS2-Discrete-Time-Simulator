#include "Event.h"
#include "Driver.h"
#include "DistributionGenerator.h"
#include "Process.h"

int main(){
  Driver driver = Driver(.06, 1, 1, 2);
  driver.run();
  std::cout << "Out of Run\n";
  return 0;
}
