;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Pac-Man hardware defines
;
;  	2004-01  Scott Lawrence
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module hardware

    ; pac hardware defines
        stack           == 0x4ff0
        vidram          == 0x4000
        colram          == 0x4400
        rambase         == 0x4c00
        dsw0            == 0x5080
        in1             == 0x5040
        in0             == 0x5000
        specreg         == 0x5000
	coinlock	== 0x5006
        speclen         == 0x00C0
        sprtbase        == 0x4ff0
        sprtlen         == 0x0010
        p1_port         == in0
		p1_up           == 0
		p1_left         == 1
		p1_right        == 2
		p1_down         == 3
        p2_port         == in1
		p2_up           == 0
		p2_left         == 1
		p2_right        == 2
		p2_down         == 3
        start_port      == in1
		start1          == 5
		start2          == 6
        coin_port       == in0
		coin1           == 5
		coin2           == 6
		coin3           == 7
        rack_port       == in0
		racktest        == 4
        svc_port        == in1
		service         == 4
        cab_port        == in1    
		cabinet         == 7

        sprtMult        == 4
        sprtColor       == 1
        sprtIndex       == 0

        sprtYFlip       == 0
        bitYFlip        == 0
        valYFlip        == 1
	YF		== 1

        sprtXFlip       == 0
        bitXFlip        == 1
        valXFlip        == 2
	XF		== 2

	XYF		== XF + YF

        nsprites        == 0x08
        spritecoords    == 0x5060
	spriteX		== 0x00
	spriteY		== 0x01
	sprite0		== 0
	sprite1		== 2
	sprite2		== 4
	sprite3		== 6
	sprite4		== 8
	sprite5		== 10
	

        v1_acc          == 0x5040
        v1_wave         == 0x5045
        v1_freq         == 0x5051	; 5050 is real base *sub freq*
        v1_vol          == 0x5055

        v2_acc          == 0x5046
        v2_wave         == 0x504a
        v2_freq         == 0x5056
        v2_vol          == 0x505a

        v3_acc          == 0x504b
        v3_wave         == 0x504f
        v3_freq         == 0x505b
        v3_vol          == 0x505f

	; for indexing off of _base:
	v1_base		== 0x5045
	v2_base		== 0x504a
	v3_base		== 0x504f
	vX_wave		== 0x00		; byte
	vX_freq		== 0x0c
	vX_vol		== 0x10

        irqen           == 0x5000
        sounden         == 0x5001
        flipscreen      == 0x5003
        coincount       == 0x5007
        watchdog        == 0x50C0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; colors (for text only)

	C_black		== 0x00
	C_white		== 0x0D

	C_pink		== 0x0B
	C_red		== 0x0C
	C_orange	== 0x02
	C_purple	== 0x03
	C_green		== 0x04
	C_dgreen	== 0x05
	C_yellow	== 0x08
	C_brown		== 0x09
	C_blue		== 0x0f

	C_bluesky	== 0x1c
	C_skyblue	== 0x1d
