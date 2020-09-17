//
// Created by eran on 29/08/2020.
//

#include "networkStats.h"

#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdio.h>

struct _colLinkedList {
    int colIndex;
    struct _colLinkedList* next;
} typedef colLinkedList;
struct _rowLinkedList {
    int rowIndex;
    struct _rowLinkedList* nextRow;
    struct _colLinkedList* colList;
    int side;
    int marked;
} typedef rowLinkedList;
void deleteNextRow(rowLinkedList* l){
    rowLinkedList* lNext= l->nextRow;
    if(lNext!=NULL)
    l->nextRow=l->nextRow->nextRow;
    free(lNext);

}

void deleteNextCol(colLinkedList* l){
    colLinkedList* lNext;
    l->next=l->next->next;
    free(lNext);

}

rowLinkedList* newRowLinkedList(int index,rowLinkedList* nextRow,colLinkedList* colList)
{
    rowLinkedList* returned=memory(sizeof(rowLinkedList),1);
    returned->rowIndex=index;
    returned->nextRow=nextRow;
    returned->colList=colList;
    returned->marked=0;
    returned->side=-10;
    return returned;

}

colLinkedList* newColLinkedList(int index,colLinkedList* next)
{
    colLinkedList* returned=memory(sizeof(colLinkedList),1);
    returned->colIndex=index;
    returned->next=next;
    return returned;

}


void freeCol(colLinkedList* list)
{
    if(list==NULL)
        return;
    freeCol(list->next);
    free(list);
}
void freeGraph(rowLinkedList* list)
{
    if(list==NULL)
        return;
    freeGraph(list->nextRow);
    freeCol(list->colList);
    free(list);
}

struct _colLinkedListSparseMatrix {
    int colIndex;
    double value;
    struct _colLinkedListSparseMatrix* next;
} typedef colLinkedListSparseMatrix;


struct _rowLinkedListSparseMatrix {
    int rowIndex;
    struct _rowLinkedListSparseMatrix* nextRow;
    struct _colLinkedListSparseMatrix* colList;
} typedef rowLinkedListSparseMatrix;

rowLinkedListSparseMatrix* newRowLinkedListSparseMatrix(int index,rowLinkedListSparseMatrix* nextRow,colLinkedListSparseMatrix* colList)
{
    rowLinkedListSparseMatrix * returned=memory(sizeof(rowLinkedListSparseMatrix),1);
    returned->rowIndex=index;
    returned->nextRow=nextRow;
    returned->colList=colList;
    return returned;

}

colLinkedListSparseMatrix* newColLinkedListSparseMatrix(int index,colLinkedListSparseMatrix* next)
{
    colLinkedListSparseMatrix* returned=memory(sizeof(colLinkedList),1);
    returned->colIndex=index;
    returned->next=next;
    return returned;

}
struct _communityDescription{
    networkStatsSet* networkStat;
    rowLinkedList* graph;
} typedef communityDescription;

communityDescription * newCommunityDescription(networkStatsSet *ns,rowLinkedList* graph)
{
    communityDescription *returned=memory(sizeof(communityDescription),1);

            returned->networkStat=ns;
            returned->graph=graph;
    return returned;
}
struct _communitiesList {
    communityDescription *communityInfo;
    struct _communitiesList* next;
} typedef communitiesList;







void freeNested(communitiesList * d)
{
if(d==NULL||d->communityInfo==NULL)
return;
freeNested(d->next);
free(d->communityInfo->networkStat);
freeGraph(d->communityInfo->graph);
free(d->communityInfo);
}

void freeCommunitiesList(communitiesList * d)
{
    int *vertexDegreeArray = d->communityInfo->networkStat->vertexDegreeArray;
    free(vertexDegreeArray);
    freeNested(d->next);
    free(d);
}

struct _tuple2{
    communityDescription* first;
    communityDescription* second;
} typedef tuple2;

struct _divisionResults{
    int errorNum; //whereas 0 stands for no error
    tuple2 * value;
} typedef divisionResults;