;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	main.asm
;
;	the main initial program loop
;
;	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Mode switching calls
;  - these get called always to switch to a new mode
;
;  NOTE: if you are adding any of the games to a multigame kit, this
;        is probably the best place to hook in to the game bank

Mo_ssaver:			; switch to the screensaver
	call	atClear
	call	at2Clear
	call	clrtimer
	call	inputclear
	call	ss_game0
	ld	a, #M_SSAVER
	ld	(runMode), a
	ret

	; on input, we need to call this to reset the screensaver
	; if the screensaver is running, disable it and reset the timeout
	; if it is not running, just reset the timeout
ss_reset:
	push	af
	ld	a, (runMode)
	cp	#M_SSAVER		; is the SSaver running?
	call	z, Mo_att		; yes.  restore Att mode
	call	nz, .ssReschedule	; no.  Just reschedule the screensaver
	pop	af
	ret

.ssReschedule:
	push	bc
	push	de
	ld	bc, #SSTIMEOUT		; screensaver timeout
	ld	de, #Mo_ssaver		; turn on the screensaver then
	call 	atDelta
	pop	de
	pop	bc
	ret


Mo_att:				; switch to attract mode
	call	atClear
	call	at2Clear
	call	clrtimer
	call	inputclear
        call    cls
        call    clrsprite
	call	draw_title		; title background, frame, credits
	call	draw_variant
	call	t_att0			; attract 0
	ld	a, #M_ATT
	ld	(runMode), a		; ATT runMode
	xor	a
	ld	(variant), a
	call	.ssReschedule		; reschedule the screensaver
	ret

Mo_gameplay:			; switch into gameplay
	ld	(nplayers), a		; set a to #1 or #2 before calling
	call	atClear
	call	at2Clear
	call	clrtimer
	call	basesAdd		; add preset bases to gameplay
	call	cls			; clear the screen
	call	clrsprite		; clear the sprites
	call	inputclear
	call	t_game0			; game 0
	ld	a, #M_GAMEPLAY
	ld	(runMode), a
	ret

Mo_death:			; switch to "player is dying"
	call	atClear
	call	at2Clear
	call	clrtimer
	call	cls
	call	clrsprite
	call	inputclear

	ld	bc, #DEATHTIME	; set up the end-of-death callback
	ld	de, #.end_Death	; set up the end-of-death callback
	call 	atDelta		; schedule it!
	ld	a, #M_DEATH	
	ld	(runMode), a
	ret

Mo_buyin:			; switch to "insert coins to continue"
	call	atClear
	call	at2Clear
	call	clrtimer
	ld	a, #M_BUYIN
	ld	(runMode), a
	ret

Mo_test:			; switch into TEST mode
	ld	a, (runMode)
	cp	#M_TEST
	ret	z		; if we've already switched, just return

	call	atClear
	call	at2Clear
	call	clrtimer
	call	cls
	call	clrsprite
	call	inputclear
	call	tm_game0
	ld	a, #M_TEST
	ld	(runMode), a
	ret

Mo_endTest:
	call	Mo_att
	ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; main routine

main:
;	call	mp_game0
;m2:
;	call	mp_game
;	jp	m2

	call 	rle2_test
	;jr	.


	cp 	#SOFTBOOT		; check for soft boot
	jp	z, softmain		; we're doing a reboot, not powerup

	call	inputclear		; clear the input counters
	call	dips			; check the dip switches

	    ; do hardboot stuff here
	call 	draw_cfl		; draw me.
	ld	bc, #0x40		; 4 second
	call	sleep			; wait a touch
softmain:
	call	rdscls			; wipe down

	call	taskinit		; initialize the task system
	call	Mo_att			; Attract/Coin Input Mode

mainLoop:
	    ; items common for all run modes
	call	atService		; service the timers
	call	checkTest		; check for test mode


.if DEBUG
	call	debugDump
.endif

	; check the runmode, and call the appropriate function
	ld	a, (runMode)

	cp 	#M_TEST
	jr	z, .run_Test		; do test mode


	call	coincheck		; check for coin drops
	ld	a, (runMode)


	cp 	#M_ATT
	jr	z, .run_Att		; attract mode/insert coins/press start

	cp	#M_GAMEPLAY
	jr	z, .run_Game		; poll the current game engine

	cp	#M_DEATH
	jr	z, .run_Death		; run the game death routine 

	cp	#M_BUYIN
	jr	z, .run_Buyin		; display the "insert coin" screen
	
	cp	#M_SSAVER
	jr	z, .run_SSaver		; do the screensaver loop instead

	jr	mainLoop		; JUST IN CASE


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

	; this needs to be simplified.
	; last was 0, this is 1 : call Mo_Test
	; last was 1, this is 0 : call Mo_endTest
checkTest:
	ld	a, (svc_port)		; check for test mode
	ld	(thistest), a		; set it aside
	bit	#service, a
	jr	nz, .ct_ext0
.ct_ent0:
	ld	a, (lasttest)
	bit	#service, a
	jr	z, .ctret
	call	Mo_test			; make sure we're in test mode
	jr	.ctret

.ct_ext0:
	ld	a, (lasttest)
	bit	#service, a
	jr	nz, .ctret
	call	Mo_endTest

.ctret:
	ld	a, (thistest)
	ld	(lasttest), a		; last = this
	ret

.run_Test:	; Test Mode
	call	tm_game
	jr	mainLoop

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

.run_Att:	; Attract Mode
	call	menuinput		; check for left/right
	call	draw_taux		; display credit information, etc
	call	t_att			; current attract graphics anim
	call	startcheck		; check for start buttons
	jr 	mainLoop

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

.run_Game:	; Game Play
	call	t_game			; poll the game loop
	jr	mainLoop

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

.run_Buyin:	; continue/game over
	call	Mo_death		; just change to death mode for now
	jr	mainLoop

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

.run_SSaver:	; Screensaver Mode
	call	ss_game			; screensaver loop
	call	menuinput		; check for left/right
	jr	mainLoop

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

.run_Death:	; Death sequence?
	call	rand
	and	#0x0f
	ld	b, a			; random horizontal
	call	rand
	and	#0x1f
	ld	c, a			; random vertical

	ld	hl, #rColors
	call	rand
	and	#0x0f
	rst	0x18			; random color from the list

	ld	hl, #text_gameover
	call	putstrB

	ld	bc, #0x0003
	call	sleep			; pause

	jp	mainLoop

.end_Death:
	call	Mo_att
	ret

text_gameover:
	.byte	13
	.ascii	"  GAME OVER  "

rColors:
	.byte	2, 3, 4, 5,  6, 8, 9, 10
	.byte	11, 13, 15
	.byte	8,  9, 10, 5, 13
	
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

; additional fuctions for variant selection



; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
; 0x0000 is bottom row
; 0x0020 is second to bottom row
; 0x03c0 is top row
; 0x03d0 is second row

.if DEBUG
debugDump:
	ld	a, (runMode)		; run mode
	ld	d, a
	ld	e, #C_green
	ld	bc, #0x03c4
	call	prtBCDbyte

	ld	a, (variant)		; variant
	ld	d, a
	ld	e, #C_white
	ld	bc, #0x03e4
	call	prtBCDbyte

	ld	e, #C_orange
	ld	hl, #timer		; timer
	ld	bc, #0x03d0
	call	prtABCDword

	; at 1
	ld	e, #C_dgreen
	ld	hl, #(atControl)
	bit	#atc_scheduled, (hl)	; see if we're active
	jr	z, .dbgd000
	ld	e, #C_yellow		; yellow if active
.dbgd000:
	ld	hl, #atFunc		; at function pointer
	ld	bc, #0x03d5
	call	prtABCDword

	ld	hl, #atTime		; at function time
	ld	bc, #0x03da
	call	prtABCDword

	; at 2
	ld	e, #C_dgreen
	ld	hl, #(atControl)
	bit	#atc_2scheduled, (hl)	; see if we're active
	jr	z, .dbgd001		; yellow if active
	ld	e, #C_yellow
.dbgd001:
	ld	hl, #at2Func		; at function pointer
	ld	bc, #0x03f5
	call	prtABCDword

	ld	hl, #at2Time		; at function time
	ld	bc, #0x03fa
	call	prtABCDword
	ret
.endif


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; coincheck - check coin drops

coincheck:
	call	Get1Coin
	cp	#0x00
	call	nz, coindrop

	call	Get2Coin
	cp	#0x00
	call	nz, coindrop

	call	Get3Coin
	cp	#0x00
	call	nz, coindrop
	ret

    ; user dropped a coin, check for number of credits
coindrop:
	ld	a, (runMode)
	cp	#M_SSAVER
	call	z, ss_reset		; clear the screensaver
	cp	#M_ATT
	call	z, ss_reset		; clear the screensaver

	ld	a, (credits)
	cp	#0x99
	ret	nc			; return if we've hit 99

	ld	bc, (credits)
	ld	b, #0
	call	incBCDbc
	ld	(credits), bc
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; menuinput - check user inputs

menuinput:
	call	Get1Up
	cp	#0x00
	call	nz, menuUP

	call	Get1Down
	cp	#0x00
	call	nz, menuDOWN

	call	Get1Left
	cp	#0x00
	call	nz, menuLEFT

	call	Get1Right
	cp	#0x00
	call	nz, menuRIGHT
	ret

menuCOMMON:
	call	ss_reset
	call	randomize
	ret

menuLRCOMMON:
	call	draw_title
	call	draw_variant
	call	t_att0
	ret

menuUP:
	call	variantnext
	call	draw_variant
	call	menuCOMMON
	ret

menuDOWN:
	call	variantprev
	call	draw_variant
	call	menuCOMMON
	ret

menuRIGHT:
	call	tasknext		; next task
	call	menuCOMMON
	call	menuLRCOMMON
	ret

menuLEFT:
	call	taskprev		; previous task
	call	menuCOMMON
	call	menuLRCOMMON
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; startcheck - check for start button presses

startcheck:
	ld	a, (credits)
	cp	#0x00
	ret	z

	call	Get1Start
	cp	#0x00
	call	nz, s1Pressed

	call	Get2Start
	cp	#0x00
	call	nz, s2Pressed
	ret

; start 1 WAS PRESSED
s1Pressed:
	call	decCredits
	ld	a, #1
	call	Mo_gameplay
	ret

; start 2 was pressed
s2Pressed:
	call	decCredits
	ld	a, #2
	call	Mo_gameplay
	ret


	; decCredits
	;	if( not freeplay ) credits--;
decCredits:
	call	randomize
	call	ss_reset		; clear the screensaver
	ld	a, (credits)
	cp	#0xff			; check for FREEPLAY
	ret	z			; return if FREEPLAY mode
	cp	#0x00
	ret	z			; 0 credits, just return (just in cases)
	dec	a
	ld	(credits), a		; credits--;
	ret
	

	; basesAdd
	;	basesLeft = basesLeft + dipBases
basesAdd:
	ld	a, (dipBases)
	ld	c, a
	ld	a, (basesLeft)
	add	a, c
	ld	(basesLeft), a
	ret

MENU_SZ = .-.TITLE_START
