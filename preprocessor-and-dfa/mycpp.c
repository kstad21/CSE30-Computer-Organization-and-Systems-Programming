/*
 * CSE Programming assignment 3 
 *
 * removes line and block comments from a source file
 * comments are replaced with a single space
 * while writing all other stdin to stdout
 * handles single and double quoted sequences
 * Ignores line continuation characters
 *
 * error handling:
 *     prints starting line for unterminated comment sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "states.h"

int
main(void)
{

// replace this line with your code
    int c = getchar();
    int state = START;
    int linenum = 1;
    int startline = 1;

    while (c != EOF) {
        switch (state) {
	case(START):
	    state = STARTstate(c);
	    break;
	case (QUOTE):
	    state = QUOTEstate(c);
	    break;
	case (APOS):
	    state = APOSstate(c);
	    break;
	case (RSA):
	    state = RSAstate(c);
	    break;
	case (RSQ):
	    state = RSQstate(c);
	    break;
	case (LEFTSLASH):
	    state = LEFTSLASHstate(c);
	    if (state == BLOCKCOM) {
	        startline = linenum;
	    }
	    break;
	case (LINECOM):
	    state = LINECOMstate(c);
	    break;
	case (BLOCKCOM):
	    state = BLOCKCOMstate(c);
	    break;
	case (ENDSTAR):
            state = ENDSTARstate(c);
	    break;
        default:
	    break;
            
	} //end switch
	c = getchar();
	if (c == '\n') {
	    linenum++;
	}
    } //end while
    
    if (state != BLOCKCOM && state != LINECOM) {
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Error: line %d: unterminated comment\n", startline);
	return EXIT_FAILURE;
    }
}
