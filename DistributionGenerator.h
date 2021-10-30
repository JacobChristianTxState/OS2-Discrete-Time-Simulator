//Made by: Jacob Christian (jbc160)

#ifndef DISTRIBUTIONGENERATOR_
#define DISTRIBUTIONGENERATOR_

#include <cmath>    //for log()
#include <cstdlib>  //for rand(), RAND_MAX

class DistributionGenerator {
    private:
        float lambda;
        int distributionType;
        float min;
        float uniformRandom();
    public:
        DistributionGenerator(float lambda, int distributionType);
        float generateExponentialDist();
};

#endif //DISTRIBUTIONGENERATOR_