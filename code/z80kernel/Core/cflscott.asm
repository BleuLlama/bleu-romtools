;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 	cflscott.asm
;
;	draws the CFL version of me (RLE compressed image)
;
;  	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module cflscott

.STARTSPLASH:
;black   0x13
;white   0x19
;red     0x14
;flesh   0x10
;green   0x11
;
;yellow  0x16
;dteal   0x17
;cyan    0x18

; none of the followint xxCn variables should be in [0x01, 0x0f]

; colors need to be >=0x10, which is why i add on 0x20 - extra bits get
; lost on the hardware, which only looks at the bottom 0x1f of the color value
	cfl_char = 162  	; solid
	xxC1	= 0x20+0x00	; black
	xxC2	= 0x20+0x1c	; shirt
	xxC3	= 0x20+0x13	; background
	xxC4	= 0x20+0x01	; skin tone
	xxC5	= 0x20+0x06	; eyes - white

; to draw:
draw_cfl:
        call    cls                     ; fill screen black, 0
        ld      hl, #(vidram)
        ld      a, #cfl_char       
        ld      b, #0x04
        call    memsetN                 ; fill screen with 'cfl_char'

        ld      hl, #(colram + 0x0040)
        ld      de, #(cfl_jerr)
        call    rledraw                 ; render the image

	ld	a, #9
	ld	hl, #(.cprt)
	ld	bc, #0x0102
	call	putstrA			; draw (c)2006...

	ld	a, #8
	ld	hl, #(.name)
	ld	bc, #0x0703
	call	putstrA			; draw Scott...

	ld	a, #7
	ld	hl, #(VERSION)
	ld	bc, #0x0000
	call	textcenter		; center the text
	call	putstrA			; version

        ret

.cprt:
	.byte	26
	.ascii	"@2006-2012 UmlautLlama.com"

.name:
	.byte	14
	.ascii	"Scott Lawrence"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; and here's the image data...
	.globl	cfl_jerr
cfl_jerr:
        .byte   0x0f, xxC3, 0x0f, xxC3, xxC3, xxC1, 0x0f, xxC3, 0x0f
        .byte   xxC3, xxC1, xxC2, 0x0f, xxC3, 0x0f, xxC3, xxC1, xxC2
        .byte   0x0f, xxC3, 0x0e, xxC3, xxC1, xxC2, xxC2, 0x0f, xxC3
        .byte   0x0e, xxC3, xxC1, xxC2, xxC2, 0x0f, xxC3, 0x0d, xxC3
        .byte   xxC1, 0x03, xxC2, 0x08, xxC3, xxC1, xxC1, 0x04, xxC3
        .byte   xxC1, 0x0d, xxC3, xxC1, 0x03, xxC2, 0x08, xxC3, xxC1
        .byte   xxC1, 0x03, xxC3, xxC1, xxC1, 0x0d, xxC3, xxC1, 0x03
        .byte   xxC2, 0x03, xxC3, xxC1, xxC1, 0x03, xxC3, 0x03, xxC1
        .byte   xxC3, 0x03, xxC1, 0x0c, xxC3, xxC1, 0x04, xxC2, 0x03
        .byte   xxC3, 0x04, xxC1, xxC3, xxC3, 0x07, xxC1, 0x0b, xxC3
        .byte   xxC1, 0x04, xxC2, 0x04, xxC3, 0x0f, xxC1, xxC1, xxC1
        .byte   0x06, xxC3, xxC1, xxC1, 0x03, xxC2, 0x05, xxC3, 0x06
        .byte   xxC1, 0x0a, xxC4, xxC1, xxC1, 0x03, xxC3, 0x04, xxC1
        .byte   xxC2, xxC2, 0x04, xxC3, 0x06, xxC1, 0x0d, xxC4, 0x03
        .byte   xxC1, 0x03, xxC4, xxC1, xxC2, xxC2, 0x05, xxC3, 0x05
        .byte   xxC1, xxC4, xxC1, xxC4, xxC4, 0x04, xxC5, 0x06, xxC4
        .byte   xxC1, 0x04, xxC4, xxC1, xxC2, xxC2, 0x04, xxC3, 0x06
        .byte   xxC1, xxC4, xxC4, xxC1, 0x03, xxC5, xxC1, xxC5, xxC5
        .byte   xxC4, xxC4, 0x05, xxC1, 0x03, xxC4, xxC1, xxC2, xxC2
        .byte   xxC3, xxC3, 0x08, xxC1, xxC4, xxC4, xxC1, xxC4, 0x04
        .byte   xxC5, 0x03, xxC4, xxC1, xxC4, 0x03, xxC1, 0x03, xxC4
        .byte   xxC1, xxC2, xxC2, xxC3, 0x0a, xxC1, xxC4, xxC4, xxC1
        .byte   xxC4, xxC4, xxC1, xxC1, xxC4, xxC1, xxC4, xxC1, xxC4
        .byte   0x07, xxC1, xxC2, xxC2, xxC3, xxC3, xxC1, xxC1, xxC3
        .byte   0x06, xxC1, 0x04, xxC4, xxC5, xxC5, xxC4, xxC1, xxC4
        .byte   xxC4, 0x05, xxC1, xxC3, xxC3, xxC1, 0x03, xxC2, 0x05
        .byte   xxC3, 0x07, xxC1, xxC4, xxC1, xxC5, xxC5, xxC1, xxC5
        .byte   0x05, xxC4, xxC1, 0x04, xxC3, xxC1, 0x03, xxC2, 0x04
        .byte   xxC3, 0x08, xxC1, xxC4, xxC1, xxC4, xxC5, xxC5, 0x05
        .byte   xxC4, xxC1, 0x05, xxC3, xxC1, 0x03, xxC2, 0x04, xxC3
        .byte   0x08, xxC1, 0x09, xxC4, xxC1, 0x06, xxC3, xxC1, 0x03
        .byte   xxC2, 0x03, xxC3, 0x04, xxC1, xxC3, xxC3, 0x0c, xxC1
        .byte   0x08, xxC3, xxC1, xxC2, xxC2, 0x04, xxC3, xxC1, xxC1
        .byte   xxC3, xxC3, 0x08, xxC1, 0x0d, xxC3, xxC1, xxC2, xxC2
        .byte   0x09, xxC3, 0x03, xxC1, xxC3, xxC1, xxC1, 0x0f, xxC3
        .byte   xxC1, xxC2, 0x08, xxC3, 0x03, xxC1, 0x03, xxC3, xxC1
        .byte   xxC1, 0x0e, xxC3, xxC1, xxC2, 0x08, xxC3, xxC1, xxC1
        .byte   0x0f, xxC3, 0x06, xxC3, xxC1, 0x0f, xxC3, 0x0f, xxC3
	.byte   0x0f, xxC3, 0x0f, xxC3, 0x04, xxC3
	.byte   0x02

SPLASH_SZ = .-.STARTSPLASH
