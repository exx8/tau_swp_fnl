//
// Created by eran on 17/09/2020.
//
#include "ds.h"
#include "output.h"
void writeInt(int k,FILE* file)
{
    fwrite(&k,sizeof(int),1,file);
}
output(communitiesList * list,char* where)
{

    communitiesList * listHolder=list;
    int counter=0;
    while(listHolder!=NULL)
{
        counter++;
        listHolder=listHolder->next;
}


FILE *file = fopen(where, "w");
assert(file!=NULL);
writeInt(counter,file);
communitiesList * writerList=list;
while(writerList!=NULL)
{
    rowLinkedList*  indicesList=writerList->communityInfo->graph;
    writeInt(writerList->communityInfo->networkStat->vertices,file);
    while(indicesList!=NULL)
{
        writeInt(indicesList->rowIndex,file);
        indicesList=indicesList->nextRow;
}
    writerList=writerList->next;

}
fclose(file);


}

