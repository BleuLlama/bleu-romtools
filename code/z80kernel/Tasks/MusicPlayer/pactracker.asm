;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Pac Menu code - pactracker music player
;
;  2004-07  Scott "Jerry" Lawrence
;
;  $Id: pactracker.asm,v 1.17 2004/12/29 09:35:13 jerry Exp $
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module pactracker

.PTMODULE_START:

pactrackerTask:
	.word	pt_title	; title text
	.byte	0x00		; version major
	.byte	0x10		; version minor
	.word	pt_desc1	; description line 1
	.word	pt_desc2	; description line 2
	.word	0x0000		; attract sequence init
	.word	0x0000		; attract sequence poll
	.word	0x0000		; game init
	.word	0x0000		; game poll
	.byte	0x00		; freeplay
	.word	0x0000		; variants

	; title text
pt_title:
	.byte	17
	;;;;;   "                 "
	.ascii	"   Pac-Tracker   "

pt_desc1:
	.byte	21
	;;;;;   "                     "
	.ascii  " Music module player "
pt_desc2:
	.byte	21
	.ascii  "                     "

; Terminology:
;   Pattern	a series of notes spaced in a clock-tick list
;   Track	one of four patterns in the current playback window
;   Frame	the current set of four tracks to be played back
;   Cache	the current track's word for each track.

; opcodes to ease hand-coding patterns/frames/songs
	PT____		= 0x00	; sustain last note
	PTDROP		= 0x08	; for wave nibble

	; 	0x01 thru 0x7f are frequencies in the below table
	
	PTVCDN		= 0x80	; volume fade to nib	vol nib is level
	PTVCUP		= 0x81	; volume build to nib 	vol nib is level
	PTVF00		= 0x82	; volume fade to 0	vol nib is speed
	PTVFFF		= 0x83	; volume build to max	vol nib is speed

	PTFCDN		= 0x84	; freq cycle down	vol-wav byte is speed
	PTFCUP		= 0x85	; freq cycle up		vol-wav byte is speed
	PTFB00		= 0x86	; freq bend down to nib	vol-wav byte is note
	PTFBFF		= 0x87	; freq bend up to nib	vol-wav byte is note

	PTWRUP		= 0x88	; wave roll up		wave nib is speed
	PTWRDN		= 0x89	; wave roll down	wave nib is speed

	PTSDN		= 0x8A	; speed down N		vol-wave byte is N
	PTSUP		= 0x8B	; speed up N		vol-wave byte is N
	PTSSET		= 0x8C	; speed set to N	vol-wave byte is N

	PTRSNG		= 0x8D	; restart song
	PTRPAT		= 0x8E	; restart pattern

	PTRNXT		= 0x8F	; next step advances to next pattern

	PTLOOP		= 0x90	; loop end point  (add on loop times for start)

	PTSTOP		= 0xA0	; stop/pause playback

	PTSVS		= 0xA1	; set vol speed: (vol,wav) steps per poll
	PTSFS		= 0xA2	; set freq speed: (vol,wav) steps per poll
	PTSWS		= 0xA3	; set wave speed: (vol,wav) steps per poll

	PTSVP		= 0xA4	; set vol speed: (vol,wav) polls per step
	PTSFP		= 0xA5	; set freq speed: (vol,wav) polls per step
	PTSWP		= 0xA6	; set wave speed: (vol,wav) polls per step

	PTRESB		= 0xB0	; RESERVED
	PTRESC		= 0xC0	; RESERVED
	PTRESD		= 0xD0	; RESERVED
	PTRESE		= 0xE0	; RESERVED

	PTFLAG		= 0xF0	; store flags

	PTTLOOP		= 0xFF
	PTTEND		= 0x00

ptNull:
	.ds	127 

	PTEND		= 0xffff

.if 0
; four byte string, address it as:
;	pt_lets + (note number * 4)
;	(adjusted for pac-text)
;	; = "-"
;	: = "#" or at least it should be,  it actually is "/"
pt_lets: 
	.byte	3
	.ascii	";;;"

	.byte	3
	.ascii	"A;0"
	.byte	3
	.ascii	"A:0"
	.byte	3
	.ascii	"B;0"

	.byte	3
	.ascii	"C;1"
	.byte	3
	.ascii	"C:1"
	.byte	3
	.ascii	"D;1"
	.byte	3
	.ascii	"D:1"
	.byte	3
	.ascii	"E;1"
	.byte	3
	.ascii	"F;1"
	.byte	3
	.ascii	"F:1"
	.byte	3
	.ascii	"G;1"
	.byte	3
	.ascii	"G:1"
	.byte	3
	.ascii	"A;1"
	.byte	3
	.ascii	"A:1"
	.byte	3
	.ascii	"B;1"

	.byte	3
	.ascii	"C;2"
	.byte	3
	.ascii	"C:2"
	.byte	3
	.ascii	"D;2"
	.byte	3
	.ascii	"D:2"
	.byte	3
	.ascii	"E;2"
	.byte	3
	.ascii	"F;2"
	.byte	3
	.ascii	"F:2"
	.byte	3
	.ascii	"G;2"
	.byte	3
	.ascii	"G:2"
	.byte	3
	.ascii	"A;2"
	.byte	3
	.ascii	"A:2"
	.byte	3
	.ascii	"B;2"

	.byte	3
	.ascii	"C;3"
	.byte	3
	.ascii	"C:3"
	.byte	3
	.ascii	"D;3"
	.byte	3
	.ascii	"D:3"
	.byte	3
	.ascii	"E;3"
	.byte	3
	.ascii	"F;3"
	.byte	3
	.ascii	"F:3"
	.byte	3
	.ascii	"G;3"
	.byte	3
	.ascii	"G:3"
	.byte	3
	.ascii	"A;3"
	.byte	3
	.ascii	"A:3"
	.byte	3
	.ascii	"B;3"

	.byte	3
	.ascii	"C;4"
	.byte	3
	.ascii	"C:4"
	.byte	3
	.ascii	"D;4"
	.byte	3
	.ascii	"D:4"
	.byte	3
	.ascii	"E;4"
	.byte	3
	.ascii	"F;4"
	.byte	3
	.ascii	"F:4"
	.byte	3
	.ascii	"G;4"
	.byte	3
	.ascii	"G:4"
	.byte	3
	.ascii	"A;4"
	.byte	3
	.ascii	"A:4"
	.byte	3
	.ascii	"B;4"

	.byte	3
	.ascii	"C;5"
	.byte	3
	.ascii	"C:5"
	.byte	3
	.ascii	"D;5"
	.byte	3
	.ascii	"D:5"
	.byte	3
	.ascii	"E;5"
	.byte	3
	.ascii	"F;5"
	.byte	3
	.ascii	"F:5"
	.byte	3
	.ascii	"G;5"
	.byte	3
	.ascii	"G:5"
	.byte	3
	.ascii	"A;5"
	.byte	3
	.ascii	"A:5"
	.byte	3
	.ascii	"B;5"

	.byte	3
	.ascii	"C;6"
	.byte	3
	.ascii	"C:6"
	.byte	3
	.ascii	"D;6"
	.byte	3
	.ascii	"D:6"
	.byte	3
	.ascii	"E;6"
	.byte	3
	.ascii	"F;6"
	.byte	3
	.ascii	"F:6"
	.byte	3
	.ascii	"G;6"
	.byte	3
	.ascii	"G:6"
	.byte	3
	.ascii	"A;6"
	.byte	3
	.ascii	"A:6"
	.byte	3
	.ascii	"B;6"
.endif

; defines for tracking:
	PT_A_0	= 0x01
	PT_As0	= 0x02
	PT_B_0	= 0x03

	PT_C_1	= 0x04
	PT_Cs1	= 0x05
	PT_D_1	= 0x06
	PT_Ds1	= 0x07
	PT_E_1	= 0x08
	PT_F_1	= 0x09
	PT_Fs1	= 0x0a
	PT_G_1	= 0x0b
	PT_Gs1	= 0x0c
	PT_A_1	= 0x0d
	PT_As1	= 0x0e
	PT_B_1	= 0x0f

	PT_C_2	= 0x10
	PT_Cs2	= 0x11
	PT_D_2	= 0x12
	PT_Ds2	= 0x13
	PT_E_2	= 0x14
	PT_F_2	= 0x15
	PT_Fs2	= 0x16
	PT_G_2	= 0x17
	PT_Gs2	= 0x18
	PT_A_2	= 0x19
	PT_As2	= 0x1a
	PT_B_2	= 0x1b

	PT_C_3	= 0x1c
	PT_Cs3	= 0x1d
	PT_D_3	= 0x1e
	PT_Ds3	= 0x1f
	PT_E_3	= 0x20
	PT_F_3	= 0x21
	PT_Fs3	= 0x22
	PT_G_3	= 0x23
	PT_Gs3	= 0x24
	PT_A_3	= 0x25
	PT_As3	= 0x26
	PT_B_3	= 0x27

	PT_C_4	= 0x28
	PT_Cs4	= 0x29
	PT_D_4	= 0x2a
	PT_Ds4	= 0x2b
	PT_E_4	= 0x2c
	PT_F_4	= 0x2d
	PT_Fs4	= 0x2e
	PT_G_4	= 0x2f
	PT_Gs4	= 0x30
	PT_A_4	= 0x31
	PT_As4	= 0x32
	PT_B_4	= 0x33

	PT_C_5	= 0x34
	PT_Cs5	= 0x35
	PT_D_5	= 0x36
	PT_Ds5	= 0x37
	PT_E_5	= 0x38
	PT_F_5	= 0x39
	PT_Fs5	= 0x3a
	PT_G_5	= 0x3b
	PT_Gs5	= 0x3c
	PT_A_5	= 0x3d
	PT_As5	= 0x3e
	PT_B_5	= 0x3f

	PT_C_6	= 0x40
	PT_Cs6	= 0x41
	PT_D_6	= 0x42
	PT_Ds6	= 0x43
	PT_E_6	= 0x44
	PT_F_6	= 0x45
	PT_Fs6	= 0x46
	PT_G_6	= 0x47
	PT_Gs6	= 0x48
	PT_A_6	= 0x49
	PT_As6	= 0x4a
	PT_B_6	= 0x4b


; these can always be condensed down to two bytes, but this is quicker
; for playback.  (that would save 150 bytes) ... although due to twice
; as many memory accesses, it probably isn't quicker at all.
; NOTE: These are totally fucked up.
pt_freqs:

;               f+3   f+2   f+1   f
	.byte 0x00, 0x00, 0x00, 0x00	; ---	0

        .byte 0x00, 0x00, 0x02, 0x03    ; A-0
        .byte 0x00, 0x00, 0x02, 0x07    ; A#0
        .byte 0x00, 0x00, 0x02, 0x09    ; B-0

        .byte 0x00, 0x00, 0x02, 0x0b    ; C-1
        .byte 0x00, 0x00, 0x02, 0x0e    ; C#1
        .byte 0x00, 0x00, 0x03, 0x01    ; D-1
        .byte 0x00, 0x00, 0x03, 0x05    ; D#1
        .byte 0x00, 0x00, 0x03, 0x08    ; E-1
        .byte 0x00, 0x00, 0x03, 0x0a    ; F-1
        .byte 0x00, 0x00, 0x03, 0x0e    ; F#1
        .byte 0x00, 0x00, 0x04, 0x03    ; G-1
        .byte 0x00, 0x00, 0x04, 0x07    ; G#1

        .byte 0x00, 0x00, 0x04, 0x09    ; A-1
        .byte 0x00, 0x00, 0x04, 0x0f    ; A#1
        .byte 0x00, 0x00, 0x05, 0x02    ; B-1

        .byte 0x00, 0x00, 0x05, 0x07    ; C-2
        .byte 0x00, 0x00, 0x05, 0x0d    ; C#2
        .byte 0x00, 0x00, 0x06, 0x02    ; D-2
        .byte 0x00, 0x00, 0x06, 0x0a    ; D#2
        .byte 0x00, 0x00, 0x07, 0x00    ; E-2
        .byte 0x00, 0x00, 0x07, 0x05    ; F-2
        .byte 0x00, 0x00, 0x07, 0x0d    ; F#2
        .byte 0x00, 0x00, 0x08, 0x06    ; G-2
        .byte 0x00, 0x00, 0x08, 0x0e    ; G#2
        .byte 0x00, 0x00, 0x09, 0x03    ; A-2
        .byte 0x00, 0x00, 0x09, 0x0e    ; A#2
        .byte 0x00, 0x00, 0x0a, 0x04    ; B-2

        .byte 0x00, 0x00, 0x0a, 0x0f    ; C-3
        .byte 0x00, 0x00, 0x0b, 0x0a    ; C#3
        .byte 0x00, 0x00, 0x0c, 0x05    ; D-3
        .byte 0x00, 0x00, 0x0d, 0x05    ; D#3
        .byte 0x00, 0x00, 0x0e, 0x00    ; E-3
        .byte 0x00, 0x00, 0x0e, 0x0b    ; F-3
        .byte 0x00, 0x00, 0x0f, 0x0b    ; F#3
        .byte 0x00, 0x01, 0x00, 0x0c    ; G-3
        .byte 0x00, 0x01, 0x01, 0x0c    ; G#3
        .byte 0x00, 0x01, 0x02, 0x0c    ; A-3
        .byte 0x00, 0x01, 0x03, 0x0c    ; A#3
        .byte 0x00, 0x01, 0x04, 0x07    ; B-3

        .byte 0x00, 0x01, 0x05, 0x0d    ; C-4	4
        .byte 0x00, 0x01, 0x07, 0x03    ; C#4
        .byte 0x00, 0x01, 0x08, 0x09    ; D-4
        .byte 0x00, 0x01, 0x0a, 0x0a    ; D#4
        .byte 0x00, 0x01, 0x0b, 0x0f    ; E-4
        .byte 0x00, 0x01, 0x0d, 0x05    ; F-4
        .byte 0x00, 0x01, 0x0f, 0x06    ; F#4
        .byte 0x00, 0x02, 0x01, 0x07    ; G-4
        .byte 0x00, 0x02, 0x03, 0x08    ; G#4
        .byte 0x00, 0x02, 0x05, 0x08    ; A-4
        .byte 0x00, 0x02, 0x07, 0x09    ; A#4
        .byte 0x00, 0x02, 0x09, 0x0a    ; B-4

        .byte 0x00, 0x02, 0x0c, 0x06    ; C-5	20
        .byte 0x00, 0x02, 0x0f, 0x01    ; C#5
        .byte 0x00, 0x03, 0x01, 0x0d    ; D-5
        .byte 0x00, 0x03, 0x05, 0x04    ; D#5
        .byte 0x00, 0x03, 0x07, 0x0f    ; E-5
        .byte 0x00, 0x03, 0x0b, 0x06    ; F-5
        .byte 0x00, 0x03, 0x0e, 0x0d    ; F#5
        .byte 0x00, 0x04, 0x02, 0x0e    ; G-5
        .byte 0x00, 0x04, 0x07, 0x00    ; G#5
        .byte 0x00, 0x04, 0x0b, 0x01    ; A-5
        .byte 0x00, 0x04, 0x0f, 0x0e    ; A#5
        .byte 0x00, 0x05, 0x03, 0x0f    ; B-5

        .byte 0x00, 0x05, 0x08, 0x0c    ; C-6	2c
        .byte 0x00, 0x05, 0x0e, 0x0e    ; C#6
        .byte 0x00, 0x06, 0x03, 0x0b    ; D-6
        .byte 0x00, 0x06, 0x0a, 0x08    ; D#6
        .byte 0x00, 0x06, 0x0f, 0x0f    ; E-6
        .byte 0x00, 0x07, 0x06, 0x0d    ; F-6
        .byte 0x00, 0x07, 0x0e, 0x05    ; F#6
        .byte 0x00, 0x08, 0x05, 0x0d    ; G-6
        .byte 0x00, 0x08, 0x0e, 0x00    ; G#6
        .byte 0x00, 0x09, 0x06, 0x03    ; A-6
        .byte 0x00, 0x09, 0x0f, 0x01    ; A#6
        .byte 0x00, 0x0a, 0x07, 0x0f    ; B-6


; song structure is:
;    song	pointer to a tracklist
;    tracklist entry:
;	byte	number of lines in the tracks
;		0x00	end of song
;		0xff	loop back to beginning instead
;	word	pointer to a track	(voice 0)
;	word	pointer to a track	(voice 1)
;	word	pointer to a track	(voice 2)
;	word	pointer to a track	(bytes are cached only)
;
;;;;;;;;;;;;;;;;;;;;

; for making composing easier, here's a default blank pattern...
ptBLANK:
    .ds	64

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; ptInterrupt
        ;; ptInterrupt	- interrupt-time service update routine
	;		- updates ram cache values
        ;               in      -
        ;               out     -
        ;               mod     af, bc	(ok to do)
ptInterrupt:
	ld	a, (ptSong)
	ld	b, a
	ld	a, (ptSong+1)		; check for null song
	add	b 
	cp	#0x00
	ret	z			; return if there's no song loaded

	ld	a, (ptCountdown)	; check for pause/stop
	cp	#0xff
	ret	z			; paused, no update if Speed=0xff
	ld	b, a			; set aside.  we might need this

	ld	a, (ptCountdown)
	dec	a			; ptCountdown =- 1
	ld	(ptCountdown), a	; stash it back
	cp	#0x00			; are we at zero?
	ret	nz			; no update yet, resume handler

	    ; okay, new step...  (advance)
	push	hl

	    ; restock ptCountdown appropriately
	ld	a, (ptSpeed)
	ld	(ptCountdown), a	; ptCountdown = ptSpeed

	ld	a, (ptFlags)
	set	PTF_STEP, a		; set ptFlags.PTF_STEP
	ld	(ptFlags), a

	    ; check to see if PSTEP is set
	bit	PTF_PSTEP, a
	jr	nz, .ptNPC		; advance to next frame

	    ; check to see if we need to switch pattern/framesets
	ld	a, (ptFLen)
	ld	b, a			; b = pattern length
	ld	a, (ptFIndex)
	inc	a
	ld	(ptFIndex), a		; (ptFIndex)++
	cp	b			; Index == length?
	jr	nz, .ptNPResume
.ptNPC:
	call	.ptNextPattern
.ptNPResume:
	pop	hl
	ret



; .ptNextPattern
; 	advance ptFrame to the next frame position
;	also handle the length=stop and length=restart flags
.ptNextPattern:
	    ; advance frame pointer
	ld	bc, #10
	ld	hl, (ptFrame)
	add	hl, bc
	ld	(ptFrame), hl
	
	    ; check for stop
	ld	a, (hl)			; grab length from frame
	cp	#PTTEND			; stop song flag
	jr	nz, .ptNP0

	    ; length == 0x00:
	ld	hl, (ptFlags)		; check for LOOP flag
	bit	PTF_LOOP, (hl)
	jr	nz, .ptNP1		; force a restart
	    ; force the stop
	ld	a, #0xff
	ld	(ptCountdown), a	; countdown = 0xff (stop playback)
	jr	.ptNP2
	
.ptNP0:
	cp	#0xff			; restart song flag
	jr	nz, .ptNP2		; nope, continue

.ptNP1:	    ; length == 0xff:
	ld 	hl, (ptSong)
	ld	(ptFrame), hl		; frame = start of song

.ptNP2:
	xor	a
	ld	(ptFIndex), a		; set beginning of frame

	call	.ptFlenInit		; setup FLen

	ld	hl, (ptFlags)
	res	PTF_PSTEP, (hl)
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; should this be changed to a bunch of ldir's?
.ptCache:
	ld	a, (ptFIndex)
	add	a
	ld	d, #0x00
	ld	e, a			; de = actual offset now
	ld	ix, #PTCACHE		; base of cache space

	ld	hl, (ptFrame)		; pointer to current frame
	inc	hl			; 
	inc	hl			; advance past length flag

	; hl now is a pointer to just past the frame length word
	; de is the index into the frame (Index*2)

	ld	b, #4
.ptC000:
	push	bc

	ld	a, (hl)
	inc	hl
	ld	c, a
	ld	a, (hl)
	inc	hl
	push	hl			; set HL aside ---v
	ld	b, a
	push	bc
	pop	hl			; HL now points to the frame

	add	hl, de			; add in the frame offset

	ld	a, (hl)			; copy over byte 0
	inc	hl
	ld	(ix), a
	inc	ix

	ld	a, (hl)			; copy over byte 1
	inc	hl			; necessary?
	ld	(ix), a	
	inc	ix			; necessary?

	pop	hl			; restore HL ---^
	pop	bc
	djnz	.ptC000			; repeat for all 4 words

	ret


; ptPoll
	;; ptPoll	- poll the update routines
	;		- reads cache values, plays the sound
	;		in	-
	;		out	-
	;		mod	ram variables
ptPoll:
	push	af
	push	bc
	push	de
	push	ix
	push	iy
	push	hl

	ld	a, (ptCountdown)
	cp	#0xff
	jr	z, .ptp0001
	ld	a, #1
	ld	(sounden), a		; fixes coindrop bug in quadblok

.ptp0001:
	ld	a, (ptFlags)
	bit	PTF_STEP, a
	jr	z, .ptPollRet
	res	PTF_STEP, a
	ld	(ptFlags), a

	    ; do stuff here...
	call	.ptCache

	ld	bc, (ptT0Cache)		; Track 0 cache
	ld	de, #v1_base
	call	.ptSound

	ld	bc, (ptT1Cache)
	ld	de, #v2_base
	call	.ptSound

	ld	bc, (ptT2Cache)
	ld	de, #v3_base
	call	.ptSound

.ptPollRet:
	pop	hl
	pop	iy
	pop	ix
	pop	de
	pop	bc
	pop	af
	ret

; .ptSound
;	handle the parsing of a single track of audio
	;		in	hl	pointer to word: track item
	;		in	de	pointer to base of audio channel
	;		mod	hl
.ptSound:
	push	de
	push	bc
	bit	7, c		; do we have an effect or a tone
	jr	nz, .ptS_SFX

.ptS_TONE:
	ld	a, c		; grab the tone byte
	and	#0x7f		; not necessary
	cp	#0x00		; empty tone, resume below
	jr	z, .ptS_WAVE

	ld	b, #0x00
	ld	c, a		; bc = tone index	0, 1, 2, 3
	ld	hl, #pt_freqs+3
	add	hl, bc		; base + 		0, 1, 2, 3
	add	hl, bc		; base + 		0, 2, 4, 6
	add	hl, bc		; base + 		0, 3, 6, 9
	add	hl, bc		; base + 		0, 4, 8, 12
	push	hl
	pop	ix		; ix = table pointer (source)

	ld	hl, #vX_freq
	add	hl, de		; hl = audio hardware (destination)

	ld	b, #4
.ptST000:
	ld	a, (ix)
	ld	(hl), a
	inc	hl
	dec	ix

	djnz	.ptST000
	jr	.ptS_WAVE	; continue below

.ptS_SFX:
	pop	bc
	pop	de
	jr	.ptS_RET

.ptS_WAVE:
	pop	bc
	pop	de
	ld	a, #1
	ld	a, b		; waveform is in low nibble
	and	#0x07		; not necessary
	ld	hl, #vX_wave
	add	hl, de
	ld	(hl), a		; poke in the waveform

.ptS_VOL:
	ld	a, b		; volume is in high nibble
	rra
	rra	
	rra
	rra
	and	#0x0f		; not necessary
	ld	hl, #vX_vol
	add	hl, de		; offset for volume in current channel
	ld	(hl), a		; poke in the volume

.ptS_RET:
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ptInit
	;; ptInit	- set up a song to be played
	; 		in 	bc	pointer to song frame list
	;		in 	a	initial speed
	;		out	-
	;		mod	-
ptInit:
	call	ptStop			; first, any currently running song
	call	ptSilence		; mute it all
	ld	(ptSpeed), a		; set initial speed
	ld	(ptSong), bc		; set song pointer
	ld	(ptFrame), bc		; Frame = soing pointer

	call	.ptFlenInit		; FLen setup

	ld	a, #0xff
	ld	(ptCountdown), a	; pause the playback
	xor	a
	ld	(ptFIndex), a		; FIndex = 0
	ld	b,a
	ld	c,a
	ld	(ptT0Cache), bc		; cache 0 = 0
	ld	(ptT1Cache), bc		; cache 1 = 0
	ld	(ptT2Cache), bc		; cache 2 = 0
	ld	(ptT3Cache), bc		; cache 3 = 0
    
	    ; set all speeds to 4
	ld	a, #4
	ld	(ptT0SVol), a
	ld	(ptT0SFreq), a
	ld	(ptT0SWave), a
	ld	(ptT1SVol), a
	ld	(ptT1SFreq), a
	ld	(ptT1SWave), a
	ld	(ptT2SVol), a
	ld	(ptT2SFreq), a
	ld	(ptT2SWave), a
	
	ret

.ptFlenInit:
	ld	hl, (ptFrame)
	ld	a, (hl)
	ld	(ptFLen), a
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ptPlay
	;; ptPlay	- play the currently configured song from the top
	;		in	-
	;		out	-
	;		mod	-
ptPlay:
	push	af
	ld	a, (ptSpeed)
	ld	(ptCountdown), a	; start playback
	ld	a, #0xff
	ld	(sounden), a
	pop	af
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ptStop
	;; ptStop	- pause/stop the currently playing song
	;		in	-
	;		out	-
	;		mod	-
ptStop:
	push	af
	ld	a, #0xff
	ld	(ptCountdown), a
	xor	a
	ld	(sounden), a
	pop	af
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ptWait
	;; ptWait	- waits until the song finishes
	;		in	-
	;		out	-
	;		mod	-
ptWait:
	push	af
.ptW00:
	call	ptPoll
	ld	a, (ptCountdown)
	cp	#0xff
	jr	nz, .ptW00
	pop	af
	call	ptSilence
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ptSilence
	;; ptSilence	- force all audio channels to volume=0
	;		in	-
	;		out	-
	;		mod	-
ptSilence:
	push	af
	xor	a
	ld	(sounden), a
	pop	af
	ret


	ld	hl, #0x4050
	ld	de, #0x20
	ex	de, hl
	add	hl, de
	ld	a, (hl)
	ld	a, (de)
	ld	a, (bc)
	xor	a
	ld	(de), a

PACTRACKER_SZ = .-.PTMODULE_START
