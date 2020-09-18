
#include "networkStats.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

typedef struct _colLinkedList {
    int colIndex;
    struct _colLinkedList* next;
}colLinkedList;
typedef struct _rowLinkedList {
    int rowIndex;
    struct _rowLinkedList* nextRow;
    struct _colLinkedList* colList;
    int side;
    int marked;
}rowLinkedList;

void deleteNextRow(rowLinkedList* l){
    rowLinkedList* lNext= l->nextRow;
    if(lNext!=NULL)
    l->nextRow=l->nextRow->nextRow;
    free(lNext);
}

void deleteNextCol(colLinkedList* l){
    l->next=l->next->next;

}

rowLinkedList* newRowLinkedList(int index,rowLinkedList* nextRow,colLinkedList* colList)
{
    rowLinkedList* returned;
    returned=smemory(sizeof(rowLinkedList),1);
    returned->rowIndex=index;
    returned->nextRow=nextRow;
    returned->colList=colList;
    returned->marked=0;
    returned->side=-10;
    return returned;

}

colLinkedList* newColLinkedList(int index,colLinkedList* next)
{
    colLinkedList* returned;
    returned=smemory(sizeof(colLinkedList),1);
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

typedef struct _colLinkedListSparseMatrix {
    int colIndex;
    double value;
    struct _colLinkedListSparseMatrix* next;
}colLinkedListSparseMatrix;

/*shiftAmount indicates the maximum column sum which is added in the matrix shifting algorithm*/
typedef struct _rowLinkedListSparseMatrix {
    int rowIndex;
    int shiftAmount;
    struct _rowLinkedListSparseMatrix* nextRow;
    struct _colLinkedListSparseMatrix* colList;
}rowLinkedListSparseMatrix;

rowLinkedListSparseMatrix* newRowLinkedListSparseMatrix(int index,rowLinkedListSparseMatrix* nextRow,colLinkedListSparseMatrix* colList)
{
    rowLinkedListSparseMatrix * returned;
    returned=memory(sizeof(rowLinkedListSparseMatrix),1);
    returned->rowIndex=index;
    returned->nextRow=nextRow;
    returned->colList=colList;
    return returned;

}

colLinkedListSparseMatrix* newColLinkedListSparseMatrix(int index,colLinkedListSparseMatrix* next)
{
    colLinkedListSparseMatrix* returned;
    returned=memory(sizeof(colLinkedList),1);
    returned->colIndex=index;
    returned->next=next;
    return returned;

}
typedef struct _communityDescription{
    networkStatsSet* networkStat;
    rowLinkedList* graph;
}communityDescription;

communityDescription * newCommunityDescription(networkStatsSet *ns,rowLinkedList* graph)
{
    communityDescription *returned;
    returned=smemory(sizeof(communityDescription),1);

            returned->networkStat=ns;
            returned->graph=graph;
    return returned;
}
typedef struct _communitiesList {
    communityDescription *communityInfo;
    struct _communitiesList* next;
}communitiesList;


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
    int *vertexDegreeArray;
    vertexDegreeArray = d->communityInfo->networkStat->vertexDegreeArray;
    free(vertexDegreeArray);
    freeNested(d->next);
    free(d);
}

typedef struct _tuple2{
    communityDescription* first;
    communityDescription* second;
}tuple2;

typedef struct _divisionResults{
    int errorNum; /*whereas 0 stands for no error*/
    tuple2 * value;
}divisionResults;