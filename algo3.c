
#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"


communitiesList * addNodeToBeginning(communityDescription *communityInfo, communitiesList *list) {
    communitiesList *newGroup;
    newGroup =smemory(sizeof(communitiesList),1);
    newGroup->communityInfo = communityInfo;
    newGroup->next = list;
    return newGroup;
}

divisionResults *divideGroup(const communitiesList *groupToDivide) {
    divisionResults * algo2Results;
    algo2Results = algo2(groupToDivide->communityInfo->graph, (groupToDivide->communityInfo->networkStat));
    if(algo2Results->errorNum!=0)
    {
        error(algo2Results->errorNum,"division failed");
    }
    return algo2Results;
}

communitiesList *algo3(communityDescription* community) {

    communitiesList *groupP;
    groupP = smemory(sizeof(communitiesList ), 1);

    groupP->next=NULL;
    communitiesList *groupO;
    groupO=smemory(sizeof(communitiesList ), 1);
    groupP->communityInfo = community;
    while (groupP != NULL) {
        communitiesList *groupToDivide;
        groupToDivide = groupP;
        divisionResults *algo2Results;
        algo2Results = divideGroup(groupToDivide);
        groupP = groupP->next;
        tuple2 *divisionResult = algo2Results->value;
        int firstGroupVerticesNum;
        firstGroupVerticesNum = divisionResult->first->networkStat->vertices;
        int secondGroupVerticesNum;
        secondGroupVerticesNum= divisionResult->second->networkStat->vertices;
        if (firstGroupVerticesNum == 0 || secondGroupVerticesNum == 0) {
            groupToDivide->next = groupO;
            groupO = groupToDivide;
        } else {
            if (firstGroupVerticesNum == 1) {
                groupO=addNodeToBeginning(divisionResult->first, groupO);

            }
            if (secondGroupVerticesNum == 1) {
                groupO=addNodeToBeginning(divisionResult->second, groupO);

            }

            if (divisionResult->first->networkStat->vertices > 1) {
                groupP=addNodeToBeginning(divisionResult->first, groupP);

            }
            if (secondGroupVerticesNum > 1) {
                groupP=addNodeToBeginning(divisionResult->second, groupP);

            }
        }
    }
    return groupO;
}