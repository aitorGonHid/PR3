#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "congress.h"
#include <string.h>
#include "organization.h"
#include "presentation.h"

// Initialize the congress
void congress_init(tCongress* object, char* name) {
    // PR1 EX4
    
    // Verify pre conditions
    assert(object != NULL);    
    assert(name != NULL);
    
    // Allocate the memory for congress name, using the length of the provided text plus 1 space for the "end of string" char '\0'. To allocate memory we use the malloc command.    
    object->name = (char*) malloc((strlen(name) + 1) * sizeof(char));   
    strcpy(object->name, name);  
   
    object->organizations = (tOrganizationTable*) malloc(sizeof(tOrganizationTable)) ;     
    // Initialize organizations
    organizationTable_init(object->organizations);  

    // PR2 EX1 
    // Initialize presentations queue
    presentationQueue_createQueue(&object->presentations); 
}

// Remove all data for a congress
void congress_free(tCongress* object) {
    // PR1 EX4
    assert(object!=NULL);
    // Remove data from organizations table
    organizationTable_free(object->organizations);
    
    // PR2 EX2    
    // Remove data from presentations queue
    presentationQueue_free(&object->presentations);
}

// Register a new organization
tError congress_registerOrganization(tCongress* object, tOrganization* organization) {

    // PR1 EX4    
    assert(object != NULL);   
    assert(organization != NULL);   
    
    tError err;
    
    // Check if the organization already exists
    int i;
    for(i=0; i < object->organizations->size;i++)
    {   
        if(organizationTable_find(object->organizations, organization->name) != NULL) {
            return ERR_DUPLICATED;
        }
    }
    
    // Add the organization to the table
    err = organizationTable_add(object->organizations, organization);
    
    return err;    
}

// Remove a organization
tError congress_removeOrganization(tCongress* object, tOrganization* organization) {
    // PR1 EX4
    assert(object != NULL); 
    assert(organization != NULL); 
    
    // Get the organization with the organizationName
    organization = organizationTable_find(object->organizations, organization->name);
    
    if(organization == NULL) {
        return ERR_NOT_FOUND;
    }
    
    return organizationTable_remove(object->organizations, organization);
}

// Find a organization
tOrganization* congress_findOrganization(tCongress* object, const char* organizationName) {
    // PR1 EX4   
    assert(object != NULL); 
    assert(organizationName != NULL);
 
    return organizationTable_find(object->organizations, organizationName);
}


tError congress_getGuests(tCongress* object, tGuestTable* guests) {
    // PR1 EX4

    assert(object != NULL);
    assert(guests != NULL);    

    int i, j;
    tError error;
    error = OK;    

    for (i=0; i<object->organizations->size; i++) {
        for (j=0; j<object->organizations->elements[i].guests->size; j++) {
            error = guestTable_add(guests, &(object->organizations->elements[i].guests->elements[j]));
        }
    }   
    return error;
}


// Add a new presentation
tError congress_addPresentation(tCongress* object, const char* organization_name, double score, const char* presentationTitle, const char* presentationTopic) {
    // PR2 EX1
    // return ERR_NOT_IMPLEMENTED;
    
    tOrganization *organization;
    tPresentation presentation;
    
    // Check preconditions
    assert(object != NULL);
    assert(organization_name != NULL);
    assert(presentationTitle != NULL);
    assert(presentationTopic != NULL);
    
    // Find organization
    organization = congress_findOrganization(object, organization_name);
    if (organization == NULL) {
        return ERR_INVALID_ORGANIZATION;
    }
    
    // ¿Find presentation?
    // Initialize the presentation structure
    presentation_init(&presentation, organization, score, presentationTitle, presentationTopic);
    
    // Enqueue the new presentation
    return presentationQueue_enqueue(&(object->presentations), presentation); 
}

// Get if an organization wins on a topic. Wins means best score on a topic
int congress_getOrganizationWins(tCongress* object, const char* organization_name, const char* topic)
{
    // PR2 EX2
    // return ERR_NOT_IMPLEMENTED;
    
    int bestScore = ERR_NOT_EXISTS; // used to save current best score. Initialized to -1 to detect as well non existing topics
    int organization_score = 0;  // used to save current best score of the organization.
    bool existDraw = false;
    tPresentationQueue tmp;
    tPresentation *presentation;
    tOrganization *organization;

    // Check preconditions
    assert(object != NULL);
    assert(organization_name != NULL);
    assert(topic != NULL);
        
    // Check if organization exists
    organization = congress_findOrganization(object, organization_name);
    if (organization == NULL)
        return ERR_NOT_EXISTS;
    
    // Make a copy of the queue
    presentationQueue_duplicate(&tmp, object->presentations);
    
    // Look for all elements on the queue
    while(!presentationQueue_empty(tmp)) {
        // Get the presentation
        presentation = presentationQueue_head(tmp);
        
        // The presentation is of topic that we are checking
        if (strcmp(presentation->presentationTopic, topic)==0) {
            if(presentation->score > bestScore) {
                // Better score
                bestScore = presentation->score;
                existDraw = false;
                if (organization_equals( organization, &presentation->organization) )
                    organization_score = presentation->score;
            } else if(presentation->score == bestScore) {
                // Draw score
                existDraw = true;
            }
        }
        
        // Remove the head element
        presentationQueue_dequeue(&tmp);
    }
    
    // Return value according best score
    if (bestScore < 0)
        return ERR_NOT_EXISTS; // For non existing topic, best score is still -1
    else {
        // Check if the organization has the best score
        if (organization_score == bestScore)
        {
            if (existDraw == false)
                return 3;
            else
                return 1;
        }
        else
            return 0;
    }
}

// Get the average score for an organization
double congress_getOrganizationScore(tCongress* object, const char* organization_name) {
    // PR2 EX3   
        
     // Check preconditions
    assert(object != NULL);
    assert(organization_name != NULL);
    
    tOrganization *organization;
    tPresentationQueue tmpQueue;
    double score;
    
    // Check preconditions
    assert(object != NULL);
    assert(organization_name != NULL);    
   
    
    // Find organization    
    organization = congress_findOrganization(object, organization_name);
    if (organization == NULL) {
        return 0;
    }
    
    // Create a copy of the matches cue
    presentationQueue_duplicate(&tmpQueue, object->presentations);    
    
    // Get organization score
    score = presentationQueue_getOrganizationScoreRecursive(&tmpQueue, organization);
    
    // Remove remaining elements
    presentationQueue_free(&tmpQueue);
    
    return score;
    
    //return ERR_NOT_IMPLEMENTED;
}


// Get a queue with all the presentations for an organization
tError congress_getOrganizationPresentations(tCongress* object, const char* organization_name, tPresentationQueue* presentations) {
    // PR2 EX3   
    
   // return ERR_NOT_IMPLEMENTED;
    // Check preconditions
    assert(object != NULL);
    assert(organization_name != NULL);
    assert(presentations != NULL);   
   
    tOrganization *organization;
    tPresentationQueue tmpQueue;
    tError err;   
  
    
    // Initialize the result queue
    presentationQueue_createQueue(presentations);       
    // Find organization
    organization = congress_findOrganization(object, organization_name);
    if (organization == NULL) {
        return ERR_INVALID_ORGANIZATION;
    }
    
    // Create a copy of the organization cue
    
    presentationQueue_duplicate(&tmpQueue, object->presentations);

    // Get organization presentations
    err = presentationQueue_getOrganizationPresentationsRecursive(&tmpQueue, organization, presentations);
    
    // Remove remaining elements
    presentationQueue_free(&tmpQueue);
    
    return err;

}

int congress_getOrganizationTop(tCongress* object, const char* organization_name){
    // PR3 EX1
	//Check preconditions
	assert(object != NULL);
	assert(organization_name != NULL);
	
	tOrganization *org;
	tPresentationQueue auxPresentations;
	tPresentationQueue orgPresentations;
	tPresentation *pres;
	int scoreTag, nMaxScores;
	
	//find the organization in the congress
	org = congress_findOrganization(object, organization_name);
	if (org == NULL) {
		//unexisting organization
		return ERR_INVALID_ORGANIZATION;
	}
	//initializing a presentation queue by organization
	presentationQueue_createQueue(&orgPresentations);
	presentationQueue_duplicate(&auxPresentations,object->presentations);
	presentationQueue_getOrganizationPresentationsRecursive(&auxPresentations,org,&orgPresentations);
	
	if (presentationQueue_empty(orgPresentations)){
		//the organization exists but has no presentations
		return 0;
	}
	
	//checking if any of the presentations in this  presentation queue by organization,  has the max score
	//get the first presentation and set the counter of max scores to 0
	pres = presentationQueue_head(orgPresentations);
	nMaxScores = 0;
	while (!presentationQueue_empty(orgPresentations)) {
		scoreTag = congress_getOrganizationWins(object,organization_name,pres->presentationTopic);
		if (scoreTag == 3 || scoreTag == 1){
			//scoreTag 3 or 1 means that has the maxScore or a first place draw in a presentation about one topic
			nMaxScores++;
		}
		//set the next presentation by organization to check
		presentationQueue_dequeue(&orgPresentations);
		pres = presentationQueue_head(orgPresentations);
	}
	return nMaxScores;
}

// Gets the average point of the organization
double congress_getOrganizationPoints(tCongress* object, const char* organization_name){
    // PR3 EX1
    //Check preconditions
	assert (object != NULL);
	assert (organization_name != NULL);
	
	int nPresentations;
	double averageScore, scoreSum;
	tOrganization *org;
	tPresentation *pres;
	tPresentationQueue auxQueue;
	tPresentationQueue presentations;
	
	// find the organization in the source congress
	org = congress_findOrganization(object, organization_name);
	
	// if organization doesn't return ERR end exit function
	if (org == NULL){
		return ERR_INVALID_ORGANIZATION;
	}
	
	// organization exists. Check how many presentations  has
	// create an auxiliary queue so as not to modify the original queue
	presentationQueue_duplicate(&auxQueue,object->presentations);
	// create a new presentation queue by organization
	presentationQueue_createQueue(&presentations);
	presentationQueue_getOrganizationPresentationsRecursive(&auxQueue, org, &presentations);
	
	// get the number of presentations and the summation of its scores to get the average score
	// set counters at 0
	scoreSum = 0;
	nPresentations = 0;
	while (!presentationQueue_empty(presentations)) {
		
		nPresentations++;
		pres = presentationQueue_head(presentations);
		scoreSum = scoreSum + pres->score;
		
		presentationQueue_dequeue(&presentations);
		
	}
	// get the average score dividing the summation of scores by the number of presentations, and return it
	averageScore = scoreSum / nPresentations;
	
	return averageScore;
}