// version 1.0 11/12/2022
#ifndef RDBUF_H
#define RDBUF_H
#ifndef __ASSEMBLER__
int rdbuf(FILE *, FILE *, int, unsigned char *, unsigned char *);
#else
.extern rdbuf
#endif
#endif
