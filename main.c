#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include "utils.h"
#include "ds.h"
#include "algo2.h"
#include "algo3.h"
#define intsize 4


int filesize(char *filePath) {
    struct stat details;

    const int status = stat(filePath, &details);
    if (status == 0)
        return details.st_size;
    assert(status != 0);
    return -1;
}

int readInt( FILE *file,  int *intPointer) {
    int numRead =fread(intPointer,intsize,1,file);
    assert(numRead==1);
}


colLinkedList* copyVertexNeighbor(FILE *file, int verticesLeft) {
    if(verticesLeft==0)
        return NULL;
    int edgeNeighborIndex = 0;
    readInt(file, &edgeNeighborIndex);
    verticesLeft--;

    colLinkedList* primary=newColLinkedList(edgeNeighborIndex, NULL);
    colLinkedList* current=primary;
    while (verticesLeft > 0) {
        readInt(file, &edgeNeighborIndex);

        current->next=newColLinkedList(edgeNeighborIndex,NULL);
        current=current->next;
        verticesLeft--;
    }
    return  primary;
}

rowLinkedList* loadAdjacencyMatrixDataStructures(FILE *file, networkStatsSet *networkStat) {

    int vertexIndex = 0;
    rowLinkedList* holder=newRowLinkedList(-1,NULL,NULL),// holder won't be returned
            *returned;
    rowLinkedList* current= holder;
    int edgeRowIndex = 0;
    while (edgeRowIndex < networkStat->vertices) {
        int verticesLeft;
        readInt(file, &verticesLeft);
        current->nextRow=newRowLinkedList(edgeRowIndex,NULL, NULL);
        current=current->nextRow;
        updateNetworkStat(networkStat, vertexIndex, verticesLeft);
        current->colList=copyVertexNeighbor(file, verticesLeft);
        edgeRowIndex++;
        vertexIndex++;

    }
    returned=holder->nextRow;
    free(holder);
    return returned;

}

communitiesList*  readInputFile(char *filePath) {
    rowLinkedList* graphData;
    communitiesList* returned;
    const fileLengthInBytes = filesize(filePath);
    FILE *file = fopen(filePath, "r");
    assert(file!=NULL);
    networkStatsSet* networkStat = getNetworkStats(file, fileLengthInBytes);
    graphData=loadAdjacencyMatrixDataStructures(file, networkStat);

    returned=algo3(newCommunityDescription(networkStat, graphData));

    fclose(file);
    return returned;
}


int getVertices( FILE *file) {
    int verticesNum = 0;
    readInt(file, &verticesNum);
    return verticesNum;
}



int main() {
    communitiesList * divisionResults=readInputFile("/home/eran/Desktop/swp_samples/graph.in");
    freeCommunitiesList(divisionResults);
    return 0;
}
