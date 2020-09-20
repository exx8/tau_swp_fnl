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

/**
 * get a file's size
 * @param filePath
 * @return the size in bits.
 */

int filesize(char *filePath) {
    struct stat details;
    int status;

    status = stat(filePath, &details);
    if (status == 0)
        return details.st_size;
    makesure(status != 0,6,"couldn't read file stats");
    return -1;
}
/**
 * reads a single bit
 * @param file a file pointer
 * @param intPointer the pointer to write into.
 * @return 1 always.
 */
int readInt( FILE *file,  int *intPointer) {
    int numRead;
    numRead = fread(intPointer,intsize,1,file);
    makesure(numRead == 1,7,"expected to read a number. no number was found.");
    return numRead;
}

/**
 * builds a graph's vertex col list
 * @param file - file to read
 * @param verticesLeft - how much vertices there are to read from file
 * @return collinkedlist of the read ones.
 */
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
/**
 * dount the number of cols in a row
 * @param col collinkedlist
 * @return the number of edges in the set.
 */
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
/**
 * given a binary file loads the main ds that describes a graph
 * @param file- a file pointer
 * @param networkStat - stats of the network
 * @return a matrix represented by its first row
 */
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
/**
 * given a binary file returns a cluster. the most important function in the program
 * @param filePath a file path
 * @return a cluster.
 */
communitiesList*  getACluster(char *filePath) {
    rowLinkedList* graphData;
    communitiesList* returned;
    int fileLengthInBytes;
    FILE *file;
    networkStatsSet* networkStat;
    communityDescription *firstCommunity;

    fileLengthInBytes = filesize(filePath);
    file = fopen(filePath, "r");
    makesure(file != NULL,8,"couldn't open file. Has it been opened with a different process?");
    networkStat = getNetworkStats(file, fileLengthInBytes);
    graphData=loadAdjacencyMatrixDataStructures(file, networkStat);

    firstCommunity = newCommunityDescription(networkStat, graphData);
    returned=algo3(firstCommunity);
    fclose(file);
    return returned;
}

/**
 * given a file returns the number of vertices
 * @param file
 * @return
 */
int getVerticesNum(FILE *file) {
    int verticesNum;
    verticesNum = 0;
    readInt(file, &verticesNum);
    return verticesNum;
}

/**
 * main function
 * @param argc must be 3: the program path, input and output otherwise stops
 * @param argv a one dimension array
 * @return 0 if succeed, or a positive error id on failure
 */

int main(int argc,char** argv) {
    communitiesList * divisionResults;
    if(argc!=3)
    {
        error(3,"too few argument");
    }
    divisionResults= getACluster(argv[1]);
    output(divisionResults,argv[2]);
    freeThemAll();

    return 0;
}
