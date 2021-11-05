//Made by: Jacob Christian (jbc160)


#include "DistributionGenerator.h"
#include <iostream>

DistributionGenerator::DistributionGenerator(float lambda) {
    this->lambda = lambda;
}

float DistributionGenerator::uniformRandom() {

    return( (float) rand()/RAND_MAX );
}

float DistributionGenerator::generateExponentialDist() {
    float x = 0;
    while (x == 0) {   
        x = -(1/this->lambda)*log(uniformRandom());
    }
   // std::cout << x << "\t" << x * 1000 << std::endl; 
    return (float) std::round(x * 1000);

}