;
; 25 map patch for pac-man
;
;  Adopted from:
;	The Sil's 6 map patch v1.3 2000/01/21 (multi maze support)
;	Ms. Pacman (Sil's code originated there, as well as new color support)
;	Hanglyman (vertical tunnel support) (via Sil's vertical map namco)
;
; $Id: pacNmap.asm,v 1.11 2004/08/17 04:19:43 jerry Exp $ 
;
; Name changed from 6-map to 25-map 2004/08/13


;
; rom layout:
; 	0x0000 - 0x0fff		pacman.6e	(Pac-Man)
; 	0x1000 - 0x1fff		pacman.6f	(Pac-Man)
; 	0x2000 - 0x2fff		pacman.6h	(Pac-Man)
; 	0x3000 - 0x3fff		pacman.6j	(Pac-Man)
;			map  0: Sil's Vertical Map
;	0x8000 - 0x8fff		maps1.rom	(maps 01-06)
;			map  1: Hangly Man
;			map  2: Puckman
;			map  3: Midway
;			map  4: Yorgle: Triple Tunnel
;			map  5: Yorgle: Illusion
;			map  6: Yorgle: Arena (Umlaut Llama)
;	0x9000 - 0x9fff		maps2.rom	(maps 07-12)
;			map  7: Sil: Ms. Pac-Man Was Here
;			map  8: Sil: Power In The Tunnel
;			map  9: Sil: Dangerous Bottom
;			map 10: Sil: The Two Crosses
;			map 11: Sil: Ms. Pac Attack was here too 
;			map 12: Sil: New York, New York
;	0xa000 - 0xafff		maps3.rom	(maps 13-18)
;			map 13: Snake Eyes: Shuffle
;			map 14: T-Bone: Slithery
;			map 15: T-Bone: Corners
;			map 16: T-Bone: Halves
;			map 17: T-Bone: Sidestepping 
;			map 18: Jeff Morris: Xen's Revenge
;	0xb000 - 0xbfff		maps4.rom	(maps 19-25)
;			map 19: Blue Justice: Crazy 1 (WavyTunnel)
;			map 20: Blue Justice: Strange 1 (Bubbles)
;			map 21: Blue Justice: Strange 6 (Cloverleafs)
;			map 22: Pac Fan: Ultra 1 (The Sides)
;			map 23: Pac Fan: Ultra 2 (The Hand)
;			map 24: Pac Fan: Ultra 4 (doublecross)
;
;  Jeff Morris:	    http://www.xensei.com/users/jeffm/www/pacman/xens-revenge
;  Marcel Silvius:  http://home.kabelfoon.nl/~msilvius/

; vertsil space:
; 0f4a - 1000
; 1fd0 - 2000
; 2fba - 3000
; 3ce0 - 3f00	- multimaze support, map tables, pellet tables
; 3fd0 - 4000
; 3a50 - 3ae2 (namco data; set 3a4f to 0x00 first though)

.area 	.CODE (ABS)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; text changes

.org    0x37ff
        .ascii  "25V@MAP@PAC;MAN@HACK"

.org    0x37eb  ; coin screen
	.ascii	"\\@2004@YORGLE@"

.org	0x386a
	;.ascii	"YORGLE@"

.org	0x270a
	ld	a, #0x01		; force alternate ghost names

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; checksum patch

.org 0x301a
	nop	; this disables the checksum compare
	nop	; Sil's original roms have adjusted checksums


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; entry points

.org 0x241c
	call	adj_map			; load new map data

.org 0x244f
	call	adj_pel			; load new pellet data

.org 0x248e
	call	adj_pel			; load new pellet data

.org 0x24dd
	jp	adj_color		; load new color data

.org 0x24f9
	jp	adj_tunnel		; fix up the special color codes

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; the new routines... (mostly flown over from mspac hacks)

.org	0x3ce0
	; new map selection subroutine
adj_map:			; mspac 0x946a
	ld	hl, #maptab	; map table
	call	mm_deref	; below dereference routine
	ld	hl, #0x4000	; restore hl
	ret

	; new pellet table selection subroutine
adj_pel:			; adopted from mspac 0x947c, 0x9481
	ld	hl, #peltab	; pellet table
	call	mm_deref	; dereference 
	ld	iy, #0x0000
	add	iy, bc		; table -> iy
	ld	hl, #0x4000	; restore hl	(patch fix)
	ret


	; new color selection subroutine
adj_color:			; mspac 0x9580
	jp	z, #0x24e1		; resume
	ld	a, (0x4e02)
	and	a
	jr	z, .col_map
	cp	#0x10
	ld	a, #0x01
	jp	nz, #0x24e1		; resume
.col_map:
	ld	a, (0x4e13)
	cp	#0x15
	jp	p, .col_range		; adjust to be within 0..15

.col_0000:
	ld	c, a
	ld	b, #0x00
	ld	hl, #colortable
	add	hl, bc
	ld	a, (hl)
	jp	0x24e1			; resume

    ; subtract from A until less than 0, then add 10 and resume above
.col_range:
	sub 	#0x15
.cr000:
	sub	#0x10
	jp	p, .cr000
	add	a, #0x15
	jr	.col_0000


adj_tunnel:			; mspac 0x95c3
	ld	a, (#0x4e13)
	cp	#3
	jp	p, #0x2534
	ld	hl, #tuntab
	call	mm_deref
	ld	hl, #0x4400
.at000:
	ld	a, (bc)
	inc	bc
	and	a
	jp	z, #0x2534
	rst	#0x10
	set	6, (hl)
	jr	.at000

; it is unclear to me exactly how these tables work.  I just copied them
; over verbatim from a mspacman bootleg romset
tuntab:
	.word	ttab1
	.word	ttab2

ttab1:
	.byte	0x49, 0x09, 0x17, 0x09, 0x18, 0x09, 0x0e, 0xe0
	.byte	0xe0, 0xe0, 0x29, 0x09, 0x17, 0x09, 0x17, 0x09
	.byte	0x00, 0x00

ttab2:
	.byte	0x42, 0x16, 0x0a, 0x16, 0x0a, 0x16, 0x0a, 0x20
	.byte	0x20, 0x20, 0xde, 0xe0

;;;;;;;;;;
; mm_deref			mspac 0x94bd
;
;	helper subroutine; dereferences the table in HL by the map order
;	list and the current map number.
;		in	hl	table to look into (pellet or map table)
;		out	bc	address of the current map/pellet
;		mod	af, hl
mm_deref:
	ld	a, (#0x4e13)	; level number
	push	hl		; store aside table for now
	cp	#tabsize	; check for 00..1f
	jp	p, .mm_dec	; keep decrementing until less than 1f

.mm_dfinish:
	ld	hl, #maporder	; order table
	rst	#0x10		; a = (hl + a)
	pop	hl		; restore our original map
		    ; a = map slot number now
	add	a, a		; convert slot number to word offset
	ld	c, a
	ld	b, #0x00	; bc = word offset (a expaned out to word size)
	add	hl, bc		; dereference the index -> address
		    ; hl now is the offset into the table 
	ld	c, (hl)
	inc	hl
	ld	b, (hl)		; bc = value in that location
	ret

	; keep decrementing by 0x1f until value is less than 0x00
.mm_dec:
	sub	#tabsize	; a = a - 0x1f
	jp	p, .mm_dec	; if > 0, do it again
	add	a, #tabsize	; adjust back into 00..1f
	jr	.mm_dfinish	; resume above


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; map and pellet reference tables
maptab:
	.word	0x3435		; map 0 	; in pacman.6j / boot4

	.word	0x8000		; map 1		; 8000 rom / boot5
	.word	0x8180		; map 2
	.word	0x8300		; map 3
	.word	0x8480		; map 4
	.word	0x8600		; map 5
	.word	0x8780		; map 6

	.word	0x9000		; map 7		; 9000 rom / boot6
	.word	0x9180		; map 8
	.word	0x9300		; map 9
	.word	0x9480		; map 10
	.word	0x9600		; map 11
	.word	0x9780		; map 12

	.word	0xa000		; map 13	; a000 rom
	.word	0xa180		; map 14
	.word	0xa300		; map 15
	.word	0xa480		; map 16
	.word	0xa600		; map 17
	.word	0xa780		; map 18

	.word	0xb000		; map 19	; b000 rom
	.word	0xb180		; map 20
	.word	0xb300		; map 21
	.word	0xb480		; map 22
	.word	0xb600		; map 23
	.word	0xb780		; map 24



peltab:
	.word	0x35b5		; map 0		; in pacman.6j / boot4

	.word	0x8900		; map 1		; 8000 rom / boot5
	.word	0x8a00		; map 2
	.word	0x8b00		; map 3
	.word	0x8c00		; map 4
	.word	0x8d00		; map 5
	.word	0x8e00		; map 6

	.word	0x9900		; map 7		; 9000 rom / boot6
	.word	0x9a00		; map 8
	.word	0x9b00		; map 9
	.word	0x9c00		; map 10
	.word	0x9d00		; map 11
	.word	0x9e00		; map 12

	.word	0xa900		; map 13	; a000 rom
	.word	0xaa00		; map 14
	.word	0xab00		; map 15
	.word	0xac00		; map 16
	.word	0xad00		; map 17
	.word	0xae00		; map 18

	.word	0xb900		; map 19	; b000 rom
	.word	0xba00		; map 20
	.word	0xbb00		; map 21
	.word	0xbc00		; map 22
	.word	0xbd00		; map 23
	.word	0xbe00		; map 24

.bound 0x3f00


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; map ordering and color table

; Originally:
; 8f00  2b 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2b  +--------------+
; 8f10  2b 20 4d 61  64 65 20 62  79 20 53 69  6c 20 20 2b  + Made by Sil  +
; 8f20  2b ff 32 30  20 4a 61 6e  20 32 30 30  30 20 20 2b  +.20 Jan 2000  +
; 8f30  2b 20 20 20  20 20 20 2d  20 20 20 20  20 20 20 2b  +      -       +
; 8f40  2b 20 20 28  43 29 20 32  30 30 30 20  20 20 20 2b  +  (C) 2000    +
; 8f50  2b 20 20 53  69 6c 6c 79  53 6f 66 74  20 20 20 2b  +  SillySoft   +
; 8f60  2b 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2b  +--------------+
; 8f70  2b 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2b  +--------------+
; 8f80  2b 20 4d 61  70 20 6f 72  64 65 72 3a  20 20 20 2b  + Map order:   +
; 8f90  01 02 03 04  05 06 06 05  04 03 02 01  02 04 06 01  ................
; 8fa0  03 05 06 04  02 05 03 01  04 05 03 06  02 04 03 02  ................
; 8fb0  2b 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2b  +--------------+
; 8fc0  2b 20 43 6f  6c 6f 72 20  74 61 62 6c  65 3a 20 2b  + Color table: +
; 8fd0  10 10 10 10  10 10 10 10  10 10 10 10  10 10 10 10  ................
; 8fe0  10 10 10 10  10 10 10 10  10 10 10 10  10 10 10 10  ................
; 8ff0  2b 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2d  2d 2d 2d 2b  +--------------+

; moving these a little to adjust for the new size of mazes..
	tabsize		= 0x1f		; number of elements in these lists

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.org 0x8f70
maporder:
	.byte	0x00
	.byte	0x01, 0x02, 0x03, 0x04, 0x05, 0x06
	.byte	0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c
	.byte	0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12
	.byte	0x13, 0x14, 0x15, 0x16, 0x17, 0x18
	.byte	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06

	.byte	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	.byte	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
	.byte	0x18, 0x19, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06
	.byte	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; 1 3 5 7  ghost colors
	; 9 red yellow outline
	; e black peach outlines
	; f green, white outline  17
	; 10 original
	; 11 blue peach outline
	; 12 white red outline
	; 14 peach white outline
	; 15 green peach outline
	; 16 cyan white outline
	; 18 pink yellow outline
	; 19 blue black outline
	; 1d peach red outline
	; 1f black white outline

.org 0x8fc0
colortable:
	.byte	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.byte	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.byte	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.byte	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10

	;.byte	0x09, 0x0e, 0x0f, 0x01, 0x03, 0x14, 0x1f, 0x10
	;.byte	0x03, 0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1d
	;.byte	0x05, 0x09, 0x10, 0x09, 0x12, 0x13, 0x0f, 0x0e
	;.byte	0x10, 0x15, 0x19, 0x09, 0x0e, 0x18, 0x16, 0x1f

	.byte	0x01, 0x03, 0x05, 0x07, 0x09, 0x0e, 0x0f, 0x11
	.byte	0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1d, 0x1f
	.byte	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.byte	0x03, 0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1d

