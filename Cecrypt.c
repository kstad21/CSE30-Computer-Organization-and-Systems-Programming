#include "Version.h"

#if defined(MYECRYPT_S)
#warning("using my ecrypt.S")
#elif !defined(MYECRYPT_C)
#warning("using solution ecrypt")
#else
#warning("using my Cecrypt.c")

// version 1.1 05/08/2024
#include <stdio.h>
#include "cipher.h"

// encrypts  inbuf with bookbuf; updating inbuf
int
ecrypt(unsigned char *inbuf, unsigned char *bookbuf, int cnt)
{
    if (cnt <= 0)
        return 0;

    // process buffers, one byte at a time 
    for (int i = 0; i < cnt; i++) {
		unsigned char *currbyte = (inbuf + i);
	unsigned char *currbook = (bookbuf + i);
		
	// swap groups of 4 bits with mask 0xF0 and 0x0F
	*currbyte = ((*currbyte & 0xF0) >> 4 | (*currbyte & 0x0F) << 4);

	// swap all the pairs with mask 0xCC and 0x33
	*currbyte = ((*currbyte & 0xCC) >> 2 | (*currbyte & 0x33) << 2);

	// swap all adjacent bits with mask 0xAA and 0x55
	*currbyte = ((*currbyte & 0xAA) >> 1 | (*currbyte & 0x55) << 1);

	// XOR the results w/ bookbuf byte
	*currbyte = *currbyte ^ *currbook;
    }

    // return number of bytes encrypted
    return cnt;
}
#endif
