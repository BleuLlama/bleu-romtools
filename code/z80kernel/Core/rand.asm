;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	rand.asm
;
;	pseudorandom number generator
;
;	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module rand

; rand
        ;; rand - get a random number
        ;               in      -
        ;               out     a       random number 0..256
        ;               mod     flags
rand:
        push    hl		; set aside registers
        push    bc

            ; compute a random number
        ld      hl, (randval)   ; hl = last random number
        push    hl
        pop     bc              ; bc = hl
        call    sine            ; a = sine (c)
        ld      c, a            ; c = sine ( last value )

        ld      a, r            ; a = R
        add     a, c            ; a += sine( last value )
        ld      c, a            ; c = sine( last value ) + R

        add     hl, bc          ; rnd += sin ( last value ) + R
        ld      bc, (timer)
        add     hl, bc          ; rnd += timer
        ld      (randval), hl   ; hl = computed random (rnd)
        ld      a, (randval)    ; a = rnd

        pop     bc		; restore registers
        pop     hl

        ret			; return


; randomize
	;; randomize - add in some extra information to randomize more
	;	   (call this from a user input routine to add time randomness)
	;		in 	-
	;		out	-
	;		mod	-
randomize:
	push	af		; preserve registers
	push	bc
	push	hl

	ld	bc, (randval)	; bc =last random number
	ld	a, r
	add	c
	ld	c, a		; bc += r (sorta)
	push	bc
	pop	hl		; hl = bc
	ld	bc, (timer)	; bc = timer
	add	hl, bc		; hl = (last + r) + timer
	ld	(randval), hl	; store the new value

	pop	hl		; restore registers
	pop	bc
	pop	af

	ret			; return
