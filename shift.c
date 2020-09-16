//
// Created by Nazar Aburas on 14/09/2020.
//

#include "time.h"
#include "math.h"
#include "ds.h"


/*Function finds the maximum sum of the columns, matrix is symmetric
therefore we find maximum of rows */

double findMaxColSum(rowLinkedListSparseMatrix * sparseMatrix){
    double result = 0;
    while (sparseMatrix!= NULL){
        double colSum = 0;
        colLinkedListSparseMatrix * currNode = sparseMatrix->colList;
        while (currNode != NULL){
            colSum += currNode->value;
            currNode = currNode->next;
        }
        if (result < colSum){
            result = colSum;
        }
        sparseMatrix = sparseMatrix->nextRow;
    }

    return result;
}

void addShiftAmount(rowLinkedListSparseMatrix * sparseMatrix, double shiftAmount){
    sparseMatrix->shiftAmount = shiftAmount;
}

void shiftSparseMatrix(rowLinkedListSparseMatrix * sparseMatrix){
    double shiftAmount = findMaxColSum(sparseMatrix);
    addShiftAmount(sparseMatrix, shiftAmount);
}


