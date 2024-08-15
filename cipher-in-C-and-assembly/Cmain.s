	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 13, 1
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #384
	stp	x28, x27, [sp, #352]            ; 16-byte Folded Spill
	stp	x29, x30, [sp, #368]            ; 16-byte Folded Spill
	add	x29, sp, #368
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w27, -24
	.cfi_offset w28, -32
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	stur	x8, [x29, #-24]
	str	wzr, [sp, #84]
	str	w0, [sp, #80]
	str	x1, [sp, #72]
	ldr	x8, [sp, #72]
	ldr	x8, [x8]
	str	x8, [sp, #48]
	ldr	w0, [sp, #80]
	ldr	x1, [sp, #72]
	add	x2, sp, #56
	add	x3, sp, #64
	bl	_setup
	adds	w8, w0, #1
	b.ne	LBB0_2
	b	LBB0_1
LBB0_1:
	mov	w8, #1
	str	w8, [sp, #84]
	b	LBB0_10
LBB0_2:
	b	LBB0_3
LBB0_3:                                 ; =>This Inner Loop Header: Depth=1
	adrp	x8, ___stdinp@GOTPAGE
	ldr	x8, [x8, ___stdinp@GOTPAGEOFF]
	ldr	x0, [x8]
	ldr	x1, [sp, #64]
	mov	w2, #128
	sub	x3, x29, #152
	add	x4, sp, #88
	bl	_rdbuf
	str	w0, [sp, #44]
	cbz	w0, LBB0_9
	b	LBB0_4
LBB0_4:                                 ;   in Loop: Header=BB0_3 Depth=1
	ldr	w8, [sp, #44]
	adds	w8, w8, #1
	b.ne	LBB0_6
	b	LBB0_5
LBB0_5:
	adrp	x8, ___stderrp@GOTPAGE
	ldr	x8, [x8, ___stderrp@GOTPAGEOFF]
	ldr	x0, [x8]
	ldr	x8, [sp, #48]
	mov	x9, sp
	str	x8, [x9]
	adrp	x1, l_.str@PAGE
	add	x1, x1, l_.str@PAGEOFF
	bl	_fprintf
	mov	w8, #1
	str	w8, [sp, #84]
	b	LBB0_10
LBB0_6:                                 ;   in Loop: Header=BB0_3 Depth=1
	ldr	x8, [sp, #56]
	ldr	w2, [sp, #44]
	sub	x0, x29, #152
	str	x0, [sp, #24]                   ; 8-byte Folded Spill
	add	x1, sp, #88
	blr	x8
	mov	x8, x0
	ldr	x0, [sp, #24]                   ; 8-byte Folded Reload
	str	w8, [sp, #40]
	ldrsw	x2, [sp, #40]
	adrp	x8, ___stdoutp@GOTPAGE
	ldr	x8, [x8, ___stdoutp@GOTPAGEOFF]
	ldr	x3, [x8]
	mov	x1, #1
	bl	_fwrite
	mov	x8, x0
	str	w8, [sp, #36]
	ldr	w8, [sp, #36]
	ldr	w9, [sp, #40]
	subs	w8, w8, w9
	b.eq	LBB0_8
	b	LBB0_7
LBB0_7:
	adrp	x8, ___stderrp@GOTPAGE
	ldr	x8, [x8, ___stderrp@GOTPAGEOFF]
	ldr	x0, [x8]
	ldr	x8, [sp, #48]
	mov	x9, sp
	str	x8, [x9]
	adrp	x1, l_.str.1@PAGE
	add	x1, x1, l_.str.1@PAGEOFF
	bl	_fprintf
	ldr	x0, [sp, #64]
	bl	_fclose
	mov	w8, #1
	str	w8, [sp, #84]
	b	LBB0_10
LBB0_8:                                 ;   in Loop: Header=BB0_3 Depth=1
	b	LBB0_3
LBB0_9:
	ldr	x0, [sp, #64]
	bl	_fclose
	str	wzr, [sp, #84]
	b	LBB0_10
LBB0_10:
	ldr	w8, [sp, #84]
	str	w8, [sp, #20]                   ; 4-byte Folded Spill
	ldur	x9, [x29, #-24]
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	subs	x8, x8, x9
	b.eq	LBB0_12
	b	LBB0_11
LBB0_11:
	bl	___stack_chk_fail
LBB0_12:
	ldr	w0, [sp, #20]                   ; 4-byte Folded Reload
	ldp	x29, x30, [sp, #368]            ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #352]            ; 16-byte Folded Reload
	add	sp, sp, #384
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%s: read failed \n"

l_.str.1:                               ; @.str.1
	.asciz	"%s: write failed\n"

.subsections_via_symbols
