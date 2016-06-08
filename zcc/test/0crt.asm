;	Run time start off for Small C.
	.module	_crt
	.globl	main
;	This will be at location 0 in the final image
.reset:
	jp	.start
	nop
	nop
	nop
	nop
	nop
.rst1:	.ds	8
.rst2:	.ds	8
.rst3:	.ds	8
.rst4:	.ds	8
.rst5:	.ds	8
.rst6:	.ds	8
.rst7:	.ds	8
;
;	NMI vector
;
	.=.+0x26
.nmi:	ret
	.=.+0x19
;
;	This should be at 0x80
;
.start:
	ld	sp,#.stack; pick up core top
	im	1
	di		;disable interrupts
;
;	call main with argc = 1 and argv = [ "main", 0 ]
;
	ld	hl,#1
	push	hl
	ld	hl,#.argv
	push	hl
	ld	a,#2
	call	main	;call main program
	pop	bc
	pop	bc
	halt
;
;*****************************************************
;						     *
;	runtime	library	for small C compiler	     *
;						     *
;	c.s - runtime routine for basic C code	     *
;						     *
;		Ron Cain			     *
;						     *
;*****************************************************
	.globl	.gchar,.gint,.pchar,.pint
	.globl	.sxt
	.globl	.or,.and,.xor
	.globl	.eq,.ne,.gt,.le,.ge,.lt,.uge,.ult,.ugt,.ule
	.globl	.asr,.asl
	.globl	.sub,.neg,.com,.lneg,.bool,.mul,.div
	.globl	.case,brkend
	.globl	etext
	.globl	edata
;
; fetch char from (HL) and sign extend into HL
.gchar:	ld	a,(hl)
.sxt:	ld	l,a
	rlca
	sbc	a,a
	ld	h,a
	ret
; fetch int from (HL)
.gint:	ld	a,(hl)
	inc	hl
	ld	h,(hl)
	ld	l,a
	ret
; store char from HL into (DE)
.pchar: ld	a,l
	ld	(de),a
	ret
; store int from HL into (DE)
.pint:	ld	a,l
	ld	(de),a
	inc	de
	ld	a,h
	ld	(de),a
	ret
; "or" HL and DE into HL
.or:	ld	a,l
	or	e
	ld	l,a
	ld	a,h
	or	d
	ld	h,a
	ret
; "xor" HL and DE into HL
.xor:	ld	a,l
	xor	e
	ld	l,a
	ld	a,h
	xor	d
	ld	h,a
	ret
; "and" HL and DE into HL
.and:	ld	a,l
	and	e
	ld	l,a
	ld	a,h
	and	d
	ld	h,a
	ret
;
;......logical operations: HL set to 0 (false) or 1 (true)
;
; DE == HL
.eq:	call	.cmp
	ret	z
	dec	hl
	ret
; DE != HL
.ne:	call	.cmp
	ret	nz
	dec	hl
	ret
; DE > HL [signed]
.gt:	ex	de,hl
	call	.cmp
	ret	c
	dec	hl
	ret
; DE <= HL [signed]
.le:	call	.cmp
	ret	z
	ret	c
	dec	hl
	ret
; DE >= HL [signed]
.ge:	call	.cmp
	ret	nc
	dec	hl
	ret
; DE < HL [signed]
.lt:	call	.cmp
	ret	c
	dec	hl
	ret
; DE >= HL [unsigned]
.uge:	call	.ucmp
	ret	nc
	dec	hl
	ret
; DE < HL [unsigned]
.ult:	call	.ucmp
	ret	c
	dec	hl
	ret
; DE > HL [unsigned]
.ugt:	ex	de,hl
	call	.ucmp
	ret	c
	dec	hl
	ret
; DE <= HL [unsigned]
.ule:	call	.ucmp
	ret	z
	ret	c
	dec	hl
	ret
; signed compare of DE and HL
;   carry is sign of difference [set => DE < HL]
;   zero is zero/non-zero
.cmp:	ld	a,e
	sub	l
	ld	e,a
	ld	a,d
	sbc	a,h
	ld	hl,#1		;preset true
	jp	m,.cmp1
	or	e		;resets carry
	ret
.cmp1:	or	e
	scf
	ret
; unsigned compare of DE and HL
;   carry is sign of difference [set => DE < HL]
;   zero is zero/non-zero
.ucmp:	ld	a,d
	cp	h
	jr	nz,.ucmp1
	ld	a,e
	cp	l
.ucmp1:	ld	hl,#1		;preset true
	ret
; shift DE right arithmetically by HL, move to HL
.asr:	ex	de,hl
.asr1:	dec	e
	ret	m
	ld	a,h
	rla
	ld	a,h
	rra
	ld	h,a
	ld	a,l
	rra
	ld	l,a
	jr	.asr1
; shift DE left arithmetically by HL, move to HL
.asl:	ex	de,hl
.asl1:	dec	e
	ret	m
	add	hl,hl
	jr	.asl1
; HL = DE - HL
.sub:	ld	a,e
	sub	l
	ld	l,a
	ld	a,d
	sbc	a,h
	ld	h,a
	ret
; HL = -HL
.neg:	call	.com
	inc	hl
	ret
; HL = ~HL
.com:	ld	a,h
	cpl
	ld	h,a
	ld	a,l
	cpl
	ld	l,a
	ret
; HL = !HL
.lneg:	ld	a,h
	or	l
	jr	z,.lneg1
	ld	hl,#0
	ret
.lneg1:	inc	hl
	ret
; HL = !!HL
.bool:	call	.lneg
	jr	.lneg
;
; HL = DE * HL [signed]
.mul:	ld	b,h
	ld	c,l
	ld	hl,#0
.mul1:	ld	a,c
	rrca
	jr	nc,.mul2
	add	hl,de
.mul2:	xor	a
	ld	a,b
	rra
	ld	b,a
	ld	a,c
	rra
	ld	c,a
	or	b
	ret	z
	xor	a
	ld	a,e
	rla
	ld	e,a
	ld	a,d
	rla
	ld	d,a
	or	e
	ret	z
	jr	.mul1
; HL = DE / HL, DE = DE % HL
.div:	ld	b,h
	ld	c,l
	ld	a,d
	xor	b
	push	af
	ld	a,d
	or	a
	call	m,.deneg
	ld	a,b
	or	a
	call	m,.bcneg
	ld	a,#16
	push	af
	ex	de,hl
	ld	de,#0
.div1:	add	hl,hl
	call	.rdel
	jr	z,.div2
	call	.cmpbd
	jp	m,.div2
	ld	a,l
	or	#1
	ld	l,a
	ld	a,e
	sub	c
	ld	e,a
	ld	a,d
	sbc	a,b
	ld	d,a
.div2:	pop	af
	dec	a
	jr	z,.div3
	push	af
	jr	.div1
.div3:	pop	af
	ret	p
	call	.deneg
	ex	de,hl
	call	.deneg
	ex	de,hl
	ret
; {DE = -DE}
.deneg: ld	a,d
	cpl
	ld	d,a
	ld	a,e
	cpl
	ld	e,a
	inc	de
	ret
; {BC = -BC}
.bcneg: ld	a,b
	cpl
	ld	b,a
	ld	a,c
	cpl
	ld	c,a
	inc	bc
	ret
; {DE <r<r 1}
.rdel:	ld	a,e
	rla
	ld	e,a
	ld	a,d
	rla
	ld	d,a
	or	e
	ret
; {BC : DE}
.cmpbd: ld	a,e
	sub	c
	ld	a,d
	sbc	a,b
	ret
; case jump
.case:	ex	de,hl			;switch value to DE
	pop	hl		;get table address
.case1:	call	.case4		;get case value
	ld	a,e
	cp	c		;equal to switch value .
	jr	nz,.case2	;no
	ld	a,d
	cp	b		;equal to switch value .
	jr	nz,.case2	;no
	call	.case4		;get case label
	jr	z,.case3	;end of table, go to default
	push	bc
	ret			;case jump
.case2:	call	.case4		;get case label
	jr	nz,.case1	;next case
.case3:	dec	hl
	dec	hl
	dec	hl
	ld	d,(hl)
	dec	hl
	ld	e,(hl)
	ex	de,hl
	jp	(hl)		;default jump
.case4:	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	inc	hl
	ld	a,c
	or	b
	ret
	.area	_DATA
etext:
brkend:	.dw	edata		;current "break"
.main:	.asciz	'main'
.argv:	.dw	.main
	.dw	0
	.area	_BSS
edata:
	.area	_STACK		;actually top of stack
.stack:
