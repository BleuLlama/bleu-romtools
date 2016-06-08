;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	settings.asm
;
;	Various settings
;
;  	2006  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module settings

; set this to 0 to remove debug hooks (extra displays, etc.)
        DEBUG		= 0

;; timeout values
;   These values determine how long each segment of the auto-screen
;	changer lasts.  This is based on a counter overflowing.  That 
;	counter overflows about once every four seconds.

; 0x0040 = 1 second
	ONESECOND = 0x0040
; 0x0100 = 4 seconds
; 0x0800 = 32 seconds
; 0x1000 = about a minute
; 0x2000 = about two minutes
	
	SSTIMEOUT	= (ONESECOND * 30)

	DEATHTIME	= (ONESECOND * 4)
