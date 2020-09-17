#include "algo2.h"
double
billinearMultipicationOfBUnoptimized( rowLinkedList *Ag,  networkStatsSet *AgStat,
                                      volatile  int vectorLength,  double *vec1);
        void splitterDiscrete(double* splitter,int splitterLen)
{
    int i=0;
    for(;i<splitterLen;i++)
        splitter[i]=splitter[i]?1:-1;
}

void modularity_maximization(double* splitter,int splitterLen,rowLinkedList* list,networkStatsSet* communityStat)
{
    int i=0;
    rowLinkedList holder1,unmovedHolder;
    holder1.nextRow = list;
    holder1.rowIndex=-1;
    unmovedHolder.rowIndex=-1;
    splitterDiscrete(splitter,splitterLen);

    rowLinkedList *rowPointer = holder1.nextRow;
    rowLinkedList * unmovedcurrent=&unmovedHolder;
    while(rowPointer != NULL) {
        int rowIndex = rowPointer->rowIndex;
        rowPointer->side=splitter[rowIndex];
        unmovedcurrent->nextRow=memory(sizeof(rowLinkedList),1);
        unmovedcurrent=unmovedcurrent->nextRow;

    }

for(;i<communityStat->vertices;i++)
{//green
    double * score=memory(sizeof(double),communityStat->vertices);

    double q0=billinearMultipicationOfBUnoptimized(holder1.nextRow,communityStat,splitterLen,splitter);
    rowLinkedList * unmovedPointer=unmovedcurrent;
    int splitterIndex=0;
    int maxIndex=-1;
    double maxModularity=-infinity;
    while(unmovedPointer!=NULL)
    {//purple
        int rowIndex = unmovedPointer->rowIndex;
        splitter[splitterIndex]=-splitter[splitterIndex];
        score[splitterIndex]=billinearMultipicationOfBUnoptimized(holder1.nextRow,communityStat,splitterLen,splitter);
        splitter[splitterIndex]=-splitter[splitterIndex];
        splitterIndex++;
    }


}

freeGraph(unmovedcurrent);
}
