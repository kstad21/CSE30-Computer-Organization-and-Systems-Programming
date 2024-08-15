/*
 * Programming assignment 4 main function
 *
 * main
 *
 * Usage: csv col [col..]
 *
 * Overview:
 *      reads a row of text (a row end in a \n") in comma separated format from
 *      standard input. It then writes this row in comma separated format to
 *      standard output while reformatting each row as specified by the command
 *      line arguments (selects which columns to output and the output order).
 *      Error messages are written to stderr.
 *
 *      See the PA writeup for more details
 *      You should remove line comments that start at the left side of the file
 *      they are hints of what to do.
 *
 * Return value
 *      Return EXIT_FAILURE is there is an error or 1 or more rows were dropped
 *      Otherwise, EXIT_SUCCESS is returned.
 *
 * Error messages:
 * 	main() writes error messages to stderr
 */ 
#include "version.h"
#ifndef MYCSV
#warning("WARNING - USING THE SOLUTION csv.c NOT MY CODE")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "splitrow.h"
#include "wrtrow.h"
#include "setorder.h"

#define DELIM   ','             // column delimiter
// NOTICE next three values are set small so corner cases are easy to test
// These three values are too small for real use.
#define BUFSZ   128             // row buffer size
#define MAXCOL  8               // Max number of columns in a row
#define MAXARG  8               // Max number of output columns

/*
 * csv main function
 */ 
int 
main(int argc, char **argv)
{
    char buf[BUFSZ];            // input row buffer
    char *ptable[MAXCOL];       // array of pointers to column starts
    int coltab[MAXARG];         // array of output column offset values
    unsigned long rowcount = 0;     // input row counter
    unsigned long dropcnt = 0;     // dropped rows counter
    int incols;                 // number of columns in the input
    int outcols;                // number of columns in the output
    char delim = ',';

    /*
     * only using argv for this assignment, argc not used
     */
    (void)argc;                 // suppress compiler warnings hack (sigh)... 

    fgets(buf, BUFSZ, stdin);

    incols = splitrow(buf, ptable, MAXCOL, delim);
    if (incols <= 1) {
	fprintf(stderr, "%s: Header row input error\n", *argv);
	return EXIT_FAILURE;
    }

    outcols = setorder(argv, coltab, MAXCOL, incols);
    if (outcols < 1) {
        return EXIT_FAILURE;
    }

    if (wrtrow(ptable, coltab, outcols, delim) != outcols) {
	    fprintf(stderr, "%s: Header row output failure, exiting\n", *argv);
	    return EXIT_FAILURE;
    }
    rowcount++;

    while (fgets(buf, BUFSZ, stdin) != NULL) {
	    rowcount++;

	if (splitrow(buf, ptable, incols, delim) != incols) {
	    fprintf(stderr, "%s: Dropped input row %lu\n", *argv, rowcount);
	    dropcnt++;
	    continue;
	}
	
	if (wrtrow(ptable, coltab, outcols, delim) != outcols) {
	    fprintf(stderr, "%s: Dropped input row %lu\n", *argv, rowcount);
	    dropcnt++;
	}


    }

    fprintf(stderr, "%s: %lu rows in, %lu dropped\n", *argv, rowcount, dropcnt);
    if (dropcnt > 0) {
	    return EXIT_FAILURE;
    }


    /*
     * the error messages for use in this file
     *
    fprintf(stderr, "%s: Header row input error\n", *argv);
    fprintf(stderr, "%s: Header row output failure, exiting\n", *argv);
    fprintf(stderr, "%s: Dropped input row %lu\n", *argv, rowcount);
    fprintf(stderr, "%s: Dropped output row %lu\n", *argv, rowcount);
    fprintf(stderr, "%s: %lu rows in, %lu dropped\n", *argv, rowcount, dropcnt);
    */

    return EXIT_SUCCESS;
}
#endif // do not delete
