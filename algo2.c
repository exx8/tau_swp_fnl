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

double *multipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat,
                         double *eigenVectorApproximationRead, double *eigenVectorApproximationWrite,
                         int vectorLength) {
    //@todo check me!!!
    double bilinearValue = 0;
    const M = AgStat->degreeSum;
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

            B_ij -= ((double) AgStat->vertexDegreeArray[rowIndex] * AgStat->vertexDegreeArray[colIndex]) /
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
billinearMultipicationOfB(const rowLinkedList *Ag, const networkStatsSet *AgStat,
                          volatile const int vectorLength, const double *vec1, const double *vec2) {
    double *ab = multipicationOfB(Ag, AgStat, vec1, vec2, vectorLength);
    double bAb = vectorMultipication(ab, vec2, vectorLength);// vector cross matrix cross vector
    return bAb;
}

double
billinearMultipicationOfBUnoptimized(const rowLinkedList *Ag, const networkStatsSet *AgStat,
                                     volatile const int vectorLength, const double *vec1) {
    double *vec2 = memory(sizeof(double), vectorLength);
    double returned = billinearMultipicationOfB(Ag, AgStat,
                                                vectorLength, vec1, vec2);
    free(vec2);
    return returned;

}


//Ag==A[g]
eigen powerIterationOnB(rowLinkedList *Ag, networkStatsSet *AgStat) {

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
    int left=1000;
    while (IS_POSITIVE(currentDiff)&&left>0) {
        //@todo think about reflection cases
        double *swap1, *swap2;
        swap1 = vec1;
        swap2 = vec2;
        vec2 = multipicationOfB(Ag, AgStat, vec1, vec2, vectorLength);
        normalizeVector(vec2, vectorLength);
        vec1 = swap2;
        vec2 = swap1;

        currentDiff = diff(vec1, vec2, vectorLength);
        left--;
    }
    returned.vector = vec1;

    double bAb = billinearMultipicationOfB(Ag, AgStat, vectorLength, vec1, vec2);
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
divisionResults returnSuccess(communityDescription* communitiesAfterDivision) {
    divisionResults result;
    result.errorNum=0;
    result.value=communitiesAfterDivision;
    return result;
}
void deleteCrossRelation(const double *splitter, const int isRowIn2ndGroup, colLinkedList *currentCol,
                         networkStatsSet *community1NetworkStats, networkStatsSet *community2NetworkStas) {
    while (currentCol&&currentCol->next != NULL) {

        if (BELONGS_TO_2ND_COMMUNITY(splitter[currentCol->next->colIndex]) != isRowIn2ndGroup) {
            const colLinkedList *nodeTodelete=currentCol->next;
            currentCol->next= currentCol->next->next;
            free(nodeTodelete);
            currentCol=currentCol->next;

        } else {
            currentCol=currentCol->next;
            if (isRowIn2ndGroup) {
                (*community2NetworkStas).degreeSum+=2;
            }
            else{
                (*community1NetworkStats).degreeSum+=2;

            }
        }

    }
}

communityDescription *splitCommunities(communityDescription communityToSplit, double *splitter) {
    rowLinkedList holder1, holder2;
    holder1.nextRow = communityToSplit.graph;
    holder2.nextRow=NULL;
    holder1.rowIndex=-1;
    holder2.rowIndex=-1;
    networkStatsSet community1NetworkStats = communityToSplit.networkStat, community2NetworkStas = emptyNetworkstats();
    rowLinkedList *current1 = holder1.nextRow, *current2 = &holder2;
    rowLinkedList *newGraphsArr[2];
    communityDescription *communityDescriptionArr = memory(sizeof(communityDescription), 2);
    community1NetworkStats.edges = 0;
    community2NetworkStas.vertexDegreeArray = community1NetworkStats.vertexDegreeArray; //they are in the same universe
    int shouldContinue=0;
    while (current1&&current1->nextRow != NULL) {
        const int isRowIn2ndGroup = BELONGS_TO_2ND_COMMUNITY(splitter[current1->nextRow->rowIndex]);
        colLinkedList colHolder, *currentCol = &colHolder;
        colHolder.colIndex=-1; //@todo delete me
        colHolder.next = current1->colList;

        if (isRowIn2ndGroup) {
            current2->nextRow = current1->nextRow;
            current1->nextRow = current1->nextRow->nextRow;
            current2 = current2->nextRow;
            current2->nextRow = NULL;
            community1NetworkStats.vertices--;
            community2NetworkStas.vertices++;


        }
        else
        {
            shouldContinue=1;
        }
        deleteCrossRelation(splitter, isRowIn2ndGroup, currentCol, &community1NetworkStats, &community2NetworkStas);
        current1->colList=currentCol->next;
        community1NetworkStats.edges = community1NetworkStats.degreeSum / 2;
        community2NetworkStas.edges = community2NetworkStas.degreeSum / 2;
        if(shouldContinue)
            current1=current1->nextRow;
    }
    newGraphsArr[0] = holder1.nextRow;
    newGraphsArr[1] = holder2.nextRow;
    communityDescriptionArr[0].networkStat = community1NetworkStats;
    communityDescriptionArr[0].graph = newGraphsArr[0];
    communityDescriptionArr[1].networkStat = community2NetworkStas;
    communityDescriptionArr[1].graph = newGraphsArr[1];
    return communityDescriptionArr;


}

divisionResults algo2(rowLinkedList *Ag, networkStatsSet *AgStat) {
    divisionResults returned;
    communityDescription currentCommunity;
    int vectorLength = AgStat->vertices;
    currentCommunity.networkStat = *AgStat;
    currentCommunity.graph = Ag;

    eigen division = powerIterationOnB(Ag, AgStat);
    if (division.value < 0) {
        return returnError(&returned, 1);
    }
    //makeVectorDiscrete(division.vector, vectorLength);
    double sBs = billinearMultipicationOfBUnoptimized(Ag, AgStat, vectorLength, division.vector);
    if (sBs < 0) {
        return returnError(&returned, 2);

    }

    communityDescription *communtiesAfterSplitting = splitCommunities(currentCommunity, division.vector);
    free(division.vector);
return returnSuccess(communtiesAfterSplitting);
}

void test(rowLinkedList *graphData, networkStatsSet networkStat) {


    algo2(graphData, &networkStat);
}