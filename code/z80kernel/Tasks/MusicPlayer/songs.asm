;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - various songs
;
;  2004-07  Scott "Jerry" Lawrence
;
;  $Id: songs.asm,v 1.11 2004/11/14 06:56:44 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module songs


.PTSONGS_START:

; songs defined in this file:
;	digdug_song
;	qb0_song
;	qb1_song


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Dig-Dug Walking song...
digdug_song:
	;   nsteps  track0	track1		track2		track3
	.word	32, dd_trkA0,	dd_trkB0,	ptNull,		dd_score
	.word	32, dd_trkA1,	dd_trkB1,	ptNull,		dd_score
	;.word	32, dd_trkA1,	dd_trkB1,	dd_trkD,	dd_score
	.word	PTTLOOP

	.word	PTTEND

	.word	6,  dd_set0,	dd_set1,	ptNull,		ptNull

dd_set0:
	.byte	PTSVP,  0x03	; volume speed = 3 polls per step
	.byte	PTSFP,  0x03	; freq speed = 3 polls per step
	.byte	PTSWP,  0x03	; wave speed = 3 polls per step
	.byte	PTSSET, 0x04	; song speed is 4 ticks per step
	.byte	PTSTOP, PT____	; stop playback
	.byte	PTRNXT, PT____	; advance to next frame

dd_set1:
	.byte	PTSVP,  0x08	; volume speed = 8 polls per step
	.byte	PTSFP,  0x03	; freq speed = 3 polls per step
	.byte	PTSWP,  0x03	; wave speed = 3 polls per step
	.byte	PTSSET, 0x04	; song speed is 4 ticks per step
	.byte	PTSTOP, PT____	; stop playback
	.byte	PTRNXT, PT____	; advance to next frame

		dd_v0	= 0x01		; voice 0 instrument
		dd_v1	= 0x06		; voice 1 instrument

	; note			vol	drop	   wave
dd_trkA0:
	.byte	PT_Fs4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_D_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_D_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_D_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0

	.byte	PT_D_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0


dd_trkA1:
	.byte	PT_Ds4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_Ds4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT_D_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_E_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT_D_4,		0xf0	+ PTDROP + dd_v0

	.byte	PT_G_4,		0xf0	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0

	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0
	.byte	PT____,		0x00	+ PTDROP + dd_v0


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
dd_trkB0:
	.byte	PT_C_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_C_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_B_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_B_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_As1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_As1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_Gs1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_G_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_As1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_B_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1


dd_trkB1:
	.byte	PT_C_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_C_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_B_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_B_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_As1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_As1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_Gs1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_A_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_G_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1

	.byte	PT_As1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_F_2,		0xe0	+ PTDROP + dd_v1
	.byte	PT_B_1,		0xe0	+ PTDROP + dd_v1
	.byte	PT_G_2,		0xe0	+ PTDROP + dd_v1

dd_trkD:
	.word	0xffff, 0xf0f0, 0xeeee, 0xe0e0, 0xdddd, 0xd0d0, 0xcccc, 0xc0c0
	.word	0xbbbb, 0xb0b0, 0xaaaa, 0xa0a0, 0x9999, 0x9090, 0x8888, 0x8080
	.word	0x7777, 0x7070, 0x6666, 0x6060, 0x5555, 0x5050, 0x4444, 0x4040
	.word	0x3333, 0x3030, 0x2222, 0x2020, 0x1111, 0x1010, 0x0000, 0x0000
	.word	0xffff, 0xf0f0, 0xeeee, 0xe0e0, 0xdddd, 0xd0d0, 0xcccc, 0xc0c0
	.word	0xbbbb, 0xb0b0, 0xaaaa, 0xa0a0, 0x9999, 0x9090, 0x8888, 0x8080
	.word	0x7777, 0x7070, 0x6666, 0x6060, 0x5555, 0x5050, 0x4444, 0x4040
	.word	0x3333, 0x3030, 0x2222, 0x2020, 0x1111, 0x1010, 0x0000, 0x0000

dd_score:
	.word	0xff00, 0x0000, 0x0000, 0x0000
	.word	0x00ff, 0x0000, 0x0000, 0x0000
	.word	0xff00, 0x0000, 0x0000, 0x0000
	.word	0x00ff, 0x0000, 0x0000, 0x0000
	.word	0xff00, 0x0000, 0x0000, 0x0000
	.word	0x00ff, 0x0000, 0x0000, 0x0000
	.word	0xff00, 0x0000, 0x0000, 0x0000
	.word	0x00ff, 0x0000, 0x0000, 0x0000


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; tetris song A

	qb0_V0	= 0x06		; foreground melody
	qb0_V1	= 0x06		; backing melody
	qb0_V2	= 0x06		; countermelody

; voice 0 = 
; voice 1 = organish
; voice 2 = trupetesque
; voice 3 = triangle trumpet
; voice 4 = froggy
; voice 5 = sick frog
; voice 6 = soft sine

qb0_song:
    .word	32, qb0_p0v0, qb0_p0v1, qb0_p0v2, ptNull	; pat 0
    .word	32, qb0_p1v0, qb0_p1v1, qb0_p1v2, ptNull	; pat 1
    .word	32, qb0_p0v0, qb0_p0v1, qb0_p0v2, ptNull	; pat 0
    .word	32, qb0_p1v0, qb0_p1v1, qb0_p1v2, ptNull	; pat 1
    .word	32, qb0_p2v0, qb0_p2v1, qb0_p2v2, ptNull	; pat 2
    .word	32, qb0_p3v0, qb0_p3v1, qb0_p3v2, ptNull	; pat 3
    .word	PTTLOOP


qb0_p0v0:	; len = 32
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
qb0_p0v1:	; len = 32
    .byte	PT_D_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_A_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_As1,   0xf0 + PTDROP + qb0_V1
qb0_p0v2:	; len = 32
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2


qb0_p1v0:	; len = 32
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_Ds4,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT_Ds4,   0xf0 + PTDROP + qb0_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
qb0_p1v1:	; len = 32
    .byte	PT_C_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_C_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_C_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_C_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_C_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_As0,   0xf0 + PTDROP + qb0_V1
    .byte	PT_As1,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_As1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_As0,   0xf0 + PTDROP + qb0_V1
    .byte	PT_F_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_F_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_A_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_1,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
qb0_p1v2:	; len = 32
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Ds3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Ds3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2


qb0_p2v0:	; len = 32
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
qb0_p2v1:	; len = 32
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
qb0_p2v2:	; len = 32
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2


qb0_p3v0:	; len = 32
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT_Fs4,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
    .byte	PT____,   0xf0 + PTDROP + qb0_V0
qb0_p3v1:	; len = 32
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT_Fs2,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb0_V1
    .byte	PT____,   0xf0 + PTDROP + qb0_V1
qb0_p3v2:	; len = 32
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_Fs3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_As3,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT_C_4,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2
    .byte	PT____,   0xf0 + PTDROP + qb0_V2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	qb1_V0	= 0x06		; foreground melody
	qb1_V1	= 0x06		; backing melody
	qb1_V2	= 0x06		; countermelody

; voice 0 = 
; voice 1 = organish
; voice 2 = trupetesque
; voice 3 = triangle trumpet
; voice 4 = froggy
; voice 5 = sick frog
; voice 6 = soft sine


qb1_song:
    .word	64, qb1_p0v0, qb1_p0v1, qb1_p0v2, ptNull	 ; pat 0
    .word	64, qb1_p1v0, qb1_p1v1, qb1_p1v2, ptNull	 ; pat 1
    .word	64, qb1_p2v0, qb1_p2v1, qb1_p2v2, ptNull	 ; pat 2
    .word	64, qb1_p3v0, qb1_p3v1, qb1_p3v2, ptNull	 ; pat 3
    .word	64, qb1_p4v0, qb1_p4v1, qb1_p4v2, ptNull	 ; pat 4
    .word	32, qb1_p5v0, qb1_p5v1, qb1_p5v2, ptNull	 ; pat 5
    .word	PTTLOOP


qb1_p0v0:	; len = 128
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
qb1_p0v1:	; len = 128
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0x70 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0x70 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0x70 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0x70 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
qb1_p0v2:	; len = 128
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2


qb1_p1v0:	; len = 64
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
qb1_p1v1:	; len = 64
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
qb1_p1v2:	; len = 64
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2


qb1_p2v0:	; len = 64
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
qb1_p2v1:	; len = 64
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_As2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_Cs3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_A_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_As2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
qb1_p2v2:	; len = 64
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2


qb1_p3v0:	; len = 64
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
qb1_p3v1:	; len = 64
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_G_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
qb1_p3v2:	; len = 64
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_5,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2


qb1_p4v0:	; len = 64
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_5,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_B_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_As4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
qb1_p4v1:	; len = 64
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_D_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
qb1_p4v2:	; len = 64
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_D_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_4,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_E_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_B_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_A_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_As3,   0xf0 + PTDROP + qb1_V2
    .byte	PT_Gs3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_F_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2


qb1_p5v0:	; len = 32
    .byte	PT_C_4,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0xf0 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
    .byte	PT____,   0x00 + PTDROP + qb1_V0
qb1_p5v1:	; len = 32
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_2,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_C_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_Cs4,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
    .byte	PT_E_3,   0xf0 + PTDROP + qb1_V1
    .byte	PT____,   0xf0 + PTDROP + qb1_V1
qb1_p5v2:	; len = 32
    .byte	PT_E_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT_G_3,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2
    .byte	PT____,   0xf0 + PTDROP + qb1_V2

PTSONGS_SZ = .-.PTSONGS_START
