;	All Js to external targets or where there is no JR are taken as JPs
	jp	.x
	j	.x
	j	.f
	jp	nz,.x
	j	nz,.x
	j	nz,.f
	jp	z,.x
	j	z,.x
	j	z,.f
	jp	nc,.x
	j	nc,.x
	j	nc,.f
	jp	po,.x
	j	po,.x
	j	po,.f
	jp	p,.x
	j	p,.x
	j	p,.f
	jp	m,.x
	j	m,.x
	j	m,.f
;	Test the backward branches
.1:	jr	.1
.2:	j	.2
.3:	jr	nz,.3
.4:	j	nz,.4
.5:	jr	z,.5
.6:	j	z,.6
.7:	jr	nc,.7
.8:	j	nc,.8
.9:	jr	c,.9
.10:	j	c,.10
;	Test the forward branches
.11:	jr	.13
.12:	j	.14
.13:	jr	nz,.15
.14:	j	nz,.16
.15:	jr	z,.17
.16:	j	z,.18
.17:	jr	nc,.19
.18:	j	nc,.20
.19:	jr	c,.21
.20:	j	c,.22
;	This should be illegal
.21:	jr	po,.21
;	But the following should become JPs
.22:	j	po,.22
.23:	j	pe,.23
.24:	j	p,.24
.25:	j	m,.25
;
.f:	nop
;	Pathological case: this one should be too far for a JR
	j	.g
	.ds	0x7F
;	But this one should be ok
.g:	j	.h
	.ds	0x7E
;	And this one too
.h:	j	.i
	.ds	0x7D
.i:	j	.j
	.ds	0x75
.j:	nop
	.globl	.x
