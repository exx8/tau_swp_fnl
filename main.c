#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

#define intsize 4
struct _networkStats {
    int vertices;
    int edges;
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


void copyVertexNeighbor( FILE *file, edge *edgePointer, int edgePrimaryIndex, int verticesLeft) {
    while (verticesLeft > 0) {
        int edgeNeighborIndex = 0;
        readInt(file, &edgeNeighborIndex);
        if (edgeNeighborIndex > edgePrimaryIndex) {
            (edgePointer)->highIndex = edgeNeighborIndex;
            (edgePointer)->lowIndex = edgePrimaryIndex;
        }
        edgePointer++;
        verticesLeft--;
    }
}

edge* buildEdgeArr( FILE *file, networkStats *networkStat) {
    edge *edgeArr = memory((*networkStat).edges, sizeof(edge));
    int edgePrimaryIndex = 0;
    while (feof(file) == 0) {
        int verticesLeft;
        readInt(file, &verticesLeft);
        copyVertexNeighbor(file, edgeArr, edgePrimaryIndex, verticesLeft);
        edgePrimaryIndex++;

    }
    return edgeArr;
}

void readInputFile(char *filePath) {
    const fileLengthInBytes = filesize(filePath);
     FILE *file = fopen(filePath, "r");
    networkStats networkStat = getNetworkStats(file, fileLengthInBytes);

    buildEdgeArr(file, &networkStat);
    fclose(file);
}


networkStats getNetworkStats(FILE *file, int fileLengthInBytes) {
    networkStats networkStat;
    int verticesNum = getVertices(file);
    const int edgesNum = (fileLengthInBytes - verticesNum - 1) / (2*4);
    networkStat.vertices = verticesNum;
    networkStat.edges = edgesNum;
    return networkStat;


}


int getVertices( FILE *file) {
     int verticesNum = 0;
    readInt(file, &verticesNum);
    return verticesNum;
}

int main() {
    readInputFile("/home/eran/Downloads/graph.in");
    printf("Hello, World!\n");
    return 0;
}
