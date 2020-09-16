//
// Created by Nazar Aburas on 06/09/2020.
//

#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"


communitiesList * addNodeToBeginning(communityDescription *communityInfo, communitiesList *list) {
    communitiesList *newGroup=memory(sizeof(communitiesList),1);
    newGroup->communityInfo = communityInfo;
    newGroup->next = list;
    return newGroup;
}

communitiesList *algo3(communityDescription community) {

    communitiesList *groupA=memory(sizeof(communitiesList ),1);

    groupA->next=NULL;
    communitiesList *groupB=memory(sizeof(communitiesList ),1);
    groupA->communityInfo = &community;
    while (groupA != NULL) {
        communitiesList *groupC = groupA;
        divisionResults * algo2Results = algo2(groupC->communityInfo->graph, (groupC->communityInfo->networkStat));
        if(algo2Results->errorNum!=0)
        {
            error(algo2Results->errorNum,"division failed");
        }

        communityDescription **algo2Division = &algo2Results->value; //it might no be an array!!
        groupA = groupA->next;
        int firstGroupVetricesNum = algo2Results->value[0].networkStat->vertices;
        int secondGroupVerticesNum = algo2Results->value[1].networkStat->vertices;
        if (firstGroupVetricesNum == 0 || secondGroupVerticesNum == 0) {
            groupC->next = groupB;
            groupB = groupC;
        } else {
            if (firstGroupVetricesNum == 1) {
                groupB=addNodeToBeginning(&algo2Results->value[0], groupB);
            }
            if (secondGroupVerticesNum == 1) {
                groupB=addNodeToBeginning(&algo2Results->value[1], groupB);
            }
            if (algo2Results->value[0].networkStat->vertices > 1) {
                groupA=addNodeToBeginning(&algo2Results->value[0], groupA);
            }
            if (secondGroupVerticesNum > 1) {
                groupA=addNodeToBeginning(&algo2Results->value[1], groupA);
            }
        }
    }
    return groupB;
}