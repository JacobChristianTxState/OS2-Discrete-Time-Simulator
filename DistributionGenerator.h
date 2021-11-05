//Made by: Jacob Christian (jbc160)

#ifndef DISTRIBUTIONGENERATOR_
#define DISTRIBUTIONGENERATOR_

#include <cmath>    //for log()
#include <cstdlib>  //for rand(), RAND_MAX

class DistributionGenerator {
    private:
        float lambda;
        int distributionType;
        float uniformRandom();
    public:
        DistributionGenerator(float lambda, int distributionType);
        float generateExponentialDist();
};

#endif //DISTRIBUTIONGENERATOR_


/*
 * while 10000 process have not been handeled{
 *  generate process add it to process ready queue
 *  generate event give it process
 *  handle events
 *    if event is arrival
 *      subtract quantum/service time from remaining service time
 *      return
 *    else
 *      set server idle
 *  delete front process from queue
 * }
 */
