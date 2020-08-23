#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
//#include "algo1.c"

#define intsize 4
struct _networkStats {
    int vertices;
    int edges;
    int * vertexDegreeArray;
    int degreeSum;

} typedef networkStats;
struct _edge {
    int highIndex;
    int lowIndex;
} typedef edge;

void *memory(int sizeOfCell, size_t numberOfCell) {
    void *buffer = calloc(sizeOfCell, numberOfCell);
    assert(buffer != NULL);
    return buffer;

}

int getVertices( FILE *file);

networkStats getNetworkStats(FILE *file, int fileLengthInBytes);

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
            (*edgePointer)++;

        }
        verticesLeft--;
    }
}

void updateNetworkStat(networkStats *networkStat, int vertexIndex, int verticesLeft) {
    networkStat->vertexDegreeArray[vertexIndex] = verticesLeft;
    networkStat->degreeSum += verticesLeft;
}

void loadAdjacencyMatrixDataStructures(FILE *file, networkStats *networkStat,edge* edgeArr) {

    int vertexIndex = 0;

    edge *edgePointer = edgeArr;
    int edgePrimaryIndex = 0;
    while (edgePrimaryIndex<networkStat->vertices) {
        int verticesLeft;
        readInt(file, &verticesLeft);

        updateNetworkStat(networkStat, vertexIndex, verticesLeft);

        copyVertexNeighbor(file, &edgePointer, edgePrimaryIndex, verticesLeft);
        edgePrimaryIndex++;

    }
}

void releaseNetworkStat(networkStats *networkStat) { free((*networkStat).vertexDegreeArray); }

edge * readInputFile(char *filePath) {
    const fileLengthInBytes = filesize(filePath);
    FILE *file = fopen(filePath, "r");
    assert(file!=NULL);
    networkStats networkStat = getNetworkStats(file, fileLengthInBytes);
    edge *edgeArr = memory((networkStat).edges, sizeof(edge));
    loadAdjacencyMatrixDataStructures(file, &networkStat,&edgeArr);

    releaseNetworkStat(&networkStat);
    fclose(file);
    return edgeArr;
}


networkStats getNetworkStats(FILE *file, int fileLengthInBytes) {
    networkStats networkStat;
    int verticesNum = getVertices(file);
    const int edgesNum = ((fileLengthInBytes)/4-verticesNum-1)/2;
    networkStat.vertices = verticesNum;
    networkStat.edges = edgesNum;
    networkStat.vertexDegreeArray = (int *) memory(networkStat.vertices , sizeof(int));
    return networkStat;


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
