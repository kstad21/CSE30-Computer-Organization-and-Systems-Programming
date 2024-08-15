#include "Version.h"
#ifndef MYFREETICKETS
#warning("WARNING - USING THE SOLUTION freetickets.c NOT MY CODE")
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
#include "freetickets.h"

/*
 * freetickets
 *      tears down the database freeing all allocated memory
 *      count the number of tickets and vehicles freed in
 *      the database 
 *      prints the counts before returning
 */
void
freetickets(void)
{
    unsigned long tcnt = 0UL;
    unsigned long vcnt = 0UL;
    unsigned int chainind = 0;

    struct vehicle *chain = *htable;

    while (chainind < tabsz) {
	if (chain == NULL) {
            chainind++;
	    if (chainind < tabsz) {
		chain = *(htable + chainind);
	    } else {
		break;
            }
	}

	struct vehicle *currvc = chain;

	while (currvc != NULL) { 
	    struct ticket *tktchain = currvc->head;

	    while (tktchain != NULL) {
		struct ticket *todelete = tktchain;
		tktchain = tktchain->next;

		free(todelete);
		todelete = NULL;
		tcnt++;
	    }

	    struct vehicle *vtodelete = currvc;
	    currvc = currvc->next;

	    free(vtodelete->plate);
	    vtodelete->plate = NULL;

	    free(vtodelete->state);
	    vtodelete->state = NULL;

	    free(vtodelete);
	    vtodelete = NULL;

	    vcnt++;
	}

	chainind++;
	if (chainind >= tabsz) {
	    break;
	}
	chain = *(htable + chainind);
    }

    printf("Total vehicles freed: %lu\n", vcnt);
    printf("Total tickets freed: %lu\n", tcnt);
    return;
}
#endif
