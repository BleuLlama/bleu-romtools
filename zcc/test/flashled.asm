;
;	Simple debugging routine extracted from shell.z8t
;	Flashes LED continuously at 1 Hz
;
	.area	_CODE
	.globl	flashled
STATUS	= 3
flashled:
	LD	BC,#STATUS
	LD	A,#0x91
	OUT	(C),A
FLASH:
	CALL	DELAY

	LD	A,#0x0A
	OUT	(C),A

	CALL	DELAY

	LD	A,#0x0B
	OUT	(C),A
	JR	FLASH
;-------------------------------;
DELAY:
	LD	DE,#2
LOOP1:
	PUSH	DE
	LD	DE,#0x9000
LOOP2:	DEC	DE
	LD	A,D
	OR	E
	JR	NZ,LOOP2
	POP	DE
	DEC	DE
	LD	A,D
	OR	E
	JR	NZ,LOOP1
	RET
