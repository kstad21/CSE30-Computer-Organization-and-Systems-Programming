/*
 * State handler routines for PA3
 * one function for each state of the DFA
 * each function is passed current input and returns next state
 */
#include <stdio.h>
#include "states.h"

/*
 * function STARTstate(int c)
 *
 * Operation: takes in an int c,
 * dtermines the next state.
 * assumes: c is not inside a 
 * comment or string literal.
 * params: c
 * returns: the next state
 */
int
STARTstate(int c)
{
    if (c == '\"') {
	putchar(c);
        return QUOTE;
    } else if (c == '\'') {
	putchar(c);
        return APOS;
    } else if (c == '/') {
    	return LEFTSLASH;
    } else {
	putchar(c);
        return START;
    }
}

/*
 * function QUOTEstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c is inside a string
 * literal defined by quotes.
 * params: c
 * returns: the next state
 */
int 
QUOTEstate(int c) {
    if (c == '\\') {
        putchar(c);
        return RSQ; 
    } else if (c == '\"') {
        putchar(c);
	return START;
    } else {
        putchar(c);
	return QUOTE; 
    }
}

/*
 * function APOSstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c is inside a string
 * literal defined by apostrophes.
 * params: c
 * returns: the next state
 */
int
APOSstate(int c) {
    if (c == '\\') {
        putchar(c);
	return RSA;
    } else if (c == '\'') {
        putchar(c);
	return START;
    } else {
	putchar(c);
	return APOS;
    }
}

/*
 * function RSQstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c comes after a right
 * slash inside a string literal
 * defined by quotes.
 * params: c
 * returns: the next state
 */
int
RSQstate(int c) {
    putchar(c);
    return QUOTE;
}

/*
 * function RSAstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c comes after a right
 * slash inside a string literal 
 * defined by apostrophes.
 * params: c
 * returns: the next state
 */
int 
RSAstate(int c) {
    putchar(c);
    return APOS;
}

/*
 * function LEFTSLASHstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c comes after a left 
 * slash and is not inside quotes
 * or apostrophes.
 * params: c
 * returns: the next state.
 */
int
LEFTSLASHstate(int c)
{
    if (c == '/') {
        putchar(' ');
	return LINECOM;
    } else if (c == '*') {
        putchar(' ');
	return BLOCKCOM;
    } else if (c == '\"') {
	putchar('/');
	putchar(c);
	return QUOTE;
    } else if (c == '\'') {
	putchar('/');
	putchar(c);
	return APOS;
    } else {
	putchar('/');
	putchar(c);
	return START;
    }
}

/*
 * function LINECOMstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c is inside a line
 * comment.
 * params: c
 * returns: the next state
 */
int
LINECOMstate(int c)
{
    if (c == '\n') {
        putchar('\n');
	return START;
    } else {
        return LINECOM;
    }
}

/*
 * function BLOCKCOMstate(int c)
 *
 * Operation: takes in an int c,
 * determines the next state.
 * assumes: c is inside a block
 * comment.
 * params: c
 * returns: the next state
 */
int
BLOCKCOMstate(int c)
{
    if (c == '\n') {
        putchar('\n');
	return BLOCKCOM;
    } else if (c == '*') {
	return ENDSTAR;
    } else {
	return BLOCKCOM;
    }
}

/*
 * function ENDSTARstate(int c)
 *
 * Operation: takes in an int c, 
 * determines the next state.
 * assumes: c is after a * inside
 * a block comment.
 * params: c
 * returns: the next state
 */
int
ENDSTARstate(int c)
{
    if (c == '\n') {
        putchar('\n');
	return BLOCKCOM;
    } else if (c == '/') {
	return START;
    } else if (c == '*') {
	return ENDSTAR;
    } else {
        return BLOCKCOM;
    }
}



