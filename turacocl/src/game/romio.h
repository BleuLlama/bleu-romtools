/*
 * romio.h
 *
 * $Id: romio.h,v 1.1 2003/05/19 21:47:12 jerry Exp $
 */

#include "driver.h"
#include "params.h"
#include "turaco.h"

#ifndef __ROMIO_H__
#define __ROMIO_H__

/*
 * romio_FindROM
 *
 *   This recurses down $(DRIVERS) to find the driver named "drivername".
 *   it puts the proper filename and path down in the buffer 'buf'
 *   if it was not found, buf[0] = NULL;
 */
char *
romio_FindROM( TuracoInstance * ti, char * buf, int bufsize, char * romname );



/* turaco_LoadROMs
 *
 *  loads the ROM data as selected in the user params 
 */
int
romio_LoadROMs( TuracoInstance * ti );



/* turaco_SaveROMs
 *
 *  saves out the internal ROM data to the rom files
 */
int
romio_SaveROMs( TuracoInstance * ti, int format );

#define ROM_FORMAT_BINARY (0)
#define ROM_FORMAT_IHX    (1)


#endif
