// version 1.2 05/08/2024
#ifndef CIPHER_H
#define CIPHER_H
#define RETRN_OK        0
#define RETRN_FAIL      -1
#ifndef __ASSEMBLER__
int ecrypt(unsigned char *, unsigned char *, int);
int dcrypt(unsigned char *, unsigned char *, int);
#else
.extern ecrypt
.extern dcrypt
#endif
#endif
