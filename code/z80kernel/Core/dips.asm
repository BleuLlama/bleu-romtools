;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;	dips.asm
;
;	Dip Switch settings
;
;	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module dips

; pac-man dipswitch settings (dsw0)
;
;	0x03	coinage:
;	0x00	 freeplay
;	0x01	 1 coin for credit
;	0x02	 1 coin for 2 credits 
;	0x03	 2 coins for 1 credit
;
;	0x0c	lives:
;	0x00	 1 life
;	0x04	 2 lives
;	0x08	 3 lives
;	0x0c	 5 lives
;
;	0x30	bonus:
;	0x00	 @ 10,000
;	0x10	 @ 15,000
;	0x20	 @ 20,000
;	0x30	 none
;
;	0x40	difficulty
;	0x00	 normal
;	0x40	 hard
;
;	0x80	names
;	0x00	 original
;	0x80	 alternate


dips:
		; check for freeplay
	ld	a, (dsw0)
	and	#0x03
	cp	#0x00		; 0x00 = freeplay
	jr	nz, .dipLives
	ld	a, #0xff
	ld	(credits), a	; set credits to FF

.dipLives:
		; check lives per coin
	ld	a, (dsw0)
	and	#0x0c
	cp	#0x00		; 1 life per
	ld	c, #1
	jr	z, .dipELives

	cp	#0x04		; 2 lives
	inc 	c
	jr	z, .dipELives

	cp	#0x08		; 3 lives
	inc 	c
	jr	z, .dipELives

	cp	#0x0c		; 5 lives
	ld	c, #5

.dipELives:
	ld	a, c
	ld	(dipBases), a	; store it aside in dipBases


.dipDifficulty:
		; check difficulty settings
	ld	a, (dsw0)
	and	#0x40
	cp	#0x00		; normal
	ld	c, #diffNORM
	jr	z, .dipEDifficulty
	ld	c, #diffHARD
.dipEDifficulty:
	ld	c, a
	ld	(difficulty), a


.dipEND:
	ret
