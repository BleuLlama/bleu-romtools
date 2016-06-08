;
; ramsnoop patch
;
; $Id: ramsnoop.asm,v 1.9 2004/07/28 02:56:49 jerry Exp $ 

; v 1.1  initial release, with bad stack snoop
; v 1.2  bugfix with sustain-between-resets (unreleased)
; v 1.3  bugfix with sustain-between-resets, fewer bugs
; v 1.4  bugfix for RST 8 ram clear.  oops.

.area 	.CODE (ABS)

; boot1  0000 - 0fff
; boot2  1000 - 1fff
; boot3  2000 - 2fff
; boot4  3000 - 3fff
; boot5  8000 - 8fff
; boot6  9000 - 9fff

	    ; some hardware defines
	watchdog = 0x50c0	; watchdog timer
	in0	 = 0x5000	; input 1: p1 inputs
	in1	 = 0x5040	; input 2: coin inputs
	vidram	 = 0x4000	; video ram base
	colram	 = 0x4400	; color ram base
	letter	 = 0x4a
	offs1	 = 0x20
	offs2	 = 0x40

        p1_port         = in0
        p1_up           = 0
        p1_left         = 1
        p1_right        = 2
        p1_down         = 3

	start_port	= in1
	start1		= 5
	start2		= 6



	; change the title text
.org	0x3d3e
	; .ascii	"YOYODYNE@PROP%@"
	.ascii  "UMLAUTLLAMA;COM"


.org	0x3808
	.ascii  "MISS@SNOOP"

.org	0x3db8
	.ascii  "@@V1%3@@@"


	; helpful defines
	stack	 = 0x4fc0	; starting stack point
	snaddr	 = stack-2	; base addr of the snoop point
	snjoy	 = stack-4	; previous joystick/button
	snoopst	 = 0x4e02	; starting snoop address

.org	0x234e
	ld	sp, #(stack-4)	; adjust the stack pointer

.org	0x30bd
	ld	sp, #(stack-4)

.org	0x3185
	ld	sp, #(stack-4)

.org	0x327c
	ld	sp, #(stack-4)


	; HACK2 : fast mod
.org	0x180b
	.db	01


; ram patch 0  (ram patch 1 is below...)
.org	0x2359
	jp	.rcpat0
        .byte   0x00, 0x00, 0x00, 0x00
        .byte   0x00, 0x00, 0x00, 0x00 
ret0:

	    ;; HACK4mod - boot patch to skip the self test
		; also initialize the snoop val and check for old val
.org	0x3000
	ld	sp, #(stack-4)
	pop	de		; flags
	pop	bc		; snaddr
	ld	a, b
	and	#0x40		; make sure addy on there is 0x4???
	cp	#0x40

	ld	sp, #(stack-4)
	jp	z, .rcpat1

	    ; store the initial values on the stack
	ld	sp, #(stack)
	ld	bc, #(snoopst)	; starting value for snooping
	push	bc		; used for the ram address
	ld	bc, #0x0000	; our flags
	push 	bc		; used for our flags

	jp	.rcpat1		; skip over to the boot area

; patch the first ram clear routine to save our two variables
.rcpat0:
	ld	de, (stack-2)
	ld	ix, (stack-4)

	    ; original code...
	ld	hl, #0x4c00
	ld	b, #0xbe
	rst	#8
	rst	#8
	rst	#8
	;rst	#8

	ld	(stack-4), ix
	ld	(stack-2), de

	jp	ret0

; patch the second ram clear routine to save our two variables
	overboot = 0x30d1	; jump position to skip the boot sequence
.rcpat1:
	ld	de, (stack-2)
	ld	ix, (stack-4)

	    ; old code, shifted
	ld	hl, #0x4c00
	ld	b, #0x04
.rc1a:
	ld	(#0x50c0), a		; kick the dog
.rc1b:
	ld	(hl), #0x00
	inc	l
	jr	nz, .rc1b
	inc	h
	djnz	.rc1a

	    ; restore values
	ld	(stack-4), ix
	ld	(stack-2), de

	jp	overboot


	    ;; HACK4 - checksum patch
.org	0x30f0
	nop
	nop
	nop


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	    ; the actual hack
.org	0x01aa
	jp	patch		; intercept the main loop into our patch
	; formerly   cdfd02

	lastcall = 0x02fd	; the end of the old game loop calls this
	jmpret	 = 0x01ad	; where to jump back (kiss myself)


.org	0x1fd0
patch:
	call	#(lastcall)	; last call from the patched loop

	    ; now our hack.
	    ; store registers
	push	hl		; no reason to save AF, it gets used on return
	push 	bc
	push	de
	push	ix

	    ; dereference the address
	    ; snaddr is the ram location containing the address to look at
	ld	hl, #(snaddr)	; hl is the word containing the address to peek
	ld	bc, #0x03	; perfect for the address
	call	drfdisp		; dereference display
	jp	parttwo

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; display a word, dereferenced from HL
.org	0x0f40
drfdisp:
	; hl: address of pointer
	; bc: offset
	push	bc
	pop	ix
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ld	hl, #0
	add	hl, bc

drf2disp:
	push	hl		; this is the actual address we need to look at
	push	hl
	push	hl
	pop	de
	push	ix
	pop	bc
	ld	b, #0

	ld	d, e
	ld	e, #0x05	; good color
	call	prtbyte
	pop	de

	ld	e, #0x05
	push	ix
	pop	bc
	inc	c
	inc	c
	call	prtbyte
	pop	hl
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; continue the code from above
parttwo:
		; display the value
	ld	bc, #0x24	; perfect for the value
	ld	d, (hl)
	ld	e, #0x0f	; color
	call	prtbyte

	xor	a
	ld	(watchdog), a	; kick the dog (just in case...)

	    ; check for p1(peek) and p2(poke)
	ld	ix, #(start_port)
	bit	#start1, (ix)
	jp	z, peekadj	; p1 pressed
	bit	#start2, (ix)
	jp	nz, restore	; p2 not pressed
	
poke:
	ld	ix, #(in0)
	bit	#p1_up, (ix)
	jp	nz, pokedn
	inc	(hl)		; adjust peeked value
	jp	delay

pokedn:
	bit	#p1_down, (ix)
	jp	nz, restore
	dec	(hl)		; adjust peeked value
	jp	delay


peekadj:
	    ; check user input
	ld	ix, #(in0)
	bit	#p1_up, (ix)
	jp	nz, dnchk
	    ; up addr
	ld	iy, #(snaddr)
	inc	1(iy)
	jp	delay

dnchk:
	bit	#p1_down, (ix)
	jp	nz, lchk
	ld	iy, #(snaddr)
	dec	1(iy)
	jp	delay

lchk:
	bit	#p1_left, (ix)
	jp	nz, rchk
		
	ld	iy, #(snaddr)
	dec	0(iy)
	jp	delay

rchk:
	bit	#p1_right, (ix)
	jp 	nz, restore
	ld	iy, #(snaddr)
	inc	0(iy)
	; fall through to the delay

	; try to slow down the game to make user input easier to deal with
	; (standard busyloop)
delay:
	ld	b, #0x12	; number of ticks to wait	
d0:
	push	bc
	ld	b, #0xff	; inner loop is $ff ticks.
d1:
	xor	a		; purposely put this in here to aid in slowdown
	ld	(watchdog), a	; kick the dog (just in case...)
	djnz	d1
	pop	bc
	djnz	d0

	    ; restore regs
restore:
	pop	ix
	pop	de
	pop	bc
	pop 	hl
	    ; resume...
	jp	jmpret


.org 0x2fc0
	; prtbyte: print out a byte to the screen
	;	assumes characters 0x00-0x0f are '0'-'F'
	; d:  byte to draw
	; e:  color
	; bc: offset into video/color buffer
prtbyte:
	    ; first, output the byte
	push	hl
	push	af
	push	de
	ld	hl, #(vidram)
	add	hl, bc
	ld	a,d
	and	#0x0f
	ld	(hl), a
	inc	hl
	ld	a, d
	rra
	rra
	rra
	rra
	and	#0x0f
	ld	(hl), a
	    ; now the color
	ld	hl, #(colram)
	add	hl, bc
	ld	a, e
	ld	(hl), a
	inc	hl
	ld	(hl), a
	pop	de
	pop	af
	pop	hl
	ret
