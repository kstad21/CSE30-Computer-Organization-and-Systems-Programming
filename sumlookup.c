#include "Version.h"
#ifndef MYSUMLOOKUP
#warning("WARNING - USING THE SOLUTION sumlookup.c NOT MY CODE")
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
#include "sumlookup.h"
#include "subs.h"

/*
 * summlookup
 *     find the vehicle with a specified summons number
 *
 * Arguments:
 *
 * summ:
 *       string of summons number
 *       make sure to convert summ string to a summons number
 *       use strsumid() - see subs.c
 *
 * returns:
 *       a pointer to the vehicle if found
 *       NULL if not found
 */
struct vehicle *
sumlookup(char *summ)
{
    errno = 0;
    unsigned long summid;
    int sumresult = strtosumid(summ, &summid);
    
    //check that the conversion worked
    if (sumresult != 0) {
        return NULL;	
    }
    
    unsigned int chainind = 0;
    struct vehicle* chain = *(htable);

    //iterate thru each chain
    while (chainind < tabsz) {
	if (chain == NULL) {
            chainind++;
	    if (chainind == tabsz) {
		break;
	    }
	    chain = *(chainind + htable);
	    continue;
	}
        struct vehicle* currvc = chain;

	while (currvc != NULL) {
	    struct ticket* tktchain = currvc->head;
	    
	    while (tktchain != NULL) {
	        if (tktchain->summons == summid) {
		    return currvc;
		}

		tktchain = tktchain->next;
	    }

	    currvc = currvc->next;
	}

	chainind++;
	if (chainind != tabsz) {
	    chain = *(chainind + htable);
	}
    }

    return NULL;
     
}
#endif
