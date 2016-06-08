	.title	Machine Independent Assembler Test
	.module	Asmtst

	.sbttl	Memory Allocation Directives

	.radix	O		;set default to octal

	.byte	0b11000000	;binary constants
	.byte	0B1110

	.byte	   24		;octal constants
	.byte	  024
	.byte	0q024
	.byte	0Q024
	.byte	0o024
	.byte	0O024

	.byte	0d024		;decimal constant
	.byte	0D024

	.byte	0h024		;hexidecimal constants
	.byte	0H024
	.byte	0x024
	.byte	0X024

	.db	0
	.dw	0

	.radix	D		;set default to decimal

	.byte	1,2,3,4,5,6,7,8,9
	.word	1,2,3,4,5,6,7,8,9

	.blkb	16
	.ds	16

	.blkw	16
	.ds	16*2

word:	.word	.+2
	.word	.-2
	.word	2+.
	.word	.-(word+2)


	.sbttl	Boundary Directives

	.even
	.byte	0
	.even
	.byte	1
	.even
	.odd
	.byte	2
	.odd
	.byte	3
	.odd
	.even
	.word	4
	.odd
	.word	5
	.even
	.word	6


	.sbttl	String Directives

	.ascii	"abcdefgh"
	.asciz	"abcdefgh"

	.page
	.sbttl	Expression Evaluation

	n0x00	=	0x00
	n0x01	=	0x01
	n0x10	=	0x10
	n0xff	=	0xff
	n0xeeff	=	0xeeff


	n	=	'A		;0x41		single character
	n	=	"AB		;0x4142		double character
	n	=	n0x01		;0x01		assignment
	n	=	n + n0x01	;0x02		addition
	n	=	n - n0x01	;0x01		subtraction
	n	=	n * 0x05	;0x05		multiplication
	n	=	n / 0x02	;0x02		division
	n	=	n0x10 % 0x05	;0x01		modulus
	n	=	n0x10 | n0x01	;0x11		or
	n	=	n0xff & n0x01	;0x01		and
	n	=	n0x01 << 4	;0x10		left shift
	n	=	n0x10 >> 4	;0x01		right shift
	n	=	n0xff ^ n0x10	;0xef		xor
	n	=	n ^ n0x10	;0xff		xor
	n	=	~n0x10		;0xffef		1's complement
	n	=	-n0x10		;0xfff0		2's complement

	n	=	n0xeeff & 0xff	;0xff		low byte
	n = (n0xeeff & 0xff00)/0x100	;0xee		high byte

	n	=	n0xeeff % 0x100	;0xff		low byte
	n	=	n0xeeff / 0x100	;0xee		high byte

	n	=	< n0xeeff	;0xff		low byte
	n	=	> n0xeeff	;0xee		high byte

	n	=	3*(2 + 4*(6))	;0x4e		expression evaluation
	n	=  2*(0x20 + <~n0x10)	;0x21e



	.page
	.sbttl	IF, ELSE, and ENDIF

	n = 0
	m = 0

	.if	0
		n = 1
	   .if	0
		m = 1
	   .else
		m = 2
	   .endif
	.else
		n = 2
	.endif

	.byte	n,m		; n = 2, m = 0

	;*******************************************************

	n = 0
	m = 0

	.if	1
		n = 1
	   .if	0
		m = 1
	   .else
		m = 2
	   .endif
	.else
		n = 2
	.endif

	.byte	n,m		; n = 1, m = 2


	.page

	n = 0
	m = 0

	.if	0
		n = 1
	   .if	1
		m = 1
	   .else
		m = 2
	   .endif
	.else
		n = 2
	.endif

	.byte	n,m		; n = 2, m = 0

	;*******************************************************

	n = 0
	m = 0

	.if	1
		n = 1
	   .if	1
		m = 1
	   .else
		m = 2
	   .endif
	.else
		n = 2
	.endif

	.byte	n,m		; n = 1, m = 1


	.page

	n = 0
	m = 0

	.if	0
		n = 1
	.else
	   .if	0
		m = 1
	   .else
		m = 2
	   .endif
		n = 2
	.endif

	.byte	n,m		; n = 2, m = 2

	;*******************************************************

	n = 0
	m = 0

	.if	1
		n = 1
	.else
	   .if	0
		m = 1
	   .else
		m = 2
	   .endif
		n = 2
	.endif

	.byte	n,m		; n = 1, m = 0


	.page

	n = 0
	m = 0

	.if	0
		n = 1
	.else
	   .if	1
		m = 1
	   .else
		m = 2
	   .endif
		n = 2
	.endif

	.byte	n,m		; n = 2, m = 1

	;*******************************************************

	n = 0
	m = 0

	.if	1
		n = 1
	.else
	   .if	1
		m = 1
	   .else
		m = 2
	   .endif
		n = 2
	.endif

	.byte	n,m		; n = 1, m = 0


	.page
	.sbttl	Local Symbols

lclsym0:
	.word	0$,1$,2$,3$,4$		;forward references
	.word	5$,6$,7$,8$,9$

0$:	.word	9$
1$:	.word	8$
2$:	.word	7$
3$:	.word	6$
4$:	.word	5$
5$:	.word	4$
6$:	.word	3$
7$:	.word	2$
8$:	.word	1$
9$:	.word	0$
10$:

	.word	0$,1$,2$,3$,4$		;backward references
	.word	5$,6$,7$,8$,9$

lclsym1:
	.word	0$,1$,2$,3$,4$		;forward references
	.word	5$,6$,7$,8$,9$

0$:	.word	9$
1$:	.word	8$
2$:	.word	7$
3$:	.word	6$
4$:	.word	5$
5$:	.word	4$
6$:	.word	3$
7$:	.word	2$
8$:	.word	1$
9$:	.word	0$
10$:

	.word	0$,1$,2$,3$,4$		;backward references
	.word	5$,6$,7$,8$,9$

	.sbttl	Offset calculations

	ofsbyte	=	(10$-0$)	;0x0014
	ofsword	=	ofsbyte/2	;0x000A

	.word	1$+ofsbyte+ofsword	;1$ + 0x001E

	.page
	.sbttl	Area Definitions

	.globl	code0
	.globl	cnstnt1,cnstnt2

	cnstnt0 == 0xabcd		; global equate

code0:	.word	a0
	.word	cnstnt0

	.area	A (OVR)
	cnstnt1 = 0x1234

a0:	.word	0x00ff

	.area	B (ABS,OVR)
	cnstnt2 = 0x5678

	.word	a1

	.area	A

	.=.+0x0020
	.word	a2

	.area	B
	.org	0x40

	.word	a0,a1,a2
	.word	B,OVR

	.org	0x2000
	nop			; at memory address 2000

	.repeat	0x42, 0x10	; repeats 42 for 0x10 bytes

	.fill 	0x99, 0x2100	; fill the rest of the block to 2100 with 99

abcdabcd::				; global symbol

	.page
	.sbttl	Assembler Output File asmtst.sym

;	assembled by:
;		 asxxxx -glosx asmtst

	.sbttl	Symbol Table

;    B          **** GX  |     OVR        **** GX  |   1 a0         0000 R
;    a1         **** GX  |     a2         **** GX  |   2 abcdabcd   004A GR
;    cnstnt0 =  ABCD G   |     cnstnt1 =  1234 G   |     cnstnt2 =  5678 G
;  0 code0      0141 GR  |   0 lclsym0    00C7 R   |   0 lclsym1    0103 R
;    m       =  0000     |     n       =  0001     |     n0x00   =  0000 
;    n0x01   =  0001     |     n0x10   =  0010     |     n0xeeff =  EEFF 
;    n0xff   =  00FF     |     ofsbyte =  0014     |     ofsword =  000A 
;  0 word       008C R

	.sbttl	Area Table

;   0 _CODE      size  145   flags 0
;   1 A          size   24   flags 4
;   2 B          size   4A   flags C

