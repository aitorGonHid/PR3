#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ranking.h"
#include "congress.h"

// Get stadistics for a Organization
tError ranking_getOrganizationStatistics(tCongress* object, const char* organization_name, tOrganizationStadistics *stadistics){
    // PR3 EX1
    // Check preconditions
	assert (object != NULL);
	assert (organization_name != NULL);
	assert (stadistics != NULL);
	
	//variable declaration
	tPresentationQueue queue;
	
	// find organizatin and save it
	stadistics->organization = congress_findOrganization(object, organization_name);
	// finish function if organization not exists
	if (stadistics->organization == NULL) {
		return ERR_NOT_EXISTS;
	}
	
	//saving average points
	stadistics->averagePoints = congress_getOrganizationPoints(object, organization_name);
	
	// saving number of presentations by organization
	// use an auxuliar queue is needed to get number or presentations by organization, using congress_getOrganizationPresentations function
	presentationQueue_createQueue(&queue);
	congress_getOrganizationPresentations(object, organization_name, &queue);
	stadistics->numPresentations = presentationQueue_getNumberTopicsIterative(&queue, stadistics->organization);
	
	// saving number of presentations with max score
	stadistics->numTopPresentations = congress_getOrganizationTop(object, organization_name);
	
	// delete the auxiliar queue
	presentationQueue_free(&queue);
	
	return OK;
}

// Compare stadistics of two Organizations to get the order in the ranking.
// 0 equal ranking, 1 stadistics1 wins, -1 stadistics2 wins
int ranking_compareStadistics(tOrganizationStadistics* stadistics1, 
							  tOrganizationStadistics* stadistics2) {
    // PR3 EX1
    //===================check conditions to win==========================================
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
	// set first & last pointers direction to NULL
	list->first = NULL;
	list->last = NULL;
	// set starting size of the list to 0
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
    // Check preconditions
	assert (list != NULL);
	
	// variable declaration
	tRankingListNode *tmp;
	tRankingListNode *aux;
	int i, size;
	
	// verify index
	size = list->size;
	if (index < 1 || index > size+1) {
		return ERR_INVALID_INDEX;
	}

	tmp = (tRankingListNode*) malloc (sizeof(tRankingListNode));
	
	// verify memory allocation	
	if (tmp == NULL){
		return ERR_MEMORY_ERROR;
	}
	
	// insert element in index position
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
		// list with some elements
		if (index == 1){
			// insert at first position
			tmp->next = list->first;
			tmp->prev = NULL;
			tmp->e = ranking;
			list->first->prev = tmp;
			list->first = tmp;
			list->size = list->size + 1;
		} else if (index == list->size+1){
			// insert at last position
			tmp->next = NULL;
			tmp->prev = list->last;
			tmp->e = ranking;
			list->last->next = tmp;
			list->last = tmp;
			list->size = list->size+1;
		} else {
			// insert between 1st and last pos
			i = 1;
			aux = list->first;
			// get position where new element must be in
			while (i < index) {
				aux = aux->next;
				i++;
			}
			// insert element and actualize the previous and next element pointers
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
    // pre condition check
	assert(list != NULL);
	
	// verify list size
	if (list->size == 0) {
		return ERR_EMPTY_LIST;
	}
	
	// verify index
	if (index < 1 || index > list->size) {
		return ERR_INVALID_INDEX;
	}
	
	tRankingListNode *tmp;
	int i = 1;
	
	// index point to the first element
	if (index == 1) {
		tmp = list->first;
		list->first = tmp->next;
		free(tmp);
		list->size = list->size-1;
	} else {
	// index points to 2nd or one of the following elemnts
		
		// get position to set tmp direction to point to
		tmp = list->first;
		while (i < index) {
			tmp = tmp->next;
			i++;			
		}
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		free(tmp);
		list->size = list->size-1;
	}
	return OK;
}

// Gets ranking from given position
tRanking* rankingList_get(tRankingList* list, int index){
    // PR3 EX2
    // Check preconditions
	assert (list != NULL);
	assert (index > 0);//assert needed this time. This function cant return ERR_INVALID_INDEX.
	
	// check invalid index
	if (index > list->size) {
		return NULL;
	}
	
	tRankingListNode *tmp;
	int i = 1;
	tRanking *elem;
	
	// index points to list first element
	if (index == 1) {
		elem = &list->first->e;
		
		return elem;
	} else {
	// index points to 2nd or other of the following elements
		
		//get position to set tmp direction to point to
		tmp = list->first;
		while (i < index) {
			tmp = tmp->next;
			i++;
		}
		elem = &tmp->e;
		
		return elem;
	}
}

// Gets true if list is empty
bool rankingList_empty(tRankingList* list){
    // PR3 EX2
    // pre condition check
	assert(list != NULL);
	
	if (list->size != 0) {
		return false;
	} else {
		return true;
	}
}

// Remove all data for a ranking list
void rankingList_free(tRankingList* list){
    // PR3 EX2
	// pre condition check
	assert (list != NULL);
	
	// while the list has some elements, delete first element
	while (!rankingList_empty(list)) {
		rankingList_delete(list, 1);
	}		
}

// Get organization with better stadistics
tOrganization* rankingList_getBestOrganization(tRankingList* list){
    // PR3 EX3
	// pre condition check
	
	assert (list != NULL);
	
	tRankingListNode *best;
	tRankingListNode *cmp;
	int i = 1;
	int result;
	
	// check list size
	if (list->size == 0) {
		return NULL;
	}
	
	// get first & second element to start comparisons
	best = list->first;
	cmp = best->next;
	
	while (i < list->size) {
		
		// get the one with better stadistics
		result = ranking_compareStadistics(best->e.stadistics,cmp->e.stadistics);
		if (result == -1) {
			best = cmp;
		}
		// get the following element
		cmp = cmp->next;
		i++;
	}
	
    return best->e.org;
}

// Get organization with worst stadistics
tOrganization* rankingList_getWorstOrganization(tRankingList* list){
    // PR3 EX3
	// Check preconditions
	assert (list != NULL);
	
	tRankingListNode *worst;
	tRankingListNode *cmp;
	int i = list->size;
	int result;
	
	// check list size
	if (list->size == 0) {
		return NULL;
	}
	
	// get first & second element to start comparisons
	worst = list->last;
	cmp = worst->prev;
	
	while (i > 1) {
		
		// get the one with better stadistics
		result = ranking_compareStadistics(worst->e.stadistics,cmp->e.stadistics);
		if (result == 1) {
			worst = cmp;
		}
		// get the following element
		cmp = cmp->prev;
		i--;
	}
	
    return worst->e.org;
}

tError rankingList_insert_sorted(tRankingList* list, tRanking ranking) {
	// PR3 EX3
	/*
	int index = list->size+1;
	rankingList_insert(list, ranking, index);
	return OK;*/
	return ERR_NOT_IMPLEMENTED;
}

//Sort list according to Organization stadistics
tRankingList* rankingList_sortInsertion(tRankingList *list){
	// PR3 EX3
	/*
	//pre condition check
	assert (list != NULL);
	
	int i, j, result;
	tRankingList *srtdList;
	tRanking *act;
	tRanking *comp;
	
	//empty list. No sorting needed
	if (rankingList_empty(list)) {
		return list;
	}
	
	//list has some elements
	rankingList_createList(srtdList);
	for (i=1; i<=list->size; i++) {
		act = rankingList_get(list, i);
		for (j=i+1;j<=list->size;j++) {
			comp = rankingList_get(list,j);
			result = ranking_compareStadistics(act->stadistics, comp->stadistics);
			if (result < 1) {
				act = rankingList_get(list, j);
			}
		}
	//insert one of the best stadistics element
	rankingList_insert_sorted(srtdList, *act);
	}
	return srtdList;*/
	return NULL;
}
