;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  MoviePlayer
;;
;;   movie playback engine
;;
;;   (c) 2007 Scott Lawrence
;;        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        



movieTask:
        .word   mp_title        ; title text
	.byte	0x01		; version major
	.byte	0x00		; version minor
        .word   mp_desc1        ; description line 1
        .word   mp_desc2        ; description line 2
        .word   mp_att0         ; attract sequence init
        .word   mp_att          ; attract sequence poll
        .word   mp_game0        ; game init
        .word   mp_game         ; game poll
        .byte   1               ; 1 credit
        .word   mp_variants     ; variant list

mp_variants:
	.byte	0

mp_title:
        .byte   17
        ;;;;;   "                 "
        .ascii  "Demo Movie Player"

mp_desc1:
	.byte	21
        ;;;;;   "                     "
        .ascii  "  Plays a FMV clip.  "

mp_desc2:
	.byte	21
        .ascii  "                     "

mp_att0:
mp_att:
	ret


mp_game0:
        call    cls

        ; draw the characters
        ld      hl, #(vidram)
        ld      a, #163			; character to color
        ld      b, #0x04
        call    memsetN
mp_game:
	; pick the right image
	ld	a, (timer)
	rr	a
	rr	a			; these slow it down
	and	#0x07
	add	a			; this is to multiply it by 2 for ptr
	ld	e, a			; DE now has 0x0007 & timer
	ld	d, #0x00
	ld	hl, #mp_table
	rst	0x10			; HL now has the right table to draw
	ex	de, hl			; DE now has it instead

        ; draw the color
        ld      hl, #(colram + 0x0040)
        call    rledraw
        ret

mp_table:
	.word 	#JFS_20
	.word 	#JFS_21
	.word 	#JFS_22
	.word 	#JFS_23

	.word 	#JFS_24
	.word 	#JFS_25
	.word 	#JFS_26
	.word 	#JFS_27


MOVIE_SZ = .-movieTask
