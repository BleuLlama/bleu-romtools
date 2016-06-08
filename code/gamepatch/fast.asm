;
; always-on fast patch
;
; $Id: fast.asm,v 1.1 2004/01/23 16:04:34 jerry Exp $ 

.area 	.CODE (ABS)

; boot1  0000 - 0fff
; boot2  1000 - 1fff
; boot3  2000 - 2fff
; boot4  3000 - 3fff
; boot5  8000 - 8fff
; boot6  9000 - 9fff

	; HACK2 : fast mod
.org	0x180b
	.db	0x01

.org	0x1ffd
	.db	0xbd
