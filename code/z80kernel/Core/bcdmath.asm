;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	bcdmath.asm
;
;	bcd math functions (16 bit math)
;
;	2004-01,02  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module bcdmath

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; increment BC with a BCD method
incBCDbc:
	push	af
	xor	a			; clear flags
	ld	a, c
	inc	a			; a++
	daa				; bcd adjust
	ld	c, a			; store it
	jr	nc, .incBCD0		; Carry?
	    ; Carry!
	xor	a			; clear flags
	ld	a, b
	inc	a			; a++
	daa				; bcd adjust
	ld	b, a			; store it
.incBCD0:
	pop	af
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;
	;	in	hl	pointer to word
	; 	in	de	addition
addToScore:
	;ld	bc, (hl)
	ld	a, c
	add	e
	daa
	jr	nc, .ats00

.ats00:
    	ret
