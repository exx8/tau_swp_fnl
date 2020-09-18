#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "utils.h"
#include "ds.h"
#include "algo2.h"
#include "shift.h"
#include "algo3.h"
#include "output.h"
#define intsize 4



int filesize(char *filePath) {
    struct stat details;

    const int status = stat(filePath, &details);
    if (status == 0)
        return details.st_size;
    makesurenot(status != 0);
    return -1;
}

int readInt( FILE *file,  int *intPointer) {
    int numRead;
    numRead = fread(intPointer,intsize,1,file);
    makesurenot(numRead == 1);
    return numRead;
}


colLinkedList* copyVertexNeighbor(FILE *file, int verticesLeft) {
    if(verticesLeft==0)
        return NULL;
    int edgeNeighborIndex;
    edgeNeighborIndex = 0;
    readInt(file, &edgeNeighborIndex);
    verticesLeft--;

    colLinkedList* primary;
    primary=newColLinkedList(edgeNeighborIndex, NULL);
    colLinkedList* current;
    current=primary;
    while (verticesLeft > 0) {
        readInt(file, &edgeNeighborIndex);

        current->next=newColLinkedList(edgeNeighborIndex,NULL);
        current=current->next;
        verticesLeft--;
    }
    return  primary;
}

rowLinkedList* loadAdjacencyMatrixDataStructures(FILE *file, networkStatsSet *networkStat) {

    int vertexIndex;
    vertexIndex= 0;
    rowLinkedList* holder,/* holder won't be returned*/
            *returned;
    holder = newRowLinkedList(-1,NULL,NULL);
    rowLinkedList* current;
    current = holder;
    int edgeRowIndex;
    edgeRowIndex= 0;
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
    return returned;

}

communitiesList*  readInputFile(char *filePath) {
    rowLinkedList* graphData;
    communitiesList* returned;
    const fileLengthInBytes;
    fileLengthInBytes= filesize(filePath);
    FILE *file;
    file = fopen(filePath, "r");
    makesurenot(file != NULL);
    networkStatsSet* networkStat;
    networkStat = getNetworkStats(file, fileLengthInBytes);
    graphData=loadAdjacencyMatrixDataStructures(file, networkStat);

    communityDescription *firstCommunity;
    firstCommunity = newCommunityDescription(networkStat, graphData);
    returned=algo3(firstCommunity);
    fclose(file);
    return returned;
}


int getVertices( FILE *file) {
    int verticesNum;
    verticesNum = 0;
    readInt(file, &verticesNum);
    return verticesNum;
}



int main(int argc,char** argv) {
    if(argc!=3)
    {
        error(3,"too few argument");
    }

    communitiesList * divisionResults;
    divisionResults=readInputFile(argv[1]);
    output(divisionResults,argv[2]);
    freeThemAll();

    return 0;
}