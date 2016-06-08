/*
	palette

	(c)2007 Scott Lawrence
	Use this for whatever, but don't blame me if something breaks.
*/


#include <sys/types.h>	/* for fstat() */
#include <sys/stat.h>	/* for fstat() */
#include <string.h>	/* for memset() */
#include <stdlib.h>	/* malloc() */

#include <math.h>	/* for sqrt() */

#include <stdio.h>
#include "SDL.h"
#include "palette.h"


/* Palette_New
**	allocate and format a new 256 color palette
*/
SDL_Palette * Palette_New( void )
{
	SDL_Palette * p;

	/* allocate space for the palette structure */
	p = (SDL_Palette *) malloc( sizeof( SDL_Palette ));
	p->ncolors = 0;

	/* allocate space for the entries */
	p->colors = (SDL_Color *) malloc( sizeof( SDL_Color ) * MAX_COLORS );

	/* start 'em off as 1's */
	memset( (unsigned char *)p->colors, 1, sizeof( SDL_Color ) * MAX_COLORS );
	return( p );
}

/* Palette_Free
**	free the palette
*/
void Palette_Free( SDL_Palette * p )
{
	if( p ) 
	{
		if( p->colors ) free( p->colors );
		free( p );
	}
}

/* Palette_AddColor
**	add the new color to the palette
**	return the index
*/
int Palette_AddColor( SDL_Palette * p, SDL_Color * n )
{
	int c;

	/* first try to find it. */
	for( c=0 ; c<p->ncolors ; c++ )
	{
		if(  ( p->colors[c].r == n->r ) &&
		     ( p->colors[c].g == n->g ) &&
		     ( p->colors[c].b == n->b ) )
		{
			return c;
		}
	}

	/* find an empty slot in the palette, and add it */
	for( c=0 ; c<MAX_COLORS-1 ; c++ )
	{
		if(  ( p->colors[c].r == 1 ) &&
		     ( p->colors[c].g == 1 ) &&
		     ( p->colors[c].b == 1 ) )
		{
			p->colors[c].r = n->r;
			p->colors[c].g = n->g;
			p->colors[c].b = n->b;
			p->colors[c].unused = n->unused;

			p->ncolors = c+1;
			return c;
		}
	}
	return -1;
}

/* Palette_Load
**	load in the html file with the palette information
*/
SDL_Palette * Palette_Load( SDL_Palette * p, char * palettefile )
{
	int n,  z;
	unsigned int r,g,b,m;
	SDL_Color c;
	FILE * fp;
	char buf[128];

	/* load in the palette file */
	fp = fopen( palettefile, "r" );
	if( !fp ) return( p );

	while( fgets( buf, 128, fp ))
	{
		n = sscanf( buf, "%d : %x %x %x : %x :", &z, &r, &g, &b, &m );
		if( n == 5 )
		{
			c.r = (unsigned char)r;
			c.g = (unsigned char)g;
			c.b = (unsigned char)b;
			c.unused = (unsigned char)m;

			Palette_AddColor( p, &c );
		}
	}

	fclose( fp );
	return( p );
}


/* Palette_Save
**      save the palette html file to "outfile"
*/
void Palette_Save( SDL_Palette * p, char * outfile )
{
	int c;
	SDL_Color cc;
	FILE * fp;

	if( outfile )
		fp = fopen( outfile, "w" );
	else
		fp = stdout;

	fprintf( fp, "<html><body bgcolor=\"black\">\n" );

	for( c=0 ; c<p->ncolors ; c++ )
	{
		cc.r = p->colors[c].r;
		cc.g = p->colors[c].g;
		cc.b = p->colors[c].b;
		cc.unused = p->colors[c].unused;


		if( cc.r == 1 && cc.g == 1 && cc.b == 1 )
			continue;

		fprintf( fp, "<font color=\"#%02x%02x%02x\">\n"
				"%d : %02x %02x %02x : %02x :\n"
				"</font><br/>\n",
				cc.r, cc.g, cc.b, c,
				cc.r, cc.g, cc.b, cc.unused );
	}

	fprintf( fp, "</body></html>\n" );

	printf( "Palette Saved.  %d colors\n", p->ncolors );

	if( outfile )
		fclose( fp );
}


/* Palette_NearestColor
**	return the color nearest to the passed-in color
*/
int Palette_NearestColor( SDL_Palette * p, SDL_Color * theColor )
{
	SDL_Color c2, cp;
	int c;

	/* reduce depth */
	c2.r = (theColor->r >>4)<<4;
	c2.g = (theColor->g >>4)<<4;
	c2.b = (theColor->b >>4)<<4;
	c2.unused  = (theColor->unused  >>4)<<4;

	/* search the above pacPalette for the closest color */
	for( c=0; c<p->ncolors ; c++ )
	{
		cp.r = (p->colors[c].r >>4)<<4;
		cp.g = (p->colors[c].g >>4)<<4;
		cp.b = (p->colors[c].b >>4)<<4;

		if(	(cp.r == c2.r)
		    &&  (cp.g == c2.g)
		    &&  (cp.b == c2.b) )
		{
			return( p->colors[c].unused );
		}

/*
		d = sqrt(
		      ((theColor->red   - pacPalette[c].red   )^2)
		    + ((theColor->green - pacPalette[c].green )^2)
		    + ((theColor->blue  - pacPalette[c].blue  )^2) );

		if( d<lowestDistance )
		{
			lowestDistance = d;
			whichIsLowest = pacPalette[c].unused;
		}
*/
	}
	return 0xff;
}

#ifdef NEVER




void Colors_DumpAsm( SDL_Palette * bi )
{
	int x,y, z;

	/* now, adjust the image data to have the pac palette colors in .unused */
	for( y=0 ; y<bi->h ; y++ )
	{
		for( x=0 ; x<bi->w ; x++ )
		{
			/* we OR it with 0x80 to make sure that the RLE doesn't screw up */
			bi->data[(y*bi->w)+x].unused = 0x80 
				| BMP_NearestColor( bi, &bi->data[(y*bi->w)+x] );
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

			printf( "0x%02x", bi->data[(y*bi->w)+x].unused );
			if( y!=7 && y!=15 && y!=23 && y!=31 )
				printf( ", " );
			z++;
		}
	}
	printf( "\n\t.byte\t0x02 ; %d\n", z );
}
#endif
