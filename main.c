#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include "utils.h"
#include "networkStats.h"
//#include "algo1.h"

#define intsize 4
struct _edge {
    int highIndex;
    int lowIndex;
    struct _edge* next;
} typedef edge;

int getVertices( FILE *file);

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


void copyVertexNeighbor( FILE *file, edge **edgePointer, int edgePrimaryIndex, int verticesLeft) {
    while (verticesLeft > 0) {
        int edgeNeighborIndex = 0;
        readInt(file, &edgeNeighborIndex);
        if (edgeNeighborIndex > edgePrimaryIndex) {

            (*edgePointer)->highIndex = edgeNeighborIndex;
            (*edgePointer)->lowIndex = edgePrimaryIndex;
            if(verticesLeft>1) {
                (*edgePointer)->next = memory(1, sizeof(edge));
                *edgePointer = (*edgePointer)->next;
            }

        }
        verticesLeft--;
    }

}

void loadAdjacencyMatrixDataStructures(FILE *file, networkStatsSet *networkStat, edge* edgeLinkedList) {

    int vertexIndex = 0;

    edge* edgePointer = edgeLinkedList;
    int edgePrimaryIndex = 0;
    while (edgePrimaryIndex<networkStat->vertices) {
        int verticesLeft;
        readInt(file, &verticesLeft);

        updateNetworkStat(networkStat, vertexIndex, verticesLeft);

        copyVertexNeighbor(file, &edgePointer, edgePrimaryIndex, verticesLeft);
        edgePrimaryIndex++;

    }
}

edge*  readInputFile(char *filePath) {
    const fileLengthInBytes = filesize(filePath);
    FILE *file = fopen(filePath, "r");
    assert(file!=NULL);
    networkStatsSet networkStat = getNetworkStats(file, fileLengthInBytes);
    edge* edgeLinkedList = (edge*)memory(1, sizeof(edge));
    loadAdjacencyMatrixDataStructures(file, &networkStat,edgeLinkedList);

    releaseNetworkStat(&networkStat);
    fclose(file);
    return edgeLinkedList;
}


int getVertices( FILE *file) {
    int verticesNum = 0;
    readInt(file, &verticesNum);
    return verticesNum;
}



int main() {
    edge* graphData=readInputFile("/home/eran/Downloads/graph.in");
    free(graphData);
    return 0;
}
