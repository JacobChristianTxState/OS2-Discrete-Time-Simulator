//Made by: Jacob Christian (jbc160)

#ifndef DISTRIBUTIONGENERATOR_
#define DISTRIBUTIONGENERATOR_

#include <cmath>    //for std::round()
#include <cstdlib>  //for rand(), RAND_MAX

class DistributionGenerator {
    private:
        float lambda;
        float uniformRandom();
    public:
        DistributionGenerator(float lambda);
        float generateExponentialDist();
};

#endif //DISTRIBUTIONGENERATOR_