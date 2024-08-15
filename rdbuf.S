#include "Version.h"

#ifndef MYRDBUF_S
    .section .note.GNU-stack,"",%progbits
    .end
#else

    // version 1.1 05/08/2024
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // global constants
    #include "cipher.h"

    .extern fread
    .text

    //////////////////////////////////////////////////////
    // int rdbuf(FILE *in, FILE *book, int cnt, unsigned char *inbuf,
    //          unsigned char *bookbuf)
    // OBSERVE this has 5 arguments!
    //
    // This function reads up to cnt bytes into inbuf from FILE *in such
    // as bytes = fread(). bytes is the actual number of bytes read.
    // Then read the same number of bytes from the FILE *book into the
    // buffer bookbuf.
    // 
    //  Arguments
    //  in       pointer of type FILE where the input data is read from
    //  fobook   pointer of type FILE where the bookfile is read from
    //  cnt      max number of bytes to read into inbuf
    //  inbuf    buffer where the input data is read into
    //  bookbuf  buffer where the bookfile data is read into
    // 
    //  Returns
    //  0        when EOF is reached on the in
    //  bytes    number of bytes actually read
    //  RETRN_FAIL
    //           when the read on bookbuf does not return bytes
    //////////////////////////////////////////////////////

    .type   rdbuf, %function
    .global rdbuf
    // determine how many registers to save and set FP_OFF
    .equ    FP_OFF, 20

   // preserved register use table fill this in to help you
   //
   // r4 fobook
   // r5 bookbuf
   // r6 fread result
   // r7 temp
   // r8 
   // r9 
 
rdbuf:
    // set up stack frame
    push        {r4-r7, fp, lr}
    add         fp, sp, FP_OFF

    // fread() from FILE *in into inbuf
    // now fread() the same number of bytes from the bookfp
    // as was read from the input FILE *in into bookbuf
    // if bytes read from bookfile is not the same as read into inbuf,
    // then return RETRN_FAIL;
    
    // mov not-yet-needed arguments into preserved registers
    ldr         r5, [fp, 4] // load the stack variable bookbuf into r5
    mov		r4, r1  // move fobook into r4
    
    // set up for fread
    mov         r1, 1  // move 1 into r1
    mov         r7, r0 // move in into r7 (temp)
    mov         r0, r3 // move inbuf into r0
    mov         r3, r7 // move in (which is in temp r7) into r3
    bl fread
    
    mov		r6, r0 // move result into r6
    mov 	r0, r5 // move bookbuf (which was loaded into r5) into r0
    mov 	r1, 1  // move 1 into r1
    mov 	r2, r6 // mov r6 (result) into r2
    mov 	r3, r4 // mov fobook into r3
    bl fread

    cmp 	r0, r6
    beq .Ldone
    mov 	r0, RETRN_FAIL 

    // return the number of bytes read
    // r0 should already have # of bytes read if we're not
    // supposed to return RETRN_FAIL
.Ldone:
    // remove stack frame
    sub sp, fp, FP_OFF
    pop {r4-r7, fp, lr}

    bx      lr

    .size   rdbuf, (. - rdbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
