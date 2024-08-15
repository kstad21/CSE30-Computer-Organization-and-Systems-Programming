#include "Version.h"
#ifndef MYINSTICKET
#warning("WARNING - USING THE SOLUTION insticket.c NOT MY CODE")
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
#include "insticket.h"
#include "subs.h"

/*
 * insertticket
 *      add a ticket (summons) to the database
 *
 *      YOU CANNOT USE CALLOC(), only malloc() is allowed.
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 *
 * Arguments
 *
 *  summ:
 *      summons id string to be added
 *      this string needs to be converted to a number to be stored in the
 *      database. This saves space and is faster to compare than a string         
 *      Conversion function is supplied. use example:
 *           unsigned long summid;
 *           if (strtosumid(summ, &summid, argv) != 0)
 *                  error handling
 *           new_ticket->summons = summid;
 *  plate:
 *      plate id string to be added
 *  state:
 *      state id string to be added
 *  code:
 *      summons code integer value used an an index into the fines table
 *      fineTab maps code number to text description & fine cost 
 *      you use this table to get the amount of the fine when calculating
 *      the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *
 * return
 *       0 if ok -1 for all errors
 */

int
insertticket(char *summ, char *plate, char *state, int code)
{

    uint32_t hashval = hash(plate) % tabsz;

    struct vehicle *match = NULL;
    struct vehicle *currvc = *(htable + hashval);

    while (currvc != NULL) {
        if (strcmp(plate, currvc->plate) == 0 && strcmp(state, currvc->state) == 0) {
            match = currvc;
	    break;
	}
        currvc = currvc->next;
    }
    
    struct ticket *newtk = (struct ticket *)malloc(sizeof(struct ticket));
    if (newtk == NULL) {
	return -1;
    }
    
    unsigned long summid;
    int sumresult = strtosumid(summ, &summid);
    if (sumresult != 0) {
	return -1;
    }

    newtk->summons = summid;
    newtk->code = code;
    newtk->next = NULL;

    if (match != NULL) {
	if (match->head == NULL) {
            match->head = newtk;
	    match->cnt_ticket = match->cnt_ticket + 1;
	    match->tot_fine += fineTab[newtk->code].fine;
	    return 0;
        } else {
	    struct ticket *currtkt = match->head;
	    while (currtkt->next != NULL) {
                if (currtkt->summons == summid && (int)currtkt->code == code) {
		    fprintf(stderr, "%s: duplicate summons %lu\n", argv0, summid);
		    return -1;
		}
		currtkt = currtkt->next;
	    }
            
            if (currtkt->summons == summid) {
		fprintf(stderr, "%s: duplicate summons %lu\n", argv0, summid);
		return -1;
	    }

	    currtkt->next = newtk;
            match->cnt_ticket = match->cnt_ticket + 1;
	    match->tot_fine += fineTab[newtk->code].fine;
	    return 0;
	}
    }

    struct vehicle *newvc = (struct vehicle *)malloc(sizeof(struct vehicle));
    if (newvc == NULL) {
	free(newtk);
	return -1;
    }
    
    char *dupplate = strdup(plate);
    char *dupstate = strdup(state);

    if (dupplate == NULL || dupstate == NULL) {
	return -1;
    }

    newvc->plate = dupplate;
    newvc->state = dupstate;
    newvc->tot_fine = fineTab[newtk->code].fine;
    newvc->cnt_ticket = 1;
    newvc->next = NULL;
    newvc->head = newtk; 

    struct vehicle *frontchain = *(htable + hashval);
    newvc->next = frontchain;
    *(htable + hashval) = newvc;

    return 0;
}
#endif
