;
; level 255 fix for pac-man (prevents the lock screen)
;
; $Id: l255fix.asm,v 1.1 2004/02/18 21:53:04 jerry Exp $ 

.area 	.CODE (ABS)

	; HACK2 : fast mod
.org	0x180b
	.db	01

	    ;; HACK4mod - boot patch to skip the self test
.org	0x3000
	overboot = 0x30c1	; jump position to skip the boot sequence
	stack = 0x4fc0
	ld	sp, #(stack)
	jp	overboot	; skip over to the boot area

	    ;; HACK4
.org	0x30f0
	nop
	nop
	nop

	    ; the hack hook
.org	0x0a90
	jp	hack
	nop

	    ; the actual hack
.org	0x0f80
	backback = 0x0a94
	ramspot  = 0x4e13
hack:
	ld	a, (ramspot)
	inc	a
	cp	#0xff		; see if we're over 254
	jr	z, back		; nope, jump back
	ld	(ramspot), a	; store value
back:
	jp	backback	; jump back
