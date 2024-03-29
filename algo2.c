
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

/*
 * this function accepts rows of a B matrix and a vector s, and outputs theirs multiplications
 */
double multipicationOfBofRow(rowLinkedList  *currentRow, double *s,networkStatsSet* ns,double shift,int vectorLength)
{
    double result;
    colLinkedList *col;
    double M=0;
    int k;

    result=0;
    col=currentRow->colList;
    M=ns->degreeSum;
    while(col!=NULL)
    {
        result+=s[col->colIndex]
       -currentRow->numOfCols*s[col->colIndex]
        +shift*s[col->colIndex];
        col=col->next;
    }
    col=currentRow->colList;

    for(k=0;k<vectorLength;k++) {
        result -= s[k] * (ns->vertexDegreeArray[k] * ns->vertexDegreeArray[currentRow->rowIndex]) / M;
    }
    return result;
}
/*
 * This function multiplies B by vector s
 */
double *multipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat,
                         double *eigenVectorApproximationRead, double *eigenVectorApproximationWrite,
                         int vectorLength,double shift) {
    rowLinkedList *AgCurrent;
    AgCurrent = Ag;
/* all matrices are symmetrical.*/
    memset(eigenVectorApproximationWrite,0,vectorLength);
    while(AgCurrent!=NULL)
    {
        eigenVectorApproximationWrite[AgCurrent->rowIndex]=multipicationOfBofRow(AgCurrent,eigenVectorApproximationRead,AgStat,shift,vectorLength);
        AgCurrent=AgCurrent->nextRow;
    }



    return eigenVectorApproximationWrite;

}
/*
 * this function calculates the norm of a vector (Euclidean)
 */
double norm(double *vector, int len) {
    double sum;
    int i;
    sum = 0;
    i = 0;
    for (; i < len; i++)
        sum += vector[i] * vector[i];

    return sqrt(sum);
}
/*
 *
 * this function calculate the dot multiplication of a and b
 */
double vectorMultipication(double *a, double *b, int vectorLength) {
    double sum;
    double *end;
    sum = 0;
    end = b + vectorLength;
    for (; b < end; b++, a++)
        sum += (*a) * (*b);

    return sum;
}
/*
 * This function normalizes given vec
 */
void normalizeVector(double *vec, int vecLength) {
    double vecNorm;
    int i;
    vecNorm = norm(vec, vecLength);
    i = 0;
    if (vecNorm != 0)
        for (; i < vecLength; i++)
            vec[i] =vec[i]/ vecNorm;
}
/*
 * returns the dif of two vector, vec1,vec2
 */
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

/*
 * calculate s^TBs where s is a given vector.
 */
double
billinearMultipicationOfB(rowLinkedList *Ag, networkStatsSet *AgStat, volatile int vectorLength, double *vec1,
                          double *vec2, double shift) {

    double *ab;
    double bAb;
    ab = multipicationOfB(Ag, AgStat, vec1, vec2, vectorLength,shift);
    bAb = vectorMultipication(ab, vec1, vectorLength);/* vector cross matrix cross vector*/
    return bAb;
}

/*
 * calculate bilinear multipication of B with a vec, vec1 via the previous functions.
 */
double
billinearMultiplicationOfBUnoptimized(rowLinkedList *Ag, networkStatsSet *AgStat, volatile int vectorLength,
                                      double *vec1, double shift) {
    double *vec2;
    double returned;
    vec2 = memory(sizeof(double), vectorLength);
    returned = billinearMultipicationOfB(Ag, AgStat,
                                         vectorLength, vec1, vec2, shift);
    free(vec2);
    return returned;

}
/*
 * calculate the eigen value of an eigen vector
 */
double eigenValue(rowLinkedList *Ag, networkStatsSet *AgStat,
               double *eigenVectorApproximationRead,
               int vectorLength,double shift)
{
    int k=0;
    double *eigenVectorApproximationWrite=smemory(sizeof(double *),vectorLength);

    multipicationOfB( Ag,  AgStat,eigenVectorApproximationRead,eigenVectorApproximationWrite,vectorLength,shift);
    for(;k<vectorLength;k++)
        if(eigenVectorApproximationRead[k]!=0)
    return eigenVectorApproximationWrite[k]/eigenVectorApproximationRead[k];
    return 0;

}
/*
 * calculate the eigen vector via power iterations
 */
/*Ag==A[g]*/
eigen powerIterationOnB(rowLinkedList *Ag, networkStatsSet *AgStat) {
    double shift;
    volatile int vectorLength;
    double currentDiff;
    double *vec1, *vec2;
    int volatile i;
    int left;
    double *swap1, *swap2;

    eigen returned;

    shift =norm1(Ag,AgStat);
    srand(time(NULL));
    vectorLength = AgStat->vertices;
    currentDiff = 1;
    i = 0;
    vec1 = memory(sizeof(double), vectorLength);
    vec2 = memory(sizeof(double), vectorLength);
    for (; i < vectorLength; i++) {
        vec1[i] = (double) rand();
        vec2[i] = (double) rand();


    }
    left =500*AgStat->vertices;
    while (IS_POSITIVE(currentDiff)&&left>0) {
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

    returned.value=eigenValue(Ag,AgStat,vec1,vectorLength,shift);

    free(vec2);
    return returned;

}
/*
 * convert a double array to an int array discreted
 */
void makeVectorDiscrete(double *vector, int vectorLength) {
    double* end;
    end = vector + vectorLength;
    for (; vector < end; vector++)
        *vector = (*vector > 0) ? 1 : -1;
}
/*
 * returns error output
 */
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
/*
 * update network stats according to the split
 * @deprecated
 */
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
                ns->degreeSum-=2;


                listToClean=listToClean->next;
            }
        }
        listOfRowsOfGroup->colList=clearedList->next;
        listOfRowsOfGroup=listOfRowsOfGroup->nextRow;
    }
return clearedList->next;
}

void filterColumn(rowLinkedList *comparator, colLinkedList *listToFilter) {
    while(listToFilter->next != NULL && listToFilter != NULL)
    {
        if(listToFilter->next->colIndex<comparator->rowIndex)
        {
            listToFilter->next=listToFilter->next->next;
        }
        else
            if(listToFilter->next->colIndex==comparator->rowIndex)
            {
                listToFilter=listToFilter->next;
                comparator=comparator->nextRow;
            }
            else
            {
                comparator=comparator->nextRow;
            }

    }
}
/**
 *  splits a community into to two
 * @param communityToSplit -the community that we are to spit
 * @param splitter the array of accordingly we split by
 * @return a tuple of (see ds.c) 2 of which one must contain a community.
 */
tuple2 *splitCommunities(communityDescription communityToSplit, double *splitter) {
    rowLinkedList holderCurrentGroup, holderMinusGroup,holderPlusGroup;
    networkStatsSet community1NetworkStats, community2NetworkStas;
    rowLinkedList *originalList, *currentMinusGroup,*currentPlusGroup;
    rowLinkedList *newGraphsArr[2];
    tuple2 *communityDescriptionArr;
    rowLinkedList* comparator;
    int spliterIndex;
    colLinkedList * listToFilter=smemory(sizeof(colLinkedList),1);
    currentPlusGroup=smemory(sizeof(currentPlusGroup),1);
    listToFilter->colIndex=-1;
    holderCurrentGroup.nextRow = communityToSplit.graph;
    holderMinusGroup.nextRow=NULL;
    holderCurrentGroup.rowIndex=-1;
    holderMinusGroup.rowIndex=-1;
    holderPlusGroup.rowIndex=-1;
    holderPlusGroup.nextRow=NULL;
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

    currentPlusGroup=holderPlusGroup.nextRow;
    currentMinusGroup=holderMinusGroup.nextRow;

    if(currentPlusGroup){
        listToFilter->next=currentPlusGroup->colList;
        comparator=currentPlusGroup;
        filterColumn(comparator, listToFilter);
    }
    if(currentMinusGroup){
        listToFilter->next=currentMinusGroup->colList;
        comparator=currentMinusGroup;
        filterColumn(comparator, listToFilter);
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
/**
 * implements the algo2 which has been described in specification. main function of the file.
 * @param Ag - a linked list of rows representing an A matrix
 * @param AgStat stats of A
 * @return results of diviison, in an object. used to contain an error safeguard but it was dropped as it was not required.
 */
divisionResults* algo2(rowLinkedList *Ag, networkStatsSet *AgStat) {

    communityDescription currentCommunity;
    int vectorLength;
    eigen division;
    double sBs;
    tuple2 *communitiesAfterSplitting;
    int isindivisible;
    double shift;
    int k=0;

    vectorLength = AgStat->vertices;
    currentCommunity.networkStat = AgStat;
    currentCommunity.graph = Ag;
    isindivisible=0;
    shift=norm1(Ag,AgStat);
    division = powerIterationOnB(Ag, AgStat);
    if ( eigenValue(Ag, AgStat,
                       division.vector,
                        vectorLength, shift) < 0) {
       isindivisible=1;
    }
    sBs = billinearMultiplicationOfBUnoptimized(Ag, AgStat, vectorLength, division.vector, shift);
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

