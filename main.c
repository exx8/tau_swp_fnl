#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

#define intsize 4
struct _networkStats {
    int vertices;
    int edges;
    ///New addition
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

edge* buildEdgeArr( FILE *file, networkStats *networkStat) {
    edge *edgeArr = memory((*networkStat).edges, sizeof(edge));

    ///Addition - Need to free memory
    networkStat->vertexDegreeArray = (int *) malloc(networkStat->vertices * sizeof(int));
    int vertexIndex = 0;

    edge *edgePointer = edgeArr;
    int edgePrimaryIndex = 0;
    while (edgePrimaryIndex<networkStat->vertices) {
        int verticesLeft;
        readInt(file, &verticesLeft);

        ///Addition
        networkStat->vertexDegreeArray[vertexIndex] = verticesLeft;
        networkStat->degreeSum += verticesLeft;

        copyVertexNeighbor(file, &edgePointer, edgePrimaryIndex, verticesLeft);
        edgePrimaryIndex++;

    }
    return edgeArr;
}

/// Addition
int fillDegreeMatrix(int * matrix, networkStats *networkStat){
    int currVertexIndex = 0;
    int overAllCounter = 0;
    int degreeMulSum = 0;
    while (currVertexIndex < networkStat->vertices){
        int i = 0;
        while (i < networkStat->vertices){
            int verticesDegreeMul = (networkStat->vertexDegreeArray[currVertexIndex] * networkStat->vertexDegreeArray[i]);
            degreeMulSum += verticesDegreeMul;
            matrix[overAllCounter] = verticesDegreeMul;
            overAllCounter++;
        }
    }

    return degreeMulSum;
}


int calculateModularity(networkStats *networkStat){

    int verticesSquared = (networkStat->vertices * networkStat->vertices);
    int * degreeMatrix = (int *) malloc (verticesSquared * sizeof(int));
    int degreeMulSum = fillDegreeMatrix(degreeMatrix, networkStat);
    int modularity = (2 * networkStat->edges) - degreeMulSum/networkStat->degreeSum;

}

edge * readInputFile(char *filePath) {
    const fileLengthInBytes = filesize(filePath);
    FILE *file = fopen(filePath, "r");
    assert(file!=NULL);
    networkStats networkStat = getNetworkStats(file, fileLengthInBytes);

    return buildEdgeArr(file, &networkStat);
    fclose(file);
}


networkStats getNetworkStats(FILE *file, int fileLengthInBytes) {
    networkStats networkStat;
    int verticesNum = getVertices(file);
    const int edgesNum = ((fileLengthInBytes)/4-verticesNum-1)/2;
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
    edge* graphData=readInputFile("/home/eran/Downloads/graph.in");
    free(graphData);
    return 0;
}
