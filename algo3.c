//
// Created by Nazar Aburas on 06/09/2020.
//

#include "ds.h"
#include "networkStats.h"
#include "time.h"
#include "algo2.h"






communitiesList *algo3(communityDescription* community) {
    communitiesList* list=memory(sizeof(communityDescription),1);
    list->communityInfo=community;
    return list;
}