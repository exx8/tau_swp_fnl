typedef struct _linkedList{
    void* address;
    struct _linkedList* next;
} linkedlist;

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#define IS_POSITIVE(X) ((X) > 0.00001)

/**
 * prints and error and ends the program
 * @param errorID- id which the user must keep it to be unique.
 * @param errorReason - a string that explains why the error has happened
 */
void error(int errorID,char* errorReason)
{
    printf("Error:%s",errorReason);
    exit(errorID);
}
/**
 * a function that makes sure a condition was met,
 * otherwise gives error the privillege to stop the process
 * @param condition a number which if condition==0 ⇒ the software halts with the right message
 * @param errorID a unique id to mark the case
 * @param str some message to be printed to explain the user why he is goodbyed.
 */
void makesure(int condition,int errorID,char* str)
{
    if(!condition)
        error(errorID,str);
}
static linkedlist* memoryList=NULL;

/**
 * the function that manages the whole memory in the software. makesure that memory was allocated successfully
 * @param sizeOfCell the size of cell
 * @param numberOfCell number of cells to be allocated
 * @return a buffer of of memory with the relevant size
 */
void *memory(int sizeOfCell, size_t numberOfCell) {
    void *buffer;
    buffer = calloc(sizeOfCell, numberOfCell);
    makesure(buffer != NULL,5,"couldn't allocate memory");
    return buffer;

}
/**
 * smart memory
 * a util that allows the memory to be auto managed throughout some of the program life cycle. self made.
 * @param sizeOfCell like in memory
 * @param numberOfCell like in memory
 * @return like in memory
 */
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
/**
 * a util that must call once in the end of the process which frees the memory without structure's dependent topology.
 */
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



