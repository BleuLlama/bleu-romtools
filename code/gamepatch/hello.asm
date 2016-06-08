;
; hello.asm
;
;   A bare minimal "Hello, World"
;	72 bytes total
;
;	Scott Lawrence
;	yorgle@gmail.com
;	May 2, 2012

; This was just an attempt to see how small of a "Hello, world!" application I 
; could make for this hardware.

.area 	.CODE (ABS)

; NOTE: This file should be assembled with 'asz80', which is 
;	in the 'zcc 0.96' package. 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; some hardware defines

        watchdog = 0x50c0       ; watchdog timer
        vidram   = 0x4000       ; video ram base
        colram   = 0x4400       ; color ram base

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.org 0x0000
	; RST 0 - initialize

	; disable interrupts
	di				; 1 byte

	; setup stack pointer
	ld	sp, #(0x4fc0)		; 3 bytes

	; start to setup screen clearing
	ld	de, #0x4005		; 40 is "@" char (' '),  5 is color (cyan)
	rst	0x10			; 1 byte - continue at 0x0010
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.org 0x0008
	; RST 8 - watchdog clear
kickdog:
	xor	a			; 1 byte
	ld	(watchdog), a		; 3 bytes
	ret				; 1 byte
	.byte	0x00, 0x00, 0x00	; filler. 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.org 0x0010
	; RST 10 - continuation of startup

	; clear the screen
	call 	clrw

	; print out the text string
	ld	hl, #0x43fd	; second row
	ld	de, #hello	; #value  #(pointer)
	ld	b, #13		; "HELLO, WORLD!" is 13 bytes
txtlp:
	ld	a, (de)		; get next char
	ld	(hl),a		; put it on the screen
	dec	hl		; move screen pointer
	inc	de		; move text pointer
	djnz	txtlp		; b--, if >0 goto txtlp
	
	; we're done,  now just spin while kicking the dog
kickloop:
	rst	0x08		; kick the watchdog (1 byte)
	;call	kickdog		; 3 bytes (4 bytes if we do it directly) 
	;ld	(watchdog), a	; 3 bytes (a isn't 0, but it's a tight loop, so OK)
	jr	kickloop	; 2 bytes

	; or...
	;halt			; 1 byte -- resets every so often
	;jp	0x0000		; 3 bytes - restart loop, flashes
	;jr	0x0000		; 2 bytes - restart loop, flashes
	;rst	0		; 1 byte - restart loop, flashes

	

; Some cross-referencing of the characters tells us that for
; "!" we need to store a "[".  Likewise, " " is "@", and "," doesn't exist, but 
; there is a character that we'll use like a comma, which we can use "l" to access.
; likewise, there's no lowercase, so we HAVE TO SHOUT IT.

hello: ; 13 bytes
	.ascii "HELLOl@WORLD["

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; clrw
;
; clear a block of the screen. 
;  this could probably be shortened if we didn't care about screen garbage.
; d is the character
; e is the color
clrw:
	; set up...
	rst	0x08		; kick the watchdog
	ld	hl, #vidram	; base of screen ram
	call	clrww		; fill character ram with d
	ld	d, e		; d = e
	call	clrww		; fill color ram with d
	ret

clrww:
	ld	b, #4		; 4 blocks
clrwwop:
	rst	0x08		; kick the watchdog
clrwwlp:
	ld	(hl), d
	inc	l
	jr 	nz, clrwwlp	; loop back
	inc	h
	djnz	clrwwop		; repeat 4 times
	ret			; and return
