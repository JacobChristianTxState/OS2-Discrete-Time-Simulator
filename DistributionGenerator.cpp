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

unsigned long DistributionGenerator::generateExponentialDist() {
    float lowerLimit, upperLimit;
    float x = 0;

    switch(distributionType) {
        case eventTypeEnums::ARR:
            lowerLimit = (1/this->lambda) - (1/(2*this->lambda));
            upperLimit = (1/this->lambda) + (1/(2*this->lambda));
            //while (x == 0) { x = -(1/this->lambda)*log(uniformRandom());}
            break;
        case eventTypeEnums::DEP:
            lowerLimit = 0.02;  
            upperLimit = 0.1;
            //wild random values for uniform genrator and interarrival times 1/0.06, keeps values close to 0.06
            //while (x < lowerLimit || x > upperLimit) { x = -(1/this->lambda)*log(uniformRandom());}
            break;
    }
    while ( x == 0 || x < lowerLimit || x > upperLimit) {
        x = -(1/this->lambda)*log(uniformRandom());
    } 
    return (unsigned long)(x * 1000);
}


