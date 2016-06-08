/* 
	palette.h
	SDL_Palette manipulations
*/

#define MAX_COLORS (256) 


/* Palette_New
**      allocate and format a new 256 color palette
*/
SDL_Palette * Palette_New( void );

/* Palette_Free
**      free the palette
*/
void Palette_Free( SDL_Palette * p );

/* Palette_AddColor
**      add the new color to the palette
**      return the index
*/
int Palette_AddColor( SDL_Palette * p, SDL_Color * n );

/* Palette_Load
**      load in the html file with the palette information
*/
SDL_Palette * Palette_Load( SDL_Palette * p, char * palettefile );

/* Palette_Save
**	save the palette html file to "outfile"
*/
void Palette_Save( SDL_Palette * p, char * outfile );

/* Palette_NearestColor
**      return the color nearest to the passed-in color
*/
int Palette_NearestColor( SDL_Palette * p, SDL_Color * theColor );

