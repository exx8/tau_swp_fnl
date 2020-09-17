//
// Created by eran on 24/08/2020.
//

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#define IS_POSITIVE(X) ((X) > 0.00001)



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