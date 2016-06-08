/* 
	bmp.h
	header for a windows .bmp file

	http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html
*/

/* file header */
typedef struct FILEH {
	unsigned char	bfType[2];	/* BM - 19778 */
	unsigned char	bfSize[4];	/* file size in bytes */
	unsigned char	res1[2];	/* reserved - 0 */
	unsigned char	res2[2];	/* reserved - 0 */
	unsigned char	offBits[4];	/* offset to start of Pixel Data - 1078 */
} FILEH;

/* bitmap header */
typedef struct BMPH {
	unsigned char	size[4];	/* header size - 40 */
	unsigned char	iwidth[4];	/* pixels wide - 100 */
	unsigned char 	iheight[4];	/* pixels tall - 100 */
	unsigned char	iplanes[2];	/* 1 */
	unsigned char	ibpp[2];	/* 1, 2, 4, (8), 16, 24, 32 */
	unsigned char	compression[4];	/* 0 = uncompressed */
	unsigned char	imgsize[4];	/* 0 for uncompressed */
	unsigned char	ppmx[4];	/* x pixels per metre - 0 */
	unsigned char	ppmy[4];	/* y pixels per metre - 0 */
	unsigned char	ncolmap[4];	/* number of color map entries used - 0 */
	unsigned char	nclrimp[4];	/* number of significant colors - 0 */
} BMPH;

/* next is color table 4 bytes per color (B G R 0) for the color map */
/* next is the pixeldata - each row is padded to 4 byte boundary */

typedef struct COLOR {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char zero; 	/* always 0 */
} COLOR;
