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

void *memory(int num, size_t size) {
    void *buffer = calloc(num, size);
    assert(buffer != NULL);
    return buffer;

}

int getVertices(const FILE *file);

networkStats getNetworkStats(FILE *file, int fileLengthInBytes);

int filesize(char *filePath) {
    struct stat details;

    const int status = stat(filePath, &details);
    if (status == 0)
        return details.st_size;
    assert(status != 0);
    return -1;
}

int readInt(const FILE *file, const int *intPointer) {
    fscanf(file, "%d", intPointer);
}


void copyVertexNeighbor(const FILE *file, edge *edgePointer, int edgePrimaryIndex, int verticesLeft) {
    while (verticesLeft > 0) {
        int edgeNeighborIndex = 0;
        readInt(file, edgeNeighborIndex);
        if (edgeNeighborIndex > edgePrimaryIndex) {
            (edgePointer)->highIndex = edgeNeighborIndex;
            (edgePointer)->lowIndex = edgePrimaryIndex;
        }
        edgePointer++;
        verticesLeft--;
    }
}

BuildEdgeArr(const FILE *file, networkStats *networkStat) {
    edge *edgeArr = memory((*networkStat).edges, sizeof(edge));
    edge *edgePointer = edgeArr;
    while (feof(file) == 0) {
        int edgePrimaryIndex = 0, verticesLeft;
        readInt(file, &verticesLeft);
        copyVertexNeighbor(file, edgePointer, edgePrimaryIndex, verticesLeft);
        edgePrimaryIndex++;

    }
    return edgeArr;
}

void readInputFile(char *filePath) {
    const fileLengthInBytes = filesize(filePath);
    const FILE *file = fopen(filePath, "r");
    networkStats networkStat = getNetworkStats(file, fileLengthInBytes);

    BuildEdgeArr(file, &networkStat);
    fclose(file);
}


networkStats getNetworkStats(FILE *file, int fileLengthInBytes) {
    networkStats networkStat;
    int verticesNum = getVertices(file);
    const int edgesNum = (fileLengthInBytes - verticesNum - 1) / 2;
    networkStat.vertices = verticesNum;
    networkStat.edges = edgesNum;
    return networkStat;


}


int getVertices(const FILE *file) {
    const int verticesNum = 0;
    readInt(file, &verticesNum);
    return verticesNum;
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
