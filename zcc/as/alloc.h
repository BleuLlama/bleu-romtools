/* alloc.h */
/* DECUS C */

extern	void	*alloc();
#ifndef __APPLE__
extern	void	*malloc();
extern	void	*calloc();
#endif
extern	void	*realloc();

