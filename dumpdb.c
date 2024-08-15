#include "Version.h"
#ifndef MYDUMPDB
#warning("WARNING - USING THE SOLUTION dumpdb.c NOT MY CODE")
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
#include "subs.h"
#include "dumpdb.h"

/*
 * dumpchain
 *
 * dump the linked list chain on a single hash chain
 * print each vehicle on the chain using printvehicle()
 *
 * Arguments:
 *
 * index:
 *          hash table chain index number
 * cnt:
 *          a pointer to an output variable that contains the count
 *          of the total number of tickets on the hash chain
 *          set *cnt = 0UL if no tickets are found
 *
 * return:
 *          number of vehicles found on the hash chain
 *          total number of tickets in chain returned in output variable *cnt
 */
unsigned long
dumpchain(uint32_t index, unsigned long *cnt)
{
    printf("Chain %u: \n", index);
    //step down each vehicle entry until end of the chain
    struct vehicle *currvc = *(htable + index);

    unsigned long vcount = 0UL;
    unsigned long tcount = 0UL;

    while (currvc != NULL) {
	vcount++;

	//call printvehicle(struct vehicle *), returns insigned int #tickets printed
	tcount += printvehicle(currvc);
        
	//go to the next vehicle
	currvc = currvc->next;
    }

    *cnt = tcount;
    
    //return the count of vehicles 
    return vcount;
}

/*
 * dumpdb
 *
 * dump the entire database one hash chain at a time from chain 0
 * in the hash table to the last entry in the table
 *
 * call dumpchain() to print each hash chain.
 *
 * Prints the total number of vehicles and tickets in the database
 *
 */
void
dumpdb(void)
{
    unsigned long vcnt = 0UL; // total vehicle count
    unsigned long tcnt = 0UL; // total ticket count

    struct vehicle *chain = *htable;
    unsigned long index = 0UL;

    while (index < tabsz) {
	if (chain == NULL) {
	    if (index == tabsz - 1) {
		break;
	    }
	    index++;
	    chain = *(htable + index);
	    continue;
	} 
	unsigned long currtktct = 0UL;

	vcnt += dumpchain(index, &currtktct);

	//add the current ticket count to the total
	tcnt = tcnt + currtktct;

        //iterate index
	index++;
	if (index == tabsz) {
	    break;
	}
	chain = *(htable + index);
    }

 
    // print the summaries
    printf("Total vehicles in database: %lu\n", vcnt);
    printf("Total tickets in database: %lu\n", tcnt);
    return;
}
#endif
