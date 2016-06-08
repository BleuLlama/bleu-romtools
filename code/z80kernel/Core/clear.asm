;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	clear.asm
;
; 	screen clear and transitions
;
;  	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module trans


; clear screen
        ;; cls - clear the screen (color and video ram)
        ;               in      -
        ;               out     -
        ;               mod     -
cls:
        push    hl              ; set aside some registers
        push    af
        push    bc

        ; hl is at the base of color ram now.
	ld	hl, #(colram)	; base of color ram
        ld      a, #0x00        ; clear the screen to 0x00 (black)
        ld      b, #0x04        ; need to set 256 bytes 4 times.
        call    memsetN         ; do it.

        ld      hl, #(vidram)   ; base of video ram
        ld      a, #0x00        ; clear the screen to 0x16 (blank)
        ld      b, #0x04        ; need to set 256 bytes 4 times.
        call    memsetN         ; do it.

        pop     bc              ; restore the registers
        pop     af
        pop     hl
        ret                     ; return


; roll down screen clearing
rdscls:
	ld	hl, #(.rdschz)
	ld	bc, #0x0000
	ld	a, #0
	    ; top two rows
	call	putstrA
	call 	.rdsslp
	inc	c
	call	putstrA
	call 	.rdsslp


	    ; get the main body
	ld	c, #0x00
	ld	b, #0x20
.rds000:
	push	bc
	ld	b, #0x00
	call	putstrB
	call 	.rdsslp

	pop	bc
	inc	c
	djnz	.rds000

	    ; get the bottom two rows
	ld	bc, #0x0002
	call	putstrA
	call 	.rdsslp
	inc	c
	call	putstrA
	call 	.rdsslp
	ret



; roll up screen clearing
ruscls:
	ld	hl, #(.rdschz)
	ld	a, #0
	ld	bc, #0x0003
	    ; get the bottom two rows
	call	putstrA
	call	.rdsslp
	dec	c
	call	putstrA
	call	.rdsslp

	    ; loop over the body of the screen
	ld 	c, #0x00
	ld	b, #0x1f
.rus000:
	push	bc
	ld	c, b
	ld	b, #0x00
	call	putstrB
	call	.rdsslp

	pop	bc
	djnz	.rus000

	    ; get the straggler
	ld	c, b
	call	putstrB
	call	.rdsslp

	    ; top two rows
	ld	bc, #0x0001
	call	putstrA
	call	.rdsslp
	dec	c
	call	putstrA
	call	.rdsslp
	ret


	; sleep timer (one tick) 
.rdsslp:
	push	bc
	ld	bc, #0x0001
	call	sleep
	pop	bc
	ret

	; the buffer to text-write to the screen
.rdschz:
	.byte	0x1c
	.byte	0,0,0,0,0,0,0,0,0,0
	.byte	0,0,0,0,0,0,0,0,0,0
	.byte	0,0,0,0,0,0,0,0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; some helpful utility functions

clrspecial:
        ;; clear the special registers
        ld      a, #0x00        ; a  = 0x00
        ld      hl, #(specreg)  ; hl = start of special registers
        ld      b, #(speclen)   ; b  = 0xC0 bytes to zero
        call    memset256       ; 0x5000-0x50C0 will get 0x00
        ret

clrsprite:
        ;; clear sprite registers
        ld      a, #0x00        ; a  = 0x00
        ld      hl, #(sprtbase) ; hl = start of sprite registers
        ld      b, #(sprtlen)   ; b  = 0x10 16 bytes
        call    memset256       ; 0x4ff0-0x4fff will get 0x00
        ret

clruser:
        ;; clear user ram
        ; we only should do this at boot time...
        ld      hl, #(rambase)  ; hl = base of RAM
        ld      a, #0x00        ; a = 0
        ld      b, #0x02        ; b = 2 blocks of 256 bytes to clear
        call    memsetN         ; clear the blocks
        ret

clrtimer:
	;; clear the timer
	push	de
	ld	de, #0x0000
	ld	(timer), de
	pop	de
	ret

