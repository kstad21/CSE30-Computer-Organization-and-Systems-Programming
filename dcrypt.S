#include "Version.h"

#ifndef MYDCRYPT_S
    .section .note.GNU-stack,"",%progbits
    .end
#else

    // version 1.1 05/08/2024
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "cipher.h"

    .text     // start of text segment

    //////////////////////////////////////////////////
    // int dcrypt(unsigned char *inbuf, unsigned char *bookbuf, int cnt)
    // decrypts inbuf using bookbuf; result in inbuf
    //////////////////////////////////////////////////
    .global dcrypt
    .type   dcrypt, %function
    .equ    FP_OFF, 28

   // r0 contains char *inbuf
   // r1 contains char *bookbuf
   // r2 contains cnt
   // r3 is ok to use
   //
   // preserved register use table
   //
   // r4 currbyte
   // r5 currbyte
   // r6 index
   // r7
   // r8
   // r9

dcrypt:
    push    {r4-r9, fp, lr}
    add     fp, sp, FP_OFF


    cmp     r2, 0               // if buffer is empty we are done
    ble     .Ldone

    // your code here
    ldrb r4, [r0] 		// actual byte for in
    ldrb r5, [r1] 		// actual byte for book
    mov r6, 0 			// set index to 0

.Lfor:
    cmp r6, r2
    bge .Ldone 			// break if index >= cnt

    eor r4, r4, r5		// exor bookbyte with inbuf byte

    and r7, r4, 0xF0		// get left 4 bits
    lsr r7, r7, 4		// shift left 4 bits right in temp
    and r8, r4, 0x0F		// get right 4 bits
    lsl r8, r8, 4		// shift right 4 bits left in temp2
    orr r4, r7, r8		// combine back into r4

    and r7, r4, 0xCC		// get first pair in each half byte
    lsr r7, r7, 2		// shift pair right in temp
    and r8, r4, 0x33		// get second pair
    lsl r8, r8, 2		// shift pair left in temp2
    orr r4, r7, r8		// combine back into r4

    and r7, r4, 0xAA		// get 1st alternating bits
    lsr r7, r7, 1		// shift bits right in temp
    and r8, r4, 0x55		// get 2nd alternating bits
    lsl r8, r8, 1		// shift bits left in temp2
    orr r4, r7, r8		// combine back into r4

    strb r4, [r0, r6] 		// store current byte r4 into memory address at r0 
				// w/ offset r6

    add r6, r6, 1		// iterate index
    ldrb r4, [r0, r6]		// reload r4 with next byte for in
    ldrb r5, [r1, r6]		// reload r5 with next byte for book

    b .Lfor 

.Ldone:
    mov r0, r6			// populate r0 with index

    sub     sp, fp, FP_OFF
    pop     {r4-r9, fp, lr}
    bx      lr

    .size dcrypt, (. - dcrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
