
#include "ds.h"
#include "float.h"
int counter(colLinkedList* col)
{
    int counter=0;
    while(col!=NULL)
    {
        counter++;
        col=col->next;
    }
    return counter;

}int norm1(rowLinkedList* rowLinkedList1)
{
    double maximum=-DBL_MAX;
    while(rowLinkedList1!=NULL)
    {
        int numberOfNodes=counter(rowLinkedList1->colList);
        maximum=numberOfNodes>maximum?numberOfNodes:maximum;

        rowLinkedList1=rowLinkedList1->nextRow;
    }
    return maximum;
}