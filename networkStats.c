//
// Created by eran on 24/08/2020.
//

#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include "utils.h"
int getVertices( FILE *file);

struct _networkStats {
    int vertices;
    int edges;
    int * vertexDegreeArray; //CAUTION: for avoiding initing array for each community, the array MAY contain data about extenrnal vertices
    int degreeSum;

} typedef networkStatsSet;


void updateNetworkStat(networkStatsSet *networkStat, int vertexIndex, int verticesLeft) {
    networkStat->vertexDegreeArray[vertexIndex] = verticesLeft;
    networkStat->degreeSum += verticesLeft;
}

void releaseNetworkStat(networkStatsSet *networkStat) { free((*networkStat).vertexDegreeArray); }

networkStatsSet* getNetworkStats(FILE *file, int fileLengthInBytes) {

    networkStatsSet* networkStat=smemory(sizeof(networkStatsSet),1);
    int verticesNum = getVertices(file);
    const int edgesNum = ((fileLengthInBytes)/4-verticesNum-1)/2;
    networkStat->vertices = verticesNum;
    networkStat->edges = edgesNum;
    networkStat->vertexDegreeArray = (int *) smemory(networkStat->vertices , sizeof(int));
     networkStat->degreeSum=0; // only initing not setting
    return networkStat;


}
networkStatsSet emptyNetworkstats(){
    networkStatsSet returned;
    returned.edges=0;
    returned.vertices=0;
    returned.vertexDegreeArray=NULL;
    returned.degreeSum=0;

    return returned;
}