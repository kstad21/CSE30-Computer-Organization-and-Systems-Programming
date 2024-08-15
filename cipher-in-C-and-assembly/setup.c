#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "cipher.h"
#include "setup.h"

// version 1.1 05/08/2024
int
setup(int argc, char **argv, int (**func)(), FILE **fpbook)
{
    int opt;
    int eflag = 0;          // did we see the -e flag
    int dflag = 0;          // did we see the -d flag
    char *bookname = NULL;  // name of bookfile to open

    /*
     * set parameters to default value to reduce segfaults when called
     * by faulty code
     */
    *fpbook = NULL;
    *func = NULL;
    
    while ((opt = getopt(argc, argv, "edb:")) != -1) {
        switch (opt) {
        case 'e':
            eflag = 1;
            *func = ecrypt;
            break;
        case 'd':
            dflag = 1;
            *func = dcrypt;
            break;
        case 'b':
            bookname = optarg;
            break;
        case '?':
            /* FALL THROUGH */
        default:
            fprintf(stderr, "Usage: %s [-d|-e] -b <bookfile>\n", *argv);
            return RETRN_FAIL;
            /* NOT REACHED */
            break;
        }
    }
    if (((dflag == 0) && (eflag == 0)) || ((dflag == 1) && (eflag == 1))) {
        fprintf(stderr, "%s: You must specify either -e or -d\n", *argv);
        fprintf(stderr, "Usage: %s [-d|-e] -b <bookfile>\n", *argv);
        *func = NULL;
        return RETRN_FAIL;
    }

    if (optind != argc) {
        fprintf(stderr, "Usage: %s [-d|-e] -b <bookfile>\n", *argv);
        *func = NULL;
        return RETRN_FAIL;
    }

    if (bookname == NULL) {
        fprintf(stderr, "%s: -b bookfile must be specified\n", *argv);
        fprintf(stderr, "Usage: %s [-d|-e] -b <bookfile>\n", *argv);
        *func = NULL;
        return RETRN_FAIL;
    }

    /* open the bookfile */
    if ((*fpbook = fopen(bookname, "r")) == NULL) {
        fprintf(stderr, "%s: Unable to open bookfile %s\n", *argv, bookname);
        *func = NULL;
        return RETRN_FAIL;
    }
    return RETRN_OK;
}
