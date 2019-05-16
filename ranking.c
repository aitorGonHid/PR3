#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ranking.h"
#include "congress.h"

// Get stadistics for a Organization
tError ranking_getOrganizationStatistics(tCongress* object, const char* organization_name, tOrganizationStadistics *stadistics){
    // PR3 EX1
    return ERR_NOT_IMPLEMENTED;
}

// Compare stadistics of two Organizations to get the order in the ranking.
// 0 equal ranking, 1 stadistics1 wins, -1 stadistics2 wins
int ranking_compareStadistics(tOrganizationStadistics* stadistics1, 
							  tOrganizationStadistics* stadistics2)
{
    // PR3 EX1
    return ERR_NOT_IMPLEMENTED;
}

// Gets the stadistics of a Organization and create the ranking
tError ranking_createOrganizationRanking(tCongress* congress, 
    const char* organization_name, tRanking *dst){

	tOrganization *organization;
    tError err;

    // Check preconditions
    assert(congress != NULL);
    assert(organization_name != NULL);
    assert(dst != NULL);

    // Find organization
    organization = congress_findOrganization(congress, organization_name);
    if ( organization == NULL) {
        return ERR_INVALID_ORGANIZATION;
    }

    // Reserve memory
    dst->stadistics = (tOrganizationStadistics*) malloc(sizeof(tOrganizationStadistics));
    if (dst->stadistics == NULL){
        return ERR_MEMORY_ERROR;
    }

    // Get Organization stadistics
    err = ranking_getOrganizationStatistics(congress, organization_name, dst->stadistics);
    if (err == OK){
        // Set Organization. No need to reserve memory because we have only one copy of every Organization stored on memory
        dst->org = organization;
    }

    return err;
}

// Create the ranking list
void rankingList_createList(tRankingList* list){
    // PR3 EX2
    return;
}

// Gets ranking node from given position
tRankingListNode* rankingList_getNode(tRankingList* list, int index){
    // PR3 EX2
    return NULL;
}

// Insert/adds a new ranking to the ranking list
tError rankingList_insert(tRankingList* list, tRanking ranking, int index){
    // PR3 EX2
    return ERR_NOT_IMPLEMENTED;
}

// Deletes a ranking from the ranking list
tError rankingList_delete(tRankingList* list, int index){
    // PR3 EX2
    return ERR_NOT_IMPLEMENTED;
}

// Gets ranking from given position
tRanking* rankingList_get(tRankingList* list, int index){
    // PR3 EX2
    return NULL;
}

// Gets true if list is empty
bool rankingList_empty(tRankingList* list){
    // PR3 EX2
    return false;
}

// Remove all data for a ranking list
void rankingList_free(tRankingList* list){
    // PR3 EX2
    return;
}

// Get organization with better stadistics
tOrganization* rankingList_getBestOrganization(tRankingList* list){
    // PR3 EX3
    return NULL;
}

// Get organization with worst stadistics
tOrganization* rankingList_getWorstOrganization(tRankingList* list){
    // PR3 EX3
    return NULL;
}

tError rankingList_insert_sorted(tRankingList* list, tRanking ranking)
{
    // Check preconditions
    return ERR_NOT_IMPLEMENTED;
}

//Sort list according to Organization stadistics
tRankingList* rankingList_sortInsertion(tRankingList *list){
    // PR3 EX3
    return NULL;
}

