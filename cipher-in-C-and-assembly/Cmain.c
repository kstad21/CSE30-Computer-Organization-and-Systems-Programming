#include "Version.h"

#if defined(MYMAIN_S)
#warning("using my main.S")
#elif !defined(MYMAIN_C)
#warning("using solution main")
#else
#warning("using my Cmain.c")

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "rdbuf.h"
#include "setup.h"
#include "cipher.h"

// version 1.1 05/08/2024

/*
 * cipher program
 * usage:  cipher [-d|-s) -b bookfile
 * cipher reads the data to be processed from standard input, either
 * encrypts (-e) or decrypts (-d) the data, and then writes the result
 * to standard output.
 */

int
main(int argc, char **argv)
{
    /*
     * local vars on stack these are given to help you write main.S later
     * it is suggested that you not change the order of the vars.
     */
    FILE *fpbook;                   // File * to bookfile
    unsigned char inbuf[BUFSZ];     // input buffer - same size as bookbuf
    unsigned char bookbuf[BUFSZ];   // output buffer - same size as inbuf
    int (*func)();                  // function pointer to ecrypt or dcrypt
    char *argv0 = *argv;            // saved argv[0] for fprintf()
    // end of required stack variables

    /*
     * call setup which does:
     * (1) parse the command line arguments, (2) func is set to point at either
     * ecrypt or dcrypt and open the book file
     */
    if (setup(argc, argv, &func, &fpbook) == RETRN_FAIL)
        return EXIT_FAILURE;

    /*
     * (1) use rdbuf() to read the input from stdin into inbuf and read 
     * the book file into bookbuf until EOF on the input file. Pass
     * rdbuf() BUFSZ as the size of the buffers 
     * (2) Call func to encrypt or decrypt
     * (3) use fwrite() to write inbuf to stdout. Make sure to check that
     *  fwrite() writes all the valid contents contained in inbuf.
     */ 
    int c;
    while ((c = rdbuf(stdin, fpbook, BUFSZ, inbuf, bookbuf)) != 0) {
        if (c == RETRN_FAIL) {
            fprintf(stderr, "%s: read failed \n", argv0);
	    return EXIT_FAILURE;
	}

        // save the result of func to nbytes
	int nbytes = func(&inbuf, &bookbuf, c);

	int writeresult = (int)fwrite(inbuf, 1, nbytes, stdout);

	//check that the correct number of bytes was written
        if (writeresult != nbytes) {
	    fprintf(stderr, "%s: write failed\n", argv0);
	    (void)fclose(fpbook);
	    return EXIT_FAILURE;
        }	    
    }

    (void)fclose(fpbook);    
    return EXIT_SUCCESS;
}
#endif
