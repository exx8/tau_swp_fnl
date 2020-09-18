
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

    return algo2Results;
}

communitiesList *algo3(communityDescription* community) {

    communitiesList *groupP;
    communitiesList *groupO;

    groupP = smemory(sizeof(communitiesList ), 1);
    groupP->next=NULL;
    groupO=smemory(sizeof(communitiesList ), 1);
    groupP->communityInfo = community;

    while (groupP != NULL) {
        communitiesList *groupToDivide;
        divisionResults *algo2Results;
        tuple2 *divisionResult;
        int firstGroupVerticesNum;
        int secondGroupVerticesNum;

        groupToDivide = groupP;
        algo2Results = divideGroup(groupToDivide);
        groupP = groupP->next;
        divisionResult = algo2Results->value;
        firstGroupVerticesNum = divisionResult->first->networkStat->vertices;
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