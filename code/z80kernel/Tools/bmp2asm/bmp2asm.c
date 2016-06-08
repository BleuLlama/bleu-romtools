/*
	bmp2asm

	load in a bmp files from the command line
	print out the RLE encoded bitmap to stdout

	(c)2007 Scott Lawrence
	Use this for whatever, but don't blame me if something breaks.
*/


/*

% foreach i ( *bmp )
foreach? convert $i -colors 8 -geometry 48x32 -crop 28x32+7+0 -contrast -contras
t cropped/$i
foreach? convert cropped/$i -type TrueColor cropped/$i
foreach? echo $i done.
foreach? end

*/

#include <sys/types.h>	/* for fstat() */
#include <sys/stat.h>	/* for fstat() */
#include <string.h>	/* for memset() */
#include <stdlib.h>	/* malloc() */

#include <math.h>	/* for sqrt() */

#include <stdio.h>
#include "bmp.h"

#define NCOLORS (256)	/* make sure we have enough for worst case scenario */

typedef struct BMP_IMAGE {
	char 	*filename;
	char 	*shortname;
	FILEH	fileh;		/* file header information */
	BMPH	bmph;		/* bitmap header information */
	COLOR 	*data;		/* the image data */

	int 	ncolors;	/* the number of colors in the palette */
	COLOR	*palette;	/* the palette */

	/* some derived information... */
	int	w;		/* image width */
	int 	h;		/* image heighth */
} BMP_IMAGE;


int BMP_AddColor( BMP_IMAGE * bi, COLOR * n )
{
	int c;

	/* first try to find it. */
	for( c=0 ; c<NCOLORS ; c++ )
	{
		if(  ( bi->palette[c].red   == n->red   ) &&
		     ( bi->palette[c].green == n->green ) &&
		     ( bi->palette[c].blue  == n->blue  ) )
		{
			return c;
		}
	}

	/* find an empty slot in the palette, and add it */
	for( c=0 ; c<NCOLORS ; c++ )
	{
		if(  ( bi->palette[c].red   == 1 ) &&
		     ( bi->palette[c].green == 1 ) &&
		     ( bi->palette[c].blue  == 1 ) )
		{
			bi->palette[c].red   = n->red;
			bi->palette[c].green = n->green;
			bi->palette[c].blue  = n->blue;
			bi->palette[c].zero  = n->zero;

			bi->ncolors = c+1;
			return c;
		}
	}
	return -1;
}

/* character 161 is solod color 1 */
/* color 1 palette numbers:

	black	00
	white	01
	green	02
	dpurp	03
	brown	04
	red	05
	yellow	07
	orange	0c
	purp	16
	lt blu	19
	dk grn	1b	
	dk blu	1f

*/

unsigned char nearestColor( BMP_IMAGE * bi, COLOR * theColor )
{
	COLOR c2, cp;
	int c;

	c2.red   = (theColor->red   >>4)<<4;
	c2.green = (theColor->green >>4)<<4;
	c2.blue  = (theColor->blue  >>4)<<4;
	c2.zero  = (theColor->zero  >>4)<<4;

	/* search the above pacPalette for the closest color */
	for( c=0; c<bi->ncolors ; c++ )
	{

/*
		printf( "%d:  %02x %02x %02x   %02x %02x %02x\n",
			c, theColor->red, theColor->green, theColor->blue,
			bi->palette[c].red, bi->palette[c].green, bi->palette[c].blue );
*/

		cp.red   = (bi->palette[c].red   >>4)<<4;
		cp.green = (bi->palette[c].green >>4)<<4;
		cp.blue  = (bi->palette[c].blue  >>4)<<4;

		if(	(cp.red == c2.red)
		    &&  (cp.green == c2.green)
		    &&  (cp.blue == c2.blue) )
		{
/*
			printf( "%02x\n", bi->palette[c].zero );
*/
			return( bi->palette[c].zero );
		}

/*
		d = sqrt(
		      ((theColor->red   - pacPalette[c].red   )^2)
		    + ((theColor->green - pacPalette[c].green )^2)
		    + ((theColor->blue  - pacPalette[c].blue  )^2) );

		if( d<lowestDistance )
		{
			lowestDistance = d;
			whichIsLowest = pacPalette[c].zero;
		}
*/
	}
	return 0xff;
}

void BMP_DumpPalette( BMP_IMAGE * bi, char * outfile )
{
	int c;
	COLOR cc;
	FILE * fp;

	if( outfile )
		fp = fopen( outfile, "w" );
	else
		fp = stdout;

	fprintf( fp, "<html><body bgcolor=\"black\">\n" );

	for( c=0 ; c<bi->ncolors ; c++ )
	{
		cc.red   = bi->palette[c].red;
		cc.green = bi->palette[c].green;
		cc.blue  = bi->palette[c].blue;
		cc.zero  = bi->palette[c].zero;


		if( cc.red == 1 && cc.green == 1 && cc.blue == 1 )
			continue;

		fprintf( fp, "<font color=\"#%02x%02x%02x\">\n"
				"%d : %02x %02x %02x : %02x :\n"
				"</font><br/>\n",
		cc.red,  cc.green,  cc.blue, c,
		cc.red,  cc.green,  cc.blue, cc.zero );
	}

	fprintf( fp, "</body></html>\n" );

	printf( "Palette Saved.  %d colors\n", bi->ncolors );

	if( outfile )
		fclose( fp );
}


void BMP_InitPalette( BMP_IMAGE * bi, char * palettefile )
{
	int n,  z;
	unsigned int r,g,b,m;
	COLOR c;
	FILE * fp;
	char buf[128];

	if( !bi ) return;
	bi->palette = (COLOR *)malloc( sizeof(COLOR) * NCOLORS );

	/* clear the palette */
	memset( (unsigned char *)bi->palette, 1, sizeof(COLOR) * NCOLORS );

	
	/* load in the palette file */
	fp = fopen( palettefile, "r" );
	if( !fp ) return;

	while( fgets( buf, 128, fp ))
	{
		n = sscanf( buf, "%d : %x %x %x : %x :", &z, &r, &g, &b, &m );
		if( n == 5 )
		{
			c.red   = (unsigned char)r;
			c.green = (unsigned char)g;
			c.blue  = (unsigned char)b;
			c.zero  = (unsigned char)m;

/*
			printf( "color %3d>  %3d %3d %3d   %3d\n", 
					BMP_AddColor( bi, &c ), r, g, b, m );
*/
			BMP_AddColor( bi, &c );
		}
	}

	fclose( fp );
}

BMP_IMAGE * BMP_Load( char * filename, char * palettefile )
{
	COLOR newColor;
	int x, y;
	struct stat sb0;
	long sz0;
	int ret;

	BMP_IMAGE * bi = NULL;
	FILE * img0 = NULL;
	
	/* attempt to open the file */
	img0 = fopen( filename, "r" );
	if( !img0 ) {
		fprintf( stderr, "ERROR: %s: unable to open!\n", filename );
		return( NULL );
	}

	/* malloc our ram */
	bi = (BMP_IMAGE *) malloc( sizeof( BMP_IMAGE ));
	if( !bi ) {
		fprintf( stderr, "ERROR: Malloc error!\n" );
		fclose( img0 );
		return( bi );
	}

	/* bring over the filename */
	bi->filename = strdup( filename );
	bi->shortname = strdup( filename );

	y = -1;
	for( x=0 ; x<strlen( bi->filename ) ; x++ )
	{
		if( bi->filename[x] == '_' ) 
			y = x;
		if( bi->filename[x] == '.' ) {
			bi->shortname[3+x-y] = '\0';
			y = -1;
		}
		if( y>0 ) {
			bi->shortname[3+x-y] = bi->filename[x];
		}
	}
	
	fprintf( stderr, "%s -> %s\n", bi->filename, bi->shortname );

	/* set up the palette */
	BMP_InitPalette( bi, palettefile );

	/* check filesizes */
	ret = stat( filename, &sb0 );
	sz0 = (long) sb0.st_size;


	fseek( img0, 0, SEEK_SET );	/* rewind */

	/* read in the headers */
	ret = fread( &bi->fileh, sizeof(FILEH), 1, img0 );
	ret = fread( &bi->bmph, sizeof(BMPH),1, img0 );

	fprintf( stderr, "%ld  +  %ld [1] =  %ld\n", 
			sizeof( FILEH ) + sizeof( BMPH ),
			sz0 - (sizeof( FILEH ) + sizeof( BMPH )),
			sz0 );

	/* since we know that the resolution will be < 255, we can
	** just look at iwidth[0], iheight[0]
	*/
	bi->w = bi->bmph.iwidth[0];
	bi->h = bi->bmph.iheight[0];
	fprintf( stderr, "%d x %d  = %d pixels  *3 =  %d bytes (should equal [1] above.)\n", 
			bi->bmph.iwidth[0], bi->bmph.iheight[0], 
			bi->bmph.iwidth[0] * bi->bmph.iheight[0], 
			bi->bmph.iwidth[0] * bi->bmph.iheight[0] * 3 );

	/* seek to the image data */
	fseek( img0, sizeof( FILEH ) + sizeof( BMPH ), SEEK_SET );

	/* allocate! */
	bi->data = (COLOR *)malloc( sizeof(COLOR) * bi->w * bi->h );

	/* load it! */
	for( y=0 ; y<bi->h ; y++ )
	{
		for( x=0; x<bi->w ; x++ )
		{
			/* read the pixel into the vidbuffer array */
			ret = fread( &newColor.blue,  1, 1, img0 );
			ret = fread( &newColor.green, 1, 1, img0 );
			ret = fread( &newColor.red,   1, 1, img0 );
			newColor.zero = 0;

			newColor.blue  = (newColor.blue  >>4)<<4;
			newColor.green = (newColor.green >>4)<<4;
			newColor.red   = (newColor.red   >>4)<<4;

			bi->data[(bi->w * (bi->h-y-1)) + x].blue  = newColor.blue;
			bi->data[(bi->w * (bi->h-y-1)) + x].green = newColor.green;
			bi->data[(bi->w * (bi->h-y-1)) + x].red   = newColor.red;
			bi->data[(bi->w * (bi->h-y-1)) + x].zero  = BMP_AddColor( bi, &newColor );
		}
	}

	fclose( img0 );
	return( bi );
}

void BMP_Free( BMP_IMAGE * bi )
{
	if( !bi ) return;
	if( bi->filename ) free( bi->filename );
	if( bi->data ) free( bi->data );
	free( bi );
}



void BMP_DumpAsm( BMP_IMAGE * bi )
{
	int x,y, z;

	/* now, adjust the image data to have the pac palette colors in .zero */
	for( y=0 ; y<bi->h ; y++ )
	{
		for( x=0 ; x<bi->w ; x++ )
		{
			/* we OR it with 0x80 to make sure that the RLE doesn't screw up */
			bi->data[(y*bi->w)+x].zero = 0x80 
				| nearestColor( bi, &bi->data[(y*bi->w)+x] );
		}
	}


	/* FINALLY, encode the colors to RLE */

	z =0;
	printf( "%s::", bi->shortname );
	for( x=bi->w-1 ; x>=0 ; x-- )
	{
		for( y=0 ; y<bi->h ; y++)
		{
			if( y==0 || y==8 || y==16 || y==24 || y==32 )
				printf( "\n\t.byte\t" );

			printf( "0x%02x", bi->data[(y*bi->w)+x].zero );
			if( y!=7 && y!=15 && y!=23 && y!=31 )
				printf( ", " );
			z++;
		}
	}
	printf( "\n\t.byte\t0x02 ; %d\n", z );
}



void usage( void )
{
	fprintf( stderr, "Usage:  \tbmp2asm COMMAND FILE.BMP\n" );
	fprintf( stderr, "	COMMAND	can be:\n" );
	fprintf( stderr, "	        SCAN	scan the image, build palette.html\n" );
	fprintf( stderr, "	        ASM	scan the image, generate .asm source\n" );
	fprintf( stderr, "	FILE.BMP needs to be a 24 bit truecolor .bmp file\n" );
}

#define PROCESS_SCAN	(1)
#define PROCESS_ASM	(2)

int main( int argc, char **argv )
{
	BMP_IMAGE * bi = NULL;
	int process = 0;

	/* count args */
	if( argc != 3 ) {
		fprintf( stderr, "ERROR: Specify input file and command!\n" );
		usage();
		return -1;
	}

	if( !strcmp( argv[1], "SCAN" ))  process = PROCESS_SCAN;
	if( !strcmp( argv[1], "ASM" ))   process = PROCESS_ASM;

	if( process == 0 ) {
		fprintf( stderr, "ERROR: Unknown COMMAND: %s\n", argv[1] );
		usage();
		return -3;
	}

	bi = BMP_Load( argv[2], "palette.html" );

	if( !bi ) {
		fprintf( stderr, "ERROR: Unable to load %s\n", argv[2] );
		return -3;
	}


	if( process == PROCESS_SCAN )
	{
		BMP_DumpPalette( bi, "palette.html" );
	}

	if( process == PROCESS_ASM )
	{
		BMP_DumpAsm( bi );
	}

	BMP_Free( bi );
	return 0;
}
