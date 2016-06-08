;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Test Mode
;;
;;   (c) 2006 Scott Lawrence
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TESTMODE_START:

; The module scanner block
testModeTask:
	.word	tm_title	; title text
	.byte	0x01		; version major
	.byte	0x00		; version minor
	.word	tm_desc1	; description line 1
	.word	tm_desc2	; description line 2
	.word	tm_att0		; attract sequence init
	.word	tm_att		; attract sequence poll
	.word	tm_game0	; game init
	.word	tm_game		; game poll
	.byte	0		; freeplay
	.word	0x0000		; variants

	; title text
tm_title:
	.byte	17
	;;;;;   "                 "
	.ascii	"    Test Mode    "

tm_desc1:
	.byte	21
	;;;;;   "                     "
	.ascii  "  Tests out hardware "
tm_desc2:
	.byte	21
	.ascii  "                     "

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; attract sequence

tm_att0:
tm_att:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Game Play

	tm_screen	= runMode1

	; initialize the game
tm_game0:
	call	clrsprite
	xor	a
	ld	(tm_screen), a		; start on screen 0
	ret

	; game poll
tm_game:
	call	tm_change		; change if applicable

	ld	a, (tm_screen)
	and	#0x07			; max number of screens
	cp	#0x00
	jp	z, tm_scr0		; display the proper screen

	cp	#0x01
	jp	z, tm_scr1

	cp	#0x02
	jp	z, tm_scr2

	cp	#0x03
	jp	z, tm_scr3

	cp	#0x04
	jp	z, tm_scr4

	cp	#0x05
	jp	z, tm_scr5

	cp	#0x06
	jp	z, tm_scr6

	cp	#0x07
	jp	z, tm_scr7

	
	ret

tm_change:
	call	.tm_deccheck
	call	.tm_inccheck
	ret

.tm_deccheck:
	ld	a, (Cp1lt)		; check for dec
	cp	#0x03
	ret	nz			; nope. return

	call	inputclear		; clear inputs
	call	cls			; clear screen
	call	clrsprite		; clear sprites

	ld	a, (tm_screen)
	dec	a
	and	#0x07
	ld	(tm_screen), a		; tm_screen --
	ret
	
.tm_inccheck:
	ld	a, (Cp1rt)		; check for inc
	cp	#0x03
	ret	nz			; nope. return

	call	inputclear		; clear inputs
	call	cls			; clear screen
	call	clrsprite		; clear sprites

	ld	a, (tm_screen)
	inc	a
	and	#0x07
	ld	(tm_screen), a		; tm_screen++
	ret
	

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; screen 0 - display user input
tm_scr0:
	ld	hl, #.tm0title
	ld	bc, #0x0000
	ld	a, #C_red
	call	putstrB

	    ; player 1 stuff
	call	Is1Start
	ld	bc, #0x0602
	ld	e, #C_white
	call	prtXYbyteB

	call	Is1Up
	ld	bc, #0x0604
	ld	e, #C_white
	call	prtXYbyteB

	call	Is1Down
	ld	bc, #0x0606
	ld	e, #C_white
	call	prtXYbyteB

	call	Is1Left
	ld	bc, #0x0405
	ld	e, #C_white
	call	prtXYbyteB

	call	Is1Right
	ld	bc, #0x0805
	ld	e, #C_white
	call	prtXYbyteB

	    ; player 2 stuff
	ld	a, (Cstart2)
	ld	bc, #0x1602
	ld	e, #C_white
	call	prtXYbyteB

	ld	a, (Cp2up)
	ld	bc, #0x1604
	ld	e, #C_white
	call	prtXYbyteB

	ld	a, (Cp2dn)
	ld	bc, #0x1606
	ld	e, #C_white
	call	prtXYbyteB

	ld	a, (Cp2lt)
	ld	bc, #0x1405
	ld	e, #C_white
	call	prtXYbyteB

	ld	a, (Cp2rt)
	ld	bc, #0x1805
	ld	e, #C_white
	call	prtXYbyteB


	    ; coins
	ld	a, (Ccoin1)
	ld	bc, #0x0c03
	ld	e, #C_white
	call	prtXYbyteB

	ld	a, (Ccoin2)
	ld	bc, #0x1003
	ld	e, #C_white
	call	prtXYbyteB

	ld	a, (Ccoin3)
	ld	bc, #0x0e05
	ld	e, #C_white
	call	prtXYbyteB

	    ; labels
	ld	a, #C_blue
	ld	hl, #.tmplayer1
	ld	bc, #0x0508
	call	putstrB

	ld	a, #C_blue
	ld	hl, #.tmplayer2
	ld	bc, #0x1508
	call	putstrB

	ld	a, #C_blue
	ld	hl, #.tmcoins
	ld	bc, #0x0c08
	call	putstrB

	    ; timer
	ld	a, #C_orange
	ld	hl, #.tmtimer
	ld	bc, #0x080d
	call	putstrB

	ld	e, #C_yellow
	ld	bc, #0x120d
	ld	a, (timer)
	call	prtXYbyteB

	ld	bc, #0x100d
	ld	a, (timer+1)
	call	prtXYbyteB

	    ; dips
	ld	a, #C_orange
	ld	hl, #.tmdip
	ld	bc, #0x0a0f
	call	putstrB

	ld	e, #C_yellow
	ld	bc, #0x100f
	ld	a, (dsw0)
	call	prtXYbyteB

	    ; inputs
	ld	a, #C_orange
	ld	hl, #.tminput0
	ld	bc, #0x0611
	call	putstrB
	ld	hl, #.tminput1
	ld	c, #0x13
	call	putstrB

	ld	e, #C_yellow
	ld	bc, #0x1011
	ld	a, (in0)
	call	prtXYbyteB
	ld	bc, #0x1013
	ld	a, (in1)
	call	prtXYbyteB

	    ; navigation info
	ld	a, #C_purple
	ld	bc, #0x001d
	ld	hl, #.tm01inst
	call	putstrB

	ld	a, #C_purple
	ld	bc, #0x001f
	ld	hl, #.tm02inst
	call	putstrB

	ret

.tmtimer:
	.byte	5
	.ascii	"Timer"

.tmdip:
	.byte	3
	.ascii	"Dip"

.tminput0:
	.byte	7
	.ascii	"Input 0"

.tminput1:
	.byte	7
	.ascii	"Input 1"

.tm0title:
	.byte	13
	.ascii	"System inputs"

.tmplayer1:
	.byte 	2
	.ascii 	"P1"

.tmplayer2:
	.byte 	2
	.ascii 	"P2"

.tmcoins:
	.byte 	4
	.ascii 	"Coin"

.tm01inst:
	.byte	28
	.ascii  "Move joystick "
	.byte	0x0d, 0x20, 0x0d, 0x20, 0x0d
	.ascii	" for NEXT"

.tm02inst:
	.byte	28
	.ascii  "Move joystick "
	.byte	0x0c, 0x20, 0x0c, 0x20, 0x0c
	.ascii	" for PREV"


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; screen 1 - dip switches

tm_scr1:
	ld	hl, #.tm1title
	ld	bc, #0x0000
	ld	a, #C_red
	call	putstrB

	ld	a, (dsw0)
	ld	bc, #0x0204

	    ; coinage
	bit	#0, a
	call	.onoff
	inc	c
	bit	#1, a
	call	.onoff

	    ; lives
	inc	c
	inc	c
	bit	#2, a
	call	.onoff
	inc	c
	bit	#3, a
	call	.onoff

	    ; bonus
	inc	c
	inc	c
	bit	#4, a
	call	.onoff
	inc	c
	bit	#5, a
	call	.onoff

	    ; difficulty
	inc	c
	inc	c
	bit	#6, a
	call	.onoff

	    ; names
	inc	c
	inc	c
	bit	#7, a
	call	.onoff

	; now print the appropriate text
	    ; coinage
	ld	a, (dsw0)
	and	#0x03
	ld	hl, #(.tm_dip_free)
	cp	#0x00
	jr	z, .tm_dip_coinage

	ld	hl, #(.tm_dip_1c1c)
	cp	#0x01
	jr	z, .tm_dip_coinage

	ld	hl, #(.tm_dip_1c2c)
	cp	#0x02
	jr	z, .tm_dip_coinage

	ld	hl, #(.tm_dip_2c1c)
.tm_dip_coinage:
	ld	bc, #0x0704
	ld	a, #C_white
	call	putstrB

	    ; loves
	ld	a, (dsw0)
	and	#0x0c
	ld	hl, #.tm_dip_1life
	cp	#0x00
	jr	z, .tm_dip_loves

	ld	hl, #.tm_dip_2life
	cp	#0x04
	jr	z, .tm_dip_loves

	ld	hl, #.tm_dip_3life
	cp	#0x08
	jr	z, .tm_dip_loves
	
	ld	hl, #.tm_dip_5life
.tm_dip_loves:
	ld	bc, #0x0707
	ld	a, #C_white
	call	putstrB

	    ; bones
	ld	a, (dsw0)
	and	#0x30

	ld	hl, #.tm_dip_10bones
	cp	#0x00
	jr	z, .tm_dip_bones

	ld	hl, #.tm_dip_15bones
	cp	#0x10
	jr	z, .tm_dip_bones

	ld	hl, #.tm_dip_20bones
	cp	#0x20
	jr	z, .tm_dip_bones

	ld	hl, #.tm_dip___bones
.tm_dip_bones:
	ld	bc, #0x070a
	ld	a, #C_white
	call	putstrB

	    ; diffeycults
	ld	a, (dsw0)
	and	#0x40
	ld	hl, #.tm_dip_normal
	cp	#0x40
	jr	z, .tm_dip_diff

	ld	hl, #.tm_dip_hard
.tm_dip_diff:
	ld	bc, #0x070d
	ld	a, #C_white
	call	putstrB

	    ; nombre
	ld	a, (dsw0)
	and	#0x80
	ld	hl, #.tm_dip_names
	cp	#0x80
	jr	z, .tm_dip_nomen

	ld	hl, #.tm_dip_nombres
.tm_dip_nomen:
	ld	bc, #0x070f
	ld	a, #C_white
	call	putstrB


	ret


.tm_dip_free:
	.byte	18
	.ascii	"Free Play         "
.tm_dip_1c1c:
	.byte	18
	.ascii	"1 coin - 1 credit "
.tm_dip_1c2c:
	.byte	18
	.ascii	"1 coin - 2 credits"
.tm_dip_2c1c:
	.byte	18
	.ascii	"2 coins - 1 credit"

.tm_dip_1life:
	.byte	18
	.ascii	"1 life per credit "
.tm_dip_2life:
	.byte	18
	.ascii	"2 lives per credit"
.tm_dip_3life:
	.byte	18
	.ascii	"3 lives per credit"
.tm_dip_5life:
	.byte	18
	.ascii	"5 lives per credit"

.tm_dip_10bones:
	.byte	15
	.ascii	"bonus at 10,000"
.tm_dip_15bones:
	.byte	15
	.ascii	"bonus at 15,000"
.tm_dip_20bones:
	.byte	15
	.ascii	"bonus at 20,000"
.tm_dip___bones:
	.byte	15
	.ascii	"no bonus       "

.tm_dip_normal:
	.byte	6
	.ascii	"normal"
.tm_dip_hard:
	.byte	6
	.ascii	"hard  "

.tm_dip_names:
	.byte	15
	.ascii	"original names "
.tm_dip_nombres:
	.byte	15
	.ascii	"alternate names"

.onoff:
	push	af
	ld	hl, #.tm1off
	ld	a, #C_red
	jr	z, .onon
	call	putstrB
	pop	af
	ret
.onon:
	ld	hl, #(.tm1on)
	ld	a, #C_green
	call	putstrB
	pop	af
	ret
	

.tm1off:
	.byte	3
	.ascii	"off"

.tm1on:
	.byte	3
	.ascii	"on "

.tm1title:
	.byte 	18
	.ascii 	"Dipswitch Settings"


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; screen 2 - palette, sprites

tm_scr2:
	ld	hl, #.tm2title
	ld	bc, #0x0000
	ld	a, #C_red
	call	putstrB

	    ; draw the pieces in the stripes
	ld	hl, #vidram+0x00c0
	ld	a, #163
	ld	b, #0x20
	call	memset256
	ld	a, #162
	ld	b, #0x20
	call	memset256
	ld	a, #161
	ld	b, #0x20
	call	memset256
	ld	a, #160
	ld	b, #0x20
	call	memset256

	    ; draw the colors
	ld	hl, #colram+0x00c0
	ld	b, #0x80		; 4 columns, 0x20 each
	xor	a			; a=0
.tm2_000:
	ld	(hl), a		; we're counting on the pallete wrap at 0x1f
	inc	a
	inc	hl
	djnz	.tm2_000

	    ; draw the color values
	ld	b, #0x20		; 1 column, 0x20 long
    	xor	a			; a=0
.tm2_001:
	push	bc
	ld	b, #0x19
	ld	c, a			; (x,y) = (0x18, A)
	ld	e, #C_white
	call	prtXYbyteB		; print the digit (right)
	ld	b, #0x13
	call	prtXYbyteB		; print the digit (left)
	inc	a
	pop	bc
	djnz	.tm2_001

	; now display some sprites

	ld	bc, #0xeeee		; XY
	ld	de, #sprite0		; sprite slot 0
	ld	hl, #.tm2s0		; cells 0
	call	dsprite

	ld	bc, #0xdddd		; XY
	ld	de, #sprite1		; sprite slot 1
	ld	hl, #.tm2s1		; cels
	call	dsprite

	ld	bc, #0xcccc		; XY
	ld	de, #sprite2		; sprite slot 2
	ld	hl, #.tm2s0		; cels
	call	dsprite

	ld	bc, #0xa0a0		; XY
	ld	de, #sprite3		; sprite slot 3
	ld	hl, #.tm2s1		; cels
	call	dsprite

	ld	bc, #0x9c9c		; XY
	ld	de, #sprite4		; sprite slot 4
	ld	hl, #.tm2s0		; cels
	call	dsprite

	ld	bc, #0x9898		; XY
	ld	de, #sprite5		; sprite slot 5
	ld	hl, #.tm2s1		; cels
	call	dsprite

	ret

.tm2title:
	.byte 	16
	.ascii 	"Palette, Sprites"

.tm2s0:
	.byte	0x05, 1*4, 1*4, 1*4, 1*4
.tm2s1:
	.byte	0x09, 1*4, 1*4, 1*4, 1*4


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; screen 3 - crosshatch

tm_scr3:
	;ld	hl, #(vidram)
	;ld	a, #0x0e
	;ld	b, #0x04
	;call	memsetN

	ld	hl, #(vidram)
	call	.tm3_top
	call	.tm3_bot
	ld	b, #14
.tm3_000:
	push	bc
	call	.tm3_rt
	call	.tm3_lt
	pop	bc
	djnz	.tm3_000
	call	.tm3_top
	call	.tm3_bot

	ld	hl, #(colram)
	ld	a, #0x06
	ld	b, #0x04
	call	memsetN

	ret

.tm3_loop:
	ld	b, #0x10
.tm3_l0:
	ld	a, d
	ld	(hl), a
	inc	hl
	ld	a, e
	ld	(hl), a
	inc	hl
	djnz	.tm3_l0
	ret


.tm3_top:
	ld	de, #0x100E
	call	.tm3_loop
	ret

.tm3_bot:
	ld	de, #0x110f
	call	.tm3_loop
	ret

.tm3_rt:
	ld	de, #0x1011
	call	.tm3_loop
	ret

.tm3_lt:
	ld	de, #0x0e0f
	call	.tm3_loop
	ret


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; screen 4,5,6 - colors

tm_scr4:
	ld	a, #0x05		; red
	jr	.tm_456
tm_scr5:
	ld	a, #0x13		; green
	jr	.tm_456

tm_scr6:
	ld	a, #0x1e		; blue
.tm_456:
	ld	hl, #(colram)
	ld	b, #0x04
	call	memsetN

	ld	hl, #(vidram)
	ld	a, #0x5e		; character after ]
	ld	b, #0x04
	call	memsetN
	ret


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; screen 7 - game list, credits

tm_scr7:
	ld	hl, #.tm7title
	ld	bc, #0x0000
	ld	a, #C_red
	call	putstrB

	    ; credits
	
	ld	a, #C_yellow
	ld	hl, #VERSION		; BL Game Kernel v0.0x
	ld	c, #0x1b
	call	textcenter
	call	putstrB

	ld	a, #C_orange
	ld	hl, #.name		; Scott Lawrence
	ld	c, #0x1d
	call	textcenter
	call	putstrB

	ld	hl, #ssTemail		; yorgle@gmail.com
	ld	c, #0x1f
	call	textcenter
	call	putstrB

	ld	hl, #.cprt		; (c)2006 UmlautLlama.com
	ld	c, #0x03
	call	textcenter
	call	putstrA



	    ; headings
	ld	a, #C_blue

	ld	hl, #.tm7Ttask
	ld	bc, #0x0103
	call	putstrB

	ld	hl, #.tm7Tname
	ld	bc, #0x0c03
	call	putstrB

	ld	hl, #.tm7Tcreds
	ld	bc, #0x1703
	call	putstrB


	    ; entries
	ld	hl, #(tasklist)		; <= FTASKS
	xor	a
.tms7_001:
	push	af			; so we can restore it at the end

	; select the right color into E
	push	af
	ld	e, #C_green
	add	a			; a=a*2
	cp	#(NTASKS)
	jr	c, .tms7_002
	ld	e, #C_dgreen
.tms7_002:
	pop	af


	; select the right position
	push	de
	push	af

	ld	c, a
	add	a, #0x05		; 5 rows down, every other line
	add	a, c
	add	a, c			; a = 5 * (a*3)

	ld	c, a			; c = 5 * (a*3)
	
	pop	af

	; task ID number to screen
	ld	b, #0x03
	call	prtXYbyteB		; print the digit

	; select the right title text
	ld	b, #0x06		; adjust position
	ld	hl, #(tasklist)
	ld	d, #0x00
	push	af
	add	a
	ld	e, a			; DE = current offset
	pop	af
	rst	0x10			; HL points to current task
	push	hl
	pop	ix
	ld	de, #TITITLE		; title offset
	rst	0x10			; HL points to string

	pop	de			; restore the color
	ld	a, e			; restore the color
	call	putstrB			; print the text

	; N credits
	push	ix
	pop	hl			; restore the task pointer
	ld	de, #TICRED
	add	hl, de
	ld	b, #0x19		; adjust horizontal position
	ld	e, a			; restore the color
	ld	a, (hl)
	cp	#0x00
	call	nz, prtXYbyteB		; print it
	call	z, .tm7free

	; version info

		; major
	inc	c			; next row


	push 	ix
	pop	hl
	ld 	de, #TIVERMJ
	add	hl, de			; index of major version number
	ld	a, (hl)
	ld 	b, #0x0d		; x
	ld	e, #0x09		; brown
	call	prtXYbyteB		; print it

		; minor
	push 	ix
	pop	hl
	ld 	de, #TIVERMN
	add	hl, de
	ld	a, (hl)
	ld	b, #0x10		; x
	ld	e, #0x09		; brown
	call	prtXYbyteB		; print it

		; embelishments
		; v
	ld	hl, #.tm7Tver
	ld	a, #0x09		; brown
	ld 	b, #0x0b		; 00 00 -> v00 00
	call	putstrB

		; .
	ld	hl, #.tm7Tdot
	ld 	b, #0x0e		; v00 00 -> v00.00
	call	putstrB

	; finish up - increment and check for done-ness
	pop	af
	inc 	a
	cp	#(FTASKS)
	jr	nz, .tms7_001

	ret

.tm7free:
	ld	hl, #.tm7Tfree
	ld	a, e
	ld	b, #0x17
	call	putstrB
	ret
	
.tm7title:
	.byte 	15
	.ascii 	"Installed Tasks:"

.tm7Ttask:
	.byte	4
	.ascii	"Task"

.tm7Tname:
	.byte	4
	.ascii	"Name"

.tm7Tdot:
	.byte	1
	.ascii	"."

.tm7Tver:
	.byte	1
	.ascii	"v"

.tm7Tcreds:
	.byte	4
	.ascii	"Cred"

.tm7Tfree:
	.byte	4
	.ascii	"Free"

TESTMODE_SZ = .-TESTMODE_START
