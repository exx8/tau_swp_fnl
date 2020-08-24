//
// Created by eran on 23/08/2020.
//

#include "utils.h"
#include "networkStats.h"
/// Addition
int fillDegreeMatrix(int * matrix,  networkStatsSet *networkStat){
    int currVertexIndex = 0;
    int overAllCounter = 0;
    int degreeMulSum = 0;
    while (currVertexIndex < networkStat->vertices){
        int i = 0;
        while (i < networkStat->vertices){
            int verticesDegreeMul = (networkStat->vertexDegreeArray[currVertexIndex] *
                                     networkStat->vertexDegreeArray[i]);
            degreeMulSum += verticesDegreeMul;
            matrix[overAllCounter] = verticesDegreeMul;
            overAllCounter++;
        }
    }

    return degreeMulSum;
}

int calculateModularity(networkStatsSet *networkStat){

    int verticesSquared = (networkStat->vertices * networkStat->vertices);
    int * degreeMatrix = (int *) memory(verticesSquared ,sizeof(int));
    int degreeMulSum = fillDegreeMatrix(degreeMatrix, networkStat);
    int modularity = (2 * networkStat->edges) - degreeMulSum/networkStat->degreeSum;
    return  modularity;

}

int algorithm1(networkStatsSet *networkStat){
     int leadEigenValue = computeLeadEigenVal(networkStat);
     int * leadEigenVector = computeLeadEigenVec(networkStat);
     if (leadEigenValue <= 0){
         printf("The network is indivisible.");
         return NULL;
     }
     int * groupDivisionVector = (int *) memory(networkStat->vertices , sizeof(int));
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

