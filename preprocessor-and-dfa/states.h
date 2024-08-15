/* 
 * Header file for PA3
 */
#ifndef STATES_H
#define STATES_H

/*
 * definitions for DFA states
 */
#define START 0
#define QUOTE 1
#define APOS 2
#define RSQ 3
#define RSA 4
#define LEFTSLASH 5
#define LINECOM 6
#define BLOCKCOM 7
#define ENDSTAR 8

/*
 *  function prototypes for each state handler of the DFA
 */
// replace this comment with your function prototype
int STARTstate(int);
int QUOTEstate(int);
int APOSstate(int);
int RSQstate(int);
int RSAstate(int);
int LEFTSLASHstate(int);
int LINECOMstate(int);
int BLOCKCOMstate(int);
int ENDSTARstate(int);

#endif
