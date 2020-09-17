#include "algo2.h"
#include "float.h"

double
billinearMultipicationOfBUnoptimized(rowLinkedList *Ag, networkStatsSet *AgStat,
                                     volatile int vectorLength, double *vec1);

void splitterDiscrete(double *splitter, int splitterLen) {
    int i = 0;
    for (; i < splitterLen; i++)
        splitter[i] = splitter[i] ? 1 : -1;
}

void modularity_maximization(double *splitter, int splitterLen, rowLinkedList *list, networkStatsSet *communityStat) {
    int i = 0;
    int k = 0, k2 = 0,k3=0;
    rowLinkedList holder1, unmovedHolder;
    holder1.nextRow = list;
    holder1.rowIndex = -1;
    unmovedHolder.rowIndex = -1;
    unmovedHolder.nextRow=NULL;
    splitterDiscrete(splitter, splitterLen);

    rowLinkedList *rowPointer = holder1.nextRow;
    rowLinkedList *unmovedcurrent = &unmovedHolder;
    double DeltaQ = 1;

    while (IS_POSITIVE(DeltaQ)) {
        while (rowPointer != NULL)  {
            int rowIndex = rowPointer->rowIndex;
            rowPointer->side = splitter[k3];
            unmovedcurrent->nextRow = memory(sizeof(rowLinkedList), 1);
            unmovedcurrent->rowIndex=rowIndex;
            unmovedcurrent = unmovedcurrent->nextRow;
            rowPointer=rowPointer->nextRow;
            k3++;
        }
        int n_g = communityStat->vertices;
        int *indices = memory(sizeof(double), n_g);
        int *improve = memory(sizeof(double), n_g);

        for (; i < n_g; i++) {//green
            double *score = memory(sizeof(double), n_g);

            double q0 = billinearMultipicationOfBUnoptimized(holder1.nextRow, communityStat, splitterLen, splitter);
            rowLinkedList *unmovedPointer = unmovedHolder.nextRow;
            int splitterIndex = 0;
            int j_tag = 0;
            double maxModularity = -DBL_MAX;
            while (unmovedPointer != NULL) {//purple
                int rowIndex = unmovedPointer->rowIndex;
                splitter[splitterIndex] = -splitter[splitterIndex];
                score[splitterIndex] = billinearMultipicationOfBUnoptimized(holder1.nextRow, communityStat, splitterLen,
                                                                            splitter) - q0;
                splitter[splitterIndex] = -splitter[splitterIndex];
                if (score[splitterIndex] > maxModularity) {
                    maxModularity = score[splitterIndex];
                    j_tag = splitterIndex;
                    //arg maxs not in the purple
                }
                splitterIndex++;
                unmovedPointer=unmovedPointer->nextRow;
            }
            splitter[j_tag] = -splitter[j_tag];
            indices[i] = j_tag;
            if (i == 0) {
                improve[0] = score[j_tag];
            } else {
                improve[i] = improve[i - 1] + score[j_tag];
            }
            rowLinkedList *nodeFinder = &holder1;
            for (k = 0; k < j_tag; k++)
                nodeFinder = nodeFinder->nextRow;
            //@todo improve me- make me more efficient


            int i_tag = -1;
            double max_value = -DBL_MAX;
            for (k = 0; k < n_g; k++) {
                if (improve[k] > max_value) {
                    i_tag = k;
                    max_value = improve[i_tag];
                }
            }
            for (k2 = n_g - 1; k2 > i_tag; k2--) {
                int j = indices[k2];
                splitter[j] = -splitter[j];
            }

            if (i_tag == n_g - 1) {
                DeltaQ = 0;
            } else {
                DeltaQ = improve[i_tag];
            }

            free(score);
        }
        free(improve);
        free(indices);
    }
    freeGraph(unmovedHolder.nextRow);

}