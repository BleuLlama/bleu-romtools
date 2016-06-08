;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - quad blok game (tetris-style game, using Ms. Pac graphics.)
;
; Based on the gameplay of "Tetris" 1988 Alexy Pajitnov
;
;  2004-02  Scott "Jerry" Lawrence
;
;  $Id: game_quadblok.asm,v 1.54 2005/04/03 08:01:59 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module quadblok
;
; QuadBlok for Pac-Hardware version 1.0  2005-03-16
;  3 game modes
;	original	- standard tetris - play as long as you want
;	timed blok	- play as long as you want against the clock
;	tidy blok	- playfiled starts out with junk in it
;

;  piece physics
;	no rotate if piece collision
;	drop on timeclick
;	drop/stick to ground
;	deposit piece into pit
; gameplay
;	collision detection	
;	game start setup
;	game end check
;	game mode differences
;	level speedup

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
quadblokTask: 			; task structure
        .word   qb_title        ; title text
	.byte	0x01		; version major
	.byte	0x00		; version minor
        .word   qb_desc1        ; description line 1
        .word   qb_desc2        ; description line 2
        .word   qb_att0         ; attract sequence init
        .word   qb_att          ; attract sequence poll
        .word   qb_game0        ; game init
        .word   qb_game         ; game poll
	.byte	1		; 1 credit per play
	.word	qb_variants	; variant list

	; variants
qb_variants:
	.byte	4		; 5 if we want timedblok, which doesn't work
	.word	qb_norm
	.word	qb_tidy
	.word	qb_tidymed
	.word	qb_tidyhard
	.word	qb_timed

		QBG_ROWCLEAR	= 0x00
		QBG_TIDYBLOK 	= 0x01
		QBG_TIDYBLOKM 	= 0x02
		QBG_TIDYBLOKH	= 0x03
		QBG_TIMEBLOK	= 0x04
qb_norm:
	.byte	17
	.ascii	"     Original    "
qb_tidy:
	.byte	17
	.ascii	"       Tidy      "
qb_tidymed:
	.byte	17
	.ascii	"   Tidy Medium   "
qb_tidyhard:
	.byte	17
	.ascii	"    Tidy Hard    "
qb_timed:
	.byte	17
	.ascii	"      Timed      "

	

qb_title:			; task - text title
	.byte	17
	.ascii	"    Quad Blok    "

qb_desc1:			; task - description
	.byte	21
	.ascii	"   Move the blocks   "

qb_desc2:			; task - description
	.byte	21
	.ascii	" Clear complete lines"

qb_att0:			; att0 entry point
	call	clrsprite
	ld	hl, #(vidram+0x0040 +(0x20*4)+2)
	ld	de, #(.attpitV)
	call	rledraw

	ld	hl, #(colram+0x0040 +(0x20*4)+2)
	ld	de, #(.attpitC)
	call	rledraw

qb_att:				; att entry point
	ret

.attpitV:			; att pit image
	.byte	167, 0x0E, 163, 168, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	0x0f, 18, 163, 0x01
	.byte	167, 0x0E, 163, 169, 0x01
	.byte	0x02

.attpitC:			; att pit colors
	.byte	0x0f, 0x1b, 0x1b, 0x01
	.byte	0x0A, 0x20, 0x03, 0x26, 0x20, 0x20, 0x1b, 0x01
	.byte	0x0A, 0x20, 0x26, 0x04, 0x2E, 0x1b, 0x01
	.byte	0x04, 0x20, 0x2A, 0x05, 0x00, 0x03, 0x25, 0x23, 0x20, 0x1b, 0x01
	.byte 	0x03, 0x20, 0x03, 0x2A, 0x05, 0x20, 0x2E, 0x25, 0x23, 0x23, 0x1b, 0x01
	.byte	0x0B, 0x20, 0x2E, 0x10, 0x10, 0x23, 0x1b, 0x01
	.byte	0x0A, 0x20, 0x26, 0x2E, 0x10, 0x10, 0x29, 0x1b, 0x01
	.byte	0x07, 0x20, 0x29, 0x03, 0x26, 0x2E, 0x20, 0x29, 0x29, 0x1b, 0x01
	.byte	0x06, 0x20, 0x29, 0x29, 0x20, 0x20, 0x2A, 0x20, 0x20, 0x29, 0x20, 0x1b, 0x01
	.byte	0x06, 0x20, 0x29, 0x03, 0x20, 0x05, 0x2A, 0x1b, 0x01
	.byte	0x0A, 0x20, 0x2A, 0x20, 0x20, 0x2a, 0x20, 0x1b, 0x01
	.byte	0x0f, 0x1b, 0x1b, 0x01
	.byte	0x02




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; game parameters

; starting speed rate
	QB_DEFSPEED	= 0x20

; bar time speed (normal difficulty)
	BARTIME  	= 0x0020

; bar time speed (hard difficulty)
	BARTIMEH 	= 0x0018
	
; amount to add to the bar when a row is cleared
	BARTIME_INC 	= 0x05

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; screen positioning of things
	PITXY	= 0x0204		; top corner of pit frame
	PITX	= 0x02			; top corner of pit frame X
	PITY	= 0x04			; top corner of pit frame Y

	PITXY_B	= 0x0404		; position of top corner of pitspace

	PIECE_X	= 0x03			; starting position of piece in play
	PIECE_Y	= 0x00

	DECK_TX	= 0x12			; "NEXT" text
	DECK_TY	= 0x07
	DECK_X	= 0x14
	DECK_Y	= 0x02

	LEVL_TX	= 0x11			; "LEVEL" text
	LEVL_TY	= 0x0b
	LEVL_VP	= 0x1a0b

	ROWS_TX	= 0x12			; "ROWS" text
	ROWS_TY	= 0x0f
	ROWS_VP	= 0x1a0f

	INST_TX = 0x12			; instructions
	INST_TY = 0x14
	INST_VP = 0x1314

	TIMBARO	= 0x03a0		; offset for the timer bar. (left)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ram defs
	qbr		= USERRAM+0x10

	qbr_hs0		= qbr + 1		; w hs word 0	....XXXX
	qbr_hs1		= qbr_hs0 + 2		; w hs word 1	XXXX....

	qbr_pA		= qbr_hs1 + 2		; qbp active piece
	qbr_pB		= qbr_pA + qbp_size	; qbp backup piece (for comp.)
	qbr_pD		= qbr_pB + qbp_size	; qbp deck/next piece

	qbr_level	= qbr_pD + qbp_size	; w current board level
	qbr_rows	= qbr_level + 2		; w number of rows cleared

	qbr_time	= qbr_rows + 2		; w last timeout

	qbr_flags	= qbr_time + 2		; b flags
	qb_collide	= qbr_flags + 1		; b collision?

	qb_counter	= qb_collide + 1	; b line counter
	qb_cline	= qb_counter + 1	; b current line

	qbr_r2		= qb_cline + 1		; b row helper

	qbr_song	= qbr_r2 + 1		; b current song

	qbr_mbar	= qbr_song + 1		; b timer bar MAJOR
	qbr_sbar	= qbr_mbar + 1		; b timer bar SUB

	qbr_exitT	= qbr_sbar + 1		; b exit condition



; flags
	qbf_step	= 0x01		; step down the current piece by 1
	qbf_fall	= 0x02		; make current piece fall to the pit

; good colors: 
				; fill		border
	qb_purples	= 0x03	; purple	pink
	qb_grnorng	= 0x04	; bright green	orange
	qb_grnyel	= 0x05	; dark green	yellow
	qb_redwht	= 0x06	; red		white
	qb_yells	= 0x08	; dk yel	yellow
	qb_brnblue	= 0x09	; brown		blue
	qb_ornpink	= 0x0a	; orange	pink
	qb_pinkblue	= 0x0b	; dark pink	blue
	qb_redbrn	= 0x0c	; red		brown
	qb_whtred	= 0x0d	; white		red
	qb_whtgreen	= 0x0e	; white		green
	qb_blugreen	= 0x0f	; blue		green
	qb_bluesky	= 0x1c	; blue		sky blue
	qb_skyblue	= 0x1d	; sky blue	blue
	qb_grnbrn	= 0x1f	; green		brown
	
qb_colors:	; colors for specific pieces
	.byte	qb_whtred	; box
	.byte	qb_whtgreen	; I
	.byte	qb_purples	; S
	.byte	qb_brnblue	; Z
	.byte	qb_ornpink	; T
	.byte	qb_grnyel	; J
	.byte	qb_redwht	; L
	.byte	qb_bluesky	; ^

qb_gcolors:	; colors for random blocks
	.byte	qb_purples, qb_ornpink, qb_skyblue, qb_brnblue
	.byte	qb_whtred, qb_redwht, qb_blugreen, qb_grnorng

; chasm graphic

	; color
c_QBpit		= 0x1B	; dark green

	; char
qb_MARKED	= 18	; box
qb_FULL		= 19	; inverted box
qb_EMPTY	= 32	; backfill

qb_chtop:		; chasm graphic - topmost bar (one of these)
	.byte 14
	.byte 32, 167, qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY
	.byte qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY, 167, 32

qb_chline:		; chasm graphic - remaining lines (9 of these)
	.byte 14
	.byte 32, 163, qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY
	.byte qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY, qb_EMPTY, 163, 32

qb_chb0:		; chasm graphic - penultimate bottom line (one of these)
	.byte 14
	.byte 32, 169, 163, 163, 163, 163, 163
	.byte 163, 163, 163, 163, 163, 168, 32

qb_chb1:		; chasm graphic - final bottom line (one of these)
	.byte 14
	.byte 32, 32, 32, 32, 32, 32, 32
	.byte 32, 32, 32, 32, 32, 32, 32



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end of game

qb_gameover:
	call	Mo_death		; death mode
	ret
		; GAME OVER
	jr	qb_exit

qb_notime:
	call	Mo_death		; death mode
	ret
		; OUT OF TIME
	jr	qb_exit


qb_exit:
	;call	ptStop			; stop music
	;call	ptSilence		; stop audio
	call	Mo_death		; death mode
	ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; song control

	; autoplay the currently selected song.
qb_songplay:		; Song - play a song
	ld	a, (qbr_song)
	and	#0x03
	cp	#0x00
	jr	z, qb_silent		; 0x00 -> no song
	cp	#0x03
	jr	z, qb_song3		; 0x03 -> song 3 (dig dug)
	cp	#0x02
	jr	z, qb_song2		; 0x02 -> song 2 (fugue)
	; fall through			; 0x01 -> song 1 (song A)

qb_song1:		; Song - tetris song A
	ld	bc, #qb0_song
	ld	a, #0x0d
.qbs00:
	;call	ptInit
	;call	ptPlay
	ret

qb_song2:		; Song - tetris song B
	ld	bc, #qb1_song
	ld	a, #0x06
	jr	.qbs00

qb_song3:		; Song - dig dug song... for some reason...
	ld	bc, #digdug_song
	ld	a, #0x0c
	jr	.qbs00

qb_silent:		; Song - no music.
	;call    ptStop                  ; first, any currently running song
	;call    ptSilence               ; mute it all
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; timer handler... 
qb_timer:				; OLD timer handler - steps pieces
	ld	hl, #qbr_flags
	ld	a, (hl)
	or	#qbf_step
	ld	(hl), a			; set the drop flag

	ld	bc, (qbr_time)
    	ld	de, #qb_timer
	call	atDelta
	ret

;;;;;;;;;;;;;;;;;;;;


; timer valve
qb_tvalve:
	ld	hl, #qbr_flags
	ld	a, (hl)
	and	#qbf_step
	cp	#qbf_step
	jr	nz, .qbv00
	call	qb_stepdown
	jr	.qbv99

.qbv00:
	ld	hl, #qbr_flags
	ld	a, (hl)
	and	#qbf_fall
	cp	#qbf_fall
	jr	nz, .qbv01
	call	qb_falldown
	jr	.qbv99
	
.qbv01:
.qbv99:
	ld	hl, #qbr_flags
	xor	a
	ld	(hl), a
	ret


; qb_falldown
;  drop the current piece to the bottom of the pit
qb_falldown:				; drops piece to the bottom
	call	qb_stepdown
	cp	#0x00
	jr	z, qb_falldown
	ret

; qb_stepdown
;  drop a piece once in the pit:
;  - if it dropped with no issues, a=0
;  - if there was a collision, a=1
;  - if it collided when making the new piece, it jumps to 'exit game' condition
qb_stepdown:				; steps a piece down one notch
	call	qbp_erase		; erase the piece; no self-collides
	push	ix
	    ; first, copy A over to B
	call	qb_A2B
	ld	ix, #qbr_pB
	ld	a, qbp_yp(ix)
	inc	a
	ld	qbp_yp(ix), a
	    ; collision check piece B in the new location
	call	qb_coll
	ld	a, (qb_collide)
	cp	#0x00
	jr	nz, .qb_stepColl	; collision, do funky stuff

	    ; drop the piece, erase the old one
	call	qbp_erase
	call	qb_B2A
	pop	ix
	call	qbp_draw		; redraw the piece
	xor	a
	ret

.qb_stepColl:
	    ; collision, leave the piece in there, create a new one..
	call	qbp_draw		; redraw the piece
	call	qb_linchk		; check for line clears
	call	qbp_Derase
	call	qb_D2A			; copy deck over
	call	qb_NewDeckPiece		; create new deck

	    ; check to see if the new one collides - GAME OVER, MAN!
	call	qb_collA
	ld	a, (qb_collide)
	cp	#0x00
	call	nz, qb_gameover		; see above - leave the game mode

	pop	ix
	ld	a, #0xff
	ret


	; vartxt
	;	display the variant name
qb_vartxt:				; display variant name
	ret
	ld	a, (variant)
	cp	#QBG_ROWCLEAR
	ld	hl, #(.qb_tCR)		; clear rows
	jr	z, .qb_99

	cp	#QBG_TIMEBLOK
	ld	hl, #(.qb_tTB)		; timed block
	jr	z, .qb_99

	;cp	#0x03
	;ld	hl, #(.qb_tP)		; practice
	;jr	z, .qb_99

	;cp	#0x04
	;ld	hl, #(.qb_tP)		; tidy practice
	;jr	z, .qb_99

	; if fell through to here, assume Tidy Blok, 
	; difficulty set by dip switch
	ld	a, (difficulty)
	cp	#diffHARD
	ld	hl, #(.qb_tTH)		; tidy block hard
	jr	z, .qb_99

	ld	hl, #(.qb_tTE)		; tidy block easy
	jr	z, .qb_99


.qb_99:
	call	putstrBP
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.qb_tCR:				; clear line
	.byte	C_blue, 0xFF, 0x1e
.qbXtCR:				; "Original"
	.byte 	8
	.ascii	"Original"

.qb_tTB:
	.byte	C_blue, 0xFF, 0x1e
.qbXtTB:				; "Timed Blok"
	.byte 	10
	.ascii	"Timed Blok"

.qb_tTH:
	.byte	C_blue, 0xFF, 0x1e
.qbXtTH:				; "Tidy - Hard"
	.byte	14
	.ascii	"Tidy Blok hard"

.qb_tTE:
	.byte	C_blue, 0xFF, 0x1e
.qbXtTE:				; "Tidy - normal"
	.byte 	9
	.ascii	"Tidy Blok" 	;@NORMAL"

.qb_tP:
	.byte	C_blue, 0xFF, 0x1e
.qbXtP:					; "Practice"
	.byte	8
	.ascii	"practice"


.qb_TIT0:				; "Title string - old style"
	.byte	17
	.ascii	"[[[ Quad Blok ]]]"
.qb_TIT1:				; copyright string
	.byte	17
	.ascii	" 2005 UmlautLlama"


	; draw the top line of the pit
.qbdtop:
	ld	a, #(c_QBpit)
	ld	hl, #(qb_chtop)		; top
	ld	bc, #PITXY
	call	putstrB
	ret


	; drawpit
	;  draw the pit (without any pieces in it)
	;  (will overwrite screen ram where pieces are
qbdrawpit:
	call	.qbdtop

	ld	hl, #(qb_chline)	; draw the line ninteen times
	ld	b, #19
.qbdp0:
	push	bc
	ld	a, b
	add	a, #PITY
	ld	c, a			; y = offset(5) + b
	ld	b, #PITX		; x = 1

	ld	a, #(c_QBpit)
	call	putstrB

	pop	bc
	djnz	.qbdp0

	ld	hl, #(qb_chb0)		; bottom
	ld	bc, #(PITXY + 0x14)
	call	putstrB
	;ld	hl, #(qb_chb1)		; final
	;ld	bc, #(PITXY + 0x15)
	;call	putstrB

	ret

	; qbtitle
	;	display title and copyright
qbtitle:
	ld	a, #0x17
	ld	bc, #0x0602
	ld	hl, #(.qb_TIT0)
	call	putstrA
	ret
	ld	a, #C_blue
	ld	bc, #0x0603
	ld	hl, #(.qb_TIT1)
	call	putstrA
	ret

qb_game0:			; game 0 entry point (setup)
	    ; reset exit code
	xor	a
	ld	(qbr_exitT), a		; reset exit type

	    ; set up song playback
	;call	qb_songplay

	; one-time setup stuff
	call	qb_vartxt		; display variant name
	call	qbtitle			; draw the title and copyright
	call	qbdrawpit		; draw the pit in its initial form
	call	qb_tbInit		; initialize the timer bar

	    ; game init setup per game configurations...
	ld	a, (variant)

	cp	#QBG_TIDYBLOK		; tidy
	jr	z, .qbi_easy

	cp	#QBG_TIDYBLOKM		; medium tidy
	jr	z, .qbi_medium

	cp	#QBG_TIDYBLOKH		; hard tidy
	jr	z, .qbi_hard

	cp	#QBG_ROWCLEAR		; original
	jr	nz, .qbi_continue

	jr	.qbi_continue

.qbi_hard:
	call	qb_9garbage		; fill in garbage (9 rows)
	jr	.qbi_continue

.qbi_medium:
	call	qb_7garbage		; fill in garbage (7 rows)
	jr	.qbi_continue

.qbi_easy:
	call	qb_5garbage		; fill in garbage (9 rows)

.qbi_continue:
	call 	qb_NewDeckPiece
	call	qb_D2A
	call 	qb_NewDeckPiece

.qb_pdtimer:
	    ; queue up the piece drop
	ld	bc, #QB_DEFSPEED
	ld	(qbr_time), bc

	ld	bc, (qbr_time)
    	ld	de, #qb_timer
	call	atDelta
	ret

	; main quadblok game loop
qb_game:			; game entry point (game poll)
	call	qb_scrtxt		; draw misc screen text
	call	qbp_Ddraw		; draw the On-Deck piece
	call	qbp_draw		; draw the piece

	call	qb_input		; check user input

	call	qb_tbar			; timer bar related things
	call	qb_tvalve		; check for timer stuff

	;call	ptPoll			; song poll

	ret				; relinquish control


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; input handlers
qb_input:	; check joystick, start buttons
	call	Get1Up
	cp	#0x00
	call	nz, qb_1UP

	call	Get1Down
	cp	#0x00
	call	nz, qb_1DOWN

	call	Get1Left
	cp	#0x00
	call	nz, qb_1LEFT

	call	Get1Right
	cp	#0x00
	call	nz, qb_1RIGHT

	call	Get1Start
	cp	#0x00
	call	nz, qb_1START

	call	Get2Start
	cp	#0x00
	call	nz, qb_2START
	ret


qb_1START:
qb_1UP:
	;call	clksnd4
	call	randomize
	call	qb_rotate
	ret

qb_2START:
	ld	hl, #qbr_song
	inc	(hl)
	call	qb_songplay
	ret

qb_1DOWN:
	call	randomize
	ld	hl, #qbr_flags
	ld	a, (hl)
	or	#qbf_fall
	ld	(hl), a			; set the drop flag
	;call	clksnd3
	ret

qb_1LEFT:
	call	randomize
	call	qb_moveL
	;call	clksnd2
	ret

qb_1RIGHT:
	call	randomize
	call	qb_moveR
	;call	clksnd2
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; screen text

qb_scrtxt:

.if 0
	ld	hl, #ptFIndex
	ld	a, (hl)
	ld	d, a

	ld	e, #(C_white)
	ld	bc, #0x03c
	call	prtBCDbyte

	ld	hl, #ptT0Cache
	ld	e, #(C_red)
	ld	bc, #0x0037
	call	prtABCDword

	ld	hl, #ptT1Cache
	ld	e, #(C_red)
	ld	bc, #0x0032
	call	prtABCDword

	ld	hl, #ptT2Cache
	ld	e, #(C_red)
	ld	bc, #0x002d
	call	prtABCDword

	ld	hl, #ptT3Cache
	ld	e, #(C_red)
	ld	bc, #0x0028
	call	prtABCDword
.endif

        ld      e, #(C_yellow)
	    ; next
	ld	hl, #(.qbNEXT)
	call	putstrBP

	    ; level
	ld	hl, #(.qbLEVEL)
	call	putstrBP

	    ; level
	ld	bc, #LEVL_VP
	ld	e, #C_white
	ld	hl, #(qbr_level)
	call	prtBCDword

	    ; rows
	ld	hl, #(.qbROWS)
	call	putstrBP

	    ; instructions
	ld	hl, #(.qbINST1)		; spin
	call	putstrBP
	ld	hl, #(.qbINST2)		; move
	call	putstrBP
	ld	hl, #(.qbINST3)		; drop
	call	putstrBP
	;ld	hl, #(.qbINST4)		; s2 music
	;call	putstrBP

	    ; song title
.if 0
	ld	a, (qbr_song)
	and	#0x03
	ld	hl, #(.qbTsng0)
	cp	#0x00
	jr	z, .qbSONTITL

	ld	hl, #(.qbTsng1)
	cp	#0x01
	jr	z, .qbSONTITL

	ld	hl, #(.qbTsng2)
	cp	#0x02
	jr	z, .qbSONTITL

	ld	hl, #(.qbTsng3)

.qbSONTITL:
	call	putstrBP
.endif

	ld	bc, #ROWS_VP
	ld	e, #C_white
	ld	hl, #(qbr_rows)
	call	prtBCDword

	ret

.qbNEXT:
	.byte	C_yellow, DECK_TX, DECK_TY, 4
	.ascii	"NEXT"

.qbLEVEL:
	.byte	C_yellow, LEVL_TX, LEVL_TY, 5
	.ascii	"LEVEL"

.qbROWS:
	.byte	C_yellow, ROWS_TX, ROWS_TY, 4
	.ascii	"ROWS"

.qbINST1:
	.byte	C_dgreen, INST_TX+1, INST_TY, 7
	.byte	0x0a
	.ascii	"  spin"

.qbINST2:
	.byte	C_dgreen, INST_TX, INST_TY+1, 8
	.byte	0x0c
	.ascii	" "
	.byte	0x0d
	.ascii	" move"

.qbINST3:
	.byte	C_dgreen, INST_TX+1, INST_TY+2, 7
	.byte	0x0b
	.ascii	"  drop"

.qbINST4:
	.byte	C_dgreen, INST_TX+1, INST_TY+3, 8
	.ascii	"S2 music"

.qbTsng0:
	.byte	C_blue, 0xFF, 0x1c, 12
	.ascii	"@@@@@@@@@@@@"
.qbTsng1:
	.byte	C_blue, 0xFF, 0x1c, 12
	.ascii	"@;QUAD@JAM;@"
.qbTsng2:
	.byte	C_blue, 0xFF, 0x1c, 12
	.ascii	";QUAD@FUGUE;"
.qbTsng3:
	.byte	C_blue, 0xFF, 0x1c, 12
	.ascii	";DIGGERTUNE;"



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; piece Structure:
	qbp_co		= 0x00	; char[8] coordinates (x0, y0, x1, y1, ...)
	qbp_xp		= 0x08	; char x position in the pit, 0,0 is upper left
	qbp_yp		= 0x09	; char y position in the pit, 0,0 is upper left
	qbp_c		= 0x0a	; char color
	qbp_pn		= 0x0b	; char piece number
	qbp_r		= 0x0c	; char rotation frame (0,1,2,3)
	qbp_rW		= 0x0d	; char west extent (retrieved @ rot() )
	qbp_rE		= 0x0e	; char east extent (retrieved @ rot() )
	qbp_size	= 0x10	; -- size of this structure --


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; piece manipulations

    ; create a new piece in the Deck location
qb_NewDeckPiece:
	ld	ix, #qbr_pD
	
	    ; pick a random piece
	call	rand
	and	#0x07			; 0..7 
	cp	#0x07
	jr	z, qb_NewDeckPiece
	ld	qbp_pn(ix), a		; store a random piece 0..6

	    ; copy over the data
	add	a			; 0, 2, 4...
	add	a			; 0, 4, 8...
	add	a			; 0, 8, 16...
	ld	b, #0x00
	ld	c, a
	ld	hl, #qb_PIECES
	add	hl, bc			; hl contains offset into qb_PIECES now
	ld	de, #(qbr_pD)		; dest
	ld	bc, #0x0008		; 8 bytes
	ldir

	    ; setup rotational stuff
	xor	a
	ld	qbp_r(ix), a		; initialize rotation
	call	qb_upExtents		; initialize extents

	    ; color
	;ld	a, r
	;and	#0x03			; XXXX
	ld	a, qbp_pn(ix)		; fixed color
	ld	hl, #qb_colors
	rst	0x18			; a = (hl+a)
	ld	qbp_c(ix), a		; random color

	    ; initial position (deck display)
	ld	a, #DECK_X
	ld	qbp_xp(ix), a		; xp = 0x10
	ld	a, #DECK_Y
	ld	qbp_yp(ix), a		; yp = 0x04
	
	ret

    ; setup the extents on the piece contained in IX
qb_upExtents:
	ld	a, qbp_pn(ix)		; a = piece number
	add	a
	add	a
	add	a			; a = offset into qb_Extents
	ld	c, a			; c = a
	ld	a, qbp_r(ix)
	add	a			; a = rotational offset for WEST
	add	a, c			; a += c
	ld	c, a			; c = a
	ld	hl, #qb_EXTENTS
	ld	b, #0x00		; bc = offset of table element
	add	hl, bc			; hl = address of table element

	ld	a, (hl)
	ld	qbp_rW(ix), a		; load WEST extent
	inc	hl
	ld	a, (hl)
	ld	qbp_rE(ix), a		; load EAST extent

	ret

qb_PIECES: 	;.x0 .y0  .x1 .y1  .x2 .y2  .x3 .y3
	.byte   0,0, 0,1, 1,0, 1,1	; Box
	.byte   0,1, 1,1, 2,1, 3,1	; I
	.byte   1,0, 2,0, 0,1, 1,1	; S
	.byte   0,0, 1,0, 1,1, 2,1	; Z
	.byte   0,1, 1,0, 1,1, 2,1	; T
	.byte   0,0, 0,1, 1,1, 2,1	; J
	.byte   2,0, 0,1, 1,1, 2,1	; L
	.byte	0,0, 0,1, 1,0, 1,0	; ^

qb_EXTENTS:	; L,R for each piece, each rotation
	.byte	0,1, 0,1, 0,1, 0,1	; Box
	.byte	0,3, 1,1, 0,3, 1,1	; I
	.byte	0,2, 1,2, 0,2, 0,1	; S
	.byte	0,2, 1,2, 0,2, 0,1	; Z
	.byte	0,2, 1,2, 0,2, 0,1	; T
	.byte	0,2, 1,2, 0,2, 0,1	; J
	.byte	0,2, 1,2, 0,2, 0,1	; L
	.byte   0,0, 0,1, 0,1, 0,1	; ^ (same as box)

; the three different ram piece structures are:
;
; qbr_pA
;	"Active piece"
;	this is the piece that is currently being displayed in the pit
;	the X and Y determine location in the pit
;
; qbr_pB
;	"Backup piece"
;	this piece does not get displayed
;	it is only used for rotation and colision checks
;
; qbr_pD
;	"Deck piece"
;	this is the next piece to go into the pit
;	the X and Y determine where on the screen to draw
;

    ; copy the Active piece to the Backup location
qb_A2B:
	ld	hl, #qbr_pA
	ld	de, #qbr_pB
	ld	bc, #qbp_size
	ldir
	ret

    ; copy the Backup piece to the Active location
qb_B2A:
	ld	hl, #qbr_pB
	ld	de, #qbr_pA
	ld	bc, #qbp_size
	ldir
	ret

    ; copy the Deck piece to the Active location
qb_D2A:
	ld	hl, #qbr_pD
	ld	de, #qbr_pA
	ld	bc, #qbp_size
	ldir

	call	qbp_erase		; erase the deck location

	ld	ix, #(qbr_pA)
	    ; initial position
	ld	a, #PIECE_X
	ld	qbp_xp(ix), a		; xp = 3
	ld	a, #PIECE_Y
	ld	qbp_yp(ix), a		; yp = 0
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; piece movement

qb_moveL:   ; try to perform a left move
	call	qbp_erase		; erase it from the pit
	call	qb_A2B			; copy A to B
	ld	ix, #qbr_pB		; work with B

	    ; pit extents
	call	.qbLcheck
	jr	z, .qbLres		; return if we're over...

	    ; pit contents collision
	ld	a, qbp_xp(ix)
	dec	a		
	ld	qbp_xp(ix), a		; install the adjusted position
	call	qb_coll
	ld	a, (qb_collide)
	cp	#0x00
	jr	nz, .qbLres		; return if we're colliding

	    ; do the move and redraw
	call	qb_B2A
.qbLres:
	call	qbp_draw
	ret

.qbLcheck:  ; extents check
	ld	a, qbp_xp(ix)
	ld	c, a			; store aside (extents)
	ld	a, qbp_rW(ix)
	add	a, c
	cp	#0x00
	ret



qb_moveR:   ; try to perform a right move
	call	qbp_erase		; erase it from the pit
	call	qb_A2B			; copy A to B
	ld	ix, #qbr_pB		; work with B

	    ; pit extents
	call	.qbRcheck
	jr	z, .qbRres		; return if we're over...

	    ; pit contents collision
	ld	a, qbp_xp(ix)
	inc	a
	ld	qbp_xp(ix), a		; install the adjusted position
	call	qb_coll
	ld	a, (qb_collide)
	cp	#0x00
	jr	nz, .qbRres		; return if we're colliding

	    ; do the move and redraw
	call	qb_B2A
.qbRres:
	call	qbp_draw
	ret

.qbRcheck:  ; extents check
	ld	a, qbp_xp(ix)
	ld	c, a			; store aside (extents)
	ld	a, qbp_rE(ix)
	add	a, c
	cp	#0x09
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; rotation routine

qb_rotate:
	call	qbp_erase
	call	qb_A2B 			; copy the piece to the backup
	    ; rotate the piece
	ld	ix, #(qbr_pB)
	ld	a, qbp_pn(ix)
	cp	#0x00			; square - finish up
	jr	z, qb_rFin

	    ; mirror along diagonal...
	ld	b, #0x04
.qbI0:
	ld	d, (ix)
	ld	e, 1(ix)
	ld	(ix), e
	ld	1(ix), d
	inc	ix
	inc	ix
	djnz	.qbI0

	    ; if necessary, invert x's
	ld	ix, #(qbr_pB)
	ld	a, qbp_pn(ix)
	cp	#0x01			; ibeam - finish up
	jr	z, qb_rFin

	    ; invert X:  0->2  1->1  2->0
	ld	b, #0x04
.qbI1:
	ld	a, (ix)
	cp	#0x01
	jr	z, .qbI1_F
	cp	#0x00
	jr	z, .qbI1_0

.qbI1_2:
	ld	a, #0x00
	jr	.qbI1_F
.qbI1_0:
	ld	a, #0x02

.qbI1_F:
	ld	(ix), a
	inc	ix
	inc	ix
	djnz	.qbI1

qb_rFin:
	    ; update rotation number and extents
	ld	ix, #(qbr_pB)
	ld	a, qbp_r(ix)
	inc	a
	and	#0x03
	ld	qbp_r(ix), a		; advance to next rotation frame
	call	qb_upExtents		; update extents

	ld	ix, #(qbr_pB)
	call	qb_Lscooch
	call	qb_Rscooch

	     ;do collision checks on the new piece
	call	qb_coll
	ld	a, (qb_collide)
	cp	#0x00
	jr	nz, .qbTres		; return if we're colliding

	    ; copy it back
	call	qb_B2A
	ld	ix, #(qbr_pA)
	call	qb_upExtents		; update extents

	    ; do nothing and return
.qbTres:
	call	qbp_draw
	ret

	    ; scooch over the piece if we're over the left side of the pit
qb_Lscooch:
	ld	ix, #(qbr_pB)		; work with the Backup piece
	call	.qbLcheck
	ret	p			; return if there's nothing to do
	ld	a, qbp_xp(ix)
	inc	a			; move to the right
	ld	qbp_xp(ix), a
	ret				; only go through once

	    ; scooch over the piece if we're over the right side of the pit
qb_Rscooch:
	ld	ix, #(qbr_pB)		; work with the Backup piece
	call	.qbRcheck
	ret	m			; return if there's nothing to do
	ret	z			; return if there's nothing to do
	ld	a, qbp_xp(ix)
	dec	a			; move to the left
	ld	qbp_xp(ix), a
	jr	qb_Rscooch		; I-beam might need double-scooch



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; draw routines


	; qbp_draw
	;	draw the deck piece to the screen
qbp_Ddraw:
	push	af
	ld	hl, #(qbr_pD)
	ld	ix, #(qbr_pD)
	ld	b, #0x04
	jr	.qbp_d0

	; qbp_draw
	;	draw the current piece to the pit
qbp_draw:
	push	af
	ld	hl, #(qbr_pA)
	ld	ix, #(qbr_pA)
	ld	b, #0x04		; 4 blocks
.qbp_d0:
	push	bc
	ld	d, (hl)
	ld	a, qbp_xp(ix)
	add	a, d
	ld	d, a			; add in x offset
	inc	hl
	ld	e, (hl)
	ld	a, qbp_yp(ix)
	add	a, e
	ld	e, a			; add in y offset
	inc	hl
	ld	a, qbp_c(ix)		; a = color
	call	qbp1_draw
	pop	bc
	djnz 	.qbp_d0
	pop	af
	ret


	; qbp1_draw
	; 	draws a single pixel into the pit
	;	    in	de	XXYY
	;	    in 	a	color
	;	    out -
	;	    mod	hl,bc
qbp1_draw:
	push	hl

	call	qb_pXY2offs	; DE -> HL offset
	push	hl

	call	qb_Coffs
	ld	(hl), a		; set color
	pop	hl

	call	qb_Voffs
	ld	a, #qb_FULL
	ld	(hl), a		; set character

	pop	hl
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; qb_pXY2offs
;	in	de	XXYY in the pit
;	out	hl	screen offset
;	
qb_pXY2offs:
	push	bc
	ld	hl, #PITXY_B
	add	hl, de
	push	hl
	pop	bc
	call	xy2offsB
	pop	bc
	ret

; qb_Coffs
;	adjusts HL to be for color ram offset
qb_Coffs:
	push	bc
	ld	bc, #colram
	add	hl, bc
	pop	bc
	ret

; qb_Voffs
;	adjusts HL to be for video ram offset
qb_Voffs:
	push	bc
	ld	bc, #vidram
	add	hl, bc
	pop	bc
	ret


;;;;;;;;;;;;;;;;;;;;

	; qbp_Derase
	;	erase the On-Deck piece from the screen
qbp_Derase:
	push	af
	ld	hl, #(qbr_pD)
	ld	ix, #(qbr_pD)
	ld	b, #0x04		; 4 blocks
	jr	.qbp_e0

	; qbp_erase
	;	erase the current piece from the pit
qbp_erase:
	push	af
	ld	hl, #(qbr_pA)
	ld	ix, #(qbr_pA)
	ld	b, #0x04		; 4 blocks
.qbp_e0:
	push	bc
	ld	d, (hl)
	ld	a, qbp_xp(ix)
	add	a, d
	ld	d, a			; add in x offset
	inc	hl
	ld	e, (hl)
	ld	a, qbp_yp(ix)
	add	a, e
	ld	e, a			; add in y offset
	inc	hl
	ld	a, #c_QBpit
	call	qbp1_erase
	pop	bc
	djnz 	.qbp_e0
	pop	af
	ret


	; qbp1_erase
	; 	erases a single pixel into the pit
	;	    in	de	XXYY
	;	    in 	a	color
	;	    out -
	;	    mod	-
qbp1_erase:
	push	hl

	call	qb_pXY2offs	; DE -> HL offset
	push	hl

	call	qb_Coffs
	ld	(hl), a		; set color
	pop	hl

	call	qb_Voffs
	ld	a, #qb_EMPTY
	ld	(hl), a		; set character

	pop	hl
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 

qb_scores:
      .word   0x0100          ; sprite 0x08
      .word   0x0200          ; sprite 0x09
      .word   0x0500          ; sprite 0x0a
      .word   0x1000          ; sprite 0x0c
      .word   0x2000          ; sprite 0x0d
      .word   0x5000          ; sprite 0x0e

      .word   0x0200          ; sprite 0x28
      .word   0x0400          ; sprite 0x29
      .word   0x0800          ; sprite 0x2a
      .word   0x1600          ; sprite 0x2b

      .word   0x0005          ; new piece



; addToScore
; floatScore

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; qb_collA
	;	collision checks A against the pit for the "filled pit" case
qb_collA:
	push	af
	xor	a
	ld	(qb_collide), a		; qb_collide = 0

	ld	hl, #(qbr_pA)		; check piece A (playfield)
	ld	ix, #(qbr_pA)
	ld	b, #0x04		; 4 blocks
	jr	.qbp_cl0		; continue below...

	; qb_coll
	; 	collision checks a piece in the pit
	;	    in	-	
	;	    out -
	;	    mod 	(qb_collide)
qb_coll:
	push	af
	xor	a			; clear the collide flag
	ld	(qb_collide), a		; qb_collide = 0

	ld	hl, #(qbr_pB)		; check piece B (playfield)
	ld	ix, #(qbr_pB)
	ld	b, #0x04		; 4 blocks
.qbp_cl0:
	push	bc
	ld	d, (hl)
	ld	a, qbp_xp(ix)
	add	a, d
	ld	d, a			; add in x offset
	inc	hl
	ld	e, (hl)
	ld	a, qbp_yp(ix)
	add	a, e
	ld	e, a			; add in y offset
	inc	hl
	ld	a, #c_QBpit
	call	qbp1_coll
	pop	bc
	djnz 	.qbp_cl0
	pop	af
	ret

	; qbp1_coll
	; 	collision checls a single pixel in the pit
	;	    in	de	XXYY
	;	    in 	a	color
	;	    out -
	;	    mod	-
qbp1_coll:
	push	hl
	push	bc
	ld	hl, #PITXY_B
	add	hl, de
	push	hl
	pop	bc			; bc = PITXY_B + de offset
	call	xy2offsB

	call	qb_Voffs
	ld	a, (hl)
	cp	#qb_EMPTY		; make sure it's not empty

	jr	z, .qb1c00

	    ; collision, return
	ld	hl, #(qb_collide)
	inc	(hl)

.qb1c00:
	pop	bc
	pop	hl
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; qb_garbage
	;	sets up the screen with a few rows of garbage
qb_9garbage:
	ld 	b, #9		; 9 rows
	jr	.qb_g00

qb_7garbage:
	ld	b, #7		; 7 rows
	jr	.qb_g00

qb_5garbage:
	ld	b, #5		; 5 rows

.qb_g00:
	push	bc

	ld	a, #20		; rows at the bottom of the pit
	sub	a, b
	ld	e, a		; e = y

	ld	b, #8		; 8 random blocks per row...
.qb_g01:
	push	bc

	call	rand		; 0..3
	and	#3
	ld	d, a

	call	rand		; 0..7
	and	#7
	add	d		; a = rnd&7 + rnd&3

	cp	#10		; if == 10, subtract 1
	jr	nz, .qb_g02
	sub	#1

.qb_g02:
	ld	d, a		; d = x

	call	rand
	and	#0x07			; random color
	ld	hl, #qb_gcolors
	rst	0x18			; a = (hl+a)

	call	qbp1_draw		; draw the block
	pop	bc
	djnz	.qb_g01			; do another piece on the row?

	pop	bc
	djnz	.qb_g00			; do another row?
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; qb_linchk
;	check for cleared lines
qb_linchk:

	ld	b, #19			; check all lines.
.qblck0:
	push	bc
	ld	e, b

	xor 	a
	ld	ix, #qb_counter
	ld	(ix), a			; reset the counter

	ld	b, #10			; check 10 columns
.qblck1:
	push	bc
	dec	b
	ld	d, b

	call	qb_pXY2offs
	;push	hl
	call 	qb_Voffs

	ld	a, (hl)
	cp	#qb_EMPTY
	jr	z, .qblck2
	inc	(ix)

.qblck2:
	pop	bc
	djnz	.qblck1
	ld	a, (ix)			; check the counter
	cp	#10			; full line?
	jr	nz, .qblck3		; nope, next line

	pop	bc			; borrow bc
	push	bc			; b = line

	ld	d, #0x00
	ld	e, b
	call	qb_pXY2offs
	call	qb_Voffs
	ld	a, #qb_MARKED
	ld	(hl), a			; mark the line

.qblck3:
	pop	bc
	djnz	.qblck0			; check the next line

	call	qb_lindrop
	call	qb_lindrop		; fix for repeat items. heh
	call	qb_lindrop		; fix for repeat items. heh

	call	.qb_pdtimer		; XXX Queue up the next piece drop
	    ; adjust score based on qb_counter
	    ; XXXX
	ret



qb_lindrop:
	; look vertically for makred rows
	; on each one, start a score trigger (score value, sprite Y)
	; lower the rows above it instantaneously

	ld	b, #19
.qbld0:
	push	bc			; set aside loop variable
	ld	a, b
	dec	a
	ld	(qb_cline), a		; current line - set aside

	ld	d, #0x00		; D- = 00,--
	ld	e, b			; -E = --,YY (where the mark is)
	call	qb_pXY2offs		; DE -> HL offset
	push	hl
	call	qb_Voffs		; HL += video offset
	ld	a, (hl)			; a = piece?
	cp	#qb_MARKED		; is it marked
	pop	hl
	jr	nz, .qbld99		; nope, skip over

	ld	b, #BARTIME_INC		; add to the time bar
	call	qb_addTime

QBpxOFFS = -32

	push	hl			; set aside for color below
	call	.qbDCHARS
	pop	hl
	call	.qbDCOLORS


	    ; redraw the top line
	call	.qbdtop

	    ; inc ROWS count
	ld	bc, (qbr_rows)
	call	incBCDbc
	ld	(qbr_rows), bc

	    ; adjust speedup/level every 5 rows
	ld	a, (difficulty)
	cp	#diffHARD
	jr	z, .qbld70
	ld	b, #10
	jr	.qbld71			; hard: we're done
.qbld70:
	ld	b, #5

.qbld71:
	ld	a, (qbr_r2)
	inc	a
	ld	(qbr_r2), a
	cp	b
	jr	nz, .qbld99		; no bump, continue below
	xor	a			; bump, reset the counter
	ld	(qbr_r2), a

	    ; new level bump
	ld	bc, (qbr_level)
	call	incBCDbc
	ld	(qbr_level), bc

	call 	speed_bump_up

	ld	a, (difficulty)		; check for hard (faster speedups)
	cp	#diffHARD
	call	z, speed_bump_up	; hard: faster speedups

.qbld99:
	pop	bc			; restore loop variable
	djnz	.qbld0			; jump back for the next row
	ret

speed_bump_up:
	ld	bc, (qbr_time)		; retrieve it
	ld	a, c			; only look at 'c'
	dec 	a			; a--
	cp	#0x00			; keep it at 1 or greater
	ret	z			; it was 0, return

	ld 	c, a			; restore it
	ld	(qbr_time), bc		; restore it to ram
	ret

	;; drop tiles
.qbDCHARS:
	call	qb_Voffs		; HL offset -> Video buffer offset
	ld	b, #10 		; loop for 10 times (pit width) to do this
.qbld1:
	push	bc
	push	hl

	    ; actually drop the characters here
	push	hl
	pop	de			; de = hl
	dec	hl			; hl--
	ld	b, #0
	ld	a, (qb_cline)
	inc	a
	ld	c, a			; bc = 00 nbytes
	lddr

	    ; adjust horizontal position
	pop	hl
	ld	de, #QBpxOFFS
	add	hl, de
	
	pop	bc			; restore horiz counter
	djnz	.qbld1			; jump back for next column
	ret


	;; drop colors
.qbDCOLORS:
	call	qb_Coffs		; HL offset -> Color buffer offset
	ld	b, #10
.qbld2:
	push	bc
	push	hl

	    ; actually drop the characters here
	push	hl
	pop	de			; de = hl
	dec	hl			; hl--
	ld	b, #0
	ld	a, (qb_cline)
	inc	a
	ld	c, a			; bc = 00 nbytes
	lddr

	    ; adjust horizontal position
	pop	hl
	ld	de, #QBpxOFFS
	add	hl, de
	
	pop	bc			; restore horiz counter
	djnz	.qbld2			; jump back for next column
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; timer-bar related functions.

qb_addTime:				; pass in inc in 'b'
	ld	a, #0x02
	ld	(qbr_sbar), a		; reset sub

	ld	a, (qbr_mbar)
	add	a, b
	ld	(qbr_mbar), a
	cp	#0x1e
	ret	c			; it was < 1f, return
	ld	a, #0x1e
	ld	(qbr_mbar), a
	ret


qb_tbInit:
	ld	a, (variant)
	cp	#QBG_TIMEBLOK
	ret	nz			; return if we're not timeblok

	ld	a, #0x1e		; major start value
	ld	(qbr_mbar), a
	ld	a, #3			; minor start value
	ld	(qbr_sbar), a
	jr	.qbtbcbsu		; schedule the callback

qb_tbAt:
	ld	a, (qbr_sbar)
	cp	#0x00
	jr	nz, .tbdecs		; not zero, decrement sub

	ld	a, #3			; sub = 3
	ld	(qbr_sbar), a

	ld	a, (qbr_mbar)
	cp	#0x00
	jp	z, qb_notime		; OUT OF TIME!
	;jr	z, qb_tbInit		; main == 0, redo for now.
	dec	a
	ld	(qbr_mbar), a		; main--
	jr	.qbtbcbsu

.tbdecs:				; sub--
	dec	a
	ld	(qbr_sbar), a

.qbtbcbsu: 				; setup a callback at timer
	ld	bc, #BARTIME		; normal timeout

	ld	a, (difficulty)		; check for hard (faster timebar)
	cp	#diffHARD
	jr 	nz, .qbtbcbsu00
	ld	bc, #BARTIMEH		; set hard speed

.qbtbcbsu00:
    	ld	de, #qb_tbAt		; finish up the callback scheduling
	call	at2Delta
	ret

qb_tbCHZ:
	.ascii	"ABCD"
	.byte	0x10, 0x12, 0x14, 0x14

qb_tbCLZ:
	.byte	0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07
	.byte	0x08, 0x08, 0x08, 0x08

	.byte	0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15	; red, orange
	.byte	0x16, 0x16, 0x16, 0x16, 0x11, 0x11, 0x11, 0x11	; yellow, green
	.byte	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11	; green
	.byte	0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11	; green
	.byte	0x11

qb_tbar:
	ld	a, (variant)
	cp	#QBG_TIMEBLOK
	ret	nz			; return if we're not timeblok

	ld	ix, #vidram + TIMBARO
	ld	iy, #colram + TIMBARO
	ld	b, #0x1f
.qbtd0:
	push	bc

	ld	a, (qbr_mbar)
	inc	a			; 0..1f -> 1..20
	cp	b
	ld	a, #0x14
	jr	nz, .qbtd1

	ld	b, #0x00
	ld	hl, #qb_tbCHZ
	ld	a, (qbr_sbar)
	ld	c, a
	add	hl, bc
	ld	a, (hl)			; get character
.qbtd1:
	ld	(ix), a

	ld	a, (qbr_mbar)
	inc	a
	pop	bc
	push	bc
	cp	b
	ld	a, #0x00		; black
	jp	m, .qbtd2

.qbtd3:
	ld	a, (qbr_mbar)
	ld	hl, #qb_tbCLZ
	ld	b, #0x00
	ld	c, a
	add	hl, bc
	ld	a, (hl)			; get color
.qbtd2:
	ld	(iy), a

	inc	ix
	inc	iy
	pop	bc
	djnz	.qbtd0
	ret

QUADBLOK_SZ	= .-quadblokTask
