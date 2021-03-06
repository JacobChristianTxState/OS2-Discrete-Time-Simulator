//Made by: Jacob Christian (jbc160)

#ifndef DISTRIBUTIONGENERATOR_
#define DISTRIBUTIONGENERATOR_

#include <cmath>    //for log()
#include <cstdlib>  //for rand(), RAND_MAX
#include "EventTypeEnum.h"


class DistributionGenerator {
    private:
        float lambda;
        int distributionType;
        float uniformRandom();
    public:
        DistributionGenerator(float lambda, int distributionType);
        unsigned long generateExponentialDist();
};

#endif //DISTRIBUTIONGENERATOR_
