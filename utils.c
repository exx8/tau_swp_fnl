//
// Created by eran on 24/08/2020.
//

#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdio.h>

void *memory(int sizeOfCell, size_t numberOfCell) {
    void *buffer = calloc(sizeOfCell, numberOfCell);
    assert(buffer != NULL);
    return buffer;

}