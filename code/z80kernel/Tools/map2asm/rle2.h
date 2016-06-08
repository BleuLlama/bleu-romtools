/* rle2.h
**
**	2008 Scott Lawrence
**
**	header file for the RLE2 compression definition
**
**	This basically embodies and explains the entire format of the
**	RLE2 data blocks.  I hope to have this be informative and 
**	defined enough that you can reference this header file and 
**	decode a block of RLE2 data by hand.
*/

/*
**	The RLE2 format is essentially a Command/Data byte with additional
**	bytes of data after the command/data byte.  Most of the commands
**	aside from _END will require data bytes that follow the command byte.
**
**	It is split as follows:
*/
#define RLE_COMMAND_MASK	(0xE0)
#define RLE_DATA_MASK		(0x1F)

/* 	For the rest of this file, the "COMMAND" portion will be referred
**	to as _C and the "DATA" portion will be referred to as _D and 
**	the combined byte will be referred to as _C_D
**
**	What follows is the explanation of all of the _C masked portions
**	of the _C_D byte.  
**	The destination screen buffer is ''Screen[]''
**	The position in the buffer is ''ScreenPos''
**	The lookup table is ''lut[]''
*/

/* 	The lookup table is passed into the decoder in the IX register.
**	it is a pointer to an array of 16 bytes.  
**	For completeness here:
**		HL gets the starting screen position
**		DE is the pointer to the RLE data block (_C_D, Data, etc)
**		IX LUT pointer (if used)
**	The LUT is essentially the 16 most used bytes in the screen 
**	information.
*/

#define RLE_CMD_SKIP		(0x20)	/* skip _D screen positions (0..1f) */
/* example:	0x23	
		ScreenPos += 3
*/

#define RLE_CMD_BIGSKIP		(0xe0)	/* skip _D*(32) positions (one col) */
/* example:	0x23
		ScreenPos += (_D*32)
*/

#define RLE_CMD_LUT_REP		(0x60)	/* repeat _D times, next byte LUT */
/* example:	0x63 0x45
		Screen[ScreenPos++] = lut[4] 
		Screen[ScreenPos++] = lut[5] 
		Screen[ScreenPos++] = lut[4] 
		Screen[ScreenPos++] = lut[5] 
		Screen[ScreenPos++] = lut[4] 
		Screen[ScreenPos++] = lut[5]
*/

#define RLE_CMD_LUT_COPY	(0x80)	/* copy _D bytes, ref thru LUT */
/* example:	0x83 0x45 0x56 0x98
		Screen[ScreenPos++] = lut[4] 
		Screen[ScreenPos++] = lut[5] 
		Screen[ScreenPos++] = lut[5] 
		Screen[ScreenPos++] = lut[6] 
		Screen[ScreenPos++] = lut[9] 
		Screen[ScreenPos++] = lut[8]
*/

#define RLE_CMD_LUT_COPY_N	(0x40)	/* copy _D lower nibs, ref thru LUT */
/* example:	0x43 0x45 0x56 0x98
		Screen[ScreenPos++] = lut[5] 
		Screen[ScreenPos++] = lut[6] 
		Screen[ScreenPos++] = lut[8]
*/

#define RLE_CMD_COPY		(0xc0)	/* copy _D following bytes */
/* example: 	0xc3 0x45 0x56 0x98
		Screen[ScreenPos++] = 0x45
		Screen[ScreenPos++] = 0x56
		Screen[ScreenPos++] = 0x98
*/

#define RLE_CMD_REP		(0xa0)	/* repeat _D times the following byte */
/* example: 	0xa3 0x45
		Screen[ScreenPos++] = 0x45
		Screen[ScreenPos++] = 0x45
		Screen[ScreenPos++] = 0x45
*/

/* if _C_D & RLE_COMMAND_MASK == 0x00 , then it's an immediate command.
	_D contains the special command number. 
	Example usage is not provided for these, as they are single
	byte commands in the data stream, and it is unnecessary.
*/
#define RLE_CMD_IMMEDIATE	(0x00)		/* _C = 0x00, _D = Cmd No. */

#define RLE_CMDI_END		(0x00 | 0x00)	/* end of image data stream */
#define RLE_CMDI_RESTART	(0x00 | 0x01)	/* restart at starting pos */
#define RLE_CMDI_NEXT		(0x00 | 0x02)	/* restart at _S+0x400 */


/* procedure for compression

1.  Load in the data file
	.bmp
	.map
	- pull out screen data
	- pull out or generate LUT data

2. arrange the data in a linear buffer

3. starting at [0], apply each of the compression versions on it, in order
	sort them according to:
		the number of bytes they cover
		the number of bytes they save
	ie: if the next bytes are 0x03 0x02, then a RLE might save and use 1
	    but if the nibble repeat notices that the full sequence is
		0x03 0x02 0x03 0x02, then that will also use the same amount
		of storage space, but saves more than the window of the first.


*/
