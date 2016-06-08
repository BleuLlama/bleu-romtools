;
; stripped.asm
;
;   A stripped down version of puckman
;
;	Scott Lawrence
;	yorgle@gmail.com
;	April 28, 2012 -

.area 	.CODE (ABS)

;  0    Initial version - Startup test elimination

; NOTE: This file should be assembled with 'asz80', which is 
;	in the 'zcc 0.96' package.  It uses the ".bound" extension
;	written by myself, which notifies you if you go past a boundary

; 	This code should be applied as a patch over 'puckman' ("namcopac.*")

; 	Documentation on the patches in this can be found at:
;		http://yorgle.org/minimalPac

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
	racktest	= 4

        start_port      = in1
        start1          = 5
        start2          = 6

; code hooks
	textout	= 0x2c5e


; Stuff we're keeping from the 3000 rom for now
; 32f9 - DATA		keep for now
; 3435 - MAP DATA	keep for now
; 35b5 - PELLET DATA	keep for now

; 36a5 - Text looup dereference 1
; 3713 - Text string data

; 3b08 - fruit table
; 3b30 - sound tables

; Test mode and power on test elimination
.org 0x01d1
	; jp	z, 0x000		; rack switch is on, reset to test mode
	.byte	0x00, 0x00, 0x00	; turn off the rack test switch.

.org 0x0038
	; jp 	0x0f3c			; go to patched handler for test mode checks
	jp	0x008d
	.byte 	0x00, 0x00, 0x00, 0x00	; keep it tidy
.bound 0x0042

	;.repeat 0x00, 0
	;.fill	0x00, .-1


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; hack to screw up sprites to hang around pac-man
.org 0x03a0
;	sub	#0 	; hahahah


; hack to skip to intermissions quickly
;0E7F: FE E4     cp      #E4             ; > #E4 ?
.org 0x0eba ; displays fruit after this number of pellets eaten
;	cp	#0x02			; 2 pellets eaten?

; 08e1  fef4    cp      #f4             ; == 244 ?
.org 0x08e1 ; advances to next stage after this many pellets eaten
;	cp	#0x02

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; disable 2 player start, 2 player option with coindrops

;0623  0609      ld      b,#09           ; load B with message #9:  "1 OR 2 PLAYERS"
;0625  2002      jr      nz,#0629        ; if >= 2 credits, skip next step
.org 0x0623
	.byte 0,0,0,0		; 1 player only
;0634  280c      jr      z,#0642         ; don't check p2 with 1 credit
.org 0x0634
	jr	0x0642		; 1 player only


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; intermission handler hack
;0a3b  3a134e    ld      a,(#4e13)       ; load A with current board level
;0a55  97 22                             ; #2297 ; cut scene 3

.org 0x0a3b
	; this area determines which intermission (if any) to do
	jp 	0x0a6f			; 3 bytes - never do any intermissions.
	;jp	0x2108			; 3 bytes - intermission 1
	;jp	0x219e			; 3 bytes - intermission 2
	;jp	0x2297			; 3 bytes - intermission 3

	; this opens up 49 more bytes of space
.bound 0x0a6e

.org 0x2108
	; intermission 1 space
.bound 0x219d

.org 0x219e
	; intermission 2 space
.bound 0x2296

.org 0x2297
	; intermission 3 space
.bound 0x230a

; this reclaims 0x2108-0x230a (0x202 bytes - 514 bytes + 49 = 563 bytes)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; map hacks
.org 0x2419
	; draw map routine
;	ret
	; 2420 - 2447

.org 0x2448
	; draw pellets routine
;	ret
	; 2448 - 2486

	; reclaims 109 bytes for these routines, plus 623 bytes of data;
	; 0x3435 - 0x35b4	maze data
	; 0x35b5 - 0x36a4	pellet data

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; see if we can get rid of Demo mode/introduction screen

.org 0x0413
	call	0x06be		; force change to demo mode from credits screen
	ret

	; this reclaims	0x0417 - 05e4
	.fill 0x00, 0x05e4
.bound 0x05e4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; let's shuffle around the song tables, and remove the intermission songs

; 1. pointers to the song tables to our new location
.org 0x2cc1
	ld	hl, #sngtbl1
.org 0x2cda
	ld	hl, #sngtbl2
.org 0x2cf3
	ld	hl, #sngtbl3

; 2. the new song tables
.org 0x0f3c
sngtbl1:
	.word pacsng1
	.word mspsng1
sngtbl2:
	.word pacsng2
	.word mspsng2
sngtbl3:
	.word ns1
	.word ns2


; 3. the new song datas
	; pacman start song
pacsng1:
	.byte 0xF1, 0x02, 0xF2, 0x03, 0xF3, 0x0F, 0xF4, 0x01
	.byte 0x82, 0x70, 0x69, 0x82, 0x70, 0x69, 0x83, 0x70
	.byte 0x6A, 0x83, 0x70, 0x6A, 0x82, 0x70, 0x69, 0x82
	.byte 0x70, 0x69, 0x89, 0x8B, 0x8D, 0x8E, 0x00
pacsng2:
	.byte 0xF1, 0x00, 0xF2, 0x02, 0xF3, 0x0F, 0xF4, 0x00
	.byte 0x42, 0x50, 0x4E, 0x50, 0x49, 0x50, 0x46, 0x50
	.byte 0x4E, 0x49, 0x70, 0x66, 0x70, 0x43, 0x50, 0x4F
	.byte 0x50, 0x4A, 0x50, 0x47, 0x50, 0x4F, 0x4A, 0x70
	.byte 0x67, 0x70, 0x42, 0x50, 0x4E, 0x50, 0x49, 0x50
	.byte 0x46, 0x50, 0x4E, 0x49, 0x70, 0x66, 0x70, 0x45
	.byte 0x46, 0x47, 0x50, 0x47, 0x48, 0x49, 0x50, 0x49
	.byte 0x4A, 0x4B, 0x50, 0x6E, 0xff

	; mspac start song (optional)
mspsng1:
;	.byte 0xf1, 0x02, 0xf2, 0x03, 0xf3, 0x0a, 0xf4, 0x02
;	.byte 0x50, 0x70, 0x86, 0x90, 0x81, 0x90, 0x86, 0x90
;	.byte 0x68, 0x6a, 0x6b, 0x68, 0x6a, 0x68, 0x66, 0x6a
;	.byte 0x68, 0x66, 0x65, 0x68, 0x86, 0x81, 0x86, 0xff
mspsng2:
;	.byte 0xf1, 0x00, 0xf2, 0x02, 0xf3, 0x0a, 0xf4, 0x00
;	.byte 0x41, 0x43, 0x45, 0x86, 0x8a, 0x88, 0x8b, 0x6a
;	.byte 0x6b, 0x71, 0x6a, 0x88, 0x8b, 0x6a, 0x6b, 0x71
;	.byte 0x6a, 0x6b, 0x71, 0x73, 0x75, 0x96, 0x95, 0x96
;	.byte 0xff

	; silence
ns1:
	.byte 0x00
ns2:
	.byte 0x00
.bound 0x0fff


; reclaimed contiguous space:

.org 0x3bc8
	; song jump tables.  (relocate)
.bound 0x3bd4

.org 0x3bd4
	; start song voice 1 - 31 bytes (relocate)
.bound 0x3bf2

.org 0x3bf3
	; intermission song a - 101 bytes
.bound 0x3c57

.org 0x3c58
	; start song voice 2 - 62 bytes (relocate)
.bound 0x3c94

.org 0x3c95
	; intermission song b - 74 bytes
.bound 0x3cdd

.org 0x3cde
	; silence
.bound 0x3cdf

	; recaiming all of this gives us 0x117 contiguous bytes (279 bytes)

	; at this point, 689 reclaimed

