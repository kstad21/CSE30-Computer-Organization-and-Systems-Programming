#include "Version.h"
#ifndef MYVEHLOOKUP
#warning("WARNING - USING THE SOLUTION vehlookup.c NOT MY CODE")
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
#include "vehlookup.h"
#include "subs.h"

/*
 * vehiclelookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 *
 * Arguments:
 *
 *  plate:
 *          plate id string to be found
 *  state:
 *          state id string to be found
 *
 * return:
 *          pointer to vehicle if found
 *          NULL otherwise
 */

struct vehicle *
vehiclelookup(char *plate, char *state)
{
    //hash by the license plate string to get the hash chain
    uint32_t hashindex =  hash(plate) % tabsz;

    struct vehicle* currv = *(htable + hashindex);
    int match = 0;
    while (currv != NULL) {
        if (strcmp(currv->plate, plate) == 0 && strcmp(currv->state, state) == 0) {
	    match = 1;
	    break;
	} 

	if (currv->next != NULL) {
	    currv = currv->next;
        }
    }

    if (match == 1) {
        return currv;
    } else {
	return NULL;
    }
}
#endif
