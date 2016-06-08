;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	sleep.asm
;
;	sleep routine
;
;  	2004,2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module sleep


        ;; sleep - wait a specified number of ticks
        ;               in      bc      number of ticks to wait
        ;               out     -
        ;               mod     -
sleep:
            ; set side some registers
        push    af
        push    bc
        push    hl

            ; compute the timeout into BC
        ld      hl, (timer)     ; hl = timer
        add     hl, bc          ; hl += ticks to wait
        push    hl              ; bc = 
        pop     bc              ;    = hl
.slp:
	    ; check coindrops
	call	coincheck	; just in case (Added 2006)

            ; loop until the timeout comes
        ld      hl, (timer)     ; hl = current time
        sbc     hl, bc          ; set flags
	jp      M, .slp         ; if (HL >= BC) then JP .slp2

            ; restore the registers
        pop     hl
        pop     bc
        pop     af
            ; return
        ret
