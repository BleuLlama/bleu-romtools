;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	at.asm
;
;	at scheduling simple scheduling for a single event at a certain time
;
;	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module at


; for the atControl variable: (bit numbers)

	atc_scheduled	= 1	; event is scheduled for execution
	; there is an event scheduled currently for timer 2

	atc_2scheduled	= 2	; event 2 is scheduled execution
	; there is an event scheduled currently for timer 2

	atc_checked	= 4	; atCheck was called
	; the check happened.  This can be cleared by user code. 
	; this is useful for once-a-tick events, and we get it for free.

	atc_service	= 5	; event 1 is ready to be run in the FG
	atc_2service	= 6	; event 2 is ready to be run in the FG


; atSchedule - sets the passed in function to be called at the specified 
;		time. The called function should be VERY short.  As 
;		explained below, the user code can change registers:
;			HL, AF, BC, DE 
;		without restoring them.
;
;	    in 	bc	time to call
;	    in	de	function to call
;	    out	-
;	    mod	-
atSchedule::
	call	atClear			; prevent race conditions
.atSched0:
	ld	(atFunc), de
	ld	(atTime), bc		; set it up
	push	hl
	ld	hl, #(atControl)
	set	#atc_scheduled, (hl)	; enable it
	pop	hl
	ret

; and the same thing for the second timer
at2Schedule::
	call	at2Clear		; prevent race conditions
.atSched2:
	ld	(at2Func), de
	ld	(at2Time), bc		; set it up
	push	hl
	ld	hl, #(atControl)
	set	#atc_2scheduled, (hl)	; enable it
	pop	hl
	ret


; atDelta - the same as atSchedule except that BC contains a delta
;		from the current time to perform the operation.
;
;	    in	bc	delta from "now" to call
;	    in	de	function to call
;	    out	-
;	    mod	-
atDelta::
	call	atClear			; prevent race conditions
	push	hl
	ld	hl, (timer)
	add	hl, bc
	push	hl
	pop	bc
	call	.atSched0		; do some of the stuff up above
	pop	hl
	ret

at2Delta::
	call	at2Clear		; prevent race conditions
	push	hl
	ld	hl, (timer)
	add	hl, bc
	push	hl
	pop	bc
	call	.atSched2		; do some of the stuff up above
	pop	hl
	ret



; atClear - used to clear out an at timer.  This is done by resetting the 
;		bit in the control register.
;
;	    in	-
;	    out	-
;	    mod	-
atClear::
	push	de
	push	hl
	ld	de, #0x0000
	ld	(atFunc), de		; not necessary, but cleaner
	ld	(atTime), de		; not necessary, but cleaner
	ld	hl, #(atControl)
	res	#atc_scheduled, (hl)	; clear that bit
	pop	hl
	pop	de
	ret

at2Clear::
	push	de
	push	hl
	ld	de, #0x0000
	ld	(at2Func), de		; not necessary, but cleaner
	ld	(at2Time), de		; not necessary, but cleaner
	ld	hl, #(atControl)
	res	#atc_2scheduled, (hl)	; clear that bit
	pop	hl
	pop	de
	ret


; atService - used in the foreground to actually make the calls if it's time to
;
;	in  - 
;	out -
;	mod - 
atService::
	ld	hl, #(atControl)
	bit	#atc_service, (hl)
	call	nz, .at1service		; service timer 1

	ld	hl, #(atControl)
	bit	#atc_2service, (hl)
	call	nz, .at2service		; service timer 2
	ret

.at1service:
	res	#atc_scheduled, (hl)
	res	#atc_service, (hl)
	ld	hl, (atFunc)		; hl = function to call
	jp	(hl)			; scheduled fcn will "ret"


.at2service:
	res	#atc_2scheduled, (hl)
	res	#atc_2service, (hl)
	ld	hl, (atFunc)		; hl = function to call
	jp	(hl)			; this will continue in the calling fcn





; atCheck - used by the interrupt routine to check to see if we need to
;		make the at calls yet or not.   This happens within the
;		interrupt routine.
;
;	    in	-
;	    out	-
;	    mod	-
atCheck::
	push	hl			; preserve registers
	push	af
	push	bc
	push	de


	ld	hl, #(atControl)
	set	#atc_checked, (hl)	; set the flag saying we were in here

	ld	de, (timer)		; de = now

	bit	#atc_scheduled, (hl)	; check to see if scheduled...
	call	nz, .atC000		; yep, continue below

	ld	hl, #(atControl)

	bit	#atc_2scheduled, (hl)	; check to see if 2nd timer is scheduled
	call	nz, .atC222		; yep, continue below

	pop	de			; restore registers
	pop	bc
	pop	af
	pop	hl
	ret				; return


	; set the service flag if we're at the right time
.atC000:
	    ; check to see if time is correct
	ld	bc, (atTime)		; bc = time to check

	    ; check the bottom byte (more likely to be different)
	ld	a, e
	cp	c
	ret	nz 			; bottom byte diff, return

	    ; check the top byte (more likely to be correct)
	ld	a, d
	cp	b
	ret	nz			; top byte diff, return

	    ; clear scheduled event flag
	res	#atc_scheduled, (hl)	; clear scheduled event flag

	    ; set the service flag
	ld	hl, #(atControl)
	set	#atc_service, (hl)
	ret


	; set the service flag if we're at the right time
.atC222:
	    ; check to see if time is correct
	ld	bc, (at2Time)		; bc = time to check

	    ; check the bottom byte (more likely to be different)
	ld	a, e
	cp	c
	ret	nz			; bottom byte diff, return

	    ; check the top byte (more likely to be correct)
	ld	a, d
	cp	b
	ret	nz			; top byte diff, return

	    ; clear scheduled event flag
	ld	hl, #(atControl)
	res	#atc_2scheduled, (hl)	; clear scheduled event flag

	    ; set the service flag
	ld	hl, (atControl)
	set	#atc_service, (hl)
	ret
