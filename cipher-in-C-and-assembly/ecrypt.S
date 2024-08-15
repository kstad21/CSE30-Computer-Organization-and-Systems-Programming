#include "Version.h"

#ifndef MYECRYPT_S
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
    // int ecrypt(unsigned char *inbuf, unsigned char *bookbuf, int cnt)
    // encrypts  inbuf with bookbuf; updating inbuf
    //////////////////////////////////////////////////
    .global ecrypt
    .type   ecrypt, %function
    .equ    FP_OFF, 28

   // r0 contains char *inbuf
   // r1 contains char *bookbuf
   // r2 contains cnt
   // r3 is ok to use
   //
   // preserved register use table
   //
   // r4 currbyte byte
   // r5 currbook byte
   // r6 index
   // r7 temp
   // r8 temp2
   // r9

ecrypt:
    push    {r4-r9, fp, lr}
    add     fp, sp, FP_OFF

    cmp     r2, 0               // if buffer is empty we are done
    ble     .Ldone

    // load byte from buffers into r4, r5
    ldrb    r4, [r0] 		//actual byte for in
    ldrb    r5, [r1] 		//actual byte for book
    mov     r6, 0

.Lfor:
    cmp     r6, r2
    bge     .Ldone        	// break if index is >= cnt

    and     r7, r4, 0xF0	// get left 4 bits
    lsr     r7, r7, 4		// shift 4 bits right in temp
    and     r8, r4, 0x0f	// get right 4 bits
    lsl     r8, r8, 4		// shift 4 bits left in temp2
    orr     r4, r7, r8		// combine back into r4

    and     r7, r4, 0xCC	// get first pair in each 4 bit
    lsr     r7, r7, 2		// shift pairs right in temp
    and     r8, r4, 0x33	// get second pair in each 4 bit
    lsl     r8, r8, 2		// shift pairs left in temp2
    orr     r4, r7, r8		// combine back into r4

    and     r7, r4, 0xAA	// get 1st alternating indiv bit
    lsr     r7, r7, 1		// shift individual bit right in temp
    and     r8, r4, 0x55	// get 2nd alternating indiv bit
    lsl     r8, r8, 1		// shift individual bit left in temp2
    orr     r4, r7, r8		// combine back into r4

    eor     r4, r4, r5		// exor with book byte

    strb    r4, [r0, r6]   	// store current byte r4 into memory address at r0 w/ 
			 	// offset in r6

    add     r6, r6, 1    	// increment index
    ldrb    r4, [r0, r6] 	// reload in byte
    ldrb    r5, [r1, r6] 	// reload book byte

    b .Lfor			
    

.Ldone:
    mov r0, r6			// populate r0 with index

    sub     sp, fp, FP_OFF
    pop     {r4-r9, fp, lr}
    bx      lr

    .size ecrypt, (. - ecrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
