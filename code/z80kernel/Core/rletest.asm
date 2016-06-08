;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;       rletest.asm
;
;       Tester for RLE compression
;
;       2007-08  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        .module rletest


rle2_test:
        ; fill character ram with numbers
        ld      hl, #vidram
        ld      b, #0x04
.rt0002:
        push    bc
        ld      b, #0x00
.rt0001: 
        ld      (hl), b
        djnz    .rt0001
        pop     bc
        djnz    .rt0002

        ; fill color ram with 0x01
        ld      hl, #colram
        ld      a, #0x01
        ld      b, #0x04
        call    memsetN

        ; render the rle2 image
        ld      hl, #colram + 0x40
        ld      de, #rle2_example
        ld      ix, #rle2_colors
        call    rledraw2

        ret

rle2_colors:
	;		r     o     y     g     b     br   cyb
        .byte   0x00, 0x0d, 0x04, 0x05, 0x0e, 0x0b, 0x0c, 0x1f
        .byte   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07

rle2_example:
	; lookup table based ops
        .byte   0x40 + 4, 0x04  ; 4 bytes of LUT 4 4 4 4
        .byte   0x40 + 4, 0x05  ; 4 bytes of LUT 5 5 5 5
        .byte   0x40 + 4, 0x06  ; 4 bytes of LUT 6 6 6 6
        .byte   0x40 + 4, 0x07  ; 4 bytes of LUT 7 7 7 7

        .byte   0x20 + 16       ; skip 16 bytes

        .byte   0x60 + 4, 0x45  ; 8 bytes 4 5 4 5 4 5 4 5
        .byte   0x60 + 4, 0x67  ; 8 bytes 6 7 6 7 6 7 6 7

        .byte   0x80 + 4, 0x76, 0x54, 0x32, 0x10 ; these LUT directly

	; non-lookup table ops

        .byte   0xc0 + 6, 0x0d, 0x0e, 0x0b,  0x0b, 0x0e, 0x0d  ; copy directly

        .byte   0xa0 + 31, 0x0c ; copy these out there out there

        .byte   0x00            ; end of data

