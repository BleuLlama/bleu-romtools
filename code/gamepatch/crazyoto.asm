;
; Crazy Otto restoration
;	Not finished.
;
; $Id: crazyoto.asm,v 1.3 2004/03/09 19:15:06 jerry Exp $ 

.area 	.CODE (ABS)

; boot1  0000 - 0fff
; boot2  1000 - 1fff
; boot3  2000 - 2fff
; boot4  3000 - 3fff
; boot5  8000 - 8fff
; boot6  9000 - 9fff

; CRAZY OTTO for title of attract screen
.org	0x3808
	.ascii	"CRAZY@OTTO"

; CRAZY OTTO for "STARRING" on attract screen... recenter it 
.org	0x3d95
	.byte	0x4e
.org	0x3d97
	.ascii	"CRAZY@OTTO"


; OTTOMEN for the test screen (text lookup table mod)
.org	0x370d
	.byte	0x4f, 0x3e

; OTTOMEN for another screen (text lookup table mod)
.org	0x370d
	.byte	0x4f, 0x3e


; recenter "STARRING"
.org	0x3e02
	.byte	0x2b	; inc by 20 to shift to the left

; remove the second line of the copyright string (null out the text) 
.org	0x3db7
	.byte	0x2f, 0x80

; gencomp logo text string, centered, dropped down a line
.org	0x3d3c
	.byte	0xbc, 0x02
	.ascii	"\\1981@"
	.byte	112, 113, 114, 115, 116, 117, 118
	.byte	0x2f, 0x8f	; color 0x0f

; eliminate midway logo routine
.org 	0x964a
	ret

; make sure people realize it's a hack, and not original roms
.org	0x0fb0 
	.ascii "Hacked by Jerry "
	.ascii "   March 2004   "
	.ascii "umlautllama.com "
