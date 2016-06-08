;  horizPac 1.0
;  (c) 2004 Jerry Carpenter

; This is a hack for Ms. Pac-Man to make it run as a horizontal game,
; in a cabinet with an unrotated monitor.
; 
; The romset used for this hack was the standard Ms. Pac-Man.  It is
; not the "hard", "fast", "Miss Pac Plus" or any other romset.
; 
; Use the "mspacmab.zip" romset as your base for running this.  If
; you want to try it in MAME, unzip mspacmab.zip, then copy the 5f,
; 5e, boot2, and boot4 roms into the generated mspacmab directory,
; and remember to set the monitor to not be rotated.
;
;  this also requires a modified graphics set, with rotated characters.
;  which is contained in the 'gfx' directory

.area	.CODE (ABS)

;textual strings
.org	0x3715
	.ascii	"EROCS@HGIH"	; high score
.org	0x3725
	.ascii	"@SDERC"	; CREDIT
.org	0x3734	
	.ascii	"YALP@EERF"	; free play
.org	0x3743
	.ascii	"ENO@REYALP"	; Player one
.org	0x375c	
	.ascii	"OWT@REYALP"	; Player one
.org	0x376c
	.ascii	"REVO@@EMAG"	; game over
.org	0x377c
	.ascii	"[YDAER"	; ready!
.org	0x3788
	.ascii	"NOTTUB@TRATS@HSUP"	; push start button
.org	0x379f
	.ascii	"YLNO@REYALP@1"	; 1 player only
.org	0x37b3
	.ascii	"SREYALP@2@RO@1" ; 1 or 2 players

.org	0x3807
	.ascii	"'NAM;CAP@SM&"	; ms.pac-man
.org	0x390c
	.ascii	"YROMEM@@KO"
.org	0x391c
	.ascii	"DAB@@@@M@R"
.org	0x392c
	.ascii	"TIDERC@1@@NIOC@1"
.org	0x3943
	.ascii	"TIDERC@1@SNIOC@2"
.org	0x395a
	.ascii	"STIDERC@2@@NIOC@1"
.org	0x3971
	.ascii	"YALP@@EERF"
.org	0x3988
	.ascii	"SUNOB@@ON@@"
.org	0x3999
	.ascii	"SUNOB"
.org	0x39b2
	.ascii	"ELBAT"
.org	0x39bf
	.ascii	"THGIRPU"

.org	0x3d3e
	.ascii	"OC@GFM@YAWDIM@\\"
.org	0x3d6c
	.ascii	"YKNILB"
.org	0x3d8b
	.ascii	"YKNIP"
.org	0x3d97
	.ascii	"NAM;CAP@SM"
.org	0x3daa
	.ascii	"YKNI"
.org	0x3db8
	.ascii	"1981:0891"
.org	0x3dcc
	.ascii	"EUS"
.org	0x3dd5
	.ascii	"ROINUJ"
.org	0x3de5
	.ascii	"HTIW"
.org	0x3df4
	.ascii	"ESAHC@ETH"

.org	0x3e04
	.ascii	"GNIRRATS"
.org	0x3e13
	.ascii	"NEM;CAP@SM"
.org	0x3e32
	.ascii	"TCA"
.org	0x3e42
	.ascii	"TEEM@YEHT"
	


;code change to fix "1UP" and "2UP" into " 1P"  and " 2P"
.org	0x036d
	ld	1(ix),#0x31	; '1'
	ld	2(ix),#0x40	; ' '

.org	0x037a
	ld	1(iy),#0x32	; '2'
	ld	2(iy),#0x40	; ' '



;and now the code modifications
.org	0x1886
	bit	3, a		; up
.org	0x1899
	bit	0, a		; down

.org	0x18c8
	bit	3, a		; up
.org	0x18cd
	bit	0, a		; down
.org	0x18d2
	bit	1, a		; right
.org	0x18d7
	bit	2, a		; left
