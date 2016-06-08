/*
 * tilemap.h
 *
 * $Id: tilemap.h,v 1.1 2003/05/05 15:50:19 jerry Exp $
 *
 * $Log: tilemap.h,v $
 * Revision 1.1  2003/05/05 15:50:19  jerry
 * Added tilemap.[ch]
 * Added the remaining bit of driver and dumper for the tilemaps
 *
 *
 *
 */

#include "turaco.h"

/* tilemap_Save
 * 
 * save out a specific tilemap
 *  returns 0 on success.
 */
int tilemap_Save( TuracoInstance * ti, int which );


/* tilemap_Load
 *
 *  load in a specific tilemap
 *  returns 0 on success.
 */
int tilemap_Load( TuracoInstance * ti, int which );


/* tilemap_SetContains
 * 
 *   determines whether a sprite number is in the selected set
 *   returns 1 if it is used in a set, 0 if it is not used
 *   on failure, return 0.
 *   **ASSUMES** the bank has been checked already
 */
int tilemap_SetContains( TuracoInstance * ti, int spriteNumber );
