;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;	core.asm
;
;	System initialization and 0x0000 code
;
;	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module core0rt
.area   .CODE (ABS)

; RESET functions
	HARDBOOT	= 0x00
	SOFTBOOT	= 0xff

; RST 00
;	power on / hard boot startup
;	this gets called to inc the poweron counter, initialize machine
.org 0x0000                             ; RST 00 - Init
	nop				; quick fix to help with things
	di				; disable interrupts
	ld	a, #HARDBOOT		; hard boot flag
        jp	.start			; go!

; RST 08
;	soft boot startup
;	this is what games should call to resume menu operation
.org 0x0008
	di				; disable interrupts
	ld	a, #SOFTBOOT		; soft boot flag
	jp	.start			; go!


; RST 10
;	pointer dereferencing
;	load HL with the core address
;	load DE with the index
;	HL will result with 16 bit address contained in (HL+DE)
.org 0x0010
	add     hl, de          ; add the offset
	ld      e, (hl)
	inc     hl
	ld      d, (hl)
	ex      de, hl          ; complete the dereferencing
	ret


; RST 18
;	array element byte retrieval
;	in 	hl	base of array
;	in	a	element to retrieve
;	out	a	element at (hl+a)
.org 0x0018
	push	bc
	ld	b, #0x00
	ld	c, a
	add	hl, bc
	pop	bc
	ld	a, (hl)
	ret

; unused Reset functions
.org 0x0020
	.ds	8

.org 0x0028
	.ds	8

.org 0x0030
	.ds	8

; Interrupt routine
.org 0x0038
        jp .interrupt

; NMI - not used normally, but we'll leave the hook for it. 
.org 0x0066
        retn			; return for now.


.interrupt:
	di                      ; disable processor interrupts
	push    af              ; store aside register 'af' to the stack
	push    bc              ; store aside register 'bc' to the stack
	xor     a               ; a = 0
	ld      (irqen), a      ; disable the external interrupt mechanism
	ld	(watchdog), a	; kick the dog

	ld      bc, (timer)     ; bc = timer
	inc     bc              ; bc++
	ld      (timer), bc     ; timer = bc

	call	atCheck		; check for events
	;call	ptInterrupt	; update music
	call	inputpoll	; poll the input centralizer

	xor	a
	ld	(watchdog), a	; kick the dog
	ld      a, #0x01	; a = 1
	ld      (irqen), a      ; enable the external interrupt mechanism

	pop     bc              ; restore register 'bc' from the stack
	pop     af              ; restore register 'af' from the stack
	ei                      ; enable processor interrupts
	reti                    ; return from interrupt routine


; Generic Pac startup stuff.
.start:
	di			; disable processor interrupts
	ld	sp, #(stack)	; setup the initial stack pointer
	push	af		; set aside the soft/hard boot flag
	ld	a, #0xff	; a = 0xff
	out	(0), a		; set interrupt vector
	ld	(ptSpeed), a	; disable music playback
	xor     a               ; a = 0x00
	ld	(watchdog), a	; kick the dog
	im	1		; setup interrupt mode 1

		; clear everything...
	call	clrspecial	; covers audio too.
	call	clrsprite
	call	clruser
	call	atClear		; clear at hook

	xor     a               ; a = 0
	ld	(watchdog), a	; kick the dog

	ld	a, #0x01	; a = 1
	ld	(irqen), a	; enable interrupts
	ei			; enable interrupts

	pop	af		; restore soft/hard boot flag
	jp	main		; run!

.ascii	"Made in 2004-2006 by Scott Lawrence yorgle@gmail.com. "
