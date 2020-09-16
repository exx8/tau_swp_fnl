//
// Created by Nazar Aburas on 16/09/2020.
//

#include <stdio.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include "utils.h"
#include "ds.h"
#include "algo2.h"
#define intsize 4

void writeDataToOutput(divisionResults * graphDivision){
    FILE *out_file = fopen("/Users/nazaraburas/Desktop/groups.out" ,"w");
    rowLinkedList *groups = graphDivision->value->graph;
    while (groups != NULL){
        int nodeCount = 0;
        colLinkedList *node = groups->colList;
        /*Iterated twice, once for num of nodes and second for indices,
         should find a solution with one iteration */
        while (node != NULL){
            nodeCount += 1;
            node = node->next;
        }
        fwrite(nodeCount, sizeof(int), 1, out_file); ///MIGHT NEED A POINTER TO NODECOUNT
        while (node != NULL){
            fwrite(node->colIndex, sizeof(int), 1, out_file);
            node = node->next;
        }
        groups = groups->nextRow;
    }
}
