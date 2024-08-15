#include "Version.h"

#if defined(MYDCRYPT_S)
#warning("using my dcrypt.S")
#elif !defined(MYDCRYPT_C)
#warning("using solution decrypt")
#else
#warning("using my Cdcrypt.c")

// version 1.1 05/08/2024
#include <stdio.h>
#include "cipher.h"

// decrypts inbuf with bookbuf; updating inbuf
int
dcrypt(unsigned char *inbuf, unsigned char *bookbuf, int cnt)
{
    if (cnt <= 0)
        return 0;
    
    for (int i = 0; i < cnt; i++) {	
        unsigned char *currbyte = (inbuf + i);
	unsigned char *currbook = (bookbuf + i);

	// XOR with bookbuf byte
	*currbyte = *currbyte ^ *currbook; 

        // swap groups of 4 bits with mask 0xF0 and 0x0F	
	*currbyte = ((*currbyte & 0xF0) >> 4 | (*currbyte & 0x0F) << 4);

	// swap pairs of bits with mask 0xCC and 0x33
	*currbyte = ((*currbyte & 0xCC) >> 2 | (*currbyte & 0x33) << 2);

	// swap adjacent bits with mask 0xAA and 0x55
	*currbyte = ((*currbyte & 0xAA) >> 1 | (*currbyte & 0x55) << 1);

	
    }

    // return number of bytes decrypted
    return cnt;
}
#endif
