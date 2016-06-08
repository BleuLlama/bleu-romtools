;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  MultiSprite
;;
;;   experimenting with multiplexing sprites
;;
;;   (c) 2012 Scott Lawrence
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; The module definition block
mspriteTask:
	.word	mspr_title	; title text
	.byte	0x00		; version major
	.byte	0x01		; version minor
	.word	mspr_desc1	; description line 1
	.word	mspr_desc2	; description line 2
	.word	mspr_att0	; attract sequence init
	.word	mspr_att	; attract sequence poll
	.word	mspr_game0	; game init
	.word	mspr_game	; game poll
	.byte	1		; 1 credit per play
	.word	mspr_variants	; variant list

	; variants...
mspr_variants:
	.byte	1
	.word	mspr_Voriginal
	.word	mspr_Valternate

	; variant names
mspr_Voriginal:
	.byte	17
	;;;;;   "                 "  ; this is just here as a reference of width
	.ascii	"     Original    "
mspr_Valternate:
	.byte	17
	;;;;;   "                 "
	.ascii	"    Alternate    "

	; title text
mspr_title:
	.byte	17
	;;;;;   "                 "
	.ascii	"   Mux Sprites   "

	; title description
mspr_desc1:
	.byte	21
	;;;;;   "                     "
	.ascii  "  Experiment to try  "
mspr_desc2:
	.byte	21
	;;;;;   "                     "
	.ascii  " multiplexing sprites"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; attract sequence

; this gets polled from the core menu system.
; it should do a single step of your attract animation sequence

mspr_att0:
	call	clrsprite

mspr_att:
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Game Play

; this gets polled from the core menu system.
; it should do a single step of your game play

msprSel		= USERRAM+0		; selector (0..3)
msprSpSl	= USERRAM+1		; sprite slots (0..6)
msprSpCt	= USERRAM+2		; number of sprites
msprDuCy	= USERRAM+3		; duty cycle

	; initialize the game
mspr_game0:
	xor	a
	ld	(msprSel), a		; Sel = 0

	ld	a, #4
	ld	(msprSpSl), a		; slots = 4

	ld	a, #2
	ld	(msprSpCt), a		; count = 2

	ld	a, #1
	ld	(msprDuCy), a		; Duty Cycle = 1

	; run the game
mspr_game:
	call 	mspr_checkInput
	call	mspr_texts
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
mspr_texts:
	ld	hl, #.msprt1	; "Slots"
	ld	a, #5		; green
	ld	bc, #0x0500	; 5,0
	call	putstrB

	ld	hl, #.msprt2	; "Sprites"
	ld	c, #2		; 5,2
	call	putstrB

	ld	hl, #.msprt3	; "Duty Cycle"
	ld	c, #4		; 5,4
	call	putstrB

	ld	e, a		; green
	ld	a, (msprSpSl)
	ld	bc, #0x0200	; 0,0
	call	prtXYbyteB

	ld	c, #2		; 0,2
	ld	a, (msprSpCt)
	call	prtXYbyteB

	ld	c, #4		; 0,4
	ld	a, (msprDuCy)
	call	prtXYbyteB

	; draw the selector arrow

	ld	a, (msprSel)	; a = current selection
	add	a		; a = current selection * 2
	ld	d, a		; d = current selection * 2

	ld	bc, #0x0000	; bc = 0,0

.msprTlp: ; loop point
	; compare a with c
	ld	a, c
	cp	d		; this == selected?
	jr	nz, .msprTsp
	ld	hl, #.msprtArr	; put out an arrow
	jr	.msprTlpf
.msprTsp:
	ld	hl, #.msprtSpc	; put out a space

.msprTlpf:
	ld	a, #6		; white with red dropshadow
	call	putstrB

	inc	c
	inc	c		; c=c+2
	ld	a, c
	cp	#6
	jr	nz, .msprTlp
	
	ret

.msprt1:
	.byte 	5
	.ascii 	"Slots"

.msprt2:
	.byte 	7
	.ascii 	"Sprites"

.msprt3:
	.byte	10
	.ascii "Duty Cycle"

.msprtArr:
	.byte	1, 13

.msprtSpc:
	.byte	1
	.ascii " "


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; decrement the selector
msprDecS:
	ld	a, (msprSel)
	dec	a
	ld	(msprSel), a
	cp	#0xff
	ret	nz		; not hit the boundary, return
	ld	a, #2
	ld	(msprSel), a
	ret

; increment the selector
msprIncS:
	ld	a, (msprSel)
	inc	a
	ld	(msprSel), a
	cp	#3
	ret	nz		; not hit the boundary, return
	xor 	a
	ld	(msprSel), a
	ret


msprGetThis:
	ld	a, (msprSel)
	cp	#0
	jr	z, .mgt0
	cp	#1
	jr	z, .mgt1
	; 2
	ld	a, (msprDuCy)
	ret
.mgt0:
	ld	a, (msprSpSl)
	ret
.mgt1:
	ld	a, (msprSpCt)
	ret

msprPutThis:
	push	af
	ld	a, (msprSel)
	cp	#0
	jr	z, .mpt0
	cp	#1
	jr	z, .mpt1
	; 2
	pop	af
	ld	(msprDuCy), a
	ret
.mpt0:
	pop	af
	ld	(msprSpSl), a
	ret
.mpt1:
	pop	af
	ld	(msprSpCt), a
	ret

msprIncThis:
	call	msprGetThis
	inc	a
	call	msprPutThis
	ret

msprDecThis:
	call	msprGetThis
	dec	a
	call	msprPutThis
	ret

mspr_checkInput:
	call 	Get1Up
	cp 	#0x00
	call	nz, msprDecS

	call	Get1Down
	cp	#0x00
	call	nz, msprIncS

	call	Get1Left
	cp	#0x00
	call	nz, msprDecThis

	call	Get1Right
	cp	#0x00
	call	nz, msprIncThis

msprNright:

	;; check for exit
	call	Is2Start
	cp	#0x00
	jp	nz, Mo_death
	ret


mspr_SZ	= .-msprTask
