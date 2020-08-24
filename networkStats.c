//
// Created by eran on 24/08/2020.
//

#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdio.h>
#include "utils.h"

struct _networkStats {
    int vertices;
    int edges;
    int * vertexDegreeArray;
    int degreeSum;

} typedef networkStatsSet;

networkStatsSet getNetworkStats(FILE *file, int fileLengthInBytes);

void updateNetworkStat(networkStatsSet *networkStat, int vertexIndex, int verticesLeft) {
    networkStat->vertexDegreeArray[vertexIndex] = verticesLeft;
    networkStat->degreeSum += verticesLeft;
}

void releaseNetworkStat(networkStatsSet *networkStat) { free((*networkStat).vertexDegreeArray); }

networkStatsSet getNetworkStats(FILE *file, int fileLengthInBytes) {
    networkStatsSet networkStat;
    int verticesNum = getVertices(file);
    const int edgesNum = ((fileLengthInBytes)/4-verticesNum-1)/2;
    networkStat.vertices = verticesNum;
    networkStat.edges = edgesNum;
    networkStat.vertexDegreeArray = (int *) memory(networkStat.vertices , sizeof(int));
    return networkStat;


}