//
// Created by Nazar Aburas on 06/09/2020.
//

#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"


communitiesList * addNodeToBeginning(communityDescription *communityInfo, communitiesList *list) {
    communitiesList *newGroup=smemory(sizeof(communitiesList),1);
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

    communitiesList *groupP=smemory(sizeof(communitiesList ), 1);

    groupP->next=NULL;
    communitiesList *groupO=smemory(sizeof(communitiesList ), 1);
    groupP->communityInfo = community;
    while (groupP != NULL) {
        communitiesList *groupToDivide = groupP;
        divisionResults *algo2Results = divideGroup(groupToDivide);
        groupP = groupP->next;
        tuple2 *divsionResult = algo2Results->value;
       // free(algo2Results);
        int firstGroupVetricesNum = divsionResult->first->networkStat->vertices;
        int secondGroupVerticesNum = divsionResult->second->networkStat->vertices;
        if (firstGroupVetricesNum == 0 || secondGroupVerticesNum == 0) {
            groupToDivide->next = groupO;
            groupO = groupToDivide;
        } else {
           // free(groupToDivide);
            if (firstGroupVetricesNum == 1) {
                groupO=addNodeToBeginning(divsionResult->first, groupO);

            }
            if (secondGroupVerticesNum == 1) {
                groupO=addNodeToBeginning(divsionResult->second, groupO);


            }

            if (divsionResult->first->networkStat->vertices > 1) {
                groupP=addNodeToBeginning(divsionResult->first, groupP);

            }
            if (secondGroupVerticesNum > 1) {
                groupP=addNodeToBeginning(divsionResult->second, groupP);

            }
        }
       // free(divsionResult);
    }
    return groupO;
}