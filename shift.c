
#include "ds.h"
#include "float.h"
double sum(colLinkedList* col, networkStatsSet* ns)
{
    double sum;
    int k;
    sum=0;
    for(k=0;k<ns->vertices;k++)
    {
        int exists;
        exists=0;
        if(col&&col->colIndex==k)
        {
            exists=1;
            col=col->next;
        }
        sum+=fabs(exists-(double)ns->vertexDegreeArray[k]*ns->vertexDegreeArray[col->colIndex]/ns->degreeSum);
    }


    return sum;

}double norm1(rowLinkedList* rowLinkedList1,networkStatsSet* ns)
{
    double maximum;
    maximum=-DBL_MAX;
    while(rowLinkedList1!=NULL)
    {
        double sumOfNodes;
        sumOfNodes= sum(rowLinkedList1->colList, ns);
        maximum= sumOfNodes > maximum ? sumOfNodes : maximum;

        rowLinkedList1=rowLinkedList1->nextRow;
    }
    return maximum;
}