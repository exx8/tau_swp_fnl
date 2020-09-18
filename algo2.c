
typedef int bool;
#define true 1
#define false 0

#include "utils.h"
#include "time.h"
#include "math.h"
#include "ds.h"
#include "string.h"
#define BELONGS_TO_2ND_COMMUNITY(X) ((X) < 0)
#include "modularity_maximization.h"
#include "shift.h"
typedef struct _eigen{
    double *vector;
    double value;
}eigen;

double *multipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat,
                         double *eigenVectorApproximationRead, double *eigenVectorApproximationWrite,
                         int vectorLength,double shift) {
    int M;
    int rowIndex, colIndex;
    rowLinkedList *AgCurrent;
    M = AgStat->degreeSum;
    AgCurrent = Ag;
/* all matrices are symmetrical.*/
    for (rowIndex = 0; rowIndex < vectorLength; rowIndex++) {
        bool isRowExists;
        double sum;
        colLinkedList *AgCurrentCol;
        isRowExists = AgCurrent->rowIndex == rowIndex ? 1 : 0;
        sum = 0;
        AgCurrentCol = AgCurrent->colList;

        for (colIndex = 0; colIndex < vectorLength; colIndex++) {
            double B_ij;
             bool isColExists, isCellExists;
             B_ij  = 0;
             isColExists = AgCurrentCol ? (AgCurrentCol->colIndex == colIndex ? 1 : 0) : 0;
             isCellExists = isRowExists && isColExists;

            B_ij -= ((double) AgStat->vertexDegreeArray[rowIndex] * AgStat->vertexDegreeArray[colIndex]) /
                    M;
            if (isCellExists) {
                B_ij++;/*Add 1 exists*/
                AgCurrentCol = AgCurrentCol->next;
            }
            eigenVectorApproximationWrite[rowIndex] = (shift+eigenVectorApproximationRead[rowIndex ])* B_ij;
        }

        eigenVectorApproximationWrite[rowIndex] = sum;
        if (rowIndex == AgCurrent->rowIndex) {
            AgCurrent = AgCurrent->nextRow;
        }
    }


    return eigenVectorApproximationWrite;

}

double norm(double *vector, int len) {
    double sum;
    int i;
    sum = 0;
    i = 0;
    for (; i < len; i++)
        sum += vector[i] * vector[i];

    return sqrt(sum);
}

double vectorMultipication(double *a, double *b, int vectorLength) {
    double sum;
    double *end;
    sum = 0;
    end = b + vectorLength;
    for (; b < end; b++, a++)
        sum += (*a) * (*b);

    return sum;
}

void normalizeVector(double *vec, int vecLength) {
    double vecNorm;
    int i;
    vecNorm = norm(vec, vecLength);
    i = 0;
    if (vecNorm != 0)
        for (; i < vecLength; i++)
            vec[i] /= vecNorm;
}

double diff( double *vec1,  double *vec2, int vectorLength) {
    double *vectorEnd;
    double sum;
    vectorEnd = vec1 + vectorLength;
    sum = 0;
    for (; vectorEnd != vec1; vec1++, vec2++) {
        sum += pow(*vec1 - *vec2, 2);
    }
    sum = sqrt(sum);
    return sum;
}

double
billinearMultipicationOfB( rowLinkedList *Ag,  networkStatsSet *AgStat,
                          volatile  int vectorLength,  double *vec1,  double *vec2) {
    double *ab;
    double bAb;
    ab = multipicationOfB(Ag, AgStat, vec1, vec2, vectorLength,0);
    bAb = vectorMultipication(ab, vec2, vectorLength);/* vector cross matrix cross vector*/
    return bAb;
}

double
billinearMultiplicationOfBUnoptimized(rowLinkedList *Ag, networkStatsSet *AgStat,
                                      volatile  int vectorLength, double *vec1) {
    double *vec2;
    double returned;
    vec2 = memory(sizeof(double), vectorLength);
    returned = billinearMultipicationOfB(Ag, AgStat,
                                                vectorLength, vec1, vec2);
    free(vec2);
    return returned;

}


/*Ag==A[g]*/
eigen powerIterationOnB(rowLinkedList *Ag, networkStatsSet *AgStat) {
    int shift;
    volatile int vectorLength;
    double currentDiff;
    double *vec1, *vec2;
    int volatile i;
    int left;
    double bAb;
    eigen returned;

    shift =norm1(Ag);
    srand(2);
    vectorLength = AgStat->vertices;
    currentDiff = 1;
    i = 0;
    vec1 = memory(sizeof(double), vectorLength);
    vec2 = memory(sizeof(double), vectorLength);
    for (; i < vectorLength; i++) {
        vec1[i] = (double) rand();
        vec2[i] = (double) rand();


    }
    left =5*AgStat->vertices;
    while (IS_POSITIVE(currentDiff)&&left>0) {
        double *swap1, *swap2;
        swap1 = vec1;
        swap2 = vec2;
        vec2 = multipicationOfB(Ag, AgStat, vec1, vec2, vectorLength,shift);
        normalizeVector(vec2, vectorLength);
        vec1 = swap2;
        vec2 = swap1;

        currentDiff = diff(vec1, vec2, vectorLength);
        left--;
    }
    returned.vector = vec1;

    bAb = billinearMultipicationOfB(Ag, AgStat, vectorLength, vec1, vec2);
    returned.value = bAb / vectorMultipication(vec2, vec2, vectorLength)-shift;
    free(vec2);
    return returned;

}

void makeVectorDiscrete(double *vector, int vectorLength) {
    double* end;
    end = vector + vectorLength;
    for (; vector < end; vector++)
        *vector = (*vector > 0) ? 1 : -1;
}

divisionResults * returnError(divisionResults *returned, int errorNum) {
    (*returned).errorNum = errorNum;
    (*returned).value = NULL;
    return (returned);
}
divisionResults * returnSuccess(tuple2* communitiesAfterDivision) {
    divisionResults *result;
    result =smemory(sizeof(divisionResults),1);
    result->errorNum=0;
    result->value=communitiesAfterDivision;
    return result;
}
void deleteCrossRelation( double *splitter,  int isRowIn2ndGroup, colLinkedList *currentCol,
                         networkStatsSet *community1NetworkStats, networkStatsSet *community2NetworkStas) {
    while (currentCol&&currentCol->next != NULL) {

        int colIndex;
        colIndex = currentCol->next->colIndex;
        if (BELONGS_TO_2ND_COMMUNITY(splitter[colIndex]) != isRowIn2ndGroup) {

            currentCol->next= currentCol->next->next;
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

tuple2 *splitCommunities(communityDescription communityToSplit, double *splitter) {
    rowLinkedList holder1, holder2;
    networkStatsSet community1NetworkStats, community2NetworkStas;
    rowLinkedList *current1, *current2;
    rowLinkedList *newGraphsArr[2];
    tuple2 *communityDescriptionArr;
    int shouldContinue;
    int spliterIndex;

    holder1.nextRow = communityToSplit.graph;
    holder2.nextRow=NULL;
    holder1.rowIndex=-1;
    holder2.rowIndex=-1;
    community1NetworkStats = *communityToSplit.networkStat;
    community2NetworkStas = emptyNetworkstats();
    current1= holder1.nextRow;
    current2 = &holder2;

    communityDescriptionArr=smemory(sizeof(tuple2),1);
    communityDescriptionArr->first=smemory(sizeof(communityDescription), 1);
    communityDescriptionArr->second=smemory(sizeof(communityDescription), 1);
    community1NetworkStats.edges = 0;
    community2NetworkStas.vertexDegreeArray = community1NetworkStats.vertexDegreeArray;

    shouldContinue = 0;
    spliterIndex=0;

    modularity_maximization( splitter, communityToSplit.networkStat->vertices, communityToSplit.graph, communityToSplit.networkStat);

        while (current1&&current1->nextRow != NULL) {
            int isRowIn2ndGroup;
            colLinkedList colHolder, *currentCol;
            isRowIn2ndGroup = BELONGS_TO_2ND_COMMUNITY(splitter[spliterIndex]);
            currentCol= &colHolder;
            colHolder.colIndex=-1;
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

        spliterIndex++;

        }
    newGraphsArr[0] = holder1.nextRow;
    newGraphsArr[1] = holder2.nextRow;
    communityDescriptionArr->first->networkStat=smemory(sizeof(networkStatsSet),1);
    memcpy(communityDescriptionArr->first->networkStat,&community1NetworkStats,sizeof(networkStatsSet));
    communityDescriptionArr->first->graph = newGraphsArr[0];
    communityDescriptionArr->second->networkStat=smemory(sizeof(networkStatsSet),1);
    memcpy(communityDescriptionArr->second->networkStat,&community2NetworkStas,sizeof(networkStatsSet));
    communityDescriptionArr->second->graph = newGraphsArr[1];
    return communityDescriptionArr;


}

divisionResults* algo2(rowLinkedList *Ag, networkStatsSet *AgStat) {
    divisionResults returned;
    communityDescription currentCommunity;
    int vectorLength;
    eigen division;
    double sBs;
    tuple2 *communitiesAfterSplitting;

    vectorLength = AgStat->vertices;
    currentCommunity.networkStat = AgStat;
    currentCommunity.graph = Ag;

    division = powerIterationOnB(Ag, AgStat);
    if (division.value < 0) {
        return returnError(&returned, 1);
    }
    sBs = billinearMultiplicationOfBUnoptimized(Ag, AgStat, vectorLength, division.vector);
    if (sBs < 0) {
        return returnError(&returned, 2);

    }

    communitiesAfterSplitting = splitCommunities(currentCommunity, division.vector);
    free(division.vector);
return returnSuccess(communitiesAfterSplitting);
}

void test(rowLinkedList *graphData, networkStatsSet networkStat) {


    algo2(graphData, &networkStat);
}