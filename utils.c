typedef struct _linkedList{
    void* address;
    struct _linkedList* next;
} linkedlist;

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#define IS_POSITIVE(X) ((X) > 0.00001)


void error(int errorID,char* errorReason)
{
    printf("Error:%s",errorReason);
    exit(errorID);
}
void makesure(int condition)
{
    if(!condition)
        error(5,"assert condition failed");
}
static linkedlist* memoryList=NULL;
void *memory(int sizeOfCell, size_t numberOfCell) {
    void *buffer;
    buffer = calloc(sizeOfCell, numberOfCell);
    makesure(buffer != NULL);
    return buffer;

}
void *smemory(int sizeOfCell, size_t numberOfCell)
{
    void* value;
    linkedlist* newNode;
    value=memory(sizeOfCell,numberOfCell);
    newNode =memory(sizeof(linkedlist),1);
    newNode->next=memoryList;
    newNode->address=value;
    memoryList=newNode;
    return value;
}
void freeThemAll()
{
    while(memoryList!=NULL)
    {
        linkedlist *next;
        next= memoryList->next;
        free(memoryList->address);
        free(memoryList);

        memoryList= next;
    }
}



