;
; Satan's Hollow for a Tron cabinet hack
;
;
; $Id: shtron.asm,v 1.5 2004/01/20 03:58:55 jerry Exp $ 

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

