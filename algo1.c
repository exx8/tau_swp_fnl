//
// Created by eran on 23/08/2020.
//
//DEPRECATED
#include "utils.h"
#include "networkStats.h"


int calculateModularity(networkStatsSet *networkStat){

    int degreeMulSum = fillDegreeMatrix(degreeMatrix, networkStat);
    int modularity = (2 * networkStat->edges) - degreeMulSum/networkStat->degreeSum;
    return  modularity;

}
int* computeLeadEigenVal(networkStatsSet* networkStat);
int* computeLeadEigenVec(networkStatsSet* networkStat);

int* algorithm1(networkStatsSet *networkStat){
    int i;
     int* leadEigenValue = computeLeadEigenVal(networkStat);
     int * leadEigenVector = computeLeadEigenVec(networkStat);
     if (leadEigenValue <= 0){
         printf("The network is indivisible.");
         return NULL;
     }
     int * groupDivisionVector = (int *) memory(networkStat->vertices , sizeof(int));
     for (i = 0; i < networkStat->vertices; i++){
         if (leadEigenValue[i] <= 0){
             groupDivisionVector[i] = -1;
         }
         else{
             groupDivisionVector[i] = 1;
         }
     }
     ///Line 4 in Algo1 description
     int result = matrixMultiply(matrixMultilpy(networkStat, transpose(leadEigenVector)), leadEigenVector);
     if (result <=0){
         printf("The network is indivisible.");
         return NULL;
     }
     return groupDivisionVector;
}

