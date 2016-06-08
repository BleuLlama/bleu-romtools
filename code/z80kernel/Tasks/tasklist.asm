;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	tasklist.asm
;
;	list of available tasks, management routines
;
;	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module tasklist

; task list table
;	- this lists all of the tasks that the menu system provides
tasklist:
	.word	mspriteTask
	.word	absurdTask
	.word	movieTask
	.word	quadblokTask
	NTASKS = .-tasklist
	.word	ssaveTask
	.word	testModeTask
	.word	pactrackerTask
	FTASKS = (.-tasklist)/2

;; no need to have this in the menu...
	.word 	ssaveTask

; each entry in the above table should contain a few items:
;
;	.word	title		; pascal string - (title)
;	.byte	version mj	; major version BCD
;	.byte	version mn	; minor version BCD
;	.word	desc1		; pascal string - (description)
;	.word	desc2		; pascal string - (description)
;	.word	att0		; function pointer - attract intialization
;	.word	att		; function pointer - attract poll
;	.word	game0		; function pointer - game initialization
;	.word	game		; function pointer - game poll
;	.byte	credits		; number of credits per play
;	.word	variant list	; list of variants
