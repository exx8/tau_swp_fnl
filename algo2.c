//
// Created by eran on 01/09/2020.
//
typedef int bool;
#define true 1
#define false 0


#include "time.h"
#include "math.h"
#include "ds.h"
struct _eigen{
    double *vector;
    double value;
} typedef eigen;
double *multipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat, networkStatsSet *AGlobalstats,
                         double* eigenVectorApproximationRead,double* eigenVectorApproximationWrite, int vectorLength) {
    //@todo check me!!!
    double bilinearValue = 0;
    const M = AGlobalstats->degreeSum;
    rowLinkedList *AgCurrent = Ag;
    int rowIndex, colIndex;
// all matrices are symmetrical.
    for (rowIndex = 0; rowIndex < vectorLength; rowIndex++) {
        bool isRowExists = AgCurrent->rowIndex == rowIndex ? 1 : 0;

        double sum = 0;
        colLinkedList *AgCurrentCol = AgCurrent->colList;

        for (colIndex = 0; colIndex < vectorLength; colIndex++) {
            double B_ij = 0;
            const bool isColExists =AgCurrentCol? ( AgCurrentCol->colIndex == colIndex ? 1 : 0):0;
            const bool isCellExists = isRowExists && isColExists;

            B_ij -= ((double )AGlobalstats->vertexDegreeArray[rowIndex] * AGlobalstats->vertexDegreeArray[colIndex] )/ M;
            if (isCellExists) {
                B_ij++;//Add 1 exists
                AgCurrentCol = AgCurrentCol->next;
            }
            sum+= eigenVectorApproximationRead[rowIndex ]* B_ij;
        }

        eigenVectorApproximationWrite[rowIndex] = sum;
        if (rowIndex == AgCurrent->rowIndex) {
            AgCurrent = AgCurrent->nextRow;
        }
    }


    return eigenVectorApproximationWrite;

}

double norm(double *vector, int len) {
    double sum = 0;
    int i = 0;
    for (; i < len; i++)
        sum += vector[i] * vector[i];

    return sqrt(sum);
}

void normalizeVector(double *vec, int vecLength) {
    double vecNorm = norm(vec, vecLength);
    int i = 0;
    if (vecNorm != 0)
        for (; i < vecLength; i++)
            vec[i] /= vecNorm;
}

double diff(const double *vec1, const double *vec2, int vectorLength) {
    const double *vectorEnd = vec1 + vectorLength;
    double sum = 0;
    for (; vectorEnd != vec1; vec1++, vec2++) {
        sum += pow(*vec1 - *vec2, 2);
    }
    sum = sqrt(sum);
    return sum;
}

//Ag==A[g]
eigen powerIterationOnB(rowLinkedList *Ag, networkStatsSet *AgStat, networkStatsSet *AGlobalstats) {

    srand(NULL);
    const volatile vectorLength = AgStat->vertices;
    const double epsilon = 0.00001;
    double currentDiff = 1;
    double *vec1, *vec2;
    int volatile i = 0;
    vec1 = memory(sizeof(double), vectorLength);
    vec2 = memory(sizeof(double), vectorLength);
    eigen returned;
    for (; i < vectorLength; i++) {
        vec1[i] = (double)rand();
        vec2[i] = (double)rand();

    }
    while (currentDiff > epsilon) {
        double *swap1, *swap2;
        swap1 = vec1;
        swap2=vec2;
        vec2 = multipicationOfB(Ag, AgStat, AGlobalstats, vec1,vec2, vectorLength);
        normalizeVector(vec2, vectorLength);
        vec1 = swap2;
        vec2 = swap1;

        currentDiff=diff(vec1, vec2, vectorLength);
    }
    free(vec2);
    returned.vector=vec1;
    returned.value;//@todo
    return returned;

}

void test(rowLinkedList * graphData,networkStatsSet networkStat)
{


powerIterationOnB(graphData,&networkStat,&networkStat);
}