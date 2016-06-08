;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	memset.asm
;
;	bulk memory set routines
;
;	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module memset

; memset256
        ;; memset256 - set up to 256 bytes of ram to a certain value
        ;               in      a       value to poke
        ;               in      b       number of bytes to set 0x00 for 256
        ;               in      hl      base address of the memory location
        ;               out     -
        ;               mod     hl, bc
memset256::
        ld      (hl), a         ; *hl = 0
        inc     hl              ; hl++
        djnz    memset256       ; decrement b, jump to memset256 if b>0
        ret                     ; return


; memsetN
        ;; memsetN - set N blocks of ram to a certain value
        ;               in      a       value to poke
        ;               in      b       number of blocks to set
        ;               in      hl      base address of the memory location
        ;               out     -
        ;               mod     hl, bc
memsetN::
        push    bc              ; set aside bc
        ld      b, #0x00        ; b = 256
        call    memset256       ; set 256 bytes
        pop     bc              ; restore the outer bc
        djnz    memsetN         ; if we're not done, set another chunk.
        ret                     ; otherwise return

