//
// Created by eran on 23/08/2020.
//

#include "algo1.h"

int algorithm1(networkStats *networkStat){
     int leadEigenValue = computeLeadEigenVal(networkStat);
     int * leadEigenVector = computeLeadEigenVec(networkStat);
     if (leadEigenValue <= 0){
         printf("The network is indivisible.");
         return NULL;
     }
     int * groupDivisionVector = (int *) malloc(networkStat->vertices * sizeof(int));
     for (int i = 0; i < networkStat->vertices; i++){
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