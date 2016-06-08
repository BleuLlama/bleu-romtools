;
; togglable fast cheat
;
; $Id: cheat.asm,v 1.2 2004/03/17 13:19:11 jerry Exp $ 

.area 	.CODE (ABS)

; boot1  0000 - 0fff
; boot2  1000 - 1fff
; boot3  2000 - 2fff
; boot4  3000 - 3fff
; boot5  8000 - 8fff
; boot6  9000 - 9fff

.org	0x1ffe
	.byte	0xbf
	.byte	0xdc

	; HACK3 : Dock Cutlip's P1P2 
.org	0x1764
	jp	invinc

.org	0x1809
	jp	speedup

.org	0x1fb0
invinc:
	ld      hl,#0x4da6
	ld      e,a    
	ld      d,#0x00
	add     hl,de
	ld      a,(hl)
	and     a     
	jp      z,i0
	ld      a,b
	ld      (#0x4da4),a
	jp      0x1767  
i0:
	ld      a,(#0x5040)     ; IN1
	and     #0x20           ; Start 1
	ret     z             ; not pressed, return
	ld      a,b
	ld      (#0x4da4),a
	jp      0x1767   

speedup:
	ld      a,(#0x5040)     ; IN1
	bit     6,a           ; Start 2
	jp      z,#0x1845       ; not pressed, jp to 1845
	ld      a,#0xff
	cp      (hl)
	jp      z,#0x1811
	dec     (hl)
	ret

