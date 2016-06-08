;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;	rle.asm
;
;	RLE compressed image rendering routines
;
;	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module rle

    ; RLE Screen Draw

; rle compression:
;   control:
;	0x01		end of line
;	0x02		end of image
;	0x03-0x0f	repeat next by this number of times
;   data:
;	0x00		data
;	0x10-0xff	data

	; hl loaded with start of screen addressing
	; de loaded with the pointer to the data

rledraw:
	ld	a, (de)
	cp	#0xff		; secial character, drop, go next byte
	jp	z, .rleff

	    ; check for 00: data
	cp	#0x00
	jp	z, .rleout
	    ; check for 01: next column skip
	cp	#0x01
	jp	z, .rleskip
	    ; check for 02: end-of-data
	cp	#0x02
	jp	z, .rledone
	    ; check for 03-0f: rle repeat of next byte
	cp	#0x10		; flags get result of (A - 0x10)
	jp	c, .rlerep	; if (a < 0x10) carry is set

	    ; 10-ff: data byte, fall through and draw it
.rleout:
	ld	(hl), a
	inc	de		; increment the pointers, continue
	inc	hl
	jp	rledraw

	; ff: draw the next byte once
.rleff:
	inc	de
	ld	a, (de)
	jp	.rleout

	; 01: skip to the next column (add 16 to dest)
.rleskip:	
	ld	bc, #0x0010
	add	hl, bc
	inc	de
	jp	rledraw


	; 02: end of processing
.rledone:
	ret
	

	; 03-0f: repeat next byte for N times
.rlerep:
	ld	b, a		; setup repeat counter
	inc	de		; de now points to data
	ld	a, (de)
.rlel0:
	ld	(hl), a
	inc	hl
	djnz	.rlel0		; repeat for B times

	inc	de		; increment the pointers, continue
	jp	rledraw


; For the next chunk of code, Pseud (Mark Spaeth) suggested that I switch
; this over from 7 compares with Z-based jumps over to a bit-test method.
; It's a clever idea, treating the 3 bits of the opcode as a tree-like
; structure, then traversing it to determine the correct routine to execute.
; His method would also elimiate storing aside the opcode byte, requiring
; that the individual operator blocks restore it.
;
; While this might save a couple of clock cycles, I'm going to stick with
; the block of code below for a few reasons:
; - This method is a bit easier to read and maintain
; - The below code is fast enough
; - There is no savings of bytes in rom for using his method
; - The number of t-states is actually more with his, on average
;
; His method would mean that you would always have 3 bit tests
; on the A register, then jr or jp to the correct block based on all of that
; My method has between 1 and 7 compares (subtracts) We'll average this to 4
; bit = 8 t-states
; cp =  4 t-states
; ld =  7 t-states
; and = 4 t-states
; jr = 12/7 t-states
; jp = 10t-states
;  my method:  2 lds, one and, 1-7 cp, 1 jr   : ~46 t-states
;  his method: 3 bit, 3 jr - two@7, one at 12 : ~50 t-states
;
; His could probably be worked down to save a couple of bytes here and there,
; but it's really not worth it for what we'd lose in readability.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	rledraw2
;		updated version of rledraw
;	
;	hl loaded with start of screen addressing
;	de loaded with the pointer to the data
;	ix loaded with 16 byte table of lookup data
;	bc - garbage 
.rle2NEXT:			; entry point for getting the next byte
	inc	de
rledraw2::			; this is the entry point
	;ld	(hl), a		; this is wrong, i think?
	ld	a, (de)		; get next opcode byte
	ld	b, a		; store it aside for later
	and	#0xe0		; mask off the opcode bits

	; handle the proper opcode
	; we want to save the most time on byteskips
	cp	#0x20	; 001x xxxx  skip this many bytes
	jr	z, rle2skip

	; next, we want to make up for the insane size of the LUT methods
	cp	#0x60	; 011x xxxx  rep count for LUT HL		12 12 12
	jr	z, rle2ulrep
	cp	#0x80	; 100x xxxx  copy via LUT this many HL		12 34 56
	jr	z, rle2ul
	cp	#0x40	; 010x xxxx  rep count for LUT L	        01 02 03
	jr	z, rle2lrep

	; next, these byte-based ones are left
	cp	#0xc0	; 110x xxxx  copy this many bytes		98 76 54
	jp	z, rle2bcopy
	cp	#0xa0	; 101x xxxx  rep count for next byte		99 99 99
	jp	z, rle2brep

	; and of course, the "end" marker
	cp	#0x00	; 000x xxxx  end of data
	;jr	z, rle2done

	; unknown opcodes will fall through, and cause it to end.
	;cp	#0xe0	; 1110 xxxx  unused for now
	
rle2done:
	ret



;	rle2LUT - look up the item in the table
;	in:
;	IX - lookup table of bytes
;	A  - offset byte in the table
;	out:
;	A - the value from the table
;	IY - garbage
rle2LUT:
	push	de
	ld	d, #0
	ld 	e, a		; de = 0x00aa
	push	ix
	pop	iy		; iy = the LUT
	add	iy, de		; iy = the index in the lut
	ld	a, (iy)		; a = the byte we care about
	pop	de
	ret



; the processing engines

	; when these below are called, 
	;	a contains the opcode byte & 0xopmask
	;	b contains the complete opcode byte
	; 	hl contains the current "screen" pointer
	;	de contains the pointer to the current opcode byte
	; 	ix contains the pointer to the LUT


rle2skip:		; 001x xxxx	skip this many bytes
	ld	a, b
	and	#0x1f
	ld	b, #0x00
	ld	c, a		; add bc onto destpos
	add	hl, bc
	jp	.rle2NEXT	; inc DE to the next input byte


rle2lrep:	; 010x xxxx  rep count for LUT L  	01->       01 01 01
	ld	a, b
	and	#0x1f
	ld	b, a		; b contains rep count
	inc	de
	ld	a, (de)		; a has data to repeat
	and	#0x0f		; get nibble 
	call	rle2LUT		; a = ix[a]
.rle2r3:
	ld	(hl), a		; fill the dest with the byte
	inc	hl		; bump next
	djnz 	.rle2r3
	jp	.rle2NEXT



rle2ulrep:	; 011x xxxx  rep count for LUT HL 	12 -> 01 02 01 02
	ld	a, b
	and	#0x1f
	ld	b, a		; b contains rep count
	inc	de
	ld	a, (de)		; a has data to repeat
	
	rrc	a
	rrc	a
	rrc	a
	rrc	a
	and	#0x0f
	ld	c, a		; c = (a>>4)&0x0f

.rle2r4:
	push	bc		; save BC

	ld	a, c		; a = c (stored above)
	call 	rle2LUT
	ld	(hl), a		; (hl) gets LUT[high]
	inc	hl
	ld	a, (de)		; restore A
	and	#0x0f		; low byte
	call	rle2LUT
	ld	(hl), a		; (hl) gets LUT[low]
	inc	hl

	pop	bc		; restore BC
	djnz	.rle2r4
	jp	.rle2NEXT



rle2ul:		; 100x xxxx  copy vai LUT this many HL		12 34 56
	ld	a, b		; *hl = IX[a]
	and	#0x1f
	ld	b, a		; b contains rep count

.rle2r5:
	push	bc

	inc	de		; go to next data BYTE
	ld	a, (de)		; load it in
	rrc	a
	rrc	a
	rrc	a
	rrc	a
	and	#0x0f		; a = ((de)>>4) & 0x0f
	call	rle2LUT
	ld	(hl), a		; *hl = IX[a]
	inc	hl		; hl++

	ld	a, (de)
	and	#0x0f		; a = (de)& 0x0f
	call	rle2LUT
	ld	(hl), a		; *hl = IX[a]
	inc	hl		; hl++

	pop	bc
	djnz	.rle2r5

	jp	.rle2NEXT



; rle2brep
;	get the next DE byte, store it directly in (HL - HL+COUNT)
rle2brep:	; 101x xxxx  rep count for next byte	        99 99 99

	ld	a, (de)
	and	#0x1f
	ld	b, a		; b contains rep count
	inc	de		; get data byte
	ld	a, (de)		; a has data byte
.r2bc1:
	ld	(hl), a		; store it in hl
	inc	hl		; increment HL position
	djnz	.r2bc1
	jp	.rle2NEXT	; inc DE to the next input byte



; rle2bcopy
;	get the next DE byte, store it directly in (HL) for COUNT times
rle2bcopy:	; 110x xxxx  copy this many bytes               98 76 54
	ld	a, (de)
	and	#0x1f
	ld	b, a		; b contains rep count
.r2bc0:
	inc	de		; get data byte
	ld	a, (de)		; a has data byte
	ld	(hl), a		; store it in hl
	inc	hl		; increment HL position
	djnz	.r2bc0

	jp	.rle2NEXT	; inc DE to the next input byte
