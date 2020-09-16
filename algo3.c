//
// Created by Nazar Aburas on 06/09/2020.
//

#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"


void addNodeToBeginning(communityDescription *communityInfo, communitiesList *list) {
    communitiesList *newGroup;
    newGroup->communityInfo = communityInfo;
    newGroup->next = &list;
    list = newGroup;
}

communitiesList *algo3(communityDescription community) {

    communitiesList *groupA=memory(sizeof(communitiesList ),1);

    groupA->next=NULL;
    communitiesList *groupB=memory(sizeof(communitiesList ),1);
    groupA->communityInfo = &community;
    while (groupA != NULL) {
        communitiesList *groupC = groupA;
        divisionResults algo2Results = algo2(groupC->communityInfo->graph, &(groupC->communityInfo->networkStat));
        if(algo2Results.errorNum!=0)
        {
            error(algo2Results.errorNum,"division failed");
        }

        communityDescription **algo2Division = algo2Results.value;
        groupA = groupA->next;
        int firstGroupVetricesNum = algo2Division[0]->networkStat.vertices;
        int secondGroupVerticesNum = algo2Division[1]->networkStat.vertices;
        if (firstGroupVetricesNum == 0 || secondGroupVerticesNum == 0) {
            groupC->next = groupB;
            groupB = groupC;
        } else {
            if (firstGroupVetricesNum == 1) {
                addNodeToBeginning(algo2Division[0], groupB);
            }
            if (secondGroupVerticesNum == 1) {
                addNodeToBeginning(algo2Division[1], groupB);
            }
            if (algo2Division[0]->networkStat.vertices > 1) {
                addNodeToBeginning(algo2Division[0], groupA);
            }
            if (secondGroupVerticesNum > 1) {
                addNodeToBeginning(algo2Division[1], groupA);
            }
        }
    }
    return groupB;
}