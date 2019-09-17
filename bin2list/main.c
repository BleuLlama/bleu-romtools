/*
 * main
 *
 *  2019 Sept 16
 *  yorgle@gmail.com
 */

/*
 * bin2list - Generates text output from binaries
 * Copyright (C) 2000-2019 Scott Lawrence yorgle@gmail.com
 *
 *  This is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage( void )
{
	fprintf(stderr, "%s v%s, Scott Lawrence\n", J_APPNAME, J_VERSION);
	fprintf(stderr, "Usage:\n" );
	fprintf(stderr, "\n");
	fprintf(stderr, "  %s file.bin -basic900     \toutput as BASIC DATA statements\n", J_APPNAME);
	fprintf(stderr, "  %s file.bin -hex          \toutput as HEX dump\n", J_APPNAME);
}


int file_exists( char * filename )
{
	FILE * fp = fopen(filename, "r");
	if (fp)
	{
		fclose(fp);
		return 1;
	}
	return 0;
}

int charsForNum( int num ) 
{
	char buf[32];
	sprintf( buf, "%d", num );
	return strlen( buf );
}

void doOutput_Basic( FILE * fp, int lineNo, int lineWidth )
{
	int linelen = 0;
	int count = 0;

	if( !fp ) return;
	int d = getc( fp );

	while( d != EOF ) {
		
		if( (count == 0)
			|| (linelen + 1 + charsForNum( lineNo ) ) > lineWidth ) {
			printf( "\n" );
			printf( "%d DATA ", lineNo++ );

			linelen = charsForNum( lineNo ) + 6;
			count = 0;
		}
		if( count > 0 ) {
			printf( "," );
			linelen++;
		}
		printf( "%d", d );
		count++;
		linelen += charsForNum( d );

		d = getc( fp );
	}
	printf( "\n" );
	printf( "%d DATA 999\n", lineNo );
}


void doOutput_Hex( FILE * fp )
{
	int offset = 0;
	int count = 0;

	char buf[32];

	if( !fp ) return;
	int d = getc( fp );

	while( d != EOF ) {

		if( count == 0 ) {
			printf( "%08x ", offset );
		}
		if( count == 8 ) {
			printf( " " );
		}

		printf( " %02x", d );

		if( (d >= 0x20) && (d<=0x7e) ) {
			buf[count] = d;
		} else {
			buf[count] = '.';
		}
		buf[count+1] = '\0';

		count++;
		offset++;


		if( count == 16 ) {
			printf( "  |%s|\n", buf );
			count = 0;
		}

		d = getc( fp );
	}
	printf( "\n" );
}



#define kOutputFormat_None		(0)
#define kOutputFormat_BASIC900	(1)
#define kOutputFormat_HEX   	(2)

int main( int argc, char ** argv )
{
	int outputFormat = kOutputFormat_None;
	FILE * fp = NULL;

	// a.out INFILE OPTION
	if( argc != 3 ) {
		usage();
		return 1;
	}

	// make sure infile exists
	if( !file_exists( argv[1] )) {
		fprintf( stderr, "ERROR: %s: Can't open the file!\n", argv[1] );
		return -10;
	}

	// make sure we have a valid output option
	if( !strcmp( argv[2], "-basic900")) {
		outputFormat = kOutputFormat_BASIC900;
	}
	if( !strcmp( argv[2], "-hex")) {
		outputFormat = kOutputFormat_HEX;
	}

	// if unknown, throw error
	if( outputFormat == kOutputFormat_None ) {
		fprintf( stderr, "ERROR: %s: unknown output format!\n", argv[2] );
		return -20;
	}

	// ok. let's do this thing!
	fp = fopen( argv[1], "rb" );
	if( !fp ) {
		fprintf( stderr, "ERROR: couldn't open the file!\n" );
		return -30;
	}

	switch( outputFormat ) {
		case( kOutputFormat_BASIC900 ):
			// NASCOM BASIC has a limit of 48 or 72 characters per line,
			// so we'll limit it to 40.
			// also let's start with DATA on line 900
			doOutput_Basic( fp, 900, 40 );
			break;

		case( kOutputFormat_HEX ):
			doOutput_Hex( fp );
			break;

		default:
			break;
	}
	fclose( fp );
	//fprintf( stderr, "Done!" );
}

