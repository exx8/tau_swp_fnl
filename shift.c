
#include "ds.h"
#include "float.h"
/**
 * sums B cols (or rows)
 * @param col a  column to be summed
 * @param ns B's networkstats
 * @param rowIndex the row index of the row we sum
 * @return
 */
double sum(colLinkedList* col, networkStatsSet* ns,int rowIndex)
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
        sum+=fabs(exists-(double)ns->vertexDegreeArray[k]*ns->vertexDegreeArray[rowIndex]/ns->degreeSum);
    }


    return sum;
/**
 * returns the norm1 of a linkedlist of a row.
 */
}double norm1(rowLinkedList* rowLinkedList1,networkStatsSet* ns)
{
    double maximum;
    maximum=-DBL_MAX;
    while(rowLinkedList1!=NULL)
    {
        double sumOfNodes;
        sumOfNodes= sum(rowLinkedList1->colList, ns,rowLinkedList1->rowIndex);
        maximum= sumOfNodes > maximum ? sumOfNodes : maximum;

        rowLinkedList1=rowLinkedList1->nextRow;
    }
    return maximum;
}