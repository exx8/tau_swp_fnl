//
// Created by Nazar Aburas on 06/09/2020.
//

#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"






communitiesList *algo3(communityDescription* community) {
    communitiesList* P=memory(sizeof(communityDescription), 1);
    communitiesList OHolder;
    OHolder.next=NULL;
    OHolder.communityInfo=NULL;
    communitiesList* Ocurrent=&OHolder;
    P->communityInfo=community;
    P->next=NULL;
    divisionResults* divisionResults1=algo2(P->communityInfo->graph,P->communityInfo->networkStat);
    if(divisionResults1->errorNum==0)
    {
        tuple2 *result = divisionResults1->value;
        communityDescription *first = result->first;
        if(first->networkStat->vertices == 0)
        {
            //freeNested(result->first);
        }
        communityDescription *second = result->second;
        if(second->networkStat->vertices == 0)
        {
           // freeNested(result->second);
        }
        if(first->networkStat->vertices > 1)
        {
            Ocurrent->next= memory(sizeof(communitiesList),1);
            Ocurrent->next->communityInfo=first;
            Ocurrent=Ocurrent->next;
            Ocurrent->next=NULL;
        }
        if(second->networkStat->vertices > 1)
        {
            Ocurrent->next= memory(sizeof(communitiesList),1);

            Ocurrent->next->communityInfo=second;
            Ocurrent=Ocurrent->next;
            Ocurrent->next=NULL;

        }

        //freeCommunityInfo(result->first);
        freeCommunityInfo(result->second);
    }
    freeNested(OHolder.next);
    free(divisionResults1->value);
    free(divisionResults1);
    free(P);
    freeCommunityInfo(community);
    return P;
}