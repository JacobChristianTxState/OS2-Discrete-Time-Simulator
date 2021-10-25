//Made by: Jacob Christian (jbc160)


#include "DistributionGenerator.h"
#include <iostream>

DistributionGenerator::DistributionGenerator(float lambda) {
    this->lambda = lambda;
}

float DistributionGenerator::uniformRandom() {
    return( (float) rand()/RAND_MAX );
}

int DistributionGenerator::generateExponentialDist() {
    float x = 0;
    while (x == 0) {   
        x = -(1/this->lambda)*log(uniformRandom());
    }
    return (int) std::round(x * 1000);

}