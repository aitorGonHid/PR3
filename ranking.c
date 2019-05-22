#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ranking.h"
#include "congress.h"

// Get stadistics for a Organization
tError ranking_getOrganizationStatistics(tCongress* object, const char* organization_name, tOrganizationStadistics *stadistics){
    // PR3 EX1
    //precondition checking
	assert (object != NULL);
	assert (organization_name != NULL);
	assert (stadistics != NULL);
	
	//variable declaration
	tPresentationQueue queue;
	
	//find organizatin and save it
	stadistics->organization = congress_findOrganization(object, organization_name);
	//finish function if organization not exists
	if (stadistics->organization == NULL) {
		return ERR_NOT_EXISTS;
	}
	
	//saving average points
	stadistics->averagePoints = congress_getOrganizationPoints(object, organization_name);
	
	//saving number of presentations by organization
	//use an auxuliar queue is needed to get number or presentations by organization, using congress_getOrganizationPresentations function
	presentationQueue_createQueue(&queue);
	congress_getOrganizationPresentations(object, organization_name, &queue);
	stadistics->numPresentations = presentationQueue_getNumberTopicsIterative(&queue, stadistics->organization);
	
	//saving number of presentations with max score
	stadistics->numTopPresentations = congress_getOrganizationTop(object, organization_name);
	
	//delete the auxiliar queue
	presentationQueue_free(&queue);
	
	return OK;
}

// Compare stadistics of two Organizations to get the order in the ranking.
// 0 equal ranking, 1 stadistics1 wins, -1 stadistics2 wins
int ranking_compareStadistics(tOrganizationStadistics* stadistics1, 
							  tOrganizationStadistics* stadistics2) {
    // PR3 EX1
    ///===================check conditions to win==========================================
	//==========FIRST condition: the one with higher average points wins ==================
	if (stadistics1->averagePoints > stadistics2->averagePoints) {
		return 1;
	} else if (stadistics1->averagePoints < stadistics2->averagePoints) {
		return -1;
	} else {
	//=========================first condition RAW=========================================
	//==========SECOND condition: the one that has more presentatios wins =================
		if (stadistics1->numPresentations > stadistics2->numPresentations) {
			return 1;
		} else if (stadistics1->numPresentations < stadistics2->numPresentations) {
			return -1;
		} else {
	//=========================second condition RAW========================================
	//==========THIRD condition: the one that has more presentatios with max score wins ===
			if (stadistics1->numTopPresentations > stadistics2->numTopPresentations) {
				return 1;
			} else if (stadistics1->numTopPresentations < stadistics2->numTopPresentations) {
				return -1;
			} else {
	//=========================third condition RAW=========================================
	//================ ORGANIZATIONS HAVE THE SAME STADISTICS==============================
				return 0;
			}
		}
	}
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
	//set first & last pointers direction to NULL
	list->first = NULL;
	list->last = NULL;
	//set starting size of the list to 0
	list->size = 0;
}

// Gets ranking node from given position
tRankingListNode* rankingList_getNode(tRankingList* list, int index){
    // PR3 EX2
    return NULL;
}

// Insert/adds a new ranking to the ranking list
tError rankingList_insert(tRankingList* list, tRanking ranking, int index){
	// PR3 EX2
    //preconditions check
	assert (list != NULL);
	
	//variable declaration
	tRankingListNode *tmp;
	tRankingListNode *aux;
	int i, size;
	
	//verify index
	size = list->size;
	if (index < 1 || index > size+1) {
		return ERR_INVALID_INDEX;
	}

	tmp = (tRankingListNode*) malloc (sizeof(tRankingListNode));
	
	//verify memory allocation	
	if (tmp == NULL){
		return ERR_MEMORY_ERROR;
	}
	
	//insert element in index position
	if (list->size == 0) {
		//empty list
		if (index == 1) {
			tmp->next = NULL;
			tmp->prev = NULL;
			tmp->e = ranking;
			list->first = tmp;
			list->last = tmp;
			list->size = list->size + 1;
		} else {
			return ERR_INVALID_INDEX;
		}
	} else {
		//list with some elements
		if (index == 1){
			//insert at first position
			tmp->next = list->first;
			tmp->prev = NULL;
			tmp->e = ranking;
			list->first->prev = tmp;
			list->first = tmp;
			list->size = list->size + 1;
		} else if (index == list->size+1){
			//insert at last position
			tmp->next = NULL;
			tmp->prev = list->last;
			tmp->e = ranking;
			list->last->next = tmp;
			list->last = tmp;
			list->size = list->size+1;
		} else {
			//insert between 1st and last pos
			i = 1;
			aux = list->first;
			//get position where new element must be in
			while (i < index) {
				aux = aux->next;
				i++;
			}
			//insert element and actualize the previous and next element pointers
			tmp->next = aux;
			tmp->prev = aux->prev;
			tmp->e = ranking;
			aux->prev->next=tmp;
			aux->prev=tmp;
			list->size = list->size + 1;
		}
	}
	return OK;
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
