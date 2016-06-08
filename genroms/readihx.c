/*
 * readihx
 *
 *  parse ihx lines  (Intel Hex file)
 */


/*
 * mkroms - Generates ROM images from IHX files
 * Copyright (C) 2000 Scott Lawrence - yorgle@gmail.com
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
#include <ctype.h>
#include <string.h>
#include "readihx.h"

/* IHX format:  (http://www.8052.com/tutintel.htm)

00 0 0 0 0 1 1
01 3 5 7 9 0 1
:llaaaattbbbbcc

:  ihx line
l  record length (number of bb bytes)
a  address to be loaded into memory
t  record type:  00 normal   01 end of file  02 extended address
b  databytes
c  checksum  - twos compliment of preceeding bytes

*/


IHX decoded;

int ascii_to_intc(char c)
{
    c = toupper (c);
    if (c < 'A')
    {
	c -= '0';
    } else {
	c -= 'A';
	c += 10;
    }

    return (int)c;
}


int ascii_to_int(int index, char * buffer)
{
    int a,b;
    a = ascii_to_intc(buffer[index]);
    b = ascii_to_intc(buffer[index+1]);

    return (a<<4) + b;
}


long hexstring_to_long(int index, char * buffer)
{
    long a = 0;

    while (buffer[index] != '\0')
    {
	a = a << 4;

	a += ascii_to_intc(buffer[index]);

	index++;
    }
    return a;
}


void next_ihx(FILE * fp)
{
    char line[80];
    unsigned int c;

    if (! fscanf(fp, ":%s\n", line) )
    {
	decoded.type = 01;
	return;
    }

    decoded.length = 0;
    decoded.address = 0;
    decoded.type = 0;
    memset(decoded.data, 0, DECODED_SIZE);
    decoded.checksum = 0;


    decoded.length  = 0x00ff & ascii_to_int(0, line);
    decoded.address = 0x00ffff & ((ascii_to_int(2, line)<<8) 
			+ ascii_to_int(4, line));
    decoded.type    = 0x00ff & ascii_to_int(6, line);

    for (c=0 ; c < decoded.length ; c++)
    {
	decoded.data[c] = 0x00ff & ascii_to_int(8+(c*2), line);
    }
    decoded.checksum = 0x00ff & ascii_to_int(8+c, line);
}

