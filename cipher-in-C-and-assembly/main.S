#include "Version.h"

#ifndef MYMAIN_S
    .section .note.GNU-stack,"",%progbits
    .end
#else

    // version 1.3 05/11/2024 //
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // imports
    #include "main.h"
    #include "rdbuf.h"
    #include "setup.h"
    #include "cipher.h"
    .extern fwrite
    .extern stdin
    .extern stdout
    .extern stderr
    //
    // cipher [-d | -e] -b bookfile
    //
    // int main(int argc, char **argv)
    //

    .section .rodata
    // string literals for fprint
.Lrdmess:  .string "%s: read failed\n"
    .align 2
.Lwrmess:  .string "%s: write failed\n"

    .text
    .global main
    .type   main, %function
    //
    // complete the stack frame layout
    //
    .equ    FP_OFF,     32          // fp offset in main stack frame

    // use the same stack allocation order as in Cmain.c
    .equ FPBOOK,	4 + FP_OFF
    .equ INBUF,		BUFSZ + FPBOOK
    .equ BOOKBUF,	BUFSZ + INBUF
    .equ FUNC,		4 + BOOKBUF
    .equ ARGV0,         4 + FUNC
    .equ PAD,           4 + ARGV0
    .equ OARG5,         4 + PAD
    .equ FRMADD,        OARG5 - FP_OFF
    

    // preserved register use table fill this in describing your use for each
    //
    // r4 counter
    // r5 
    // r6
    // r7
    // r8
    // r9
    // r10 argv0

main:
    push    {r4-r10, fp, lr}        // observe: odd number of regs saved
    add     fp, sp, FP_OFF          // set frame pointer to frame base

    // allocate space for locals and outgoing args below
    add     sp, sp, -FRMADD 	    
     
    // call int setup(argc, argv, &func, &fpbook)
    // pass a pointer to *func in r2, pointer to *fpbook in r3
    add     r2, fp, -FUNC	    // get FUNC address
    add	    r3, fp, -FPBOOK	    // get FPBOOK address
    ldr     r10, [r1] 		    // save argv0 adress
    bl 	    setup		    // call setup

    cmp     r0, RETRN_FAIL	    // compare to see if branch needed
    beq     .Lfail

    //
    // top of loop
    //
    // int rdbuf(r0=stdin, r1=fpbook, r2=BUFSZ, r3=inbuf,OARG5=bookbuf)
    // cnt = rdbuf(FILE *, FILE*, int, unsigned char *, unsigned char *)
    //
.Lwhile:
    ldr     r0, =stdin		    // get the address of stdin
    ldr     r0, [r0]		    // load stdin
    ldr     r1, [fp, -FPBOOK]	    // load fpbook
    mov     r2, BUFSZ		    // mov BUFSZ to r2
    add     r3, fp, -INBUF	    // get INBUF address
    add     r4, fp, -BOOKBUF	    // get BOOKBUF address
    str     r4, [fp, -OARG5]	    // store BOOKBUF in OARG5

    bl 	    rdbuf 		    // call rdbuf
    cmp     r0, RETRN_FAIL	    // check results
    beq     .Lread
    cmp     r0, 0
    beq     .Ldone

    //
    // cnt = func(unsigned char *inbuf, unsigned char *bookbuf, int cnt)
    //
    mov     r2, r0		    // move cnt
    add     r0, fp, -INBUF	    // get INBUF address
    add     r1, fp, -BOOKBUF	    // get BOOKBUF address
    ldr     r5, [fp, -FUNC]	    // load FUNC
    blx     r5			    // branch to FUNC

    //
    // fwrite(inbuf, 1, cnt, stdout)
    // 
    mov     r2, r0		    // mov cnt to r0
    mov     r5, r0		    // save cnt
    sub     r0, fp, INBUF	    // get INBUF address
    mov     r1, 1		    // move 1 into r1
    ldr     r3, =stdout		    // get stdout address
    ldr     r3, [r3]		    // load stdout
    bl 	    fwrite		    // call fwrite
    cmp     r0, r5		    // compare the two byte counts
    bne     .Lwrite
    bl 	    .Lwhile

    
    //
    // bottom of loop
    //
    //
    // error handling code
    //
.Lwrite:
    ldr     r0, =stderr		    // get stderr address
    ldr     r0, [r0]		    // load stderr
    ldr     r1, =.Lwrmess	    // load string literal
    mov     r2, r10		    // get argv0
    bl 	    fprintf	  	    // call printf
    b 	    .Lfail

.Lread:
    ldr     r0, =stderr		    // get stderr address
    ldr     r0, [r0]		    // load stderr
    ldr     r1, =.Lrdmess	    // load string literal
    mov     r2, r10		    // get argv0
    bl 	    fprintf		    // call printf
    

    //
    // end of error handling code
    //
.Lfail:
    ldr     r0, [fp, -FPBOOK]	    // get fpbook address
    bl 	    fclose		    // close fpbook
    mov     r0, EXIT_FAILURE	    // return exit failure
    b 	    .Lend

.Ldone:
    ldr     r0, [fp, -FPBOOK]	    // get fpbook address
    bl      fclose		    // close fpbook
    mov     r0, EXIT_SUCCESS	    // return exit success

.Lend:
    sub     sp, fp, FP_OFF	   
    pop     {r4-r10, fp, lr}
    bx      lr


    .size   main, (. - main)
    .section .note.GNU-stack,"",%progbits
.end
#endif
