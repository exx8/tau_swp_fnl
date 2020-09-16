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

divisionResults *divideGroup(const communitiesList *groupToDivide) {
    divisionResults * algo2Results = algo2(groupToDivide->communityInfo->graph, (groupToDivide->communityInfo->networkStat));
    if(algo2Results->errorNum!=0)
    {
        error(algo2Results->errorNum,"division failed");
    }
    return algo2Results;
}

communitiesList *algo3(communityDescription* community) {

    communitiesList *groupP=memory(sizeof(communitiesList ), 1);

    groupP->next=NULL;
    communitiesList *groupO=memory(sizeof(communitiesList ), 1);
    groupP->communityInfo = community;
    while (groupP != NULL) {
        communitiesList *groupToDivide = groupP;
        divisionResults *algo2Results = divideGroup(groupToDivide);

        groupP = groupP->next;
        int firstGroupVetricesNum = algo2Results->value->first->networkStat->vertices;
        int secondGroupVerticesNum = algo2Results->value->second->networkStat->vertices;
        if (firstGroupVetricesNum == 0 || secondGroupVerticesNum == 0) {
            groupToDivide->next = groupO;
            groupO = groupToDivide;
        } else {
            if (firstGroupVetricesNum == 1) {
                groupO=addNodeToBeginning(algo2Results->value->first, groupO);
            }
            if (secondGroupVerticesNum == 1) {
                groupO=addNodeToBeginning(algo2Results->value->second, groupO);
            }
            if (algo2Results->value->first->networkStat->vertices > 1) {
                groupP=addNodeToBeginning(algo2Results->value->first, groupP);
            }
            if (secondGroupVerticesNum > 1) {
                groupP=addNodeToBeginning(algo2Results->value->second, groupP);
            }
        }

    }
    return groupO;
}