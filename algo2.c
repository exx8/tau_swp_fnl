
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
            sum+=(shift+eigenVectorApproximationRead[colIndex ])* B_ij;
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
    bAb = vectorMultipication(ab, vec1, vectorLength);/* vector cross matrix cross vector*/
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
    double dominator;
    double shift;
    volatile int vectorLength;
    double currentDiff;
    double *vec1, *vec2;
    int volatile i;
    int left;
    double bAb;
    double *swap1, *swap2;

    eigen returned;

    shift =norm1(Ag);
    srand(2);
    vectorLength = AgStat->vertices;
    currentDiff = 1;
    i = 0;
    vec1 = memory(sizeof(double), vectorLength);
    vec2 = memory(sizeof(double), vectorLength);
    for (; i < vectorLength; i++) {
        vec1[i] = (double) i;
        vec2[i] = (double) i;


    }
    left =5*AgStat->vertices;
    while (IS_POSITIVE(currentDiff)&&left>0) {
        swap1 = vec1;
        swap2 = vec2;
        vec2 = multipicationOfB(Ag, AgStat, vec1, vec2, vectorLength,0);
        normalizeVector(vec2, vectorLength);
        vec1 = swap2;
        vec2 = swap1;

        currentDiff = diff(vec1, vec2, vectorLength);
        left--;
    }
    returned.vector = vec1;

    bAb = billinearMultipicationOfB(Ag, AgStat, vectorLength, vec1, vec2);
    dominator = vectorMultipication(vec2, vec2, vectorLength);
    returned.value = dominator!=0?(bAb / dominator ):0- shift;
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

colLinkedList* cleanFromCrossEdges(rowLinkedList* list,networkStatsSet* ns)
{
    colLinkedList *clearedList;
    rowLinkedList *listOfRowsOfGroup,*listToCompare;
    colLinkedList *listToClean,*tempNode;
    clearedList=smemory(sizeof(colLinkedList),1);
    clearedList->colIndex=-1;
    clearedList->next=NULL;
    listOfRowsOfGroup=list;
    while(listOfRowsOfGroup != NULL)
    {
        listToCompare=list;
        listToClean=listOfRowsOfGroup->colList;
        while(listToClean != NULL&&listToCompare!=NULL)
        {
            if(listToClean->colIndex > listToCompare->rowIndex)
                listToCompare=listToCompare->nextRow;
            else if(listToCompare->rowIndex == listToClean->colIndex&&listToClean->next!=NULL)
            {
                tempNode=listToClean;
                clearedList->next=listToClean;
                listToCompare=listToCompare->nextRow;
                listToClean=listToClean->next;
                tempNode->next=NULL;
            }
            else
            {
               /*/ ns->vertexDegreeArray[listToClean->colIndex]=listToClean->colIndex-1;*/
                ns->degreeSum-=2;


                listToClean=listToClean->next;
            }
        }
        listOfRowsOfGroup->colList=clearedList->next;
        listOfRowsOfGroup=listOfRowsOfGroup->nextRow;
    }
return clearedList->next;
}

tuple2 *splitCommunities(communityDescription communityToSplit, double *splitter) {
    rowLinkedList holderCurrentGroup, holderMinusGroup,holderPlusGroup;
    networkStatsSet community1NetworkStats, community2NetworkStas;
    rowLinkedList *originalList, *currentMinusGroup,*currentPlusGroup;
    rowLinkedList *newGraphsArr[2];
    tuple2 *communityDescriptionArr;
    int shouldContinue;
    int spliterIndex;

    holderCurrentGroup.nextRow = communityToSplit.graph;
    holderMinusGroup.nextRow=NULL;
    holderCurrentGroup.rowIndex=-1;
    holderMinusGroup.rowIndex=-1;
    holderPlusGroup.rowIndex=-1;
    currentMinusGroup=&holderMinusGroup;
    currentPlusGroup=&holderPlusGroup;
    originalList= communityToSplit.graph;
    spliterIndex=0;

    community1NetworkStats = emptyNetworkstats();
    community2NetworkStas = emptyNetworkstats();
    modularity_maximization( splitter, communityToSplit.networkStat->vertices, communityToSplit.graph, communityToSplit.networkStat);

    while(originalList!=NULL)
    {
        if(splitter[spliterIndex]==1)
        {
            currentPlusGroup->nextRow=originalList;
            currentPlusGroup=currentPlusGroup->nextRow;
            originalList=originalList->nextRow;
            currentPlusGroup->nextRow=NULL;
        }
        else
        {
            currentMinusGroup->nextRow=originalList;
            currentMinusGroup=currentMinusGroup->nextRow;
            originalList=originalList->nextRow;
            currentMinusGroup->nextRow=NULL;
        }
    }

    communityDescriptionArr=smemory(sizeof(tuple2),1);
    communityDescriptionArr->first=smemory(sizeof(communityDescription), 1);
    communityDescriptionArr->second=smemory(sizeof(communityDescription), 1);
    community1NetworkStats.edges = 0;
    community2NetworkStas.vertexDegreeArray = community1NetworkStats.vertexDegreeArray;





        cleanFromCrossEdges(holderCurrentGroup.nextRow, &community1NetworkStats);
        cleanFromCrossEdges(holderMinusGroup.nextRow, &community2NetworkStas);



    newGraphsArr[0] = holderCurrentGroup.nextRow;
    newGraphsArr[1] = holderMinusGroup.nextRow;
    communityDescriptionArr->first->networkStat=smemory(sizeof(networkStatsSet),1);
    memcpy(communityDescriptionArr->first->networkStat,&community1NetworkStats,sizeof(networkStatsSet));
    communityDescriptionArr->first->graph = newGraphsArr[0];
    communityDescriptionArr->second->networkStat=smemory(sizeof(networkStatsSet),1);
    memcpy(communityDescriptionArr->second->networkStat,&community2NetworkStas,sizeof(networkStatsSet));
    communityDescriptionArr->second->graph = newGraphsArr[1];
    return communityDescriptionArr;


}

divisionResults* algo2(rowLinkedList *Ag, networkStatsSet *AgStat) {

    communityDescription currentCommunity;
    int vectorLength;
    eigen division;
    double sBs;
    tuple2 *communitiesAfterSplitting;
    int isindivisible;
    int k=0;

    vectorLength = AgStat->vertices;
    currentCommunity.networkStat = AgStat;
    currentCommunity.graph = Ag;
    isindivisible=0;

    division = powerIterationOnB(Ag, AgStat);
    if (division.value < 0) {
       isindivisible=1;
    }
    sBs = billinearMultiplicationOfBUnoptimized(Ag, AgStat, vectorLength, division.vector);
    if (sBs < 0) {
        isindivisible=1;

    }
    if(isindivisible)
    {
        for(k=0;k<vectorLength;k++)
            division.vector[k]=1;
    }
    communitiesAfterSplitting = splitCommunities(currentCommunity, division.vector);
    free(division.vector);
return returnSuccess(communitiesAfterSplitting);
}

void test(rowLinkedList *graphData, networkStatsSet networkStat) {


    algo2(graphData, &networkStat);
}