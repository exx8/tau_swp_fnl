//
// Created by eran on 01/09/2020.
//
typedef int bool;
#define true 1
#define false 0

#include "utils.h"
#include "time.h"
#include "math.h"
#include "ds.h"

#define BELONGS_TO_2ND_COMMUNITY(X) ((X) < 0)

struct _eigen {
    double *vector;
    double value;
} typedef eigen;

double *multipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat, networkStatsSet *AGlobalstats,
                         double *eigenVectorApproximationRead, double *eigenVectorApproximationWrite,
                         int vectorLength) {
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
            const bool isColExists = AgCurrentCol ? (AgCurrentCol->colIndex == colIndex ? 1 : 0) : 0;
            const bool isCellExists = isRowExists && isColExists;

            B_ij -= ((double) AGlobalstats->vertexDegreeArray[rowIndex] * AGlobalstats->vertexDegreeArray[colIndex]) /
                    M;
            if (isCellExists) {
                B_ij++;//Add 1 exists
                AgCurrentCol = AgCurrentCol->next;
            }
            sum += eigenVectorApproximationRead[colIndex] * B_ij;
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

double vectorMultipication(double *a, double *b, int vectorLength) {
    double sum = 0;
    const double *end = b + vectorLength;
    for (; b < end; b++, a++)
        sum += (*a) * (*b);

    return sum;
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

double
billinearMultipicationOfB(const rowLinkedList *Ag, const networkStatsSet *AgStat, const networkStatsSet *AGlobalstats,
                          volatile const int vectorLength, const double *vec1, const double *vec2) {
    double *ab = multipicationOfB(Ag, AgStat, AGlobalstats, vec1, vec2, vectorLength);
    double bAb = vectorMultipication(ab, vec2, vectorLength);// vector cross matrix cross vector
    return bAb;
}

double
billinearMultipicationOfBUnoptimized(const rowLinkedList *Ag, const networkStatsSet *AgStat,
                                     const networkStatsSet *AGlobalstats,
                                     volatile const int vectorLength, const double *vec1) {
    double *vec2 = memory(sizeof(double), vectorLength);
    double returned = billinearMultipicationOfB(Ag, AgStat, AGlobalstats,
                                                vectorLength, vec1, vec2);
    free(vec2);
    return returned;

}


//Ag==A[g]
eigen powerIterationOnB(rowLinkedList *Ag, networkStatsSet *AgStat, networkStatsSet *AGlobalstats) {

    srand(NULL);
    const volatile vectorLength = AgStat->vertices;
    double currentDiff = 1;
    double *vec1, *vec2;
    int volatile i = 0;
    vec1 = memory(sizeof(double), vectorLength);
    vec2 = memory(sizeof(double), vectorLength);
    eigen returned;
    for (; i < vectorLength; i++) {
        vec1[i] = (double) rand();
        vec2[i] = (double) rand();


    }
    while (IS_POSITIVE(currentDiff)) {
        //@todo think about reflection cases
        double *swap1, *swap2;
        swap1 = vec1;
        swap2 = vec2;
        vec2 = multipicationOfB(Ag, AgStat, AGlobalstats, vec1, vec2, vectorLength);
        normalizeVector(vec2, vectorLength);
        vec1 = swap2;
        vec2 = swap1;

        currentDiff = diff(vec1, vec2, vectorLength);
    }
    returned.vector = vec1;

    double bAb = billinearMultipicationOfB(Ag, AgStat, AGlobalstats, vectorLength, vec1, vec2);
    returned.value = bAb / vectorMultipication(vec2, vec2, vectorLength);
    free(vec2);
    return returned;

}

void makeVectorDiscrete(double *vector, int vectorLength) {
    const end = vector + vectorLength;
    for (; vector < end; vector++)
        *vector = (*vector > 0) ? 1 : -1;
}

divisionResults returnError(divisionResults *returned, int errorNum) {
    (*returned).errorNum = errorNum;
    (*returned).value = NULL;
    return (*returned);
}

communityDescription *splitCommunities(communityDescription communityToSplit, double *splitter) {
    rowLinkedList holder1, holder2;
    holder1.nextRow = communityToSplit.graph;
    networkStatsSet community1NetworkStats = communityToSplit.networkStat, community2NetworkStas = emptyNetworkstats();
    rowLinkedList *current1 = holder1.nextRow, *current2 = &holder2;
    rowLinkedList *newGraphsArr[2];
    communityDescription *communityDescriptionArr = memory(sizeof(communityDescription), 2);
    community1NetworkStats.edges = 0;
    community2NetworkStas.vertexDegreeArray = community1NetworkStats.vertexDegreeArray; //they are in the same universe

    while (current1->nextRow != NULL) {
        const int isRowIn2ndGroup = BELONGS_TO_2ND_COMMUNITY(splitter[current1->nextRow->rowIndex]);
        colLinkedList colHolder, *currentCol = &colHolder;
        colHolder.next = current1->colList;

        if (isRowIn2ndGroup) {
            current2->nextRow = current1->nextRow;
            current1->nextRow = current1->nextRow->nextRow;
            current2 = current2->nextRow;
            current2->nextRow = NULL;
            community1NetworkStats.vertices--;
            community2NetworkStas.vertices++;


        }
        while (currentCol->next != NULL) {
            const colLinkedList *nextCol = currentCol->next;
            if (BELONGS_TO_2ND_COMMUNITY(splitter[nextCol->colIndex]) != isRowIn2ndGroup) {
                currentCol = nextCol->next;
                free(nextCol);
            } else if (isRowIn2ndGroup)
                community2NetworkStas.degreeSum++;
            else
                community1NetworkStats.degreeSum++;
        }
        community1NetworkStats.edges = community1NetworkStats.degreeSum / 2;
        community2NetworkStas.edges = community2NetworkStas.degreeSum / 2;
    }
    newGraphsArr[0] = holder1.nextRow;
    newGraphsArr[1] = holder2.nextRow;
    communityDescriptionArr[0].networkStat = community1NetworkStats;
    communityDescriptionArr[0].graph = newGraphsArr[0];
    communityDescriptionArr[1].networkStat = community2NetworkStas;
    communityDescriptionArr[1].graph = newGraphsArr[1];
    return communityDescriptionArr;


}

divisionResults algo2(rowLinkedList *Ag, networkStatsSet *AgStat, networkStatsSet *AGlobalstats) {
    divisionResults returned;
    communityDescription currentCommunity;
    int vectorLength = AgStat->vertices;
    currentCommunity.networkStat = *AgStat;
    currentCommunity.graph = Ag;

    eigen division = powerIterationOnB(Ag, AgStat, AGlobalstats);
    if (division.value < 0) {
        return returnError(&returned, 1);
    }
    //makeVectorDiscrete(division.vector, vectorLength);
    double sBs = billinearMultipicationOfBUnoptimized(Ag, AgStat, AGlobalstats, vectorLength, division.vector);
    if (sBs < 0) {
        return returnError(&returned, 2);

    }

    communityDescription *communtiesAfterSplitting = splitCommunities(currentCommunity, division.vector);

}

void test(rowLinkedList *graphData, networkStatsSet networkStat) {


    algo2(graphData, &networkStat, &networkStat);
}