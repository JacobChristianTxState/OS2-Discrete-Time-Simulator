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
        DistributionGenerator operator= (const DistributionGenerator&);
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
