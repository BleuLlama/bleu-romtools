;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;	sprites.asm
;
;	Sprite manipulations and animations
;
; 	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module sprites.asm


	;ld	hl, arry
	;ld	de, #rs_a3		; anim array 3
	;rst	0x10			; hl contains array 3
	;ld	de, #(sprite3)		; sprite3
	;ld	bc, #0xe052		; left bottom
	;call	dsprite


;	dsprite
;	 draw a sprite to the screen
;	 b  = x position
;	  c = y position
;	 de = hardware sprite slot number
;	 hl = animation array pointer
;	      (color) (f0) (f1) (f2) (f3)   f-> sprite *4 | flags
dsprite:
	push	af
	xor 	a
	ld	(watchdog), a
	pop	af

	    ; set up x and y
	ld	iy, #(spritecoords)
	add	iy, de

        ld      spriteX(iy), b		; X
        ld      spriteY(iy), c		; Y


	    ; set up sprite color and frame
        ld      ix, #(sprtbase)
	add	ix, de

	    ; animation frame based on timer
	ld	bc, (timer)
	ld	a, c
	rra
	rra				; add in more of these for slower
	rra
	and	#0x03			; we just want the bottom 3 bits
	ld	b, #0
	ld	c, a 			; bc now contains the array index number


	ld	a, (hl)			; color
        ld      sprtColor(ix), a	; load in the color

	inc	hl			; hl points to beginning of array
	add	hl, bc			; bc now points to the proper element
	ld	a, (hl)			; a is the sprite
    ;; the next three opcodes removed so that we can flip sprites and such
;	rla
;	rla				; bottom two bits are 00 (no flips)	
;	and	#0xfc			; make sure bottom bits are cleared

        ld      sprtIndex(ix), a	; load the sprite number in
	ret

; sprite
;	draw a sprite at a specific location
;	 de = sprite slot number
;	 b  = input x position
;	  c = input y position
;	 h  = sprite number
;	  l = sprite color
sprite:
	ld	iy, #(spritecoords)
	add	iy, de
	ld	ix, #(sprtbase)
	add	ix, de

	ld	spriteX(iy), b
	ld	spriteY(iy), c

	ld	a, l
	ld	sprtColor(ix), a
	ld	a, h
	ld	sprtIndex(ix), a
	ret


; the following are useful for converting sprite to screen positioning


;	addrAtXY
;	 find the screen address at the pixel position x,y
;	 b  = input x position
;	  c = input y position
;	 hl = returned screen address offset
addrAtXY::
	; x
	ld	a, b
	rlca
	rlca
	rlca			; shift backwards cause it's quicker
	and 	#0x03
	ld	h, a

	ld	a, b
	rlca
	rlca
	rlca			; shift backwards cause it's quicker
	and	#0xe0
	ld	l, a		; hl now contains 0..0x3e0 for horizontal

	; y
	ld	a, c
	rrca
	rrca
	rrca			; shift backwards cause it's quicker
	and	#0x1f
	neg			; invert it   a *= -1
	add	a, #0x1f	; a now is 1f..00 for vertical

	; now mix them
	add	a, l
	ld	l, a		; should now be screen address offset
	ret

;	XYAtAddr
;	 find the pixel position for the screen address
;	 hl = the screen address offset
;	 b  = output x position
;	  c = output y position
XYAtAddr::
	ld	a, l
	and	#0x1f		; vertical is 0..1f, backwards
	rlca
	rlca
	rlca
	neg			; invert it   a *= -1
	ld	c, a		; vertical is now 0..256 rightways

	ld	a, h
	rrca
	rrca
	rrca			; backwards shift the top 3 bits in
	and	#0xE0
	ld	b, a		; store those 3 bits for now

	ld	a, l
	rrca
	rrca
	rrca			; shift 'em over, make room
	and	#0x1c		; mask out the dead bits

	add	a, b		; a is now the horizontal
	ld	b, a		; store it
	ret
