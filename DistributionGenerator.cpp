//Made by: Jacob Christian (jbc160)


#include "DistributionGenerator.h"
#include <iostream>

DistributionGenerator::DistributionGenerator(float lambda, int distributionType) {
    this->lambda = lambda;
    this->distributionType = distributionType;
}

float DistributionGenerator::uniformRandom() {
    return( (float) rand()/RAND_MAX );
}

float DistributionGenerator::generateExponentialDist() {
    float lowerLimit, upperLimit;
    float x = 0;

    switch(distributionType) {
        case ARR:
            lowerLimit = (1/this->lambda) - (1/(2*this->lambda));
            upperLimit = (1/this->lambda) + (1/(2*this->lambda));
            //while (x == 0) { x = -(1/this->lambda)*log(uniformRandom());}
            break;
        case DEP:
            lowerLimit = 0.05;  
            upperLimit = 0.07;
            //wild random values for uniform genrator and interarrival times 1/0.06, keeps values close to 0.06
            //while (x < lowerLimit || x > upperLimit) { x = -(1/this->lambda)*log(uniformRandom());}
            break;
    }
    while ( x == 0 || x < lowerLimit || x > upperLimit) {
        x = -(1/this->lambda)*log(uniformRandom());
    }  
    return x * 1000;
}


