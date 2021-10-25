#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "DistributionGenerator.h"

int main() {

    float globalAverageArrivalTime = 0.0;
    float globalAverageServiceTime = 0.0;
    for (int i = 1; i< 30; i++) {   //run lambda 1 through 10

        DistributionGenerator arrivalTime(i);
        DistributionGenerator serviceTime(1/0.06);

        float localAvgArrivalTime = arrivalTime.generateExponentialDist();
        float localAvgServiceTime = serviceTime.generateExponentialDist();

        for (int j = 1; j < 10000; j++) {  //run 20 processes

            int currentArrivalTime = arrivalTime.generateExponentialDist();
            int currentServiceTime = serviceTime.generateExponentialDist();

            localAvgArrivalTime += currentArrivalTime;
            localAvgServiceTime += currentServiceTime;
        }
        globalAverageArrivalTime += localAvgArrivalTime;
        globalAverageServiceTime += localAvgServiceTime;
    }
    //200 total iterations (10 of i time 20 of j)
    globalAverageArrivalTime /= (30.0*10000.0); 
    globalAverageServiceTime /= (30.0*10000.0); 

    printf("Global Average Arrival Time: %f ms\n", globalAverageArrivalTime);
    printf("Global Average Service Time: %f ms\n", globalAverageServiceTime);

    return 0;
}

// float urand()
// {
// 	return( (float) rand() / (float)RAND_MAX );
// }

// float genexp(float lambda)
// {
// 	float x = 0;
// 	while (x == 0) { x = (-1/lambda)*log(urand());}
// 	return x;
// }

// int main() {
//     float global_average_s = 0.0;
//     float global_average_a = 0.0;
//     for (int i = 1; i< 10; i++) {
//         float average_genexp_s = genexp(1/0.06);
//         float average_genexp_a = genexp(i); //Lambda
//         for (int j = 1; j < 10; j++) {
//             float uhh_s = genexp(1/0.06);
//             float uhh_a = genexp(i); //Lambda
//             average_genexp_s += uhh_s;
//             average_genexp_a += uhh_a;
//             printf("%f", uhh_s);
//             printf("\t%f\n", uhh_s);
//         }
//         global_average_s += average_genexp_s;
//         global_average_a += average_genexp_a;
//         average_genexp_s /= 10;
//         average_genexp_a /= 10;
//         printf("\tAverage Burst Time: %f\n", average_genexp_s);
//         printf("\tAverage Arrival Time: %f\n\n", average_genexp_a);
//     }
//     global_average_s /= 100;
//     global_average_a /= 100;
//     printf("\tGlobal Average Burst Time: %f\n", global_average_s);
//     printf("\tGlobal Average Arrival Time: %f\n\n", global_average_a);

// }