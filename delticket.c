#include "Version.h"
#ifndef MYDELTICKET
#warning("WARNING - USING THE SOLUTION delticket.c NOT MY CODE")
#else
#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"
#include "delticket.h"
#include "subs.h"

/*
 * delticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a unsigned int using using strtosumid.
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string 
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to free() all space that was malloc()'d 
 *
 * Arguments:
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *
 * returns 0 if ok -1 if not found or error
 */

int
delticket(char *plate, char *state, char *summ)
{
    uint32_t hashval = hash(plate) % tabsz;

    struct vehicle *currvc = *(htable + hashval);
    struct vehicle *prevvc = NULL;
    while (currvc != NULL) {
	if (strcmp(currvc->plate, plate) == 0 && strcmp(currvc->state, state) == 0) {
            break;
	}
	prevvc = currvc;
	currvc = currvc->next;
    }
    
    if (currvc == NULL) {
        return -1; //we didn't find a vehicle match
    }

    unsigned long summid;
    int sumresult = strtosumid(summ, &summid);

    if (sumresult == -1) {
        return -1;
    }
	
    struct ticket *currtkt = currvc->head;
    struct ticket *prev = NULL;
    while (currtkt != NULL) {
	if (currtkt->summons == summid) {
	    break;
	}
	prev = currtkt;
	currtkt = currtkt->next;
    }
	
    if (currtkt == NULL) {
	return -1;
    }
	
    struct fine *f = fineTab + currtkt->code;
    
    currvc->cnt_ticket--;
    currvc->tot_fine = currvc->tot_fine - f->fine;

    if (currtkt->next == NULL) {
	if (prev != NULL) {
	    prev->next = NULL;
	} else {
	    currvc->head = NULL;
	}
	
	free(currtkt);
    } else { 
	if (prev == NULL) { //if prev is null, the ticket is the head
	    currvc->head = currtkt->next;		    
	} else {
            prev->next = currtkt->next;
	}
	free(currtkt);
    }

    if (currvc->cnt_ticket == 0) {
	if (prevvc != NULL) {
            prevvc->next = currvc->next;
        } else {
	    *(htable + hashval) = currvc->next;
	}

	free(currvc->plate);
	free(currvc->state);
	free(currvc);
    }

    return 0;
}
#endif
