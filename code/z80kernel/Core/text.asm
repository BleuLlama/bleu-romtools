;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	text.asm
;
;	text rendering routines
;
;  	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module text

; text justification
        ;; textcenter - adjust the x ordinate 
        ;               in      hl      pascal string
        ;               in      b       x ordinate
        ;               in      c       y ordinate  BC ->  0xXXYY
        ;               out     -
        ;               mod     b       adjusted for center
        hscrwide = 14
textcenter::
            ; set aside registers
        push    af
            ; halve the width
        ld      b, (hl)         ; b = length of text
        jp      NC, .tcrr       ; make sure carry is cleared
        ccf
.tcrr:
        rr      b               ; b = half of text length
            ; add on the center position
        ld      a, #hscrwide    ; a = screenwidth/2
        sub     b               ; a = screenwidth/2 - textlength/2
        ld      b, a            ; b = that result
            ; restore registers
        pop     af
            ; return
        ret


        ;; textright - adjust the x ordinate 
        ;               in      hl      pascal string
        ;               in      b       x ordinate
        ;               in      c       y ordinate  BC ->  0xXXYY
        ;               out     -
        ;               mod     b       adjusted for right
textright::
            ; set aside registers
        push    af
            ; halve the width
        ld      a, b            ; a = start location
        ld      b, (hl)         ; b = length of text
        sub     b               ; a = start loc - length
        ld      b, a            ; b = new position
            ; restore registers
        pop     af
            ; return
        ret


; xy2offsB
        ;; xy2offsB - get the vid/color buffer offset of the X Y coordinates
        ;               in      b       x ordinate
        ;               in      c       y ordinate  BC ->  0xXXYY
        ;               out     hl      offset
        ;               mod     -
xy2offsB::
            ; set aside registers
        push    af
        push    bc
        push    de
        push    ix
            ; set aside Y for later in DE
        ld      d, #0x00        ; d = 0
        ld      e, c            ; shove Y into E
            ; get the base offset
        ld      ix, #(.scroffs) ; ix = offset table base
            ; add in X component
                ;; XXXXJJJJJ This can probably be shortened if we 
                ;;              drop the range check.
        ld      a, b            ; shove X into A
        and     a, #0x1f        ; make sure X is reasonable
        rlc     a               ; x *= 2
        ld      c, a            ; c = offset * 2
        ld      b, #0x00        ; b = 0
        add     ix, bc          ; ix += bc
            ; retrieve that value into HL
        ld      b, 1(ix)
        ld      c, 0(ix)
        push    bc
        pop     hl              ; hl = scroffs[x]
            ; add in Y component
        add     hl, de          ; hl += DE   hl = scroffs[x]+y
            ; restore registers 
        pop     ix
        pop     de
        pop     bc
        pop     af
            ; return
        ret


        ;; xy2offAC - get the vid/color buffer offset of the X Y coordinates
        ;               in      b       x ordinate
        ;               in      c       y ordinate  BC ->  0xXXYY
        ;               out     hl      offset
        ;               mod     -
xy2offsAC::
            ; set aside registers
        push    bc
        push    de
        push    ix
            ; generate the X component into DE
        ld      d, #0x00        ; d = 0
        ld      e, b            ; e = X
            ; get the base offset
        ld      ix, #(.acoffs)  ; ix = offset table base
            ; add in the y component. (BC)
        ld      b, #0x00        ; zero B (top of BC)
        rlc     c               ; y *= 2
        add     ix, bc          ; offset += index
            ; retrieve that value into HL
        ld      b, 1(ix)
        ld      c, 0(ix)
        push    bc
        pop     hl              ; hl = acroffs[x]
            ; subtract out the X component.
        sbc     hl, de          ; hl -= DE   hl = acoffs[y]-x
            ; restore registers 
        pop     ix
        pop     de
        pop     bc
            ; return
        ret


; putstrA
        ;; putstrA - get the vid/color buffer offset of the X Y coordinates
        ;               in      hl      pointer to the string (asciz)
        ;               in      b       x position
        ;               in      c       y position
        ;               in      a       color
        ;               out     -
        ;               mod     -
putstrA::
            ; set aside registers
        push    bc
.psChook:                       ; this is where putstrC joins in...
        push    hl
        push    de
        push    ix
        push    iy
            ; compute the offsets
        push    hl              ; set aside the string pointer
        call    xy2offsAC
        push    hl
        pop     ix              ; move the offset into ix (char ram)
        push    hl
        pop     iy              ; move the offset into iy (color ram)
        ld      de, #(vidram)   ; base of video ram
        add     ix, de          ; set IX to appropriate location in vid ram
        ld      de, #(colram)   ; base of color ram
        add     iy, de          ; set IY to appropriate location in color ram
            ; prep for the loop
        pop     hl
        ld      b, (hl)         ; b is the number of bytes (pascal string)
        inc     hl              ; HL points to the text now
.pstra1:
            ; loop for each character
        ld      c, (hl)         ; c = character
        ld      (ix), c         ; vidram[b+offs] = character
        ld      (iy), a         ; colram[b+offs] = color
            ; adjust pointers
        inc     hl              ; inc string location
        dec     ix              ; dec char ram pointer
        dec     iy              ; dec color ram pointer
        djnz    .pstra1         ; dec b, jump back if not done
            ; restore registers
        pop     iy
        pop     ix
        pop     de
        pop     hl
        pop     bc
            ; return
        ret


	;; putstrBP - same as putstrB, but first 3 items are C(a), X and Y (bc)
	;		in	hl	pointer to the string
	;		out 	-
	;		mod	bc, af
putstrBP::
	ld	a, (hl)		; a = (hl++)
	inc	hl
	push	af		; stash 'a' aside
	ld	a, (hl)		; a = (hl++)
	inc	hl
	ld	b, a		; b = a
	ld	a, (hl)		; a = (hl++)
	inc	hl
	ld	c, a		; c = a
	    ; if b is 0xff, call 'textcenter'
	ld	a, b
	cp	#0xff
	jr	nz, .pstrRes	; not 0xff, resume below
	call	textcenter	; = 0xff, autocenter the text
.pstrRes:
	pop	af		; restore 'a'
	;jr	putstrB		; fall through
	
        ;; putstrB - get the vid/color buffer offset of the X Y coordinates
        ;               in      hl      pointer to the string (pascal ascii)
        ;               in      b       x position
        ;               in      c       y position
        ;               in      a       color
        ;               out     -
        ;               mod     -
offsadd = -32
offssub = 32
putstrB::
            ; set aside registers
        push    hl
        push    bc
        push    de
        push    ix
        push    iy
        push    hl
            ; compute the offsets
        call    xy2offsB        ; hl = core offset
        push    hl
        pop     ix              ; move the offset into ix (char ram)
        push    hl
        pop     iy              ; move the offset into iy (color ram)
        ld      de, #(vidram)   ; base of video ram
        add     ix, de          ; set IX to appropriate location in vid ram
        ld      de, #(colram)   ; base of color ram
        add     iy, de          ; set IY to appropriate location in color ram
            ; prep for the loop
        pop     hl
        ld      b, (hl)         ; b is the number of bytes (pascal string)
        inc     hl              ; HL points to the text now
        ld      de, #offsadd    ; set up the column offset
.pstrb1:
            ; loop for each character
        ld      c, (hl)         ; c = character
        ld      (ix), c         ; vidram[b+offs] = character
        ld      (iy), a         ; colram[b+offs] = color
            ; adjust pointers
        inc     hl              ; inc string location
        add     ix, de          ; add in offset into char ram
        add     iy, de          ; add in offset into color ram
        djnz    .pstrb1         ; dec b, jump back if not done
            ; restore registers
        pop     iy
        pop     ix
        pop     de
        pop     bc
        pop     hl
            ; return
        ret


        ;; putstrC - get the vid/color buffer offset of the X Y coordinates
        ;               in      hl      pointer to the string (asciz)
        ;               in      b       x position
        ;               in      c       y position
        ;               in      a       color
        ;               out     -
        ;               mod     -
putstrC::
            ; set aside registers
        push    bc
        inc     c               ; just change indexing 0,1 into 2,3
        inc     c
        jp      .psChook        ; jump back into putstrA

; The XY-offset table
.scroffs:
        .word   0x03a0, 0x0380, 0x0360, 0x0340
        .word   0x0320, 0x0300, 0x02e0, 0x02c0
        .word   0x02a0, 0x0280, 0x0260, 0x0240
        .word   0x0220, 0x0200, 0x01e0, 0x01c0
        .word   0x01a0, 0x0180, 0x0160, 0x0140
        .word   0x0120, 0x0100, 0x00e0, 0x00c0
        .word   0x00a0, 0x0080, 0x0060, 0x0040

; The Region A and C offset table
.acoffs:
        .word   0x03dd          ; Region A row 'E' -> AC row 0
        .word   0x03fd          ; Region A row 'F' -> AC row 1   
        .word   0x001d          ; Region C row 'A' -> AC row 2
        .word   0x003d          ; Region C row 'B' -> AC row 3


;; prtXYbyteB
;;	outputs a byte to region B via coordinates
;	a:  byte to draw
;	e:  color
;	bc: offset XY
prtXYbyteB:
	push	bc
	call	xy2offsB
	push	hl
	pop	bc
	ld	d, a
	call	prtBCDbyte
	pop	bc
	ret

;; prtBCDbyte
;;	outputs a byte to regions A or C of the screen
	; d:  byte to draw
	; e:  color
	; bc: offset into video/color buffer
prtBCDbyte:
	    ; first, output the byte
	push	hl
	push	af
	push	de
	ld	hl, #(vidram)
	add	hl, bc
	ld	a,d
	call 	.prt_nib	; print out the nibble
	call	.prthlinc	; next pos
	ld	a, d
	rra       
	rra
	rra
	rra
	call	.prt_nib	; print out the nibble

	    ; then the color
	ld	hl, #(colram)
	add	hl, bc
	ld	a, e
	ld	(hl), a
	call	.prthlinc	; next pos
	ld	(hl), a
	pop	de
	pop	af
	pop	hl
	ret

.prthlinc:
	push 	af
	ld	a, b
	cp	#0x03
	jr 	nz, .prtn3
	ld	a, c
	cp	#0xbf
	jr	nc, .prtn0	; greater than 0x03bf, .prtn0
	
.prtn3:
	ld	a, b
	cp	#0x00
	jr	nz, .prtn2
	ld	a, c
	cp	#0x40
	jr	c, .prtn0	; less than 0x0040, .prtn0

.prtn2:
	    ; add offset (section B)
	push	de
        ld      de, #offssub    ; set up the column offset
	add	hl, de
	pop	de
	pop	af
	ret

.prtn0:	    ; add offset (section A or C)
	pop	af
	inc	hl
	ret


.prt_nib:
	and	#0x0f		; mask it off
	push	af		; store it aside
	cp	#0x0a		; check for > 9
	jr	c, .b0_09
	pop	af
	add	a, #0x41-#0x0a	; print 'A' instead of 0x0a
	jr	.b0_prt
.b0_09:
	pop	af
	add	a, #0x30	; print '0' instead of 0x00
.b0_prt:
	ld	(hl), a
	ret


;; prtBCDword
;;	outputs a byte to the screen (Region B)
	; hl: address of word to draw
	; e:  color
	; bc: xy position
prtBCDword:
	push	hl
	call	xy2offsB
	push	hl
	pop	bc

	pop	hl
	ld	a, (hl)
	ld	d, a
	call	prtBCDbyte

	inc	hl
	ld	a, (hl)
	ld	d, a
	push	hl
	push	bc
	pop	hl

	    ; change this section for region A/C
        ld      bc, #offssub    ; set up the column offset
	add	hl, bc
	add	hl, bc

	push	hl
	pop	bc
	pop	hl
	call	prtBCDbyte
	ret


;; prtABCDword
;;	outputs a word to the screen (Region A or C)
	; hl	address of word to draw
	; e	color
	; bc 	address
prtABCDword:
	push	hl
	ld	a, (hl)
	ld	d, a
	call	prtBCDbyte	; lsb

	pop	hl
	inc	hl
	inc	bc
	inc	bc
	ld	a, (hl)
	ld	d, a
	call	prtBCDbyte	; msb
	ret
