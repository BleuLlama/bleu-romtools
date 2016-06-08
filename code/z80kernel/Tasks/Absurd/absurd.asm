;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Absurd
;;
;;   game engine
;;
;;   (c) 2006 Scott Lawrence
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; The module scanner block
absurdTask:
	.word	ab_title	; title text
	.byte	0x00		; version major
	.byte	0x06		; version minor
	.word	ab_desc1	; description line 1
	.word	ab_desc2	; description line 2
	.word	ab_att0		; attract sequence init
	.word	ab_att		; attract sequence poll
	.word	ab_game0	; game init
	.word	ab_game		; game poll
	.byte	1		; 1 credit per play
	.word	ab_variants	; variant list

	; variants...
ab_variants:
	.byte	3
	.word	ab_Voriginal
	.word	ab_Vhard
	.word	ab_Vspeedround

ab_Voriginal:
	.byte	17
	.ascii	"     Original    "
ab_Vhard:
	.byte	17
	.ascii	"      Hard       "
ab_Vspeedround:
	.byte	17
	.ascii	"   Speed Round   "

	; title text
ab_title:
	.byte	17
	;;;;;   "                 "
	.ascii	"     Absurd!     "

ab_desc1:
	.byte	21
	;;;;;   "                     "
	.ascii  " Save animals before "
ab_desc2:
	.byte	21
	;;;;;   "                     "
	.ascii  "   the Moon rises.   "

absTask2:
	.word	ab_t2
	.word	ab_att0		; attract sequence init
	.word	ab_att		; attract sequence poll
	.word	ab_game0	; game init
	.word	ab_game		; game poll

absTask3:
	.word	ab_t3
	.word	ab_att0		; attract sequence init
	.word	ab_att		; attract sequence poll
	.word	ab_game0	; game init
	.word	ab_game		; game poll

ab_t2:
	.byte	3
	.ascii	"Two"
ab_t3:
	.byte	5
	.ascii	"Three"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; attract sequence

ab_att0:
	    ; backfill blue
	call	clrsprite
	call	ab_attWater

	    ; draw the graphic in the center of the region
	ld	hl, #(vidram+0x0040+2+(0x0020*2))
	ld	de, #(.i0gfx)
	call	rledraw		; draw it

	ld	hl, #(colram+0x0040+2+(0x0020*2))
	ld	de, #(.i0col)
	call	rledraw		; draw it

ab_att:
	ld	hl, #ab_aa0	; gator
	ld	bc, #0x50e0	; position
	ld	de, #(sprite0)	; slot 0
	call	dsprite		; draw the sprite

	ld	hl, #ab_aa1	; monkey
	ld	bc, #0x80a0
	ld	de, #(sprite1)
	call	dsprite

	ld	hl, #ab_aa2	; crystal
	ld	bc, #0x40a8
	ld	de, #(sprite2)
	call 	dsprite

	ld	hl, #ab_aa3	; cursor
	ld	bc, #0x5080
	ld	de, #(sprite3)
	call	dsprite

	ld	hl, #ab_aa4	; dragon
	ld	bc, #0xa0f0
	ld	de, #(sprite4)
	call	dsprite

	ret

ab_aa0: .byte	5, 22*4, 23*4, 22*4|valXFlip, 23*4|valXFlip	; gator
ab_aa1:	.byte	4, 57*4, 58*4, 57*4, 58*4	; monkey
ab_aa2:	.byte	0x16, 4*4|valXFlip, 4*4|valXFlip|valYFlip, 4*4|valYFlip, 4*4		; crystal
ab_aa3:	.byte	6, 0, 0, 0, 0			; cursor
ab_aa4:	.byte	0x0d, 33*4, 33*4, 32*4, 32*4	; DRAGON! 
; valYFlip valXFlip	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; attract graphics

	; backfill the water area completely (so we can do bigger than 16x16)
	; - this is set up awkwardly because if we just drop down a huge
	;   blue region, when the RLE decoder plops the graphic over it,
	;   we get a color/graphics flicker-glitch.  This is more complex, 
	;   but looks much nicer.
ab_attWater:
	    ; left strip
	ld      a, #0x12		; color
	ld      hl, #(.ABALEFT)		; left graphic
	ld	e, #0x07		; x position
	call	.abAttVert
	    ; right strip
	ld	hl, #(.ABARIGHT)	; right graphic
	ld	e, #0x17		; x position
	call	.abAttVert

	ld	hl, #(.ABATOP)		; top/bottom graphic
	ld	bc, #0x0700		; top 0
	call	putstrB
	inc	c			; top 1
	call	putstrB
	ld	c, #0x12		; bottom 0
	call	putstrB
	inc	c			; bottom 1
	call	putstrB
	ret

	; a subroutine to draw the vertical stripe on the left and right
.abAttVert:
	ld      c, #2			; y pos
	ld      b, #0x10		; y count
.abav000:
	push    bc
	ld      b, e
	call    putstrB
	pop     bc 
	inc     c
	djnz    .abav000 
	ret

        .SB      = 161


.ABALEFT:			; left stripe element
        .byte   0x03
        .byte   .SB,.SB,.SB

.ABARIGHT:			; right stripe element
	.byte	0x05
        .byte   .SB, .SB, .SB, .SB, .SB

.ABATOP:			; top/bottom stripe element
	.byte	0x15
	.byte	.SB, .SB, .SB, .SB,  .SB, .SB, .SB, .SB
	.byte	.SB, .SB, .SB, .SB,  .SB, .SB, .SB, .SB
        .byte   .SB, .SB, .SB, .SB,  .SB


	; attract sequence island (16x16)
.i0gfx:
	.byte   15, IJ, IJ, 1						; 0
	.byte	8, IJ, IC, 3, IF, II, 3, IJ, 1				; 1
	.byte	IJ, IJ, IC, IF, II, IJ, IJ, IC, 4, IE, IH, 3, IJ, 1	; 2
	.byte 	IJ, IC, 3, IE, IF, IF, 5, IE, IH, 3, IJ, 1		; 3

	.byte	IJ, IB, 10, IE, IH, 3, IJ, 1				; 4
	.byte	IJ, IB, 10, IE, IG, 3, IJ, 1				; 5
	.byte	IJ, IB, 9, IE, IG, 4, IJ, 1				; 6
	.byte	IJ, IB, 8, IE, IH, 5, IJ, 1				; 7

	.byte	IJ, IB, 8, IE, IH, 5, IJ, 1				; 8
	.byte	IJ, IA, ID, ID, 7, IE, II, ST, SM, SR, IJ, 1		; 9
	.byte	4, IJ, IA, 7, IE, IF, IF, II, IJ, 1			; 10
	.byte	5, IJ, IA, 8, IE, IH, IJ, 1				; 11

	.byte	6, IJ, IA, ID, ID, 5, IE, IG, IJ, 1			; 12
	.byte	9, IJ, IA, 3, ID, IG, IJ, IJ, 1				; 13
	.byte	15, IJ, IJ, 1						; 14
	.byte	15, IJ, IJ, 1						; 15

	.byte	0x02

.i0col:				; color map for the attract island
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	15, 0x1E, 0x1E, 1
	.byte	2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Map stuff

	RIPPLE1 == 192
	RIPPLE2 == 193
	RIPPLE3 == 194
	RIPPLE4 == 195

; ab_waves
;	draw out some random waves
ab_waves:
	ld	hl, #(vidram + 0x40)
	ld	de, #0x20
	ld	b, #28
.abwlp:
	push	bc

	call	.abwlpr		; ripple a random spot on this column
	call	.abwlpr		; ripple a random spot on this column

	add	hl, de
	pop	bc
	djnz	.abwlp
	ret

.abwlpr:
	call	rand
	and	#0x03
	add	a, #RIPPLE1
	ld	b, a
	call	ab_wave
	ret


; ab_wave
;	in the passed in address, pick a random point for the water ripple
;	hl - screen ram
;	b - character to inject
ab_wave:
	push	hl
	push 	de

	xor	a
	ld	d, a
	call	rand
	and	#0x1f
	ld 	e, a

	add	hl, de
	
	ld	a, (hl)
	cp	a, #WATER
	jr	nz, .abwr0001
	
	ld	(hl), b
.abwr0001:

	pop	de
	pop	hl
	ret
	

ab_newmap:
	call	cls

	;; top half map
	ld	hl, #(vidram + 0x0040)
	ld	de, #(ab_mapT001)
	call	rledraw

	;; bottom half map
	ld	hl, #(vidram + 0x0040 + 16)
	ld	de, #(ab_mapT002)
	call	rledraw

	; generate waves
	call 	ab_waves

	;; top half color
	ld	hl, #(colram + 0x0040)
	ld	de, #(ab_mapT001c)
	call	rledraw
	;; bottom half color
	ld	hl, #(colram + 0x0040 + 16)
	ld	de, #(ab_mapT002c)
	call	rledraw
	ret
	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Game Play

ABRAM	== USERRAM
	AB_JSX	== ABRAM + 0
	AB_JSY	== ABRAM + 1
	AB_MOON	== ABRAM + 2


	; initialize the game
ab_game0:
	call 	ab_newmap
	xor	a
	ld	b, #6
	ld	hl, #(USERRAM)
	call	memset256

	ld	hl, #AB_JSX
	ld	a, #0x80
	ld	(hl), a
	inc	hl
	ld	(hl), a

	; run the game
ab_game:
	call 	ab_jstst
	call	ab_jsxbound
	call	ab_jsybound
	call	ab_curs
	call	ab_moon
	ret



JSX_LOW	== 0x1B
JSX_HI	== 0xFF-0x13
JSY_LOW == 0x0D
JSY_HI  == 0xFF-0x01

; ab_jsxbound
;	test joystick position bounds for x
ab_jsxbound:
	ld	hl, #(AB_JSX)
	ld	a, (hl)
	cp	#JSX_LOW	; low X bound 
	jr	nc, .abjsb0
	ld	a, #JSX_LOW
	ld	(hl), a
	ret
.abjsb0:
	cp	#JSX_HI
	ret	c
	ld	a, #JSX_HI
	ld	(hl), a
	ret

; ab_jsybound
;	test joystick position bounds for y
ab_jsybound:
	ld	hl, #(AB_JSY)
	ld	a, (hl)
	cp	#JSY_LOW	; low Y bound 
	jr	nc, .abjsb1
	ld	a, #JSY_LOW
	ld	(hl), a
	ret
.abjsb1:
	cp	#JSY_HI
	ret	c
	ld	a, #JSY_HI
	ld	(hl), a
	ret


; ab_jstst
;	test user input on the joystick
ab_jstst:
	call	Is1Up
	cp	#0x00
	jr	z, .abjtd
	ld	hl, #(AB_JSY)
	inc	(hl)

.abjtd:
	call	Is1Down
	cp	#0x00
	jr	z, .abjtl
	ld	hl, #(AB_JSY)
	dec	(hl)

.abjtl:
	call	Is1Left
	cp	#0x00
	jr	z, .abjtr
	ld	hl, #(AB_JSX)
	inc	(hl)

.abjtr:
	call	Is1Right
	cp	#0x00
	ret	z
	ld	hl, #(AB_JSX)
	dec	(hl)
	ret


	; sprite utilisation:

	AB_S_CURSOR	== sprite0
	AB_S_MOON	== sprite1


; ab_curs
;	draw the amiga-style cursor
ab_curs:
	ld	de, #(AB_S_CURSOR)
	ld	hl, #(AB_JSX)
	ld	b, (hl)
	ld	hl, #(AB_JSY)
	ld	c, (hl)
	ld	hl, #0x0006
	call	sprite
	ret

	; cursor outline
	ld	hl, #0x0805
	ld	de, #(sprite1)
	call	sprite

	ret

; ab_moon
;	draw the moon 
ab_moon:
	ld	a, (timer)
	sra	a
	sra	a
	sra	a
	sra	a
	and	#0x0f
	push	af
	ld	a, (timer+1)
	sla	a
	sla	a
	sla	a
	sla	a
	and	#0xf0
	pop	bc
	or	b
	ld	hl, #(AB_MOON)
	ld	(hl), a

	ld	hl, #(AB_MOON)
	ld	a, (hl)
	sra	a
	sra	a
	sra	a
	sra	a
	and	a, #0x0f	; reduce the skew to sprite width
	add	a, #0x0f	; make it slide in from the right
	ld	b, a		; x position

	ld	hl, #(AB_MOON)
	ld	c, (hl)		; y position
	ld	de, #(AB_S_MOON)	; moon sprite slot
	ld	h, #(3<<2)	; sprite 3, no flips
	ld	l, #0x19	; color 0x19
	call	sprite		; draw it!
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Levels

	; these are arranged as such (simple hex island)
	;	IJ IJ IJ IJ IJ
	;	IJ IA IB IC IJ
	;	IJ ID IE IF IJ
	;	IJ IG IH IK IJ
	;	IJ IJ IJ IJ IJ
	
	IA 	= 164
	IB	= 165
	IC	= 166

	ID	= 196
	IE	= 197
	IF	= 198

	IG	= 228
	IH	= 229
	II	= 230

	IJ	= 163

		; ship graphic
	ST 	= 224
	SM 	= 225
	SR 	= 227


ABSURD_SZ	= .-absurdTask
