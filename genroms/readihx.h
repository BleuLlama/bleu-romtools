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


#define DECODED_SIZE (32)

typedef struct __ihx {
    unsigned int length;
    unsigned int address;
    int type;
    unsigned char data[DECODED_SIZE];
    int checksum;
} IHX;

extern IHX decoded;

int ascii_to_intc(char c);
int ascii_to_int(int index, char * buffer);
long hexstring_to_long(int index, char * buffer);


void next_ihx(FILE * fp);

