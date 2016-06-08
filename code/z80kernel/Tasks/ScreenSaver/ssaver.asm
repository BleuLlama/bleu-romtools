;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Screen Saver
;;
;;   (c) 2006 Scott Lawrence
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; The module scanner block
ssaveTask:
	.word	ss_title	; title text
	.byte	0x01		; version major
	.byte	0x00		; version minor
	.word	ss_desc1	; description line 1
	.word	ss_desc2	; description line 2
	.word	ss_att0		; attract sequence init
	.word	ss_att		; attract sequence poll
	.word	ss_game0	; game init
	.word	ss_game		; game poll
	.byte	0		; freeplay
	.word	0x0000		; variants

	; title text
ss_title:
	.byte	17
	;;;;;   "                 "
	.ascii	"   ScreenSaver   "

ss_desc1:
	.byte	21
	;;;;;   "                     "
	.ascii  "  Prevents burn-in   "
ss_desc2:
	.byte	21
	.ascii  "                     "

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; attract sequence

ss_att0:
	call	clrsprite
ss_att:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Game Play


	; initialize the game
ss_game0:
        call    clrsprite
	call    ruscls

	call	.ss_posrand

	xor 	a
	ld	(USERRAM+8), a		; text string
	ret

	; game poll
ss_game:
	    ; move the sprites into current timer-based positions

	    ; sprite 0: dragon flying in circles
	call	ss_xyBC			; sine position for now
	ld	de, #(sprite0)
	ld	hl, #ssSdragon
	call	dsprite

	    ; sprite 1: alligator walking right
	ld	de, (timer)
	ld	a, (USERRAM+0)		; x
	sub	a, e
	ld	b, a			; increment X by timer&0x0ff
	ld	a, (USERRAM+1)		; y
	ld	c, a
	ld	de, #(sprite1)
	ld	hl, #ssSgator
	call	dsprite

	    ; sprite 2: bunny hopping right
	ld	de, (timer)
	ld	a, (USERRAM+2)		; x
	add	a, e
	add	a, a			; move him fast
	ld	b, a			; increment X by timer&0x0ff
	ld	a, (USERRAM+3)		; y
	ld	c, a
	ld	de, #(sprite2)
	ld	hl, #ssSbunny
	call	dsprite

	    ; sprite 3: monkey walking left
	ld	de, (timer)
	ld	a, (USERRAM+4)		; x
	add	a			; twice speed
	sub	a, e
	ld	b, a			; increment X by timer&0x0ff
	ld	a, (USERRAM+5)		; y
	ld	c, a
	ld	de, #(sprite3)
	ld	hl, #ssSmonkey
	call	dsprite

	    ; sprite 4: llama diagonal
	ld	de, (timer)
	ld	a, (USERRAM+6)		; X
	add	a, e
	ld	b, a			; inc X by timer&0x0ff
	ld	a, (USERRAM+7)
	add	a			; x2
	add	a, e
	ld	c, a			; inc Y by timer&0x0ff
	ld	de, #(sprite4)
	ld	hl, #ssSllama
	call	dsprite

	ld	a, (timer)
	and	#0x7f
	ret	nz

	; put new text on the screen, re-shuffle the sprites
ssNewScreen:
	call	cls
	call	.ss_posrand

	ld	a, (USERRAM+8)
	inc	a
	ld 	d, #00
	ld	(USERRAM+8), a
	and	#0x07			; determine text string to use
	add	a
	ld	e, a
	ld	hl, #ssText
	rst	0x10
	push 	hl			; hl contains text string

	call	rand
	and	#0x03
	ld	b, a			; random horizontal
	call	rand
	and	#0x1f
	ld	c, a			; random vertical

	ld	hl, #ssColors
	call	rand
	and	#0x07
	rst	0x18			; random color from our list

	pop	hl
	call	putstrB

	ret


	; compute sine and cosine from the timer and store it in BC
	; generates a lissajous
ss_xyBC:
	ld	bc, (timer)
	add	a, c		; add in offset 'a'
	ld	c, a
	call	sine		; sine of timer + a
	add	#0x20

	ld	b, a
	push	bc		; save aside X in B

	ld	bc, (timer)
	add	a, c		; add in offset 'a' (tainted by the above code)
	ld	c, a
	call	cosine
	add	#0x20

	pop	bc		; restore X in B
	ld	c, a            ; bc now contains XY

	ret     


	    ; preload our positions with random values
.ss_posrand:
	call	rand
	ld	(USERRAM+0), a		; 1x
	call	rand
	ld	(USERRAM+1), a		; 1y

	call	rand
	ld	(USERRAM+2), a		; 2x
	call	rand
	ld	(USERRAM+3), a		; 2y

	call	rand
	ld	(USERRAM+4), a		; 3x
	call	rand
	ld	(USERRAM+5), a		; 3y

	call	rand
	ld	(USERRAM+6), a		; 4x
	call	rand
	ld	(USERRAM+7), a		; 4y

	ret

    ; sprite animations
ssSdragon: 	.byte	0x0d,	33*4, 33*4, 32*4, 32*4
ssSgator: 	.byte 	0x05,	22*4, 23*4, 22*4, 23*4
ssSmonkey:	.byte	0x04,	60*4, 61*4, 62*4, 63*4
ssSbunny:	.byte	0x0a,	27*4|valXFlip, 28*4|valXFlip, 29*4|valXFlip, 30*4|valXFlip
ssSllama:	.byte	0x09,	1*4, 1*4, 1*4, 1*4

    ; text list table
ssText:
	.word	ssTblank	;
	.word	VERSION		; BL Game Kernel v0.xx
	.word	ssTmadeby	; Made By
	.word	.name		; Scott Lawrence
	.word	ssTemail	; yorgle@gmail.com
	.word	.cprt		; (c)2006 UmlautLlama.com

	.word	ssTmovestick	; Move stick to return
	.word	ssTtomenu	; to game menu

    ; some of our text strings
ssTmovestick:
	.byte 	20
	.ascii	"Move stick to return"

ssTtomenu:
	.byte	17
	.ascii	"to the game menu."

ssTmadeby:
	.byte	7
	.ascii	"Made By"

ssTblank:
	.byte	1
	.ascii	" "

ssTemail:
	.byte	16
	.ascii	"yorgle@gmail.com"

    ; colors to pick from for the text strings
ssColors:
	.byte	0x01, 0x03, 0x04, 0x05,  0x06, 0x08, 0x0b, 0x0D

SCREENSAVER_SZ	= .-ssaveTask
