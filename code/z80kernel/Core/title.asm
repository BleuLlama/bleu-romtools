;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;	title.asm
;
;	Title Screen - influenced heavily by Sonic 2 level screens
;
; 	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module titlescreen

.TITLE_START:

	LLAMACOL = 0x29

; to draw:
draw_title:
        call    cls                     ; fill screen black, 0

	    ; draw horizontal stripe
	ld	hl, #(.thoriz)
	ld	bc, #0x0714
	ld	a, #LLAMACOL
	call	putstrB
	ld	c, #0x15
	call	putstrB

	    ; llama to the left
	ld	bc, #0x0014
	call	putstrB
	ld	c, #0x15
	call	putstrB

	    ; draw vertical stripe
        ld      hl, #(colram + 0x0040 + 0x02a0)
        ld      de, #(titlecol)
        call    rledraw                 ; render the image

        ld      hl, #(vidram + 0x0040 + 0x02a0)
        ld      de, #(titlescr)
        call    rledraw                 ; render the image

	    ; draw the title text
	call	tTitleToHL		; fill HL with the pascal string pointer
	ld	bc, #0x0917
	ld	a, #0x08		; yellow
	call	putstrB

	    ; draw the descriptions
	call	tDesc1ToHL		; fill HL with line 1 of the description
	ld	bc, #0x071c
	ld	a, #0x05		; dk green
	call	putstrB

	call	tDesc2ToHL		; fill HL with line 1 of the description
	inc	c
	call	putstrB

	    ; draw control arrows
	ld	a, #NTASKS
	cp	#2
	ret	z			; don't draw if we just have one game
	;ld	a, #9			; blue/brown
	ld	a, #5			; yellow/green
	ld	hl, #(.tleft)
	ld	bc, #0x0716
	call	putstrB			; <-
	ld	hl, #(.tright)
	ld	b, #0x1b
	call	putstrB			; ->
	ret

draw_variant:
	call	tNVarsToA
	cp	#0x00
	ret	z			; no variants, no arrows

	ld	a, #9
	ld	bc, #0x0819
	ld	hl, #(.tup)		; ^
	call	putstrB
	ld	b, #0x1a
	ld	hl, #(.tdown)		; v
	call	putstrB

	call	tVarToHL		; variant name
	ld	bc, #0x0919
	ld	a, #C_orange		; orange
	call	putstrB
	ret

.tup:
	.byte 	1, 0x0a
.tdown:
	.byte 	1, 0x0b
.tleft:
	.byte 	1, 0x0c
.tright:
	.byte 	1, 0x0d

.thoriz:
	.byte 	21
	.byte	95, 95, 95, 95, 95, 95, 95, 95, 95, 95
	.byte	95, 95, 95, 95, 95, 95, 95, 95, 95, 95
	.byte	95

titlescr:	; Draw the llama on a checkered stripe
	.byte	0x0f, 95, 0x05, 95,   124, 126, 0x0a, 95
	.byte	0x0f, 95, 0x05, 95,   123, 125, 0x0a, 95
	.byte 	0x02

titlecol:
	.byte 	0x0f, LLAMACOL, 0x0f, LLAMACOL, LLAMACOL, LLAMACOL ; DUCK
	.byte 	0x0f, LLAMACOL, 0x0f, LLAMACOL, LLAMACOL, LLAMACOL ; DUCK
	.byte	0x02


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; draw additional info to the screen
draw_taux:
		; reddish - instruction
	call	tCredsToA		; check for 0 credit game
	ld 	hl, #(.ttpress)		; "Press start"
	cp	#0x00
	jr	z, .dta000

	ld	a, (credits)		; check for number of credits inserted
	cp	#0x00
	jr 	nz, .dta000
	ld 	hl, #(.ttcoin)		; "Insert Coin"

.dta000:
	ld	bc, #0x0c1f
	ld	a, #0x0c		; red
	call	putstrB			; print it


		; pink - credits
	ld	a, (credits)		; number of credits
	cp	#0xff			; 0xff is freeplay flag
	jr	z, .dtafree

	ld	hl, #(.ttcred)		; "Credits"
	ld	bc, #0x0103
	ld	a, #0x0b		; pink
	call	putstrA			; print it

	    ; credit count
	ld	hl, #credits		; number of credits
	ld	a, (credits)
	ld	d, a
        ld      e, #0x0b		; pink
        ld      bc, #0x0033
        call    prtBCDbyte 		; print it

	ret

.dtafree:
	ld	hl, #(.ttfree)		; "FREE PLAY"
	ld	bc, #0x0103
	ld	a, #0x0b
	call	putstrA			; print it
	ret

.ttpress:
	.byte 	11
	.ascii	"Press start"

.ttcoin:
	.byte	11
	.ascii	"Insert coin"

.ttfree:
	.byte	9
	.ascii	"FREE PLAY"

.ttcred:
	.byte	7
	.ascii	"Credits"

