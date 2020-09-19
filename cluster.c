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
    int status;

    status = stat(filePath, &details);
    if (status == 0)
        return details.st_size;
    makesure(status != 0);
    return -1;
}

int readInt( FILE *file,  int *intPointer) {
    int numRead;
    numRead = fread(intPointer,intsize,1,file);
    makesure(numRead == 1);
    return numRead;
}


colLinkedList* copyVertexNeighbor(FILE *file, int verticesLeft) {
    int edgeNeighborIndex;
    colLinkedList* primary;
    colLinkedList* current;
    if(verticesLeft==0)
        return NULL;
    edgeNeighborIndex = 0;
    readInt(file, &edgeNeighborIndex);
    verticesLeft--;
    primary=newColLinkedList(edgeNeighborIndex, NULL);
    current=primary;
    while (verticesLeft > 0) {
        readInt(file, &edgeNeighborIndex);

        current->next=newColLinkedList(edgeNeighborIndex,NULL);
        current=current->next;
        verticesLeft--;
    }
    return  primary;
}
double countColLIst(colLinkedList* col)
{
    double counter;
    counter=0;
    while(col!=NULL)
    {
        counter++;
    col=col->next;
    }
    return counter;
}

rowLinkedList* loadAdjacencyMatrixDataStructures(FILE *file, networkStatsSet *networkStat) {

    int vertexIndex;
    rowLinkedList* holder,/* holder won't be returned*/
    *returned;
    rowLinkedList* current;
    int edgeRowIndex;
    vertexIndex= 0;
    holder = newRowLinkedList(-1,NULL,NULL);
    current = holder;
    edgeRowIndex= 0;
    while (edgeRowIndex < networkStat->vertices) {
        int verticesLeft;
        readInt(file, &verticesLeft);
        current->nextRow=newRowLinkedList(edgeRowIndex,NULL, NULL);
        current=current->nextRow;
        updateNetworkStat(networkStat, vertexIndex, verticesLeft);
        current->colList=copyVertexNeighbor(file, verticesLeft);
        current->numOfCols=countColLIst(current->colList);
        edgeRowIndex++;
        vertexIndex++;

    }
    returned=holder->nextRow;
    return returned;

}

communitiesList*  readInputFile(char *filePath) {
    rowLinkedList* graphData;
    communitiesList* returned;
    int fileLengthInBytes;
    FILE *file;
    networkStatsSet* networkStat;
    communityDescription *firstCommunity;

    fileLengthInBytes = filesize(filePath);
    file = fopen(filePath, "r");
    makesure(file != NULL);
    networkStat = getNetworkStats(file, fileLengthInBytes);
    graphData=loadAdjacencyMatrixDataStructures(file, networkStat);

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
    communitiesList * divisionResults;
    if(argc!=3)
    {
        error(3,"too few argument");
    }
    divisionResults=readInputFile(argv[1]);
    output(divisionResults,argv[2]);
    freeThemAll();

    return 0;
}
