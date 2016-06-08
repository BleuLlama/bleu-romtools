;
; multigame hack - multiple mspac games within a single romset, with user menu
;
; $Id: mpdmspac.asm,v 1.44 2007/09/02 19:48:09 sdl Exp $ 

.area 	.CODE (ABS)

; 0.4 - basic visual layout, menu done
; 0.5 - all hacks implemented
; 0.6 - map color to zero when paused
; 0.7 - beginnings of Mode J
; 0.8 - buy-in implemented with timeout RC0
; 009 - added more dog kicking for real hardware
;     - fixed P1/P2 buy-in text
;     - changed version format
;     - fixed the crash bug wit the cheat
;     - found a hook for map color
; 010 - screen turns off and back on with pellets/fruit eaten
;     - ghosts toggle random movement
;  11 - fixed (sorta) the map 5 bug
;     - added in three more map modes (Attack, Plus, Remix)
;     - added order hack
;  12 - fixed clear screen for attract mode on "Game Over"
;     - Mode J now uses all of the maps from all variants
;  13 - horizontal support
;  14 - C000-safe patch
;  15 - Backward added, map hacks updated to the new mechanism
;     - P1/P2 start to reset menu screen (if credits dropped)

; NOTE: This file should be assembled with 'asz80', which is 
;	in the 'zcc 0.96' package.  It uses the ".bound" extension
;	written by myself, which notifies you if you go past a boundary

;  TODO:
;	reorganize and clean up the code; 
;	- move all of the text strings to be in order, together
;	- move all of the code to either 
;		- be out in A000,B000 with test mode (boot7,8)
;		- be in the base code area (boot1-6)
;	- figure out a way to make the multiple versions available easily
;	- figure out what that last item means

; allow more than just the base game?
extragames = 1

; for home-use or on-route use?
homeuse = 1

; is it a horizontal version?
horizontal = 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Features:
;
;   Map variants:
;	original
;	Ms Pac Attacks
;	Miss Pac Plus
;
;   Mode J: (new game style)
;	maze disappears
;	re-appears after eating 4 ghosts or fruit
;	player is always fast
;	pause is disabled
;	maze selection is random from Attack and Original maze sets
;	maze order is random
;	maze color is reasonably random
;	
;   Other Features:
;	buy-in to continue games (adds 1 point each time)
;	On-Route or Home-Use selectable at build time
;	horizontal hack support selectable at build time
;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; space use:
; 0068 - 008c	37	unused
; 0f3c - 0fff	195	start screen menu code
; 1fa9 - 1fff	87	unused
; 2fba - 2fff	70	ram reset patch, 2 text strings
; 3006 - 30c0   187	most text strings
; 30fb - 3173		text string overflow 2
; 3193 - 32f8		Hack implementations, menu color, Mode J
; 3a4f - 3b07		Buy-In code
; 3b19 - 3b2f		Buy-In continuation
; 3ce0 - 3cff	31+-	order hack



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; some hardware defines

        watchdog = 0x50c0       ; watchdog timer
        in0      = 0x5000       ; input 1: p1 inputs
        in1      = 0x5040       ; input 2: coin inputs
	sounden  = 0x5001	; sound enable
	inten	 = 0x5000	; interrupt enable
        vidram   = 0x4000       ; video ram base
        colram   = 0x4400       ; color ram base
	sprites  = 0x4ff0	; sprite control registers
        letter   = 0x4a
        offs1    = 0x20
        offs2    = 0x40

	playerup   = 0x4e09
	credram    = 0x4e6e
	livesleft  = 0x4e14
	livesdisp  = 0x4e15
	livesstart = 0x4e6f

        p1_port         = in0
        p1_up           = 0
        p1_left         = 1
        p1_right        = 2
        p1_down         = 3

        start_port      = in1
        start1          = 5
        start2          = 6

; mspac code hooks

	textout	= 0x2c5e

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; stack adjustments (we use the top 4 bytes for our own use...)
;   (guaranteed to not be used or cleared when running after these hacks)

	stack	= 0x4fc0

; here's how we're gonna use the set-aside RAM
;  NOTE: manually keep R* and h_* flags in sync with text options

	gamesel = stack-1
	    Rmspac	= 0x00
	    Rrmspac	= 0x01
	    Rmspacatk	= 0x02
	    Rmspacplus	= 0x03
	    Rmspacrmx	= 0x04
	    RmodeJ	= 0x05

	hackflags = stack-2
	    h_normal	= 0x00
	    h_fast	= 0x01
	    h_pause	= 0x02
	    h_fpause	= 0x03
	    h_cheat	= 0x04

    ; for our input routine
	thisin0 = stack-3
	lastin0 = stack-4
	trans0  = stack-5

    ; for screen coloring
	scrcolor = stack-3

    ; for our random seed
	rseed   = stack-6

    ; which we'll also use for our timer
	timerD  = colram-1 ;stack-3
	timerE  = colram-2 ;stack-4

    ; set aside this many bytes for our own use (above)
	forus	= 6


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; stack adjustment patches

.org 0x234e
	ld	sp, #(stack-forus)	; main program init fix

.org 0x30bd
	;ld	sp, #(stack-forus)	; in test routine (skipped)

.org 0x3185
	ld	sp, #(stack-forus)	; main test init

.org 0x327c
	;ld	sp, #(stack-forus)	; in test grid draw routine (skipped)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ram clear patches

; these shift around memory such that we can store aside a 
;  few bytes for our own use.

.org 0x2359
	jp	.rcpat0
	.byte	0x00, 0x00, 0x00, 0x00
	.byte	0x00, 0x00, 0x00, 0x00
.rcpatR:

.org 0x2fba
.rcpat0:
	ld	de, (stack-2)
	ld	ix, (stack-4)

	    ; original code
	ld	hl, #0x4c00
	ld	b, #0xbe
	rst	#8
	rst	#8
	rst	#8
	;rst	#8

	ld	(stack-4), ix
	ld	(stack-2), de
	jp	.rcpatR


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; startup test removal patches (HACK7)

.org	0x318c	; skip the crosshatch display
	ld	sp, #(stack-forus)
	jp	0x234b		; to game loop

.org	0x01d1
	.byte	0x00, 0x00, 0x00	; disable test switch reset 

.org	0x3000	; skip the startup
	ld	sp, #(stack-forus)
	jp	0x30c1

.org	0x30f0
	.byte	0x00, 0x00, 0x00	; skip checksum final

.org	0x30f3
	.byte	0x00, 0x00		; 
	.byte	0x00, 0x00, 0x00	; disable "MEMORY OK"


	; change the map ordering
.org	0x94df
	;.byte 2
	;.byte 3, 2, 1, 0
	;.byte 3, 2, 1, 0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; start screen additions

; sounds: 4ebc
;	0x01	gobble light
;	0x02	chlamp
;	0x04	fruit eat
;	0x08	ghost eat

; sounds: 4e9c
;	0x01	alert sound
;	0x02	coin gobble

	; left/right variant sound
	sndregA = 0x4ebc
	sndA	= 0x04

	; up/down hack sound
	sndregB = 0x4ebc
	sndB	= 0x01


.org 0x061b
	jp startext
startret:

    ;; Start Extension
    ;  - checks for the user joystick input
    ;  - save the new control input aside
    ;  - display the two items based on the above
.org 0x0f3c

startext:
	ld	b, #0x18
	call	textout		; MOVE JOYSTICK AND
	ld	b, #0x19
	call	textout		; SELECT VARIANT THEN

	xor	a
	ld	(watchdog), a

	ld	a, (p1_port)
	ld	(thisin0), a
	ld	b, a
	ld	a, (lastin0)
	xor	b		; a now contains the different ones
	ld	b, a		; store it aside in b
	ld	a, (lastin0)	; reload
	and	b		; a now contains only the ones active now
	ld	b, a		; back it up for later
				
.if( extragames )
	call 	.inright
	call	.inleft
.endif
	xor	a
	ld	(watchdog), a

	ld	a, (gamesel)
	cp	#RmodeJ
	jr	z, st000

	ld	a, b
	call	.inup
	call	.indown

st000:
	ld	a, (thisin0)
	ld	(lastin0), a

	xor	a
	ld	(watchdog), a

.if( extragames )
	    ; display text for left-right options
	ld	a, (gamesel)
	ld	hl, #(lrtable)
	rst	#0x10
	ld	b, a
	call	textout
.endif
	xor	a
	ld	(watchdog), a

	    ; skip options if we're Mode J
	ld	a, (gamesel)
	cp	#RmodeJ
	jr	nz, st001

	ld	b, #0x29
	call	textout

	jr	st002
st001:
	    ; display text for up-down options
	ld	a, (hackflags)
	ld	hl, #(udtable)
	rst	#0x10
	ld	b, a
	call	textout

st002:
	xor	a
	ld	(watchdog), a

	    ; store aside the current 'r'
	ld	a, r
	ld	(rseed), a

	; add in an extra little reset for home use...
	; many people leave their machine on regular coin drops, and 
	; people come over to their house, drop in oodles of credits,
	; play two games, and walk away, leaving it on the "press start"
	; screen.  This lets you walk over to the machine, hold down both
	; start buttons, and reset the credits.
.if( homeuse )
	    ; check for p1 p2 start both held down
	ld	de, #0x0000
st004:
	ld     	a, (start_port)	; IN1
	bit	start2, a
	jp	nz, st003
	bit	start1, a
	jp	nz, st003

	xor	a			; kick the dog
	ld	(watchdog), a

	inc	de			; inc the counter
	ld	a, d
	cp	#0xff			; cc00 = about 3 seconds
	jr	nz, st004

	    ; user held down the buttons for the first segment; display text
	ld	b, #0x29		; print "RESETTING"
	call	textout
	ld	b, #0x2e
	call	textout

	    ; give the user one last chance to let go of the controls
st005:
	ld	a, (start_port) 	; in1
	bit	start2, a
	jp	nz, st003
	bit	start1, a
	jp	nz, st003

	xor	a			; kick the dog
	ld	(watchdog), a

	inc	de			; inc the counter
	ld	a, d
	cp	#0x80			; 8000 = about 2 seconds
	jr	nz, st005

	rst	#0x00			; reset the game
.endif

.bound 0x0fff
.org 0x9800
	
st003:
	    ; resume old code
	call 	#0x2ba1
	jp 	startret

.inright:
	push	af 			; transition detect
	bit	#p1_right, a
	jr	z, .irl0

	    ; INC
	ld	a, (gamesel)
	cp	#maxLR
	jr	z, rrr0
	inc	a
	jr	rrr1
rrr0:	xor	a
rrr1:	ld	(gamesel), a

.irls0:
	ld	a, #sndA
	ld	(sndregA), a		; play a sound
.irl0:
	pop	af
	ret


.inleft:
	push	af
	bit	#p1_left, a
	jp	z, .irl0

	    ; DEC
	ld	a, (gamesel)
	cp	#0
	jr	z, lll0
	dec	a
	jr	lll1
lll0: 	ld	a, #maxLR
lll1: 	ld	(gamesel), a

	jp	.irls0			; play a sound


.inup:
	push	af
	bit	#p1_up, a
	jr	z, .iud0

	    ; DEC
	ld	a, (hackflags)
	cp	#0
	jr	z, uuu0
	dec	a
	jr	uuu1
uuu0: 	ld	a, #maxUD
uuu1: 	ld	(hackflags), a

.iuds0:
	ld	a, #sndB
	ld	(sndregB), a		; play a sound
.iud0:
	pop	af
	ret


.indown:
	push	af
	bit	#p1_down, a
	jp	z, .iud0

	    ; INC
	ld	a, (hackflags)
	cp	#maxUD
	jr	z, ddd0
	inc	a
	jr	ddd1
ddd0:	xor	a
ddd1:	ld	(hackflags), a

	jp	.iuds0			; play a sound


; should not go past 0x0fff !
.bound 0x9fff


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; change positioning of some of the older text entries

.org 0x3786
	.byte	0xe9	; PUSH START BUTTON
.org 0x379d
	.byte	0xac	; 1 PLAYER ONLY
.org 0x37b1
	.byte	0xac	; 1 OR 2 PLAYERS

; color of "ADDITIONAL..."
.org 0x3d1e
	.byte	0x89

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; new text table entries

.org 0x3006

    ; string triple dereferencing tables!
    ; the string indexes to use for each incremental left-right menu option
lrtable: ; gamesel
	.byte	0x1c		; original
	.byte	0x2d		; reverse
	.byte	0x2a		; attack
	.byte	0x2b		; plus
	.byte	0x2c		; remix
	.byte	0x1d		; hyper
	.byte	0x29
	maxLR	= 5

    ; the string indexes to use for each incremental up-down menu option
udtable: ; hackflags
.if( homeuse )
	.byte 	0x1a		; normal
	.byte	0x1b		; fast
	.byte	0x20		; pause
	.byte	0x21		; fast pause
	.byte	0x1f		; s1s2 cheat
	maxUD	= 4
.else
	.byte 	0x1a		; normal
	.byte	0x1b		; fast
	maxUD	= 1
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; string offset table
t_18:
	.byte 0xe5, 0x02
.if( horizontal )
	.ascii "DNA@KCITSYOJ@EVOM"
.else
	.ascii "MOVE@JOYSTICK@AND"
.endif
	.byte 0x2f
	.byte 0x87

t_19:
	.byte 0x07, 0x03
.if( horizontal )
	.ascii "NEHT@TNAIRAV@TCELES"
.else
	.ascii "SELECT@VARIANT@THEN"
.endif
	.byte 0x2f
	.byte 0x87

t_1a:
	.byte 0xb2, 0x02
	.byte 0x0a
.if( horizontal )
	.ascii "@@@LAMRON@@@"
.else
	.ascii "@@@NORMAL@@@"
.endif
	.byte 0x0b
	.byte 0x2f
	.byte 0x85

t_1b:
	.byte 0xb2, 0x02
	.byte 0x0a
.if( horizontal )
	.ascii "@@@@TSAF@@@@"
.else
	.ascii "@@@@FAST@@@@"
.endif
	.byte 0x0b
	.byte 0x2f
	.byte 0x85

t_1c:
	.byte 0xb0, 0x02
	.byte 0x0c
.if( horizontal )
	.ascii "@@LANIGIRO@@"
.else
	.ascii "@@ORIGINAL@@"
.endif
	.byte 0x0d
	.byte 0x2f
	.byte 0x85


t_1d:
	.byte 0xb0, 0x02
	.byte 0x0c
.if( horizontal )
	.ascii "@EDOM@REPYH@"
.else
	.ascii "@HYPER@MODE@"
.endif
	.byte 0x0d
	.byte 0x2f
	.byte 0x85

t_1e:	; copyright addition (version, url)
	.byte 0x7f, 0x03
.if( horizontal )
.if( homeuse )
	.ascii "H"
.else
	.ascii "R"
.endif
	.ascii "51@DPM"			; centered under Midway logo
.else
	.ascii "MPD@15"			; centered under Midway logo
.if( homeuse )
	.ascii "H"
.else
	.ascii "R"
.endif
.endif
.if( horizontal )
	.ascii "@@MOC%AMALLTUALMU"	; centered under copyright text
.else
	.ascii "@@UMLAUTLLAMA%COM"	; centered under copyright text
.endif
	.byte 0x2f
	.byte 0x81

t_1f:
	.byte 0xb2, 0x02
	.byte 0x0a
.if( horizontal )
	.ascii "@TAEHC@2S1S@"
.else
	.ascii "@S1S2@CHEAT@"
.endif
	.byte 0x0b
	.byte 0x2f
	.byte 0x85

; this should not reach past 0x30c0
.bound	0x30c0

.org 0x2fd8
t_20:
	.byte 0xb2, 0x02
	.byte 0x0a
.if( horizontal )
	.ascii "@@@ESUAP@@@@"
.else
	.ascii "@@@PAUSE@@@@"
.endif
	.byte 0x0b
	.byte 0x2f
	.byte 0x85

t_21:
	.byte 0xb2, 0x02
	.byte 0x0a
.if( horizontal )
	.ascii "@ESUAP@TSAF@"
.else
	.ascii "@FAST@PAUSE@"
.endif
	.byte 0x0b
	.byte 0x2f
	.byte 0x85

; this should not reach past 0x2fff
.bound	0x2fff


.org 0x30fb

t_22:
	.byte 0x52, 0x02
	.ascii "@@@@@@"
	.byte 0x2f
	.byte 0x80

t_23:
	.byte 0x52, 0x02
.if( horizontal )
	.ascii "DESUAP"
.else
	.ascii "PAUSED"
.endif
	.byte 0x2f
	.byte 0x81

t_24:
	.byte 0x52, 0x02
.if( horizontal )
	.ascii "DESUAP"
.else
	.ascii "PAUSED"
.endif
	.byte 0x2f
	.byte 0x89

t_25:
	.byte 0x8a, 0x02
.if( horizontal )
	.ascii "NOIC@TRESNI"
.else
	.ascii "INSERT@COIN"
.endif
	.byte 0x2f
	.byte 0x85

t_26:
	.byte 0x8a, 0x02
.if( horizontal )
	.ascii "TRATS@SSERP"
.else
	.ascii "PRESS@START"
.endif
	.byte 0x2f
	.byte 0x85

t_27:
	.byte 0x8e, 0x02
.if( horizontal )
	.ascii "RUNITNOC@OT"
.else
	.ascii "TO@CONTINUE"
.endif
	.byte 0x2f
	.byte 0x85

t_28:
	.byte 0x91, 0x02
.if( horizontal )
	.ascii "STROBA@EVOM"
.else
	.ascii "MOVE@ABORTS"
.endif
	.byte 0x2f
	.byte 0x83

; this should not reach past 0x3173
.bound	0x3173

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; some patches on the text dereference table

.org 0x36d5
	.word	t_18, t_19, t_1a, t_1b, t_1c, t_1d, t_1e, t_1f
	.word	t_20, t_21, t_22, t_23, t_24, t_25, t_26, t_27
	.word	t_28, t_29, t_2a, t_2b, t_2c, t_2d, t_2e;, t_2f


; table entries are defined as:
;   .byte (offs l), (offs h)    ; so an offset of #0234 would be #34, #02
;   .ascii "STRING"
;   .byte #2f                   ; termination with 2f
;   .byte colordata:       
;       if the color data byte's high bit (#80) is set, the entire string
;       gets colored with (colordata & 0x7f)
;       if the color data byte's high bit is not set, then:
;       .byte   ncolors         ; number of bytes to set color
;       .byte   color1          ; first character's color
;       .byte   color2          ; second character's color
;               ...             ; etc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; copyright patch (asthetic)

.org 0x964e
	jp	logoshift	; move the logo over to the left a little
lsret:


.org 0x967a
	jp	cprtpatch	; our text addition patch


    ; shift the logo over the left a little
.org 0x97c4
logoshift:
	ld	de, #0x0040
	add	hl, de
	ld	de, #0x001d
	jp	lsret


    ; add our own text string to the copyright stuff
cprtpatch:
	    ; patched code
	jp	nz, #0x9654

	    ; new code
	ld	b, #0x1e
	call	textout
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; the hack implementations


; invincibility hack vaguely based on Dock Cutlip's code
.org 0x1764
	jp	invEntry

.org 0x1809
    	jp	fastEntry

.org 0x3193
; 4da6 - 1 when ghost eatable, otherwise 0
; 4da4 - 2 when dead, otherwise 0
invEntry:
	ld	a, (hackflags)
	cp	#h_cheat
	jr	nz, invRet		; not active, resume

	ld	a, (start_port)
	and	#0x20			; start 1 pressed?
	jr	nz, invRet

invAct:
	ld	a, (0x4da6)
	cp	#1			; ghosts eatable?
	ret	nz			; return disables collision detect
	    ; fall through if ghosts are blue
invRet:
	ld	a, b
	ld	(#0x4da4), a
	jp	0x1767			; jp back - standard behavior

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

fastEntry:
	ld	a, (gamesel)
	cp	#RmodeJ
	jr	nz, fastHacks
	    ; mode J support
	call	FE_hyper
	ld	a, #h_fpause
	ld	(hackflags), a
	jp	pauseON

fastHacks:
	xor	a
	ld	(watchdog), a
	ld	a, (hackflags)
	cp	#h_fast
	jr	z, fastON
	cp	#h_pause
	jr	z, pauseON
	cp	#h_fpause
	jr	z, pauseON
	cp	#h_cheat
	jr	z, cheatON
fastOFF:
	ld	a, #0xff
	cp	(hl)
	jp	z, #0x1811
	dec	(hl)
	ret

cheatON:
	    ; do the fast hack
	ld	a, (start_port)
	bit	start2, a
	jr	nz, #fastOFF

fastON:
	jp	0x1845

pauseON:
	ld	a, (start_port)
	bit	start1, a
	jp	nz, fastpause
	bit	start2, a
	jp	z, fastpause	; fix the both-at-once bug

	push	de		; save de
	push	hl		; save hl
	push	af		; save af

	ld	a, (colram+0x41) ; store aside screen color
	push 	af

	ld	e, #0x00
	call	colorMap	; darken the screen somewhat

	xor	a               ; a=0
	ld	(sounden),a	; disable sound
	ld	(watchdog),a	; kick dog
	ld	(inten),a	; disable interrupts
	di			; disable interrupts

ph0:
	ld	b, #0x23
	call	textout		; print "PAUSED" color A

	ld	b, #0x24
	call	textout		; print "      "

	xor	a		; a=0
	ld     	(watchdog),a	; kick dog

	ld	b, #0x23
	call	textout		; print "PAUSED" color B

	ld	b, #0x24
	call	textout		; print "      "

	ld     	a,(start_port)	; IN1
	bit    	start2, a	; start 2
	jr     	nz, ph0		; not pressed, loop back
	bit	start1, a	; start 1
	jr	z, ph0		; pressed, loop back

	ld	b, #0x22
	call	textout		; clear "PAUSED"

	
        ; turn it back on
	ld    	a,#01		; a=1
	ld     	(inten),a	; enable interrupts
	ld     	(sounden),a	; enable sound
	ei			; enable interrupts

	pop	af		; restore the map color
	ld	e, a
	call	colorMap
	
	pop	af		; restore af
	pop	hl		; restore hl
	pop	de		; restore de

fastpause:
	    ; check for pause
	ld	a, (hackflags)
	cp	#h_fpause
	jp	z, fastON
	jr	fastOFF


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; misc functions

; colormap
;	sets only the map tiles (not pellets or text) to a color
;	input:	e - the color to set it to
;	mod:	ix, iy, bc, af
colorMap:
	ld	ix, #(vidram + 0x40)	; start it past the scoring region
	ld	iy, #(colram + 0x40)	; start it past the scoring region
	ld	b, #0xe0	; 224 bytes to set ( subtract off 0x80 total)
	ld	c, #0x04	; set them 4 times
cm0:
	ld	a, (ix)		; load the current character...
	bit	#7, a		; if bit 7 is set, we're on a map character
	jr	z, cm1		; skip low-ordered bytes
	ld	(iy), e		; set it to this color
cm1:	inc	ix		; inc vidram
	inc 	iy		; inc colram
	djnz	cm0		; inner loop (256x loop)
	xor	a
	ld	(watchdog), a	; kick the dog, just in case
	dec	c
	jr	nz, cm0		; outer loop (4x loop)
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mode J hacks

; clear the screen depending on map number and number of pellets eaten
; redisplay the screen when all four ghosts have been eaten

	pelseaten	= 0x4e0e
	boardnumber	= 0x4e13
	ghostmove	= 0x4dc1

; from the 'fast' routine
FE_hyper:
	    ; first check to see if we need to do it...
	ld	a, (pelseaten)
	and	#0x3f
	cp	#0x3f
	ret	nz

	xor 	a
	ld	(watchdog), a

	    ; toggle ghost movement logic
	ld	a, (ghostmove)
	bit	#0x0, a
	jr	nz, FE1
	and	#0xfe			; bit 0 turn off
	jr	FE_A
FE1:
	or	#0x01			; bit 0 turn on
FE_A:

FE_changeColor:
	ld	a, (colram + 0x41)
	cp	#0x00
	ret	z			; return if color == 0

	ld	e, #0x00
	call	colorMap
	
	ret

; this should not reach past 0x32f8
.bound	0x32f8

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; buy-in

.org 0x136e
	jp	buyin
buyinret:

.org 0x3a4f

buyclear:
	    ; clear screen (blacken)
	ld	hl, #(colram)
	xor	a
	ld	b, #0x00
	rst	#8
	rst	#8
	rst	#8
	rst	#8
	ret

buyin:
	ld	a, (#0x4e14)
	and	a			; check if bases==0
	jr	z, buyOffer

	    ; resume game...
buyReturn:
	call	#0x2675
	jp	buyinret

buyCReturn:
	call	buyclear	; clear the screen
	jr	buyReturn

buyOffer:
	    ; clear sprites
	xor 	a
	ld	(watchdog), a
	ld	hl, #(sprites)
	ld	b, #0x10
	rst	#8

	call	buyclear

	    ; re-light the top bar... or try to
    	ld	hl, #(colram + 0x400 - 0x40)
	ld	a, #0x0f		; color F
	ld	b, #0x40		; 0x40 bytes
	rst	#8

	    ; draw out the timer digits
	ld	a, #0x09		; yellow text
	;ld	(colram + 0x0213), a
	ld	(colram + 0x01f3), a
	xor	a
	;ld	(vidram + 0x0213), a
	ld	(vidram + 0x01f3), a

timerReset:
	ld	a, #0xff
	ld	(timerE), a
	ld	a, #0x12
	ld	(timerD), a

bo_loop:
	xor	a
	ld	(watchdog), a

	ld	a, (timerE)
	ld	e, a
	ld	a, (timerD)
	ld	d, a
	dec	de
	ld	a, e
	ld	(timerE), a
	ld	a, d
	ld	(timerD), a

	rra
	and	#0x0f
	ld	(vidram + 0x01f3), a

	    ; check for timeout
	xor	a
	cp	d
	jr	nz, notzero
	cp	e
	jr	nz, notzero
	jr	buyCReturn

notzero:
	xor	a
	ld	(watchdog), a

	ld	a, (credram)
	ld	(vidram), a	; store aside current credit number

	call 	#0x0267		; coin input/add credits routine
	call	#0x02ad		; coin input/add credits routine
	call	#0x2ba1		; draw credits on screen routine
	;call	#0x03c8
	;call	#0x2d0c		; poll the sound routines
	call	#0x2cc1		; sound poll 2

	ld	a, (playerup)
	add	a, #0x03	; 3="PLAYER 1"  4="PLAYER 2"
	ld	b, a
	;ld	b, #0x03
	call	textout		; print it out

	ld	b, #0x27
	call	textout		; "to continue"
	ld	b, #0x28
	call	textout		; "stick to cancel"

	    ; see if there's a new credit; if so, reset the timer
	ld	a, (credram)
	ld	d, a
	ld	a, (vidram)	; retrieve last credit number
	cp	d
	jr	nz, timerReset	; reset the timer!

	    ; check for user input
	ld	a, (credram)
	cp	#0x00
	jp	z, nocreds

	ld	b, #0x26
	call	textout		; "press start"

	    ; check for start button
	ld	a, (start_port)
	bit	#start1, a
	jp	z, biAccept
	jr	canccheck

; this should not reach past 0x3b07
.bound	0x3b07

.org 0x3b19
nocreds:
	ld	b, #0x25
	call	textout		; "insert coins"

canccheck:
	    ; check for cancel -- press down
	ld	a, (p1_port)
	and	#0x0f
	cp	#0x0f
	jp	nz, buyCReturn

	jp	bo_loop

; this should not reach past 0x3b2f
.bound	0x3b2f

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; l255 fix
.org 0x0a90
	jp	l255fix
	nop
l255b:

	
.org 0x8000
; l255 fix
l255fix:
	ld	a, (gamesel)
	cp	#RmodeJ
	jr	nz, l2_0
	ld	a, r
	cp	#0x0ff			; make sure it's not level 255
	jr	nz, l2_1
	dec	a
	jr	l2_1

l2_0:
	ld	a, (boardnumber)
	inc	a
	cp	#0xff			; see if we're over 254
	jr	z, l2_2			; nope, jump back
l2_1:
	ld	(boardnumber), a	; store value
l2_2:
	jp	l255b

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; buy in additional code..

biAccept:
	    ; decrease credits
	ld	a, (credram)
	dec	a
	ld	(credram), a

	    ; increase lives
	ld	a, (livesstart)
	ld	(livesleft), a
	ld	(livesdisp), a

	    ; increase score by 1.  shhh!
	    ; might as well use some existing code again..
	ld	hl, #0x0001		; pretend we're in the table
	call	#0x2a64			; add hl onto high scores

	jp	buyCReturn

; this should not reach past 0x81f0
.bound	0x80ff

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; color screen hack

.org 0x95a0
	jp	mazec
	
.org 0x8100
mazec:
	ld	b, a
	ld	a, (gamesel)
	cp	#RmodeJ
	ld	a, b
	jp	nz, 0x24e1		; not in Mode J, just return

	ld	a, (rseed)		; RANDOM COLOR!
	ld	b, a
	ld	a, (boardnumber)
	add	b
	and	#0x0f			; random between 0 and 15
	cp	#0x05			; board 5 hack
	jr	nz, mc0			; inc if 5
	inc	a
	
mc0:
	ld	hl, #(mazecolors)
	rst	#0x10			; load A with that color from the table
	ld	(scrcolor), a 		; set aside the screen color
	jp	0x24e1

; table of maze colors
mazecolors:
	.byte	0x16, 0x18, 0x19
	.byte	0x09, 0x12, 0x14, 0x15
	.byte	0x01, 0x03, 0x05, 0x07
	.byte   0x0e, 0x1d, 0x1e, 0x0f
	.byte	0x17, 0x11

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; score hack (reshow the screen)

scoreHack:
	push	af
	push	bc
	push	de
	push	ix
	push 	iy
	ld	a, (gamesel)
	cp	#RmodeJ
	jr	nz, sh0

	ld	a, b
	cp	#0x05		; ghost 4 or fruit...
	jr	c, sh0

	ld	a, (scrcolor)
	ld	e, a
	call	colorMap

	xor	a
	ld	(watchdog), a

sh0:
	pop	iy
	pop	ix
	pop	de
	pop	bc
	pop	af
sh1:
	ld	hl, #0x2b17
	jp	scoreRet


t_29:
	.byte 0xb2, 0x02
	;.ascii "@@@@@;@@;@@@@@"
	.ascii "@@@@@@@@@@@@@@"
	.byte 0x2f
	.byte 0x85


; for some reason, if 5 is not the first one, bad things happen
; this has to do with the slot number, NOT the map data... ????

; 0 - normal "tiki monster 1"
; 1 - normal "t-u maze"
; 2 - normal "tiki monster 2"
; 3 - normal "plusses"

; 4 - attack "zigzag" 	"cascading corners"
; 5 - empty
; 6 - attack "the trap"
; 7 - attack "red eye run" (city blocks)
; 8 - attack "the hook"	"skyhooks"

Oordertable:	; 0 1 2 3 
	.byte 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3

Aordertable: 	; 4 6 7 8
	.byte 4, 4, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8

Pordertable:	; 8 7 6 4
	.byte 8, 8, 7, 7, 7, 6, 6, 6, 6, 4, 4, 4, 4

Mordertable:
	.byte 7, 1, 8, 2, 6, 0, 3, 4, 2, 8, 0, 3, 6

Jordertable:
	.byte 0, 1, 2, 3, 4, 6, 7, 8, 3, 4, 6, 7, 8

t_2a:
	.byte 0xb0, 0x02
	.byte 0x0c
.if( horizontal )
	.ascii "@@@KCATTA@@@"
.else
	.ascii "@@@ATTACK@@@"
.endif
	.byte 0x0d
	.byte 0x2f
	.byte 0x85

t_2b:
	.byte 0xb0, 0x02
	.byte 0x0c
.if( horizontal )
	.ascii "@@@@SULP@@@@"
.else
	.ascii "@@@@PLUS@@@@"
.endif
	.byte 0x0d
	.byte 0x2f
	.byte 0x85

t_2c:
	.byte 0xb0, 0x02
	.byte 0x0c
.if( horizontal )
	.ascii "@@@XIM;ER@@@"
.else
	.ascii "@@@RE;MIX@@@"
.endif
	.byte 0x0d
	.byte 0x2f
	.byte 0x85

.bound 0x81f0
.org 0xbe00

t_2d:
	.byte 0xb0, 0x02
	.byte 0x0c
.if( horizontal )
	.ascii "@@DRAWKCAB@@"
.else
	.ascii "@@BACKWARD@@"
.endif
	.byte 0x0d
	.byte 0x2f
	.byte 0x85

t_2e:
	.byte 0xb0, 0x02
.if( horizontal )
	.ascii "@@GNITTESER@@@"
.else
	.ascii "@@RESETTING@@@"
.endif
	.byte 0x2f
	.byte 0x85

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ordering mappings

        ; normal order
        ;       for original and attack
nmOrder:        ; ( also at 0x94df, but we're ignoring that.)
        .byte   0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3
        ; original: bank 0
        ;   attack: bank 1

        ; reverse order
        ;       for plus and reverse    
rvOrder:
        .byte   3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0
        ; reverse: bank 0
        ;    plus: bank 1

        ; remix order
rmOrder:
        .byte   2, 3, 3, 0, 1, 2, 0, 1, 0, 2, 1, 3, 3   ; board & 0x07
        ; remix: bank = (board & 0x01) xor 1

        ; mode J order 
mjOrder:
        .byte   0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 0   ; board & 0x07
        ; modej: bank = (board & 0x01) xor 1

.bound 0xbf00

;;;;;;;;;;;;;;; entry hooks

;;; maze select
.org 0x946a
        call    sel_maze

;;; pellet map select
.org 0x9485
        call    sel_pel

;;; pellet count select
.org 0x94a2
        call    sel_npel

;;; pellet position select
.org 0x9504
        call    sel_ppel

;;; fruit path select
.org 0x8784
;       call    sel_fruit

;;; order select
.org 0x94c6
        call      orderHack             ; to select a new map

;; board selection dereference hack
.org 0x94bd
        call    boardHack



.org 0xa100

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; map order table hack code:
orderHack:
            ; a little wrapper to make coding easier below...
        push    af
        call    oh2
        pop     af
        ret

oh2:
        ld      a, (gamesel)     ; orange
            ;; check for reverse order
        ld      hl, #rvOrder
        cp      #Rrmspac		; reverse original
        ret     z
        cp      #Rmspacplus		; mspacplus
        ret     z

            ;; check for remix order
        ld      hl, #rmOrder
        cp      #Rmspacrmx		; remix
        ret     z

            ;; check for mode j order
        ld      hl, #mjOrder
        cp      #RmodeJ			; mode J
        ret     z

            ;; assume normal order, return
        ld      hl, #nmOrder
        ret

.bound 0xa400

.org 0x2a60
	jp	scoreHack
scoreRet:

.org 0xa400
;;;;;;;;;;;;;;; selector tables

;;;;;;;;;;;;;;; mazes
mazes:
        .word   mazes0
        .word   mazes1

mazes0:
        .byte   0xc1, 0x88      ; 0 0
        .byte   0xae, 0x8b      ; 0 1
        .byte   0xa8, 0x8e      ; 0 2
        .byte   0x79, 0x91      ; 0 3

mazes1:
        .byte   0xc1, 0xa8      ; 1 0
        .byte   0xae, 0xab      ; 1 1
        .byte   0xa8, 0xae      ; 1 2
        .byte   0x79, 0xb1      ; 1 3

;;;;;;;;;;;;;;; maze pellets
pels:
        .word   pels0
        .word   pels1

pels0:
        .byte   0x3b, 0x8a
        .byte   0x27, 0x8d
        .byte   0x18, 0x90
        .byte   0xec, 0x92

pels1:
        .byte   0x3b, 0xaa
        .byte   0x27, 0xad
        .byte   0x18, 0xb0
        .byte   0xec, 0xb2

;;;;;;;;;;;;;;; maze number of pellets
npels:
        .word   npels0
        .word   npels1

npels0:
        .byte   0x2c, 0x8b
        .byte   0x17, 0x8e
        .byte   0x09, 0x91
        .byte   0xf9, 0x93

npels1:
        .byte   0x2c, 0xab
        .byte   0x17, 0xae
        .byte   0x09, 0xb1
        .byte   0xf9, 0xb3

;;;;;;;;;;;;;;; maze pellet locations
peloc:
        .word   peloc0
        .word   peloc1

peloc0:
        .byte   0x35, 0x8b
        .byte   0x20, 0x8e
        .byte   0x12, 0x91
        .byte   0xfa, 0x93

peloc1:
        .byte   0x35, 0xab
        .byte   0x20, 0xae
        .byte   0x12, 0xb1
        .byte   0xfa, 0xb3


;;;;;;;;;;;;;;; fruit paths
fruits:
        .word   fruits0
        .word   fruits1

fruits0:
        .byte   0x4b, 0x8b
        .byte   0x40, 0x8e
        .byte   0x1a, 0x91
        .byte   0x0a, 0x94

fruits1:
        .byte   0x4b, 0xab
        .byte   0x40, 0xae
        .byte   0x1a, 0xb1
        .byte   0x0a, 0xb4


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; selectors, using the correct map bank (called from hooks above)

sel_maze:
        ld      hl, #mazes
        call    deref
        ret

sel_pel:
        ld      hl, #pels
        call    deref
        ret

sel_npel:
        ld      hl, #npels
        call    deref
        ret

sel_ppel:
        ld      hl, #peloc
        call    deref
        ret

    ; this one doesn't work for some reason...
sel_fruit:
        ld      hl, #fruits
        call    deref
        ret

    ; this is the guts of it.
deref:
        push    bc
        push    af
        ld      a, (gamesel)
        call    setBwithBank
        rst     0x18
        pop     af
        pop     bc
        ret

setBwithBank:
        ld      b, #0           ; always bank 0
        cp      #Rmspac			; original
        ret     z
        cp      #Rrmspac		; reverse original
        ret     z

        ld      b, #1           ; always bank 1
        cp      #Rmspacatk		; attack
        ret     z
        cp      #Rmspacplus		; plus
        ret     z

            ; from here, we assume either remix or mode j
        ;cp     #Rmspacrmx		; remix
        ;cp     #RmodeJ			; modeJ

            ; b = (boardnumber & 0x01) xor 1
        ld      a, (boardnumber)
        xor     #0x01
        and     #0x01
        ld      b, a

        ret


;;;;;;;;;;;;;;; select the correct board
boardHack:
            ; switch based on the active game...
        ld      a, (gamesel)
        cp      #RmodeJ			; modeJ
        jr      z, board7Hack
        cp      #Rmspacrmx		; remix
        jr      z, board7Hack

        ld      a, (boardnumber)	; uses 0..14 (strange number)
        ret

board7Hack:
        ld      a, (boardnumber)
        and     #0x07			; 0..7 instead of 0..14
        ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  C000 ram hacks (not really necessary...)

mirrorPatch:
	ld      e, (hl)         ; patch
	inc     hl              ; patch
	ld      a, (hl)         ; patch (almost)
	and     #0x7f           ; mask off the high bit (top/bottom flag)
	ld      d, a            ; d cleared of that bit now (C000-safe!)
	ld      a, (hl)         ; set aside A for part 2, below
	jp      mirrorRet

.bound 0xa8c0

.org 0x2c62
	jp	mirrorPatch
mirrorRet:

.org 0x2c75
        bit     #7, a



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Horizontal hacks

.if( horizontal )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; text strings

.org    0x3715
        .ascii  "EROCS@HGIH"    ; high score
.org    0x3725
        .ascii  "@SDERC"        ; CREDIT
.org    0x3734
        .ascii  "YALP@EERF"     ; free play
.org    0x3743
        .ascii  "ENO@REYALP"    ; Player one
.org    0x375c
        .ascii  "OWT@REYALP"    ; Player one
.org    0x376c
        .ascii  "REVO@@EMAG"    ; game over
.org    0x377c
        .ascii  "[YDAER"        ; ready!
.org    0x3788
        .ascii  "NOTTUB@TRATS@HSUP"     ; push start button
.org    0x379f
        .ascii  "YLNO@REYALP@1" ; 1 player only
.org    0x37b3
        .ascii  "SREYALP@2@RO@1" ; 1 or 2 players

.org    0x3807
        .ascii  "'NAM;CAP@SM&"  ; ms.pac-man
.org    0x390c
        .ascii  "YROMEM@@KO"
.org    0x391c
        .ascii  "DAB@@@@M@R"
.org    0x392c
        .ascii  "TIDERC@1@@NIOC@1"
.org    0x3943
        .ascii  "TIDERC@1@SNIOC@2"
.org    0x395a
        .ascii  "STIDERC@2@@NIOC@1"
.org    0x3971
        .ascii  "YALP@@EERF"
.org    0x3988
        .ascii  "SUNOB@@ON@@"
.org    0x3999
        .ascii  "SUNOB"
.org    0x39b2
        .ascii  "ELBAT"
.org    0x39bf
        .ascii  "THGIRPU"

.org    0x3d3e
        .ascii  "OC@GFM@YAWDIM@\\"
.org    0x3d6c
        .ascii  "YKNILB"
.org    0x3d8b
        .ascii  "YKNIP"
.org    0x3d97
        .ascii  "NAM;CAP@SM"
.org    0x3daa
        .ascii  "YKNI"
.org    0x3db8
        .ascii  "1981:0891"
.org    0x3dcc
        .ascii  "EUS"
.org    0x3dd5
        .ascii  "ROINUJ"
.org    0x3de5
        .ascii  "HTIW"
.org    0x3df4
        .ascii  "ESAHC@ETH"

.org    0x3e04
        .ascii  "GNIRRATS"
.org    0x3e13
        .ascii  "NEM;CAP@SM"
.org    0x3e32
        .ascii  "TCA"
.org    0x3e42
        .ascii  "TEEM@YEHT"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 1UP / 2UP hacks
;code change to fix "1UP" and "2UP" into " 1P"  and " 2P"
.org    0x036d
        ld      1(ix),#0x31     ; '1'
        ld      2(ix),#0x40     ; ' '

.org    0x037a
        ld      1(iy),#0x32     ; '2'
        ld      2(iy),#0x40     ; ' '

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; player input hacks

;and now the code modifications
.org    0x1886
        bit     3, a            ; up
.org    0x1899
        bit     0, a            ; down

.org    0x18c8
        bit     3, a            ; up
.org    0x18cd
        bit     0, a            ; down
.org    0x18d2
        bit     1, a            ; right
.org    0x18d7
        bit     2, a            ; left

.endif
