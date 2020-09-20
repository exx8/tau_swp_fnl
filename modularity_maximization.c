#include "algo2.h"
#include "float.h"
#include "shift.h"
double
billinearMultiplicationOfBUnoptimized(rowLinkedList *Ag, networkStatsSet *AgStat, volatile int vectorLength,
                                      double *vec1, double shift);

void splitterDiscrete(double *splitter, int splitterLen) {
    int i;
    i = 0;
    for (; i < splitterLen; i++)
       if(IS_POSITIVE(splitter[i]))
           splitter[i]=1;
       else
           splitter[i]=-1;
}

void modularity_maximization(double *splitter, int splitterLen, rowLinkedList *list, networkStatsSet *communityStat) {
    int i;
    int k, k2, k3;
    rowLinkedList holder1, unmovedHolder;
    rowLinkedList *rowPointer;
    rowLinkedList *unmovedcurrent;
    double DeltaQ;
    int n_g;
    int *indices;
    double *improve;
    int i_tag;
    double max_value;
    double shift;

    i=0;
    k=0;
    k2=0;
    k3=0;
    holder1.nextRow = list;
    holder1.rowIndex = -1;
    unmovedHolder.rowIndex = -1;
    unmovedHolder.nextRow=NULL;
    splitterDiscrete(splitter, splitterLen);
    rowPointer= holder1.nextRow;
    unmovedcurrent= &unmovedHolder;
    DeltaQ = 1;
    shift=norm1(list,communityStat);
    while (IS_POSITIVE(DeltaQ)) {
        while (rowPointer != NULL)  {
            int rowIndex;
            rowIndex = rowPointer->rowIndex;
            rowPointer->side = splitter[k3];
            unmovedcurrent->nextRow = memory(sizeof(rowLinkedList), 1);
            unmovedcurrent->rowIndex=rowIndex;
            unmovedcurrent = unmovedcurrent->nextRow;
            rowPointer=rowPointer->nextRow;
            k3++;
        }
        n_g = communityStat->vertices;
        indices = memory(sizeof(double), n_g);
        improve = memory(sizeof(double), n_g);

        for (; i < n_g; i++) {/*green*/
            double *score;
            double q0;
            rowLinkedList *unmovedPointer;
            int splitterIndex;
            int j_tag;
            double maxModularity;
            rowLinkedList *nodeFinder;


            score = memory(sizeof(double), n_g);
            q0 = billinearMultiplicationOfBUnoptimized(holder1.nextRow, communityStat, splitterLen, splitter, shift);
            unmovedPointer = unmovedHolder.nextRow;
            splitterIndex = 0;
            j_tag = 0;
            maxModularity = -DBL_MAX;
            while (unmovedPointer != NULL) {/*purple*/
                splitter[splitterIndex] = -splitter[splitterIndex];
                score[splitterIndex] =
                        billinearMultiplicationOfBUnoptimized(holder1.nextRow, communityStat, splitterLen, splitter,
                                                              shift) - q0;
                splitter[splitterIndex] = -splitter[splitterIndex];
                if (score[splitterIndex] > maxModularity) {
                    maxModularity = score[splitterIndex];
                    j_tag = splitterIndex;
                    /*arg maxs not in the purple*/
                }
                splitterIndex++;
                unmovedPointer = unmovedPointer->nextRow;
            }
            splitter[j_tag] = -splitter[j_tag];
            indices[i] = j_tag;
            if (i == 0) {
                improve[0] = score[j_tag];
            } else {
                improve[i] = improve[i - 1] + score[j_tag];
            }
            nodeFinder = &holder1;
            for (k = 0; k < j_tag; k++)
                nodeFinder = nodeFinder->nextRow;
            free(score);

        }
        /*blue*/
            i_tag = -1;
            max_value = -DBL_MAX;
            for (k = 0; k < n_g; k++) {
                if (improve[k] > max_value) {
                    i_tag = k;
                    max_value = improve[i_tag];
                }
            }
            for (k2 = n_g - 1; k2 > i_tag; k2--) {
                int j;
                j = indices[k2];
                splitter[j] = -splitter[j];
            }

            if (i_tag == n_g - 1) {
                DeltaQ = 0;
            } else {
                DeltaQ = improve[i_tag];
            }


        free(improve);
        free(indices);
    }
    freeGraph(unmovedHolder.nextRow);

}