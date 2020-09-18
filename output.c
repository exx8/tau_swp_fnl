
#include "ds.h"
#include "output.h"

void writeInt(int k, FILE *file) {
    fwrite(&k, sizeof(int), 1, file);
}

void output(communitiesList
* list,
char *where
)
{

communitiesList *listHolder = list;
int counter;
counter= 0;
while(listHolder!=NULL)
{
counter++;
listHolder = listHolder->next;
}


FILE *file;
file = fopen(where, "w");
makesurenot(file != NULL);
writeInt(counter, file
);
communitiesList *writerList;
writerList = list;
while(writerList!=NULL)
{
if(writerList->communityInfo){
rowLinkedList *indicesList;
indicesList = writerList->communityInfo->graph;
writeInt(writerList->communityInfo->networkStat->vertices,file);
while(indicesList!=NULL)
{
writeInt(indicesList->rowIndex,file);
indicesList = indicesList->nextRow;
}

}
writerList = writerList->next;

}
fclose(file);


}
