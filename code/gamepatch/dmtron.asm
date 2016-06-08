;
; Domino Man for a Tron cabinet hack
;  NOTE: this doesn't work, although it should.
;
; $Id: dmtron.asm,v 1.2 2004/03/19 22:40:40 jerry Exp $ 


; ref: http://www.tc.umn.edu/~johns594/arcade/tech/mcr.html
;
;	Domino Man	Tron
;
; IN0
;   0	coin 1		coin 1
;   1	coin 2		coin 2
;   2	start 1		start 1
;   3	start 2		start 2
;   4	1 button	1 button
;   5	tilt		tilt
;   6	coin 3		coin 3
;   7 	service		service
;
; IN1
;   0	1 left		Spinner
;   1	1 right
;   2   1 up
;   3   1 down
;
; IN2
;   0	2 left		1 left
;   1	2 right		1 right
;   2	2 up		1 up
;   3	2 down		1 down
;   4	2 button	2 left
;   5			2 right
;   6			2 up
;   7			2 down

; IN3 - DIPS
;   1	skin color	coin meters
;   2	cabinet		cabinet
;   3	# meters	continue?


.area 	.CODE (ABS)

	; fix the rom checksum routine to ignore good and bad roms
.org	0x109e
	nop

	; adjust the shield input
.org	0x4a15
	in	a, (#00)		; shield
.org	0x4a27
	and	#0x04			; map to start1

	; adjust fire button check
.org	0x4bc4
	in	a, (#00)		; fire button check
.org	0x4bd6
	and	#0x10			; map to trigger button

	; adjust player left and right input
.org	0x4aa7	
	in	a, (#02)		; left and right check


	; pat myself on the back
.org	0x41a5
	;.ascii	"  COPYRIGHT MCMLXXXI"
	.ascii	"1981 BALLY MIDWAY CO"
.org	0x41ba
	;ascii	"BALLY MIDWAY MFG CO"
	.ascii	"                   "

.org	0x41ce
	.ascii	" TRON HACK BY JERRY"
	;.ascii	"ALL RIGHTS RESERVED"

; the remaining ones are for the menu and text entry screens...
; let's just let these be somewhat wrong (left and right to move the
; menu, and up and down to select) since it's easier and I'm lazy

.org	0x4201
	in	a, (#02)

.org	0x43bc
	in	a, (#02)

.org	0x43c2
	in	a, (#02)

.org	0x476b
	in	a, (#02)

.org	0x4f87
	in	a, (#02)

.org	0x4f9a
	in	a, (#02)

.org	0x4fb1
	in	a, (#02)


.org	0x5cc7
	in	a, (#02)

.org	0x5ce6
	in	a, (#02)

