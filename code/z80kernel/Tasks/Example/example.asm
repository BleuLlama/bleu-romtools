;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Example
;;
;;   game engine example
;;
;;   (c) 2007 Scott Lawrence
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; The module definition block
xxTask:
	.word	xx_title	; title text
	.byte	0x00		; version major
	.byte	0x01		; version minor
	.word	xx_desc1	; description line 1
	.word	xx_desc2	; description line 2
	.word	xx_att0		; attract sequence init
	.word	xx_att		; attract sequence poll
	.word	xx_game0	; game init
	.word	xx_game		; game poll
	.byte	1		; 1 credit per play
	.word	xx_variants	; variant list

	; variants...
xx_variants:
	.byte	2
	.word	xx_Voriginal
	.word	xx_Valternate

	; variant names
xx_Voriginal:
	.byte	17
	;;;;;   "                 "  ; this is just here as a reference of width
	.ascii	"     Original    "
xx_Valternate:
	.byte	17
	;;;;;   "                 "
	.ascii	"    Alternate    "

	; title text
xx_title:
	.byte	17
	;;;;;   "                 "
	.ascii	"  Example Title  "

	; title description
xx_desc1:
	.byte	21
	;;;;;   "                     "
	.ascii  " Description line 1. "
xx_desc2:
	.byte	21
	;;;;;   "                     "
	.ascii  " Description line 2. "


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; attract sequence

; this gets polled from the core menu system.
; it should do a single step of your attract animation sequence

xx_att0:
	call	clrsprite

xx_att:
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Game Play

; this gets polled from the core menu system.
; it should do a single step of your game play

	; initialize the game
xx_game0:

	; run the game
xx_game:
	ret


XX_SZ	= .-xxTask
