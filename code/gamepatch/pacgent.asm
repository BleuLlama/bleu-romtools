; Pac-Gentleman
; (c)2007 Scott Lawrence  http://umlautllama.com
; based on: 
; http://technabob.com/blog/2007/11/01/pac-man-thats-pac-gentleman-to-you/

.area	.CODE (ABS)

; first. some game play changes 
        ; HACK2 : fast mod
.org    0x180b
        ;.byte     0x01

            ;; HACK4mod - boot patch to skip the self test
.org    0x3000  
        overboot = 0x30c1       ; jump position to skip the boot sequence
        stack = 0x4fc0
        ld      sp, #(stack)
        jp      overboot        ; skip over to the boot area

            ;; HACK4            
.org    0x30f0
        nop     
        nop
        nop
        


; make sure people realize it's a hack, and not original roms
.org    0x0fb0
        .ascii "Hacked by Scott "
        .ascii "   Nov. 2007    "
        .ascii "umlautllama.com "


; change text strings that can fit in their original containers
.org    0x3715
        .ascii  "@TOP@RAFT@"    ; high score
.org    0x3725
        .ascii  "@@BITS"        ; CREDIT
.org    0x3734  
        .ascii  "FREE@PLAY"     ; free play
.org    0x3743
        .ascii  "CORRIVAL@1"    ; Player one
.org    0x375c  
        .ascii  "CORRIVAL@2"    ; Player one
.org    0x376c
        .ascii  "@@SACKED@@"    ; game over
.org    0x377c
        .ascii  "MARKS["        ; ready!
.org    0x3788
        .ascii  "@WIND@CRANK@FOR@@"     ; push start button
.org    0x379f
        .ascii  "@1@CORRIVAL@@" ; 1 player only
.org    0x37b3
        .ascii  "@2@CORRIVALS@@" ; 1 or 2 players
.org    0x3df4  
        .ascii  "YE@CHASE@"

; change text strings that need to be moved elsewhere
.org	0x36bd
	.word	n_title
	.word	n_ghost1
.org	0x36c3
	.word	n_ghost2
.org	0x3703
	.word	n_ghost3
.org	0x3707
	.word	n_ghost4
.org	0x370b
	.word	n_pacgent
.org	0x36cb
	.word	n_cprt1
.org	0x370f
	.word	n_cprt2
	

.org	0x30fb
n_title:
	.word	0x0365
	.ascii	"@@@@@@PAC;GENTLEMAN"
	.byte	0x2f
	.byte	0x89
	.byte	0x2f, 0x80

n_ghost1:
	.word	0x026e
	.ascii	"@BREWSTER@@"
	.byte	0x2f, 0x81
	.byte	0x2f, 0x80

n_ghost2:
	.word	0x026e
	.ascii	"@REGINALD@@"
	.byte	0x2f, 0x83
	.byte	0x2f, 0x80

n_ghost3:
	.word	0x026e
	.ascii	"@@CHESTER@@"
	.byte	0x2f, 0x85
	.byte	0x2f, 0x80

n_ghost4:
	.word	0x026e
	.ascii	"@@@SADIE@@@"
	.byte	0x2f, 0x87
	.byte	0x2f, 0x80

n_pacgent:
	.word	0x026e
	.ascii	"PAC;GENTLEMAN"
	.byte	0x2f, 0x89
	.byte	0x2f, 0x80
.bound	0x3173


; "copyright" strings
.org	0x2fc0
n_cprt1:
	.word	0x037a
        .ascii  "\\1880@MESSRS@NAM-NAM@-@C."
	.byte	0x2f, 0x81
	.byte	0x2f, 0x80

n_cprt2:
	.word	0x02dd
	.ascii	"@"
        ;.ascii  "NAM-NAM@AND@CO@"
	.byte	0x2f, 0x81
	.byte	0x2f, 0x80
.bound	0x2fff	


.org	0x3e02
	.byte	0x8b	; move "STARRING" over so it looks better


; disable the MIDWAY double-M logo 
.org	0x964a
	ret


; code change to fix "1UP" and "2UP" into "No1", "No2"
.org    0x0369
        ld      2(ix),#0x4e     ; 'N'
        ld      1(ix),#0x2e     ; 'o'
        ld      0(ix),#0x31     ; '1'

.org    0x0376
        ld      2(iy),#0x4e     ; 'N'
        ld      1(iy),#0x2e     ; 'o'
        ld      0(iy),#0x32     ; '2'
