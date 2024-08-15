/*
 * setorder
 *
 * usage:   argv array passed to main(): command name + arg list
 *          each argument is a column number of a column that will be output
 *          (1) convert each argument to an integer using strtol()
 *          and check the conversion as described in the PA writeup. If 
 *          there is an error, print an error message and return -1
 *          (2) Each converted value is checked to be in the range 1 to maxcol
 *          print an error message and return -1 if not
 *          (3) The converted value is adjusted to be an array offset value
 *          and is stored in the array coltab in the same order the args are
 *          listed in argv.
 *          (4) If the number of args is greater than maxcol print an error
 *          message and return -1
 *          (5) Otherwise all ok, return the number of entries in indxtab
 *
 * arguments:
 * argv     argv array of pointers to strings as passed to main()
 * coltab   points at an int array. Each entry specifies an input row
 *          column to be output. The order the columns appear in coltab
 *          is the order of the columns in the output row
 * tabsz    the size (number of elements) in the array coltab
 * maxcol   number of columns in a row (no arg can be greater than this value)
 *
 * Return values:
 *           -1 if if any error is detected
 *           otherwise return the number of entries stored in indxtab
 *
 * error messages:
 *	     this function writes error messages to stderr
 */

#include "version.h"
#ifndef MYSETORDER
#warning("WARNING - USING THE SOLUTION setorder.c NOT MY CODE")
#else
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include "setorder.h"

int setorder(char **argv, int *coltab, int tabsz, int maxcol)
{
    const char *fname = *argv++;
    char *endptr;
    int numargs = 0;
    int number;

    if (*argv == NULL) {
        fprintf(stderr, "%s: at least one column must be specified\n", fname);
	    fprintf(stderr, "usage: %s col [col ...]\n", fname);
	    return -1;
    }

    while (*argv != NULL) {
	    if (numargs >= tabsz) {
	        fprintf(stderr, "%s: too many arguments %d max\n", fname, tabsz);
	        fprintf(stderr, "usage: %s col [col ...]\n", fname);
	        return -1;
	    }

	    errno = 0;
        number = (int)strtol(*argv, &endptr, 10);

	    if ((errno != 0) || (*endptr != '\0') || (number <= 0)) {
	        fprintf(stderr, "%s: improper argument: %s\n", fname, *argv);
	        return -1;
	    }

	    if (!(number >= 1 && number <= maxcol)) {
	        fprintf(stderr, "usage: %s col [col ...]\n", fname);
	        return -1;
	    }

	    *coltab = number - 1;
	    coltab++;
    	numargs++;
    	argv++;

    } //endwhile
    
    return numargs;


/*
 * the error messages for use in this file
 * fprintf(stderr, "%s: improper argument: %s\n", *argv, *ptrargv);
 * fprintf(stderr, "%s: at least one column must be specified\n", *argv);
 * fprintf(stderr, "%s: too many arguments %d max\n", *argv, tabsz);
 * fprintf(stderr, "usage: %s col [col ...]\n", *argv);
 */

}
#endif // do not delete this line
