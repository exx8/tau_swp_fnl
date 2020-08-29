//
// Created by eran on 29/08/2020.
//

#include "networkStats.h"
#include "utils.h"
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
} typedef rowLinkedList;

rowLinkedList* newRowLinkedList(int index,rowLinkedList* nextRow,colLinkedList* colList)
{
    rowLinkedList* returned=memory(sizeof(rowLinkedList),1);
    returned->rowIndex=index;
    returned->nextRow=nextRow;
    returned->colList=colList;
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
void freeData(rowLinkedList* list)
{
    if(list==NULL)
        return;
    freeData(list->nextRow);
    freeCol(list->colList);
    free(list);
}