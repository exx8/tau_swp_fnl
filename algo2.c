//
// Created by eran on 01/09/2020.
//
typedef int bool;
#define true 1
#define false 0

#include "ds.h"
#include "networkStats.h"

double bilinearMultipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat, networkStatsSet *AGlobalstats,
                              int *eigenVectorApproximation) {
    //@todo check me!!!
    double bilinearValue=0;
    const M = AGlobalstats->degreeSum;
    const vectorLength = AgStat->vertices;
    rowLinkedList *AgCurrent = Ag;
    double *partialMultipicationOfEigenAndMatrix = memory(sizeof(double), vectorLength);
    int rowIndex, colIndex;
// all matrices are symmetrical.
    for (rowIndex = 0; rowIndex < vectorLength; rowIndex++) {
        bool isRowExists = AgCurrent->rowIndex == rowIndex ? 1 : 0;

        int sum = 0;
        colLinkedList *AgCurrentCol = AgCurrent->colList;

        for (colIndex = 0; colIndex < vectorLength; colIndex++) {
            double B_ij = 0;
            const bool isColExists = AgCurrentCol->colIndex == colIndex ? 1 : 0;
            const bool isCellExists = isRowExists && isColExists;

            B_ij -= AGlobalstats->vertexDegreeArray[rowIndex] * AGlobalstats->vertexDegreeArray[colIndex] / M;
            if (isCellExists) {
                B_ij++;//Add 1 exists
                AgCurrentCol = AgCurrentCol->next;
            }
            partialMultipicationOfEigenAndMatrix[rowIndex] = eigenVectorApproximation[rowIndex * B_ij];
        }

        partialMultipicationOfEigenAndMatrix[rowIndex] = sum;
        if (rowIndex == AgCurrent->rowIndex) {
            AgCurrent = AgCurrent->nextRow;
        }
    }
    for(rowIndex=0;rowIndex<vectorLength;rowIndex++)
    {
        bilinearValue+=eigenVectorApproximation[rowIndex]*partialMultipicationOfEigenAndMatrix[rowIndex];
    }

    free(partialMultipicationOfEigenAndMatrix);
    return bilinearValue;

}
int norm(double* vector,int len)
{
    int sum=0;
    int i=0;
    for(;i<len;i++)
        sum+=vector[i]*vector[i];
    return sum;
}