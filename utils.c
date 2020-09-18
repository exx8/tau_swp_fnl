//
// Created by eran on 24/08/2020.
//

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#define IS_POSITIVE(X) ((X) > 0.00001)
struct linkedList{
    void* address;
    struct linkedList* next;
}typedef liknedList;



static liknedList* memoryList=NULL;

void *smemory(int sizeOfCell, size_t numberOfCell)
{
    void* value=memory(sizeOfCell,numberOfCell);
    liknedList* newNode=memory(sizeof(liknedList),1);
    newNode->next=memoryList;
    newNode->address=value;
    memoryList=newNode;
    return value;
}
void freeThemAll()
{
    liknedList * currentNode=memoryList;
    while(memoryList!=NULL)
    {
        struct linkedList *next = memoryList->next;
        free(memoryList->address);
        free(memoryList);

        memoryList= next;
    }
}

void error(int errorID,char* errorReason)
{
    printf("Error:%s",errorReason);
    exit(errorID);
}
makesure(int condition)
{
    if(condition)
        error(5,"assert condition failed");
}
void *memory(int sizeOfCell, size_t numberOfCell) {
    void *buffer = calloc(sizeOfCell, numberOfCell);
    makesure(buffer != NULL);
    return buffer;

}