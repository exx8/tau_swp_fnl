//
// Created by Nazar Aburas on 06/09/2020.
//

#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"


struct _networkStatSetList {
    networkStatsSet networkInfo;
    struct _networkStatSetList* next;
} typedef networkStatsSetList;


void addNodeToBeginning(networkStatsSet networkInfo, networkStatsSetList* list){
    networkStatsSetList* newGroup;
    newGroup->networkInfo = networkInfo;
    newGroup->next = &list;
    list = newGroup;
}

networkStatsSetList* algo3(networkStatsSet stats) {

    networkStatsSetList* groupA;
    networkStatsSetList* groupB;
    groupA->networkInfo = stats;
    while(groupA != NULL){
        networkStatsSetList* groupC = &groupA;
        networkStatsSet[2] algo2Division = algo2(groupC->networkInfo);
        groupA = groupA->next;
        if(algo2Divison[0].vertices == 0 || algo2Divison[1].vertices == 0){
            groupC->next = groupB;
            groupB = groupC;
        }
        else{
            if(algo2Divison[0].vertices == 1 ){
                addNodeToBeginning(algo2Division[0], groupB);
            }
            if(algo2Divison[1].vertices == 1 ){
                addNodeToBeginning(algo2Division[1], groupB);
            }
            if(algo2Divison[0].vertices > 1 ){
                addNodeToBeginning(algo2Division[0], groupA);
            }
            if(algo2Divison[1].vertices > 1 ){
                addNodeToBeginning(algo2Division[1], groupA);
            }
        }
    }
    return groupB;
}