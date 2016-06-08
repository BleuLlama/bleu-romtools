;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	input.asm
;
;	input centralizer, debouncing routines
;
;	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module input


; this is the loop polled by the interrupt routine.  this, in turn, calls 
; each of the polling routines below, to debounce input (only looks for 
; transitions from pressed to released)  Even though some of the port bytes
; are shared, they are separated for logical reasons, and also because this
; can more easily be ported to systems that have unshared bytes in the future.

inputpoll:
	push	af
	push	hl
	call	.ip_coins	; coin slots
	call	.ip_start	; start buttons
	call	.ip_p1joy	; player 1 stick
	call	.ip_p2joy	; player 2 stick
	pop	hl
	pop	af
	ret

; clear all of the current input counters
inputclear:
	xor	a		; a = 0
	ld	(Cp1up), a
	ld	(Cp1dn), a
	ld	(Cp1lt), a
	ld	(Cp1rt), a
	ld	(Cp2up), a
	ld	(Cp2dn), a
	ld	(Cp2lt), a
	ld	(Cp2rt), a
	ld	(Cstart1), a
	ld	(Cstart2), a
	ld	(Ccoin1), a
	ld	(Ccoin2), a
	ld	(Ccoin3), a
	ret
	
	



; in the following blocks, only the first set of new code will be heavily 
; commented.  in a lot of cases, it's the same code repeated multiple times
; for each of the ports or bits on those ports.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; coin slot input

.ip_coins:
	ld	a, (coin_port)		; a = current coin slot information
	ld	hl, #(thiscoin)		; hl = address of "thiscoin"
	ld	(hl), a			; store current value in "thiscoin"

	push	hl			; store this aside for a moment

	bit	#coin1, (hl)		; check the coin1 bit on CURRENT
	jr	nz, .ipcc2		; not set, skip to the next coin slot
	ld	hl, #(lastcoin)		; hl = PREVIOUS 
	bit	#coin1, (hl)		; check the coin1 bit on PREVIOUS
	jr	z, .ipcc2		; set, skip to the next coin slot

	    ; yup!  there was a level change (difference between the two)
	ld	a, (Ccoin1)		; a = Ccoin1
	inc	a			; a++
	ld	(Ccoin1), a		; Ccoin1 = a

; note, this might be replaced by something like:
;	ld 	a, (lastcoin)
;	ld	c, a
;	ld	a, (thiscoin)
;	xor	c			; a = differences between prev and this
;	and	MASK FOR THIS COIN
;	cp	MASK FOR THIS COIN
;	etc.
; but the problem is that this is a similar number of cycles, but will catch 
; both the rising and falling edges of the user input, which means that each
; trigger will cause two events.   you can then reload thiscoin (or lastcoin)
; and and with that... if it's nonzero, then cause a trigger event, but frankly
; that seems more complicated than the above.

.ipcc2:
	pop	hl
	push	hl
	bit	#coin2, (hl)
	jr	nz, .ipcc3
	ld	hl, #(lastcoin)
	bit	#coin2, (hl)
	jr	z, .ipcc3

	ld	a, (Ccoin2)
	inc	a
	ld	(Ccoin2), a

.ipcc3:
	pop	hl
	bit	#coin3, (hl)
	jr	nz, .ipcRET
	ld	hl, #(lastcoin)
	bit	#coin3, (hl)
	jr	z, .ipcRET
	
	ld	a, (Ccoin3)
	inc	a
	ld	(Ccoin3), a

.ipcRET:
	ld	a, (thiscoin)		;          = thiscoin
	ld	(lastcoin),a		; lastcoin =
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Coin accessors

; these will either return with 0 if the requested event was not triggered,
; or it will return with a 1 if the event has been triggered.
; they will also decrement the counter if it was greater than zero

Get1Coin:
	ld	a, (Ccoin1)		; a = Coin 1 counter
	cp	#0x00			; is it 0?
	ret	z			; if yes, just return.
	dec	a			; a--
	ld	(Ccoin1), a		; Coin 1 counter = a
	ld	a, #1			; a = 1 (for the caller, return value)
	ret				; return!

Get2Coin:
	ld	a, (Ccoin2)
	cp	#0x00
	ret	z
	dec	a
	ld	(Ccoin2), a
	ld	a, #1
	ret

Get3Coin:
	ld	a, (Ccoin3)
	cp	#0x00
	ret	z
	dec	a
	ld	(Ccoin3), a
	ld	a, #1
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; start buttons

.ip_start:
	ld	a, (start_port)
	ld	hl, #(thisstart)	; this = current
	ld	(hl), a

.ip_s1:
        push    hl
        bit     #start1, (hl)		; check for start button 1
        jr      nz, .ip_s2
        ld      hl, #(laststart)
        bit     #start1, (hl)
        jr      z, .ip_s2

	    ; start 1 was pressed
	ld	a, (Cstart1)
	inc	a
	ld	(Cstart1), a

.ip_s2:
	pop	hl
	bit     #start2, (hl)		; check for start button 2
        jr      nz, .ip_sdone
        ld      hl, #(laststart)
        bit     #start2, (hl)
        jr      z, .ip_sdone

	    ; start 2 was pressed
	ld	a, (Cstart2)
	inc	a
	ld	(Cstart2), a

.ip_sdone:
	ld	a, (thisstart)
	ld	(laststart), a		; last = this
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; start accessors


Get1Start:
	ld	a, (Cstart1)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cstart1), a
	ld	a, #1
	ret

Get2Start:
	ld	a, (Cstart2)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cstart2), a
	ld	a, #1
	ret


; I can break out these a little, putting some re-used code in one
; place, but since it's just like 3 opcodes, the added jump time,
; and the jump opcode almost balances it out.  so for just a few
; bytes more, we'll just keep 'em together.

Is1Start:
	xor	a
	ld	hl, #(start_port)
	bit	#start1, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is2Start:
	xor	a
	ld	hl, #(start_port)
	bit	#start2, (hl)
	ret	nz
	ld	a, #0x01
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; input player 1 checker

.ip_p1joy:
	ld	a, (p1_port)
	ld	hl, #(thisp1p)
	ld	(hl), a			; thisp1p = p1_port

.ip_1_up:
	push	hl
	bit	#p1_up, (hl)
	jr	nz, .ip_1_down
	ld	hl, #(lastp1p)
	bit	#p1_up, (hl)
	jr	z, .ip_1_down
	
	ld	a, (Cp1up)
	inc	a
	ld	(Cp1up), a

.ip_1_down:
	pop	hl
	push	hl
	bit	#p1_down, (hl)
	jr	nz, .ip_1_left
	ld	hl, #(lastp1p)
	bit	#p1_down, (hl)
	jr	z, .ip_1_left
	
	ld	a, (Cp1dn)
	inc	a
	ld	(Cp1dn), a

.ip_1_left:
	pop	hl
	push	hl
	bit	#p1_left, (hl)
	jr	nz, .ip_1_right
	ld	hl, #(lastp1p)
	bit	#p1_left, (hl)
	jr	z, .ip_1_right
	
	ld	a, (Cp1lt)
	inc	a
	ld	(Cp1lt), a

.ip_1_right:
	pop	hl
	bit	#p1_right, (hl)
	jr	nz, .ip1jdone
	ld	hl, #(lastp1p)
	bit	#p1_right, (hl)
	jr	z, .ip1jdone
	
	ld	a, (Cp1rt)
	inc	a
	ld	(Cp1rt), a

.ip1jdone:
	ld	a, (thisp1p)
	ld	(lastp1p), a		; last = this
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; player 1 accessors

Get1Up:
	ld	a, (Cp1up)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp1up), a
	ld	a, #1
	ret

Get1Down:
	ld	a, (Cp1dn)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp1dn), a
	ld	a, #1
	ret

Get1Left:
	ld	a, (Cp1lt)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp1lt), a
	ld	a, #1
	ret

Get1Right:
	ld	a, (Cp1rt)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp1rt), a
	ld	a, #1
	ret


;;; and the "Is" versions

Is1North:
Is1Up:
	xor	a
	ld	hl, #(p1_port)
	bit	#p1_up, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is1South:
Is1Down:
	xor	a
	ld	hl, #(p1_port)
	bit	#p1_down, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is1West:
Is1Left:
	xor	a
	ld	hl, #(p1_port)
	bit	#p1_left, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is1East:
Is1Right:
	xor	a
	ld	hl, #(p1_port)
	bit	#p1_right, (hl)
	ret	nz
	ld	a, #0x01
	ret


;;; player 2

Is2Up:
	xor	a
	ld	hl, #(p2_port)
	bit	#p2_up, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is2Down:
	xor	a
	ld	hl, #(p2_port)
	bit	#p2_down, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is2Left:
	xor	a
	ld	hl, #(p2_port)
	bit	#p2_left, (hl)
	ret	nz
	ld	a, #0x01
	ret

Is2Right:
	xor	a
	ld	hl, #(p2_port)
	bit	#p2_right, (hl)
	ret	nz
	ld	a, #0x01
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; input player 2 checker

.ip_p2joy:
	ld	a, (p2_port)
	ld	hl, #(thisp2p)
	ld	(hl), a			; thisp2p = p2_port

.ip_2_up:
	push	hl
	bit	#p2_up, (hl)
	jr	nz, .ip_2_down
	ld	hl, #(lastp2p)
	bit	#p2_up, (hl)
	jr	z, .ip_2_down
	
	ld	a, (Cp2up)
	inc	a
	ld	(Cp2up), a

.ip_2_down:
	pop	hl
	push	hl
	bit	#p2_down, (hl)
	jr	nz, .ip_2_left
	ld	hl, #(lastp2p)
	bit	#p2_down, (hl)
	jr	z, .ip_2_left
	
	ld	a, (Cp2dn)
	inc	a
	ld	(Cp2dn), a

.ip_2_left:
	pop	hl
	push	hl
	bit	#p2_left, (hl)
	jr	nz, .ip_2_right
	ld	hl, #(lastp2p)
	bit	#p2_left, (hl)
	jr	z, .ip_2_right
	
	ld	a, (Cp2lt)
	inc	a
	ld	(Cp2lt), a

.ip_2_right:
	pop	hl
	bit	#p2_right, (hl)
	jr	nz, .ip2jdone
	ld	hl, #(lastp2p)
	bit	#p2_right, (hl)
	jr	z, .ip2jdone
	
	ld	a, (Cp2rt)
	inc	a
	ld	(Cp2rt), a

.ip2jdone:
	ld	a, (thisp2p)
	ld	(lastp2p), a		; last = this
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; player 2 accessors

Get2Up:
	ld	a, (Cp2up)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp2up), a
	ld	a, #2
	ret

Get2Down:
	ld	a, (Cp2dn)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp2dn), a
	ld	a, #2
	ret

Get2Left:
	ld	a, (Cp2lt)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp2lt), a
	ld	a, #2
	ret

Get2Right:
	ld	a, (Cp2rt)
	cp	#0x00
	ret	z
	dec	a
	ld	(Cp2rt), a
	ld	a, #2
	ret
